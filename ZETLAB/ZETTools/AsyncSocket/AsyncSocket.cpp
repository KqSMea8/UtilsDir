#include "stdafx.h"
#include "AsyncSocket.h"
#include "WS2tcpip.h"

const DWORD PACKET_SIGNATURE_SIZE = sizeof(unsigned int);

AsyncSocket::AsyncSocket(unsigned int signature): _signature(signature), _working(false), ClientSocket(NULL)
{
	InitializeCriticalSection(&_crSectionRecv);
}

AsyncSocket::~AsyncSocket()
{
	Stop();
	DeleteCriticalSection(&_crSectionRecv);
}

inline long AsyncSocket::IsWorking() const
{
	return _working;
}

void AsyncSocket::Stop()
{
	Break();
	_recvState.Clear();
	if(ClientSocket != NULL)
	{
		int result = closesocket(ClientSocket);
		if (result == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
		}
		ClientSocket = NULL;
	}
}

void AsyncSocket::Break()
{
	SetWorkingState(0);
	clearReceiveQueue();
}

bool AsyncSocket::InitializeClientSocket(SOCKET socket)
{
	if((socket == NULL) || (socket == INVALID_SOCKET))
	{
		return false;
	}
	ClientSocket = socket;

	BOOL yes = TRUE;
	int r(0);
	r = setsockopt(ClientSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&yes, sizeof(yes));
	r = setsockopt(ClientSocket, SOL_SOCKET, SO_OOBINLINE, (const char*)&yes, sizeof(yes));
	SetWorkingState(1);

	return true;
}

bool AsyncSocket::Connect(const char* ip, unsigned short port)
{
	Stop();

	if(ip == NULL)
	{
		return false;
	}

	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ClientSocket == INVALID_SOCKET)
	{
		int error = WSAGetLastError();
		ClientSocket = NULL;
		return false;
	}
	BOOL yes = TRUE;
	int r(0);
	r = setsockopt(ClientSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&yes, sizeof(yes));
	r = setsockopt(ClientSocket, SOL_SOCKET, SO_OOBINLINE, (const char*)&yes, sizeof(yes));

	sockaddr_in clientService = { 0 };
	clientService.sin_family = AF_INET;
	clientService.sin_port = htons(port);
	inet_pton(AF_INET, ip, &clientService.sin_addr.s_addr);
	
	int result = connect(ClientSocket, (SOCKADDR*)&clientService, sizeof (clientService));
	if (result == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		result = closesocket(ClientSocket);
		if (result == SOCKET_ERROR)
		{
			int error(WSAGetLastError());
		}
		ClientSocket = NULL;
		return false;
	}
	SetWorkingState(1);

	return true;
}

bool AsyncSocket::Receive()
{
	if(ClientSocket != NULL)
	{
		TIMEVAL timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 1;

		if(WaitFor(ClientSocket, true, &timeout))
		{
			int blockCount = receiveData(ClientSocket);
			if(blockCount == 0)
			{
				Break();
			}
			return blockCount > 0;
		}
	}

	return false;
}

// If is it header packet - add signature in begin of message.
bool AsyncSocket::Send(BYTE* buffer, int size, bool headerPacket)
{
	if(ClientSocket != NULL && buffer != NULL)
	{
		TIMEVAL timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 1;

		int nSent = 0;
		BYTE* pBuffer;

		/*if(headerPacket)
		{
			BYTE* signatureBuffer = (BYTE*)&_signature;
			// отправка сигнатуры в составе пакета с данными
			//size += PACKET_SIGNATURE_SIZE;
			while( nSent < PACKET_SIGNATURE_SIZE && IsWorking())
			{
				if(WaitFor(ClientSocket, false, &timeout))
				{
					// отправка отдельного пакета с сигнатурой
					int byteSent = sendData(ClientSocket, signatureBuffer + nSent, PACKET_SIGNATURE_SIZE);
					
					//int byteSent = sendData(ClientSocket, signatureBuffer + nSent, size);
					
					if (byteSent == SOCKET_ERROR)
					{
						Break();
						return false;
					}
					nSent += byteSent;
				}
			}
			nSent = 0;
		}*/

		// отправка сигнатуры в составе пакета с данными
		if (headerPacket)
		{
			pBuffer = new BYTE[size + PACKET_SIGNATURE_SIZE];
			memcpy_s(pBuffer, PACKET_SIGNATURE_SIZE, &_signature, PACKET_SIGNATURE_SIZE);
			memcpy_s(pBuffer + PACKET_SIGNATURE_SIZE, size, buffer, size);
			size += PACKET_SIGNATURE_SIZE;
		}
		else
			pBuffer = buffer;
						
		while(nSent < size && IsWorking())
		{
			if(WaitFor(ClientSocket, false, &timeout))
			{
				int byteSent = sendData(ClientSocket, pBuffer + nSent, size - nSent);
				if (byteSent == SOCKET_ERROR)
				{
					Break();
					return false;
				}
				nSent += byteSent;
			}
		}

		if (headerPacket)
		{
			delete []pBuffer;
			pBuffer = nullptr;
		}
		
		if(nSent == size)
		{
			return true;
		}
	}

	return false;
}

RecvData* AsyncSocket::GetReceiveData()
{
	RecvData *recvData = NULL;
	if(IsWorking())
	{
		EnterCriticalSection(&_crSectionRecv);
		if(!_recvQueue.empty())
		{
			recvData = _recvQueue.front();
			_recvQueue.pop_front();
		}
		LeaveCriticalSection(&_crSectionRecv);
	}
	return recvData;
}

void AsyncSocket::SetWorkingState(long isWorking)
{
	InterlockedExchange(&_working, isWorking);
}

