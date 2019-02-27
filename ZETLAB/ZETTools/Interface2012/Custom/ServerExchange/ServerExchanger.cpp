#include "stdafx.h"

#include "ServerExchanger.h"
#include "ImageWorker.h"

ServerExchanger::ServerExchanger(): _hExchangeThread(NULL)
{
	_mappingClient = NULL;
}

ServerExchanger::~ServerExchanger()
{
	Disconnect();
	if(_mappingClient != NULL)
	{
		delete _mappingClient;
	}
}

BOOL ServerExchanger::IsInitialized()
{
	return _manager.IsInitialized() > 0 && _mappingClient && _mappingClient->IsSuccessInitializated();
}

/// Инициализация.
BOOL ServerExchanger::Initialize(HWND parentWindow, IScreenBuffering *main, const CString &clientGuid)
{
	if(!IsInitialized())
	{
		_manager.SetInitializeState(1);
		_parentWindow = parentWindow;
		_main = main;
		_clientGuid = clientGuid;
		if(!_clientGuid.IsEmpty())
		{
			if(_mappingClient != NULL)
			{
				delete _mappingClient;
			}
			_mappingClient = new ImageMapClient(_clientGuid);
		}
	}
	return IsInitialized();
}

void ServerExchanger::Disconnect()
{
	if(IsInitialized())
	{
		_manager.SetInitializeState(0);
		_mappingClient->StopExchange();
		if(_hExchangeThread != NULL)
		{
			WaitForSingleObject(_hExchangeThread, INFINITE);
			CloseHandle(_hExchangeThread);
			_hExchangeThread = NULL;
		}
	}
}

///// Ожидание.
bool ServerExchanger::Exchange()
{
	if(IsInitialized())
	{
		_hExchangeThread = CreateThread(NULL, 0, ThreadExchangeClassMethod<ServerExchanger, &ServerExchanger::sendScreenToServer>, this, 0, NULL);
	}
	return true;
}

bool ServerExchanger::sendImage(DWORD size, BYTE* bytes, POINT point, bool partUpdate)
{
	bool result = true;

	// Определяем дополнительный размер в случае отправки только различия в кадрах.
	DWORD shift = sizeof(BYTE);//, partShift = partUpdate ? sizeof(long) * 2 : 0, headerSize = sizeof(BYTE) + partShift;
	
	PBYTE data = _mappingClient->GetMappedData();

	data[0] = static_cast<byte>(partUpdate ? ReceiveImagePart : ReceiveImage);

	//if(partUpdate) // Прилепляем положение области различия к заголовку, чтобы не трогать уже сформированный массив картинки.
	//{
		shift += ImageExchangeHub::GetBytesFromPOINT(data + shift, point);
	//}

	shift += type2bytes(size, data + shift);

	_mappingClient->SetMappedDataSize(shift + size);
	
	memcpy(data + shift, bytes, size + shift);

	return result;
}

void ServerExchanger::resolveClientCommands()
{
	PBYTE data = _mappingClient->GetMappedData(), commandData;
	DWORD allSize =_mappingClient->GetMappedDataSize(), size, curShift = 0;
	BYTE commandCode;

	if(allSize <= 0)
	{
		return;
	}

	while(curShift < allSize)
	{
		commandCode = *(data + curShift++);
		if(commandCode == 0)
		{
			break;
		}

		curShift += bytes2type(data + curShift, &size);

		commandData = data + curShift;
		curShift += size;

		switch(static_cast<ServerCommands>(commandCode))
		{
		case RequestImage:
			{
				if(size == sizeof(long) * 2)
				{
					SIZE size;
					ImageExchangeHub::GetSIZEFromBytes(&size, commandData);
					_manager.SetClientSize(size);
				}
				break;
			}
		case MouseCommand:
			{
				// Заглушка.
				//break;

				if(size == sizeof(BYTE) + sizeof(float) * 2)
				{
					MouseCommandStruct mouseStruct;
					ImageExchangeHub::GetMouseCommandFromBytes(&mouseStruct, commandData);
					resolveMouseCommands(mouseStruct);
				}
				break;
			}
		default:
			{
				break;
			}
		}
	}
}

