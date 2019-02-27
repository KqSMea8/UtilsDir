#pragma once

#define BUFFER_SIZE		512
#define WM_RECEIVE		WM_USER + 100
#define MAX_NUMBER		10

class CCOMPort
{
public:
	CCOMPort(void);
	~CCOMPort(void);

	int OpenCOMPort();
	int CloseCOMPort();
	int SendCommand(CString sCommand);
	int SendArray(long* pArray, int size);

	int SetPortNumber(int nPortNumber);
	int SetBaudRate(int nBaudRate, int* baudRate);
	int SetParity(short nParity, short* parity);
	int SetByteSize(short nByteSize, short* byteSize);
	int SetStopBits(short nStopBits, short* stopBits);
	void SetParent(HWND hwnd);
	void SetIndex(int index);
	void ReadBuffer(unsigned char* ucBuffer);

	void StartReading();
	void StopReading();

	long GetReadTimeoutInterval(void)				{ return structCOMMTIMOUTS.ReadIntervalTimeout; };
	void SetReadTimeoutInterval(long timeout)		{ structCOMMTIMOUTS.ReadIntervalTimeout = timeout; };

	long GetReadTimeoutMultiplier(void)				{ return structCOMMTIMOUTS.ReadTotalTimeoutMultiplier; };
	void SetReadTimeoutMultiplier(long timeout)		{ structCOMMTIMOUTS.ReadTotalTimeoutMultiplier = timeout; };

	long GetReadTimeoutConstant(void)				{ return structCOMMTIMOUTS.ReadTotalTimeoutConstant; };
	void SetReadTimeoutConstant(long timeout)		{ structCOMMTIMOUTS.ReadTotalTimeoutConstant = timeout; };

	long GetWriteTimeoutMultiplier(void)			{ return structCOMMTIMOUTS.WriteTotalTimeoutMultiplier; };
	void SetWriteTimeoutMultiplier(long timeout)	{ structCOMMTIMOUTS.WriteTotalTimeoutMultiplier = timeout; };

	long GetWriteTimeoutConstant(void)				{ return structCOMMTIMOUTS.WriteTotalTimeoutConstant; };
	void SetWriteTimeoutConstant(long timeout)		{ structCOMMTIMOUTS.WriteTotalTimeoutConstant = timeout; };

	int iIndex;

protected:
	HANDLE hThreadRead;
	HANDLE hThreadWrite;

	COMMTIMEOUTS structCOMMTIMOUTS;  	

	struct COMStruct
	{
		int nPortNumber;
		int nBaudRate;
		short nParity;
		short nByteSize;
		short nStopBits;

		COMStruct::COMStruct()
		{
			nPortNumber = 1;
			nBaudRate = CBR_2400;
			nParity = NOPARITY;
			nByteSize = 8;
			nStopBits = ONESTOPBIT;
		}
	} comStruct;
};

