#pragma once

#include <DataHelper\ConvertBytesHelper.h>
#include <deque>

const DWORD MAX_BUFFER_SIZE = 1048576;
const DWORD PACKET_DATA_SIZE = sizeof(DWORD);

namespace TrendRequests
{
	enum RequestType
	{
		RequestCancel = 0,
		RequestInfo = 1,
		RequestPrepare = 2,
		RequestCommonData = 3,
		RequestData = 4,
		RequestSourcePrepare = 5,
		RequestSource = 6,
		RequestLog = 7,
		RequestPart = 8,
		RequestReader = 9,
		RequestErrorMessage = 10,
		RequestWarningMessage = 11
	};

	enum CompletedStatus
	{
		Partitially,
		Completed,
		Cancelled
	};
}

struct RecvData
{
	DWORD Size;
	BYTE* Data;

	RecvData(): Size(0), Data(nullptr)	{}
	~RecvData()
	{
		if(Data != NULL)
		{
			delete[] Data;
			Data = nullptr;
		}
	}
	void AllocateData(int size)
	{
		Size = size;
		Data = new BYTE[Size];
		int a = 0;
	}
};

class AsyncSocket
{
public:
	AsyncSocket(unsigned int signature);
	virtual ~AsyncSocket();

	inline long IsWorking() const;
	bool InitializeClientSocket(SOCKET socket);
	bool Connect(const char* ip, unsigned short port);
	virtual void Stop();
	void Break();
	bool Receive();
	// If is it header packet - add signature in begin of message.
	bool Send(BYTE* buffer, int size, bool headerPacket = false);

	RecvData* GetReceiveData();

protected:
	void SetWorkingState(long isWorking);
	bool WaitFor(SOCKET socket, bool waitRead, const TIMEVAL* timeout);

	SOCKET ClientSocket;

private:

	enum ReceiveStep
	{
		Signature,
		Size,
		Data
	};

	struct RecvState
	{
		ReceiveStep Step;
		BYTE *Buffer;
		DWORD Offset;
		DWORD Shift;
		DWORD DataShift;
		unsigned int PacketSignature;
		RecvData* TempRecvData;

		RecvState(): Step(Signature), Offset(0), Shift(0), DataShift(0), PacketSignature(0), TempRecvData(NULL)
		{
			Buffer = new BYTE[MAX_BUFFER_SIZE];
		}
		~RecvState()
		{
			Clear();
			delete[] Buffer;
			Buffer = nullptr;
		}
		void ResetData()
		{
			Step = Signature;
			PacketSignature = 0;
			TempRecvData = new RecvData;
			DataShift = 0;
		}
		void Clear()
		{
			if(TempRecvData != NULL)
			{
				delete TempRecvData;
				TempRecvData = nullptr;
			}
		}
		void CheckTempData()
		{
			if(TempRecvData == NULL)
			{
				Offset = 0;
				Shift = 0;
				ResetData();
			}
		}
	};

	bool checkPacketSignature(unsigned int signature) { return _signature == signature; }

	int receiveData(SOCKET socket);
	int sendData(SOCKET socket, BYTE* buffer, int size);
	
	// Сигнатура данного объекта передачи данных, уникальна для каждого типа передачи, в виде константы.
	// Проверяется в начале каждого пакета.
	unsigned int _signature;

	CRITICAL_SECTION _crSectionRecv;
	long _working;
		
	std::deque<RecvData*> _recvQueue;
	RecvState _recvState;

	void clearReceiveQueue();
	void addReceiveQueue(RecvData* recvData);
};

struct ErrorStruct
{
	wchar_t Caption[MAX_PATH];
	wchar_t Message[MAX_PATH];

	ErrorStruct()
	{
		ZeroMemory(Caption, sizeof(Caption));
		ZeroMemory(Message, sizeof(Message));
	}
};