void ServerExchanger::resolveMouseCommands(MouseCommandStruct &mouseStruct)
{
	//CRect rcClient;
	//GetWindowRect(0, &rcClient);
	//GetWindowRect(_parentWindow, &rcClient);
	//int scrWidth = GetSystemMetrics (SM_CXSCREEN), scrHeight = GetSystemMetrics (SM_CYSCREEN);
	//int scrWidth = rcClient.Width(), scrHeight = rcClient.Height();
	SIZE &mainSize = _main->GetBufferSize();

	//int x = rcClient.left + (scrWidth - 1) * mouseStruct.Point.x, y = rcClient.top + (scrHeight - 1) * mouseStruct.Point.y;
	int x = (mainSize.cx - 1) * mouseStruct.Point.x, y = (mainSize.cy - 1) * mouseStruct.Point.y;
	
	UINT message = 0;

	//SetCursorPos(x, y); 
	//int mouseButtons = 0, delta = 0;
	switch(mouseStruct.Command)
	{
	case MouseMove:
		{
			message = WM_MOUSEMOVE;
			break;
		}
	case MouseLeftDown:
		{
			//mouseButtons |= MOUSEEVENTF_LEFTDOWN;
			message = WM_LBUTTONDOWN;
			break;
		}
	case MouseLeftUp:
		{
			//mouseButtons |= MOUSEEVENTF_LEFTUP;
			message = WM_LBUTTONUP;
			break;
		}
	case MouseRightDown:
		{
			//mouseButtons |= MOUSEEVENTF_RIGHTDOWN;
			message = WM_RBUTTONDOWN;
			break;
		}
	case MouseRightUp:
		{
			//mouseButtons |= MOUSEEVENTF_RIGHTUP;
			message = WM_RBUTTONUP;
			break;
		}
	case MouseWheelIn:
		{
			message = WM_MOUSEWHEEL;
			//mouseButtons |= MOUSEEVENTF_WHEEL;
			//delta = 120;
			break;
		}
	case MouseWheelOut:
		{
			message = WM_MOUSEWHEEL;
			//mouseButtons |= MOUSEEVENTF_WHEEL;
			//delta = -120;
			break;
		}
	}
	if(message != 0)
	{
		//mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, x, y, 0, 0);
		//mouse_event(MOUSEEVENTF_ABSOLUTE | mouseButtons, x, y, delta, 0);
		LPARAM lParam =  MAKELPARAM(x, y);
		
		::PostMessage(_parentWindow, message, 0, lParam);
	}
}

//void ClientConnection::messageToMainWindow(ClientConnectionEvent clientEvent)
//{
//	::PostMessage(_parentWindow, WM_USER+100, clientEvent, 0);
//}
//
//DWORD ClientConnection::connectAndReceiveFromClient()
//{
//	messageToMainWindow(ServerListen);
//	if(_asyncSocket.Listen())
//	{
//		messageToMainWindow(ClientConnected);
//		HANDLE hSendThread = CreateThread(NULL, 0, ThreadClassMethod<ClientConnection, &ClientConnection::sendScreenToClient>, this, 0, NULL);
//
//		while(_asyncSocket.IsWorking())
//		{
//			if(_asyncSocket.Receive())
//			{
//				DWORD a = GetTickCount64();
//				resolveClientCommands();
//			}
//		}
//
//		WaitForSingleObject(hSendThread, INFINITE);
//		CloseHandle(hSendThread);
//	}
//
//	_asyncSocket.Stop();
//
//	if(IsInitialized() && connect())
//	{
//		Listen();
//	}
//	else
//	{
//		messageToMainWindow(ServerStopped);
//	}
//
//	return 0;
//}

DWORD ServerExchanger::sendScreenToServer()
{
	ImageWorker imageWorker(true);
	if(imageWorker.IsInitialized() && (_mappingClient && _mappingClient->IsSuccessInitializated()))
	{
		/*_mappingClient->SetMappedDataSize(sizeof(BYTE));
		PBYTE data = _mappingClient->GetMappedData();
		data[0] = static_cast<byte>(RequestSize);
		_mappingClient->GiveSignalOtherSide();*/

		SIZE clientSize;
		bool firstStart = true, sizeUpdated;
		CByteArray buffer;
		ScreenShotInfo info;
		ImageBuffer* imageBuffer = imageWorker.GetImageBuffer();

		clientSize.cx = 0;
		clientSize.cy = 0;

		while(_manager.IsInitialized() && _mappingClient->WaitForOtherSide())
		{
			if(firstStart)
			{
				firstStart = false;

				_mappingClient->SetMappedDataSize(sizeof(BYTE));
				PBYTE data = _mappingClient->GetMappedData();
				data[0] = static_cast<byte>(RequestSize);
				
				//////////////////////////////////////////////////////////////////////////
				_mappingClient->GiveSignalOtherSide();
				//////////////////////////////////////////////////////////////////////////

				continue;
			}

			resolveClientCommands();

			clientSize = _manager.GetClientSize(&sizeUpdated);
			imageBuffer->Size = _main->GetBufferSize();
			if(clientSize.cx > 0 && clientSize.cy > 0 && imageBuffer->Size.cx > 0 && imageBuffer->Size.cy > 0 && _main->GetImageBuffer(&imageBuffer->Buffer, imageBuffer->Size, sizeUpdated))
			{
				if(imageWorker.GetDiffBuffer(&buffer, &info, sizeUpdated))
				{
					sendImage(buffer.GetSize(), buffer.GetData(), info.UpdateRectangle.TopLeft(), info.PartStatus == Partitially);
					
					//////////////////////////////////////////////////////////////////////////
					_mappingClient->GiveSignalOtherSide();
					//////////////////////////////////////////////////////////////////////////

					Sleep(1);
				}
				else
				{
					if(info.PartStatus == NoChanges)
					{
						//////////////////////////////////////////////////////////////////////////
						_mappingClient->SetMappedDataSize(0);
						_mappingClient->GiveSignalOtherSide();
						//////////////////////////////////////////////////////////////////////////

						Sleep(40);
					}
					else
					{
						break;
					}
				}
			}
			else
			{
				//////////////////////////////////////////////////////////////////////////
				_mappingClient->SetMappedDataSize(0);
				_mappingClient->GiveSignalOtherSide();
				//////////////////////////////////////////////////////////////////////////

				Sleep(100);
			}
		}
		buffer.RemoveAll();
	}

	return 0;
}