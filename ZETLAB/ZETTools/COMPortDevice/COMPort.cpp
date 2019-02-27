#include "StdAfx.h"
#include "COMPort.h"

HANDLE hCOMPort[MAX_NUMBER];
OVERLAPPED structOVERLAPPEDRead[MAX_NUMBER];
OVERLAPPED structOVERLAPPEDWrite[MAX_NUMBER];
unsigned char pBufferRead[MAX_NUMBER][BUFFER_SIZE];
unsigned char pBufferWrite[MAX_NUMBER][BUFFER_SIZE];
int iSizeToWrite[MAX_NUMBER];
bool bThreadRead[MAX_NUMBER];
bool bThreadWrite[MAX_NUMBER];
HWND hWnd[MAX_NUMBER];
int gi_Index = 0;

UINT ReadThread(LPVOID lpParam)
{
	int index = int(lpParam);
	COMSTAT structCOMSTAT;
	DWORD dwReceivedSize(0), dwTemp(0), dwEventMask(0);

	structOVERLAPPEDRead[index].hEvent = CreateEvent(NULL, true, true, NULL);
	TRACE("Запуск потока %d\n");
	SetCommMask(hCOMPort[index], EV_RXCHAR);           		
	while (bThreadRead[index])
	{
		WaitCommEvent(hCOMPort[index], &dwEventMask, &structOVERLAPPEDRead[index]);
		if (structOVERLAPPEDRead[index].hEvent != NULL)
 			WaitForSingleObject(structOVERLAPPEDRead[index].hEvent, INFINITE);

			if (GetOverlappedResult(hCOMPort[index], &structOVERLAPPEDRead[index], &dwTemp, true))
			{
				if ((dwEventMask & EV_RXCHAR) != 0)
				{
					Sleep(20);
					ClearCommError(hCOMPort[index], &dwTemp, &structCOMSTAT);
					dwReceivedSize = structCOMSTAT.cbInQue;
					while (dwReceivedSize > BUFFER_SIZE)
					{
						TRACE(L"!!!Received Size %d symbols in %d\n", dwReceivedSize, hCOMPort[index]);
						ReadFile(hCOMPort[index], pBufferRead[index], BUFFER_SIZE, &dwTemp, &structOVERLAPPEDRead[index]);
						dwReceivedSize -= BUFFER_SIZE;
					}
					if (dwReceivedSize)
					{
						ReadFile(hCOMPort[index], pBufferRead[index], dwReceivedSize, &dwTemp, &structOVERLAPPEDRead[index]);
						::SendMessage(hWnd[index], WM_RECEIVE, dwReceivedSize, 0);
						TRACE(L"Send SIZE = %d to %d\n", dwReceivedSize, hWnd[index]);
					}
				}
			}

		}
		if (structOVERLAPPEDRead[index].hEvent != NULL)
			CloseHandle(structOVERLAPPEDRead[index].hEvent);
	
	return 0;
}


UINT WriteThread(LPVOID lpParam)
{
	int index = int(lpParam);
	DWORD dwTemp(0);

	structOVERLAPPEDWrite[index].hEvent = CreateEvent(NULL, true, true, NULL);
	WriteFile(hCOMPort[index], pBufferWrite[index], iSizeToWrite[index], &dwTemp, &structOVERLAPPEDWrite[index]);
	if (structOVERLAPPEDWrite[index].hEvent != NULL)
	{
		if (WaitForSingleObject(structOVERLAPPEDWrite[index].hEvent, INFINITE) == WAIT_OBJECT_0)
		{ 
			memset(pBufferWrite[index], 0x0, BUFFER_SIZE);
			iSizeToWrite[index] = 0;
			bThreadWrite[index] = false;
			CloseHandle(structOVERLAPPEDWrite[index].hEvent);
		}
	}
	return 0;
}

CCOMPort::CCOMPort(void)
: hThreadRead(NULL)
, hThreadWrite(NULL)
, iIndex(0)
{
	iIndex = gi_Index;
	++gi_Index;
	for (int i = 0; i < MAX_NUMBER; ++i)
	{
		hCOMPort[i] = NULL;
		bThreadRead[i] = false;
		bThreadWrite[i] = false;
		memset(pBufferRead[i], 0x0, BUFFER_SIZE);
		memset(pBufferWrite[i], 0x0, BUFFER_SIZE);
		iSizeToWrite[i] = 0;
	}
	structCOMMTIMOUTS.ReadIntervalTimeout = 0;
	structCOMMTIMOUTS.ReadTotalTimeoutConstant = 0;
	structCOMMTIMOUTS.ReadTotalTimeoutMultiplier = 0;
	structCOMMTIMOUTS.WriteTotalTimeoutConstant = 0;
	structCOMMTIMOUTS.WriteTotalTimeoutMultiplier = 0;
}

