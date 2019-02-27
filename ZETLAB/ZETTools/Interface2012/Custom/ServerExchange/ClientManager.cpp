#include "stdafx.h"

#include "ClientManager.h"

ClientManager::ClientManager(): _isInitialized(0), _sizeUpdated(false)
{
	InitializeCriticalSection(&_crSection);
	_clientSize.cx = 0;
	_clientSize.cy = 0;
}
ClientManager::~ClientManager()
{
	DeleteCriticalSection(&_crSection);
}

long ClientManager::IsInitialized()
{
	return _isInitialized > 0;
}
void ClientManager::SetInitializeState(long isInitialized)
{
	InterlockedExchange(&_isInitialized, isInitialized);
}

SIZE ClientManager::GetClientSize(bool* sizeUpdated)
{
	SIZE size;
	EnterCriticalSection(&_crSection);
	size = _clientSize;
	if(_sizeUpdated)
	{
		*sizeUpdated = true;
		_sizeUpdated = false;
	}
	else
	{
		*sizeUpdated = false;
	}
	LeaveCriticalSection(&_crSection);
	return size;
}
void ClientManager::SetClientSize(SIZE &size)
{
	EnterCriticalSection(&_crSection);
	_clientSize = size;
	_sizeUpdated= true;
	LeaveCriticalSection(&_crSection);
}