bool AsyncSocket::WaitFor(SOCKET socket, bool waitRead, const TIMEVAL *timeout)
{
	if(socket == NULL)
	{
		return false;
	}	

	FD_SET readFDSet, writeFDSet;
	FD_SET *pReadFD, *pWriteFD, *usedFDSet;
	if(waitRead)
	{
		pReadFD = &readFDSet;
		pWriteFD = NULL;
		usedFDSet = pReadFD;
	}
	else
	{
		pReadFD = NULL;
		pWriteFD = &writeFDSet;
		usedFDSet = pWriteFD;
	}

	FD_ZERO(usedFDSet);
	int bReadySock;

	while (IsWorking())
	{
		FD_SET(socket, usedFDSet);
		if ((bReadySock = select(0, pReadFD, pWriteFD, NULL, timeout)) == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			return false;
		}

		if (FD_ISSET(socket, usedFDSet))
		{
			return true;
		}
	}
	

	return false;
}

void AsyncSocket::clearReceiveQueue()
{
	EnterCriticalSection(&_crSectionRecv);
	RecvData *recvData;
	while(!_recvQueue.empty())
	{
		recvData = _recvQueue.front();
		_recvQueue.pop_front();
		delete recvData;
		recvData = nullptr;
	}
	LeaveCriticalSection(&_crSectionRecv);
}

void AsyncSocket::addReceiveQueue(RecvData* recvData)
{
	if(IsWorking())
	{
		EnterCriticalSection(&_crSectionRecv);
		_recvQueue.push_back(recvData);
		LeaveCriticalSection(&_crSectionRecv);
	}
}

int AsyncSocket::sendData(SOCKET socket, BYTE* buffer, int size)
{
	int nSent = send(socket, (LPSTR)buffer, size, 0);
	
	switch( nSent )
	{
	case 0:
		{/*--- соединение было разорвано ---*/ 
			break;
		}
	case SOCKET_ERROR:
		{/*--- ошибка при отправлении ---*/  
			if( WSAGetLastError() == WSAEWOULDBLOCK )
			{
				// Операция блокирована, это нормально, просто выходим.
				nSent = 0;
			}
			break;
		}
	default:
		{
			break;
		}
	}
	
	return nSent;
}

int AsyncSocket::receiveData(SOCKET socket)
{
	_recvState.CheckTempData();

	int result = 0;
	int nRead = recv(socket, (LPSTR)(_recvState.Buffer + _recvState.Offset), MAX_BUFFER_SIZE - _recvState.Offset, 0);
	//TRACE("receiveData socket = %d\tnread = %d\n ", long(socket), nRead);
	// вот здесь чтение возвращает -1
	switch( nRead )
	{
	case 0:
		{/*--- соединение было разорвано ---*/ 
			//TRACE("receiveData switch 0\n");
			break;
		}
	case SOCKET_ERROR:
		{/*--- ошибка при получении ---*/ 
			int err = WSAGetLastError();
			//TRACE("receiveData switch -1\terror = %d\n", err);
			//if( WSAGetLastError() == WSAEWOULDBLOCK )
			if (err == WSAEWOULDBLOCK)
			{
				// Операция блокирована, это нормально, просто выходим.
				result = -1;
			}
			break;
		}
	default:
		{
			//TRACE("receiveData switch %d\n", nRead);

			result++;
			_recvState.Offset += nRead;
			bool readyForActions(true);

			while(readyForActions)
			{
				readyForActions = false;
				if(_recvState.Step == Signature)
				{
					if(_recvState.Offset - _recvState.Shift >= PACKET_SIGNATURE_SIZE)
					{
						unsigned int signature;
						_recvState.Shift += bytes2type(_recvState.Buffer + _recvState.Shift, &signature);

						if(!checkPacketSignature(signature))
						{
							return 0; // Сигнатуры не совпадают, некорректная ситуация, выходим с ошибкой для закрытия сокета!
						}

						_recvState.Step = Size;
					}
				}

				if(_recvState.Step == Size)
				{
					if(_recvState.Offset - _recvState.Shift >= PACKET_DATA_SIZE)
					{
						DWORD dataSize;
						_recvState.Shift += bytes2type(_recvState.Buffer + _recvState.Shift, &dataSize);
						
						//если мы прочитали не нулевой размер
						//выделяем память под массив
						if(dataSize > 0)
						{
							_recvState.TempRecvData->AllocateData(dataSize);
							_recvState.Step = Data;
						}
						else
						{
							_recvState.Clear();
							_recvState.ResetData();
							readyForActions = true;
						}						
					}
				}

				if(_recvState.Step == Data)
				{
					DWORD readDataSize = _recvState.Offset - _recvState.Shift;
					if (readDataSize > 0 && _recvState.TempRecvData != nullptr)
					{
						bool endOfData = readDataSize >= (_recvState.TempRecvData->Size - _recvState.DataShift);
						DWORD moveDataSize = endOfData ? _recvState.TempRecvData->Size - _recvState.DataShift : readDataSize;
						memmove(_recvState.TempRecvData->Data + _recvState.DataShift, _recvState.Buffer + _recvState.Shift, moveDataSize);
						_recvState.DataShift += moveDataSize;
						_recvState.Shift += moveDataSize;

						//после выполненной комманды сдвигаем буфер и проверяем дальше.
						if(endOfData)
						{
							addReceiveQueue(_recvState.TempRecvData);
							_recvState.ResetData();
								readyForActions = true;
						}

						_recvState.Offset -= _recvState.Shift;
						if(_recvState.Offset > 0)
						{
							memmove(_recvState.Buffer, _recvState.Buffer + _recvState.Shift, _recvState.Offset);
							readyForActions = true;
						}
						_recvState.Shift = 0;
					}
				}
			}
			break;
		}
	}

	return result;
}