CCOMPort::~CCOMPort(void)
{
	if (bThreadWrite[iIndex])
		bThreadWrite[iIndex] = false;

	if (NULL != hThreadWrite)	// если поток работает
	{
		if (WaitForSingleObject(hThreadWrite, 100) != WAIT_OBJECT_0)
			TerminateThread(hThreadWrite, 0);

		if (NULL != hThreadWrite)
		{
			CloseHandle(hThreadWrite);		// удаляем хэндел
			hThreadWrite = NULL;
		}
	}
}

int CCOMPort::OpenCOMPort()
{
	if (hCOMPort[iIndex] == NULL)
	{
		CString sPortName;     	
		DCB structDCB;           	
		//COMMTIMEOUTS structCOMMTIMOUTS;  	

		sPortName.Format(L"\\\\.\\COM%d", comStruct.nPortNumber);

		hCOMPort[iIndex] = CreateFile(sPortName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
		if(hCOMPort[iIndex] == INVALID_HANDLE_VALUE)            
		{
			hCOMPort[iIndex] = NULL;
			//Не удалось открыть порт
			return -2;
		}

		structDCB.DCBlength = sizeof(DCB);
		if(!GetCommState(hCOMPort[iIndex], &structDCB))
		{
			CloseCOMPort();
			//Не удалось считать DCB
			return -3;
		}
		structDCB.DCBlength = sizeof(DCB);
		structDCB.fBinary = TRUE;    
		structDCB.BaudRate = DWORD (comStruct.nBaudRate);
		structDCB.ByteSize = BYTE (comStruct.nByteSize);
		structDCB.Parity = BYTE (comStruct.nParity);                                
		structDCB.StopBits = BYTE (comStruct.nStopBits);                            

		if(!SetCommState(hCOMPort[iIndex], &structDCB))
		{
			CloseCOMPort();
			//Не удалось установить DCB
			return -4;
		}
				
		comStruct.nBaudRate = structDCB.BaudRate;
		comStruct.nByteSize = structDCB.ByteSize;
		comStruct.nParity = structDCB.Parity;
		comStruct.nStopBits = structDCB.StopBits;

		//structCOMMTIMOUTS.ReadIntervalTimeout = 20;
		//structCOMMTIMOUTS.ReadTotalTimeoutMultiplier = 15;
		//structCOMMTIMOUTS.ReadTotalTimeoutConstant = 300; 
		//structCOMMTIMOUTS.WriteTotalTimeoutMultiplier = 0;
		//structCOMMTIMOUTS.WriteTotalTimeoutConstant = 0;
		
		if(!SetCommTimeouts(hCOMPort[iIndex], &structCOMMTIMOUTS))
		{
			CloseCOMPort();
			//Не удалось установить тайм-ауты;
			return -5;
		}

		if (!SetupComm(hCOMPort[iIndex], 512, 512))
		{
			CloseCOMPort();
			//Не удалось инициализировать коммуникационные параметры
			return -6;
		}

		if (!PurgeComm(hCOMPort[iIndex], PURGE_RXCLEAR))
		{
			CloseCOMPort();
			//Не удалось очистить буфер ввода
			return - 7;
		}
		return 0;
	}
	else
		return -1;
}

int CCOMPort::CloseCOMPort()
{
	StopReading();
	if (hCOMPort[iIndex] != NULL)
	{
		CloseHandle(hCOMPort[iIndex]);
		hCOMPort[iIndex] = NULL;
		
		return 0;
	}
	else
		return -1;
}

int CCOMPort::SetPortNumber(int nPortNumber)
{
	if (hCOMPort[iIndex] != NULL)
	{
		bool bThr = bThreadRead[iIndex];
		CloseCOMPort();
		comStruct.nPortNumber = nPortNumber;
		OpenCOMPort();
		if (bThr)
		{
			StartReading();
		}
	}
	else
	{
		comStruct.nPortNumber = nPortNumber;
	}
	return 0;
}

int CCOMPort::SetBaudRate(int nBaudRate, int* baudRate)
{
	if (hCOMPort[iIndex] != NULL)
	{
		DCB structDCB;    
		structDCB.DCBlength = sizeof(DCB);
		if(!GetCommState(hCOMPort[iIndex], &structDCB))
		{
			CloseCOMPort();
			//Не удалось считать DCB
			return -3;
		}
		comStruct.nBaudRate = nBaudRate;
		structDCB.BaudRate = comStruct.nBaudRate;
		if(!SetCommState(hCOMPort[iIndex], &structDCB))
		{
			CloseCOMPort();
			//Не удалось установить DCB
			return -4;
		}

		comStruct.nBaudRate = structDCB.BaudRate;
		*baudRate = comStruct.nBaudRate;
	}
	else
	{
		comStruct.nBaudRate = nBaudRate;
		*baudRate = comStruct.nBaudRate;
	}
	return 0;
}

int CCOMPort::SetParity(short nParity, short* parity)
{
	if (hCOMPort[iIndex] != NULL)
	{
		DCB structDCB;    
		structDCB.DCBlength = sizeof(DCB);
		if(!GetCommState(hCOMPort[iIndex], &structDCB))
		{
			CloseCOMPort();
			//Не удалось считать DCB
			return -3;
		}

		comStruct.nParity = nParity;
		structDCB.Parity = BYTE(comStruct.nParity);
		if(!SetCommState(hCOMPort[iIndex], &structDCB))
		{
			CloseCOMPort();
			//Не удалось установить DCB
			return -4;
		}

		comStruct.nParity = structDCB.Parity;
		*parity = comStruct.nParity;
	}
	else
	{
		comStruct.nParity = nParity;
		*parity = comStruct.nParity;
	}
	return 0;
}

int CCOMPort::SetByteSize(short nByteSize, short* byteSize)
{
	if (hCOMPort[iIndex] != NULL)
	{
		DCB structDCB;    
		structDCB.DCBlength = sizeof(DCB);
		if(!GetCommState(hCOMPort[iIndex], &structDCB))
		{
			CloseCOMPort();
			//Не удалось считать DCB
			return -3;
		}

		comStruct.nByteSize = nByteSize;
		structDCB.ByteSize = BYTE(comStruct.nByteSize);
		if(!SetCommState(hCOMPort[iIndex], &structDCB))
		{
			CloseCOMPort();
			//Не удалось установить DCB
			return -4;
		}

		comStruct.nByteSize = structDCB.ByteSize;
		*byteSize = comStruct.nByteSize;
	}
	else
	{
		comStruct.nByteSize = nByteSize;
		*byteSize = comStruct.nByteSize;
	}
	return 0;
}

int CCOMPort::SetStopBits(short nStopBits, short* stopBits)
{
	if (hCOMPort[iIndex] != NULL)
	{
		DCB structDCB;    
		structDCB.DCBlength = sizeof(DCB);
		if(!GetCommState(hCOMPort[iIndex], &structDCB))
		{
			CloseCOMPort();
			//Не удалось считать DCB
			return -3;
		}

		comStruct.nStopBits = nStopBits;
		structDCB.StopBits = BYTE(comStruct.nStopBits);
		if(!SetCommState(hCOMPort[iIndex], &structDCB))
		{
			CloseCOMPort();
			//Не удалось установить DCB
			return -4;
		}

		comStruct.nStopBits = structDCB.StopBits;
		*stopBits = comStruct.nStopBits;
	}
	else
	{
		comStruct.nStopBits = nStopBits;
		*stopBits = comStruct.nStopBits;
	}
	return 0;
}

void CCOMPort::StartReading()
{
	if (!bThreadRead[iIndex])
	{
		bThreadRead[iIndex] = true;
		hThreadRead = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReadThread, LPVOID(iIndex), 0, NULL);
	}
}

