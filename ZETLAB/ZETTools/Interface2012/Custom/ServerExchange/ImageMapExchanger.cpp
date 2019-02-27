#include "StdAfx.h"
#include <windows.h>
#include "ImageMapExchanger.h"

const long MAX_MAPPED_SIZE_IN_BYTES = 1 * 1024 * 1024;

#pragma region Constructor&Destructor

ImageMapExchanger::ImageMapExchanger(const CString &guidString): BaseName(guidString), IsSuccess(true), ServerEvent(NULL), ClientEvent(NULL), StopExchangeEvent(NULL), Mapping(NULL), MappedData(NULL)
{
}

ImageMapExchanger::~ImageMapExchanger()
{
	releaseResources();
}

ImageMapServer::ImageMapServer(const CString &guidString) : ImageMapExchanger(guidString)
{
	ServerEvent = CreateEvent(NULL, FALSE, FALSE, BaseName + L"_SEVENT");
	ClientEvent = CreateEvent(NULL, FALSE, FALSE, BaseName + L"_CEVENT");
	StopExchangeEvent = CreateEvent(NULL, TRUE, FALSE, BaseName + L"_STOPEVENT");
	Mapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0,  MAX_MAPPED_SIZE_IN_BYTES, BaseName + L"_DATA");
	if(Mapping)
	{
		MappedData = MapViewOfFile(Mapping, FILE_MAP_WRITE, 0, 0, MAX_MAPPED_SIZE_IN_BYTES);
	}
	if(CheckInitialization())
	{
		WaitHandles[0] = ClientEvent;
		WaitHandles[1] = StopExchangeEvent;
	}
}

ImageMapClient::ImageMapClient(const CString &guidString) : ImageMapExchanger(guidString)
{
	ServerEvent = OpenEvent(SYNCHRONIZE | EVENT_MODIFY_STATE, FALSE, BaseName + L"_SEVENT");
	ClientEvent = OpenEvent(SYNCHRONIZE | EVENT_MODIFY_STATE, FALSE, BaseName + L"_CEVENT");
	StopExchangeEvent = OpenEvent(SYNCHRONIZE | EVENT_MODIFY_STATE, FALSE, BaseName + L"_STOPEVENT");
	Mapping = OpenFileMapping(FILE_MAP_WRITE, FALSE, BaseName + L"_DATA");
	if(Mapping)
	{
		MappedData = MapViewOfFile(Mapping, FILE_MAP_WRITE, 0, 0, MAX_MAPPED_SIZE_IN_BYTES);
	}
	if(CheckInitialization())
	{
		WaitHandles[0] = ServerEvent;
		WaitHandles[1] = StopExchangeEvent;
	}
}

#pragma endregion Constructor&Destructor

#pragma region Public base methods

bool ImageMapExchanger::GetNewGuid(GUID *guid)
{
	return CoCreateGuid(guid) == S_OK;
	/*RPC_STATUS status = UuidCreate(guid);
	if(status == RPC_S_OK || status == RPC_S_UUID_LOCAL_ONLY)
	{
		return true;
	}
	return false;*/
}

CString ImageMapExchanger::GetGuidString(const GUID &guid)
{
	//CString resutlStr;
	//wchar_t* szGUID = 0;
	wchar_t szGUID[34];
	
	swprintf_s( szGUID, 33,
		L"%0x%0x%0x%0x%0x%0x%0x%0x%0x%0x%0x",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7] );

	//resutlStr = szGUID;
	/*if(RPC_STATUS status = UuidToString(&guid, (RPC_WSTR*)&szGUID) == RPC_S_OK)
	{
		resutlStr = szGUID;
		status = RpcStringFree((RPC_WSTR*)&szGUID);
	}*/
	return szGUID;
}

bool ImageMapExchanger::IsSuccessInitializated()
{
	return IsSuccess;
}

bool ImageMapExchanger::CheckStoppedExchange()
{
	DWORD waitResult = WaitForSingleObject(StopExchangeEvent, 0);
	return waitResult == WAIT_OBJECT_0;
}

bool ImageMapExchanger::WaitForOtherSide()
{
	DWORD waitResult = WaitForMultipleObjects(2, WaitHandles, FALSE, INFINITE);
	return waitResult == WAIT_OBJECT_0;
}

void ImageMapExchanger::StopExchange()
{
	SetEvent(StopExchangeEvent);
}

PBYTE ImageMapExchanger::GetMappedData()
{
	return ((PBYTE)MappedData) + sizeof(DWORD);
}

DWORD ImageMapExchanger::GetMappedDataSize()
{
	return *((PDWORD)MappedData);
}

void ImageMapExchanger::SetMappedDataSize(DWORD size)
{
	*((PDWORD)MappedData) = size;
}

void ImageMapServer::GiveSignalOtherSide()
{
	SetEvent(ServerEvent);
}
void ImageMapClient::GiveSignalOtherSide()
{
	SetEvent(ClientEvent);
}

#pragma endregion Public base methods

#pragma region Protected methods

bool ImageMapExchanger::CheckInitialization()
{
	if(ServerEvent == NULL || ClientEvent == NULL || StopExchangeEvent == NULL || Mapping == NULL || MappedData == NULL)
	{
		releaseResources();
		IsSuccess = false;
	}
	return IsSuccess;
}

#pragma endregion Protected methods

#pragma region Private base methods

void ImageMapExchanger::releaseResources()
{
	StopExchange();
	if(ServerEvent)
	{
		CloseHandle(ServerEvent);
	}
	if(ClientEvent)
	{
		CloseHandle(ClientEvent);
	}
	if(StopExchangeEvent)
	{
		CloseHandle(StopExchangeEvent);
	}
	if(MappedData)
	{
		UnmapViewOfFile(MappedData);
	}
	if(Mapping)
	{
		CloseHandle(Mapping);
	}
}

#pragma endregion Private base methods