#include "StdAfx.h"
#include "ImageExchangeHub.h"

ImageExchangeHub::ImageExchangeHub(const GUID &guid) : _hubGuid(guid), _mappingServer(ImageMapExchanger::GetGuidString(guid))
{
	InitializeCriticalSection(&_crSection);
}

ImageExchangeHub::~ImageExchangeHub()
{
	EnterCriticalSection(&_crSection);

	CommandStruct* comStruct;

	while(!_commandQueue.empty())
	{
		comStruct = _commandQueue.front();
		_commandQueue.pop_front();
		delete comStruct;
	}

	LeaveCriticalSection(&_crSection);

	_mappingServer.StopExchange();

	DeleteCriticalSection(&_crSection);
}


bool ImageExchangeHub::IsInitializated()
{
	return _mappingServer.IsSuccessInitializated();
}

void ImageExchangeHub::AddCommandData(BYTE commandCode, DWORD size, PBYTE data)
{
	EnterCriticalSection(&_crSection);

	_commandQueue.push_back(new CommandStruct(commandCode, size, data));

	LeaveCriticalSection(&_crSection);
}
void ImageExchangeHub::SendCommands()
{
	EnterCriticalSection(&_crSection);

	PBYTE data = _mappingServer.GetMappedData();

	if(_commandQueue.empty())
	{
		_mappingServer.SetMappedDataSize(0);
	}
	else
	{
		CommandStruct* comStruct;
		DWORD allSize = 0, result;
		while(!_commandQueue.empty())
		{
			comStruct = _commandQueue.front();
			_commandQueue.pop_front();

			result = comStruct->WriteData(data + allSize);
			delete comStruct;
			if(result > 0)
			{
				allSize += result;
			}
		}
		_mappingServer.SetMappedDataSize(allSize);
	}

	_mappingServer.GiveSignalOtherSide();

	LeaveCriticalSection(&_crSection);
}

bool ImageExchangeHub::WaitForClient()
{
	return _mappingServer.WaitForOtherSide();
}

bool ImageExchangeHub::CheckStoppedClient()
{
	return _mappingServer.CheckStoppedExchange();
}

DWORD ImageExchangeHub::GetMappedDataSize()
{
	return _mappingServer.GetMappedDataSize();
}
PBYTE ImageExchangeHub::GetMappedData()
{
	return _mappingServer.GetMappedData();
}

DWORD ImageExchangeHub::GetGuidFromBytes(GUID *guid, PBYTE bytes)
{
	int shift = bytes2type(bytes, &guid->Data1);
	shift += bytes2type(bytes + shift, &guid->Data2);
	shift += bytes2type(bytes + shift, &guid->Data3);
	for(int i = 0; i < 8; i++)
	{
		shift += bytes2type(bytes + shift, &guid->Data4[i]);
	}
	return shift;
}
DWORD ImageExchangeHub::GetBytesFromGUID(PBYTE bytes, const GUID &guid)
{
	int shift = type2bytes(guid.Data1, bytes);
	shift += type2bytes(guid.Data2, bytes + shift);
	shift += type2bytes(guid.Data3, bytes + shift);
	for(int i = 0; i < 8; i++)
	{
		shift += type2bytes(guid.Data4[i], bytes + shift);
	}
	return shift;
}

DWORD ImageExchangeHub::GetSIZEFromBytes(SIZE *size, PBYTE bytes)
{
	int shift = bytes2type(bytes, &size->cx);
	shift += bytes2type(bytes + shift, &size->cy);
	return shift;
}
DWORD ImageExchangeHub::GetBytesFromSIZE(PBYTE bytes, const SIZE &size)
{
	int shift = type2bytes(size.cx, bytes);
	shift += type2bytes(size.cy, bytes + shift);
	return shift;
}

DWORD ImageExchangeHub::GetPOINTFromBytes(POINT *point, PBYTE bytes)
{
	short x, y;
	int shift = bytes2type(bytes, &x);
	shift += bytes2type(bytes + shift, &y);
	point->x = x;
	point->y = y;
	return shift;
}
DWORD ImageExchangeHub::GetBytesFromPOINT(PBYTE bytes, const POINT &point)
{
	int shift = type2bytes((short)point.x, bytes);
	shift += type2bytes((short)point.y, bytes + shift);
	return shift;
}

DWORD ImageExchangeHub::GetMouseCommandFromBytes(MouseCommandStruct *mouseStruct, PBYTE bytes)
{
	BYTE command;
	int shift = bytes2type(bytes, &command);
	mouseStruct->Command = static_cast<MouseCommands>(command);
	shift += bytes2type(bytes + shift, &mouseStruct->Point.x);
	shift += bytes2type(bytes + shift, &mouseStruct->Point.y);
	return shift;
}
DWORD ImageExchangeHub::GetBytesFromMouseCommand(PBYTE bytes, const MouseCommandStruct &mouseStruct)
{
	BYTE command = static_cast<BYTE>(mouseStruct.Command);
	int shift = type2bytes(command, bytes);
	shift += type2bytes(mouseStruct.Point.x, bytes + shift);
	shift += type2bytes(mouseStruct.Point.y, bytes + shift);
	return shift;
}