#pragma once

class ClientManager
{
private:
	bool _sizeUpdated;
	SIZE _clientSize;
	long _isInitialized;

	CRITICAL_SECTION _crSection;

public:
	ClientManager();
	~ClientManager();
	
	long IsInitialized();
	void SetInitializeState(long isInitialized);

	SIZE GetClientSize(bool* sizeChanged);
	void SetClientSize(SIZE& size);
};