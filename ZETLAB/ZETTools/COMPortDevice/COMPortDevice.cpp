#include "stdafx.h"
#include "COMPortDevice.h"

CCOMPortDevice::CCOMPortDevice(void)
: serial(io)
{
}

CCOMPortDevice::~CCOMPortDevice(void)
{
}

int CCOMPortDevice::OpenCOMPort()
{
	using namespace boost::asio;

	boost::system::error_code err;
	serial.open("\\\\.\\COM" + boost::lexical_cast<std::string>(comStruct.nPortNumber), err);
	if (err == 0)
	{
		if (serial.is_open())
		{
			serial.set_option(serial_port::baud_rate(comStruct.nBaudRate));
			serial.set_option(serial_port::parity(serial_port::parity::type(comStruct.nParity)));
			serial.set_option(serial_port::character_size(comStruct.nByteSize));
			serial.set_option(serial_port::stop_bits(serial_port::stop_bits::type(comStruct.nStopBits)));

			readThread = boost::thread(boost::bind(&CCOMPortDevice::ReadThread, this));
		}
	}
	else
	{
		return 1;
	}
	
	return 0;
}

int CCOMPortDevice::CloseCOMPort()
{
	if(serial.is_open())
	{
		serial.cancel();
		TRACE(L"Serial cancel\n");
		serial.close();
		TRACE(L"Serial close\n");
		readThread.interrupt();
		TRACE(L"ReadThread interrupted\n");
		readThread.join();
		TRACE(L"ReadThread joined\n");
	}

	return 0;
}

int CCOMPortDevice::SendCommand(CString command)
{
	boost::system::error_code err;
	serial.write_some(boost::asio::buffer(std::string(CStringA(command)), command.GetLength()), err); 
	return err.value();
}

void CCOMPortDevice::ReadThread()
{
	while (serial.is_open())
	{
		readSize = boost::asio::read(serial, boost::asio::buffer(&read_buffer, 1), readErr);
		if (readErr == 0 && readSize > 0)
		{
			readMessage.Append(CString(read_buffer));
			if (readMessage.Find(comStruct.sEndOfMessage) != -1)
			{
				readMessage.Replace(comStruct.sEndOfMessage, L"");
				if (readMessage.GetLength() > 0)
					sigReceive(readMessage);
				readMessage.Empty();
			}
		}
	}
}

int CCOMPortDevice::SetPortNumber(int nPortNumber)
{
	if (serial.is_open())
	{
		CloseCOMPort();
		comStruct.nPortNumber = nPortNumber;
		return OpenCOMPort();
	}
	else
	{
		comStruct.nPortNumber = nPortNumber;
	}

	return 0;
}

int CCOMPortDevice::SetBaudRate(int nBaudRate, int* baudRate)
{
	if (serial.is_open())
	{
		boost::system::error_code err;
		boost::asio::serial_port::baud_rate current;
		serial.get_option(current);
		serial.set_option(boost::asio::serial_port::baud_rate(nBaudRate), err);
		if (err == 0)
		{
			serial.get_option(current);
			comStruct.nBaudRate = current.value();
			*baudRate = comStruct.nBaudRate;
		}
		else
		{
			comStruct.nBaudRate = current.value();
			*baudRate = comStruct.nBaudRate;
		}
	}
	else
	{
		comStruct.nBaudRate = nBaudRate;
		*baudRate = comStruct.nBaudRate;
	}

	return 0;
}

int CCOMPortDevice::SetParity(short nParity, short* parity)
{
	if (serial.is_open())
	{
		boost::system::error_code err;
		boost::asio::serial_port::parity current;
		serial.get_option(current);
		serial.set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::type(nParity)), err);
		if (err == 0)
		{
			serial.get_option(current);
			comStruct.nParity = current.value();
			*parity = comStruct.nParity;
		}
		else
		{
			comStruct.nParity = current.value();
			*parity = comStruct.nParity;
		}
	}
	else
	{
		comStruct.nParity = nParity;
		*parity = comStruct.nParity;
	}

	return 0;
}

int CCOMPortDevice::SetByteSize(short nByteSize, short* byteSize)
{
	if (serial.is_open())
	{
		boost::system::error_code err;
		boost::asio::serial_port::character_size current;
		serial.get_option(current);
		serial.set_option(boost::asio::serial_port::character_size(nByteSize), err);
		if (err == 0)
		{
			serial.get_option(current);
			comStruct.nByteSize = current.value();
			*byteSize = comStruct.nByteSize;
		}
		else
		{
			comStruct.nByteSize = current.value();
			*byteSize = comStruct.nByteSize;
		}
	}
	else
	{
		comStruct.nByteSize = nByteSize;
		*byteSize = comStruct.nByteSize;
	}

	return 0;
}

int CCOMPortDevice::SetStopBits(short nStopBits, short* stopBits)
{
	if (serial.is_open())
	{
		boost::system::error_code err;
		boost::asio::serial_port::stop_bits current;
		serial.get_option(current);
		serial.set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::type(nStopBits)), err);
		if (err == 0)
		{
			serial.get_option(current);
			comStruct.nStopBits = current.value();
			*stopBits = comStruct.nStopBits;
		}
		else
		{
			comStruct.nStopBits = current.value();
			*stopBits = comStruct.nStopBits;
		}
	}
	else
	{
		comStruct.nStopBits = nStopBits;
		*stopBits = comStruct.nStopBits;
	}

	return 0;
}

int CCOMPortDevice::SetEndOfMessage(CString endOfMessage)
{
	comStruct.sEndOfMessage = endOfMessage;
	return 0;
}

boost::signals2::connection CCOMPortDevice::ConnectToReceive(const boost::function<int(CString)> &f)
{
	return sigReceive.connect(f);
}