void CCOMPort::StopReading()
{
	if (bThreadRead[iIndex])
	{
		bThreadRead[iIndex] = false;
		WaitForSingleObject(hThreadRead, 100);
		TerminateThread(hThreadRead, 0);
		CloseHandle(hThreadRead);
		hThreadRead = NULL;
	}
}

void CCOMPort::SetParent(HWND hwnd)
{
	hWnd[iIndex] = hwnd;
}

void CCOMPort::ReadBuffer(unsigned char* ucBuffer)
{
	memcpy_s(ucBuffer, BUFFER_SIZE, pBufferRead[iIndex], BUFFER_SIZE);
	TRACE(L"Read buffer\n");
}

int CCOMPort::SendCommand(CString sCommand)
{
	if (hCOMPort[iIndex] != NULL)
	{
		if (!bThreadWrite[iIndex])
		{
			bThreadWrite[iIndex] = true;
			if (sCommand.GetLength() >= BUFFER_SIZE)
				sCommand.Delete(BUFFER_SIZE, sCommand.GetLength() - BUFFER_SIZE);
			iSizeToWrite[iIndex] = sCommand.GetLength();
			memcpy_s(pBufferWrite[iIndex], iSizeToWrite[iIndex] * sizeof(char), CStringA(sCommand).GetString(), iSizeToWrite[iIndex] * sizeof(char));
			CloseHandle(hThreadWrite);
			hThreadWrite = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WriteThread, LPVOID(iIndex), 0, NULL);
			TRACE(L"Send command " + sCommand + L"\n");
		}
	}
	return 0;
}

int CCOMPort::SendArray(long* pArray, int size)
{
	if (hCOMPort[iIndex] != NULL)
	{
		if (!bThreadWrite[iIndex])
		{
			bThreadWrite[iIndex] = true;
			iSizeToWrite[iIndex] = size;
			for (int i = 0; i < size; i++)
				pBufferWrite[iIndex][i] = (unsigned char)pArray[i];
			CloseHandle(hThreadWrite);
			hThreadWrite = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WriteThread, LPVOID(iIndex), 0, NULL);
			TRACE("Send array ");
			for (int i = 0; i < size; i++)
				TRACE(L"%02x ", pArray[i]);
			TRACE("\n");
		}
	}
	return 0;
}

void CCOMPort::SetIndex( int index )
{
	iIndex = index;
}
