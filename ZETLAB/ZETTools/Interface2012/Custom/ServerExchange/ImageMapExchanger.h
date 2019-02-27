#pragma once
class ImageMapExchanger
{
	void releaseResources();

protected:
	CString BaseName;
	bool IsSuccess;
	HANDLE ServerEvent, ClientEvent, StopExchangeEvent;
	HANDLE Mapping;
	PVOID MappedData;
	HANDLE WaitHandles[2];

	ImageMapExchanger(const CString &guidString);
	
	bool CheckInitialization();
	
public:
	virtual ~ImageMapExchanger() = 0;

	bool IsSuccessInitializated();
	bool CheckStoppedExchange();

	virtual bool WaitForOtherSide();
	virtual void GiveSignalOtherSide() = 0;

	PBYTE GetMappedData();
	DWORD GetMappedDataSize();
	void SetMappedDataSize(DWORD size);

	void StopExchange();

	static bool GetNewGuid(GUID *guid);
	static CString GetGuidString(const GUID &guid);
};

class ImageMapServer : public ImageMapExchanger
{
public:
	ImageMapServer(const CString &guidString);

	 void GiveSignalOtherSide();
};

class ImageMapClient : public ImageMapExchanger
{
public:
	ImageMapClient(const CString &guidString);

	void GiveSignalOtherSide();
};
