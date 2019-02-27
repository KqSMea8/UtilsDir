#pragma once
#ifdef _DLL
	#define BOOST_THREAD_DYN_LINK
#endif
#include <boost\asio.hpp>
#include <boost\lexical_cast.hpp>
#include <boost\thread.hpp>
#include <boost\signals2\signal.hpp>
#include <boost\bind.hpp>

#define SLEEP_TIME	20

class CCOMPortDevice
{
public:
	CCOMPortDevice(void);
	~CCOMPortDevice(void);

	int SetPortNumber(int nPortNumber);
	int SetBaudRate(int nBaudRate, int* baudRate);
	int SetParity(short nParity, short* parity);
	int SetByteSize(short nByteSize, short* byteSize);
	int SetStopBits(short nStopBits, short* stopBits);
	int SetEndOfMessage(CString endOfMessage);

	int OpenCOMPort();
	int CloseCOMPort();
	int SendCommand(CString command);
	boost::signals2::connection ConnectToReceive(const boost::function<int(CString)> &f);
	
protected:
	void ReadThread();
	
	struct COMStruct
	{
		int nPortNumber;
		int nBaudRate;
		short nParity;
		short nByteSize;
		short nStopBits;
		CString sEndOfMessage;

		COMStruct::COMStruct()
		{
			nPortNumber = 1;
			nBaudRate = CBR_2400;
			nParity = NOPARITY;
			nByteSize = 8;
			nStopBits = ONESTOPBIT;
			sEndOfMessage = L"\r";
		}
	} comStruct;
		
	boost::asio::io_service io;
	boost::asio::serial_port serial;
	boost::system::error_code readErr;
	boost::thread readThread;
	boost::signals2::signal <int(CString)> sigReceive;
	char read_buffer;
	std::size_t readSize;
	CString readMessage;
};

