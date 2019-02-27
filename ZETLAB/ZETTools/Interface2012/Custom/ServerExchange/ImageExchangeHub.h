#pragma once

#include "C:\ZETTools\DataHelper\ConvertBytesHelper.h"
#include "ImageMapExchanger.h"
#include <deque>

const DWORD GUID_SIZE_IN_BYTES = sizeof(unsigned long) + sizeof(unsigned short) * 2 + sizeof(unsigned char) * 8;

enum ClientHub
{
	MainClientHub = 0,
	ChildClientHub
};

enum ServerCommands
{
	RequestImage = 1,
	MouseCommand,
};

enum HubTypes
{
	MainHub = 0,
	ChildHub
};

enum ClientCommands
{
	ReceiveImage = 0,
	ReceiveImagePart,
	RequestSize,
	CloseHub
};

enum MouseCommands
{
	MouseLeftDown = 0,
	MouseLeftUp,
	MouseRightDown,
	MouseRightUp,
	MouseWheelIn,
	MouseWheelOut,
	MouseMove
};

struct MouseCommandStruct
{
	MouseCommands Command;
	POINTF Point;
	MouseCommandStruct() {}
	MouseCommandStruct(MouseCommands command, POINTF point): Command(command), Point(point)	{}
};

struct CommandStruct
{
	BYTE CommandCode;
	DWORD Size;
	CByteArray Data;
	CommandStruct() : CommandCode(0), Size(0) {}
	CommandStruct(BYTE commandCode, DWORD size, PBYTE data) : CommandCode(commandCode), Size(size)
	{
		if(CommandCode == 0 && Size == 0)
		{
			return;
		}
		Data.SetSize(Size);
		memcpy(Data.GetData(), data, Size);
	}
	//DWORD GetCommonSize() { return sizeof(BYTE) + Size; }
	DWORD WriteData(PBYTE bytes)
	{
		DWORD shift = 0;
		if(bytes && CommandCode != 0 && Size > 0)
		{
			shift = sizeof(BYTE);
			bytes[0] = CommandCode;
			shift += type2bytes(Size, bytes + shift);
			memcpy(bytes + shift, Data.GetData(), Size);
			shift += Size;
			*(bytes + shift) = 0;
		}
		return shift;
	}
};

class ImageExchangeHub
{
	const GUID _hubGuid;
	ImageMapServer _mappingServer;
	
	CRITICAL_SECTION _crSection;
	std::deque<CommandStruct*> _commandQueue;

public:
	ImageExchangeHub(const GUID &guid);
	~ImageExchangeHub();

	bool IsInitializated();

	void StopExchange() { _mappingServer.StopExchange(); }

	const GUID& Guid() { return _hubGuid; }

	void AddCommandData(BYTE commandCode, DWORD size, PBYTE data);
	void SendCommands();

	bool WaitForClient();
	bool CheckStoppedClient();

	DWORD GetMappedDataSize();
	PBYTE GetMappedData();

	static DWORD GetGuidFromBytes(GUID *guid, PBYTE bytes);
	static DWORD GetBytesFromGUID(PBYTE bytes, const GUID &guid);

	static DWORD GetSIZEFromBytes(SIZE *size, PBYTE bytes);
	static DWORD GetBytesFromSIZE(PBYTE bytes, const SIZE &size);

	static DWORD GetPOINTFromBytes(POINT *point, PBYTE bytes);
	static DWORD GetBytesFromPOINT(PBYTE bytes, const POINT &point);

	static DWORD GetMouseCommandFromBytes(MouseCommandStruct *mouseStruct, PBYTE bytes);
	static DWORD GetBytesFromMouseCommand(PBYTE bytes, const MouseCommandStruct &mouseStruct);
};

