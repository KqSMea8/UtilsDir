//#include "stdafx.h"

#include "C:\ZETTools\Interface2012\Custom\IScreenBuffering.h"
#include "ClientManager.h"
#include "ImageExchangeHub.h"

template<typename T, DWORD (T::*Method)()> // ���, ����������� ������� � ������ ����� ������, � �� ���������� �������.
DWORD WINAPI ThreadExchangeClassMethod(void* pClass)
{
	return (static_cast<T*>(pClass)->*Method)();
}

//////////////////////////////////////////////////////////////////////////
/// �����, ����������� ����� ����������� � ������������ �����������.
//////////////////////////////////////////////////////////////////////////

class ServerExchanger
{
public:
	ServerExchanger();
	~ServerExchanger();

	BOOL IsInitialized();

	/// �������������.
	BOOL Initialize(HWND parentWindow, IScreenBuffering *main, const CString &clientGuid);
	void Disconnect();

	bool Exchange();

private:
	HANDLE _hExchangeThread;

	HWND _parentWindow;
	IScreenBuffering* _main;
	CString _clientGuid;
	ImageMapClient *_mappingClient;
	ClientManager _manager;

	//bool connect();
	void resolveClientCommands();
	void resolveMouseCommands(MouseCommandStruct& mouseStruct);
	bool sendImage(DWORD size, BYTE* bytes, POINT point, bool partUpdate);
	//void messageToMainWindow(ClientConnectionEvent clientEvent);

	//DWORD receiveCommandsFromServer();
	DWORD sendScreenToServer();
};