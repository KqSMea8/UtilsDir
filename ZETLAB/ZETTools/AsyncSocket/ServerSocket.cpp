#include "stdafx.h"
#include "ServerSocket.h"

ServerSocket::ServerSocket(unsigned int signature) : AsyncSocket(signature), _listenSocket(NULL), _wasWorking(false)
{
}

ServerSocket::~ServerSocket()
{
}

bool ServerSocket::Start(u_short port)
{
	Stop();

	bool result = false;
	SOCKET socket = initializeServerSocket();
	if(socket != NULL)
	{
		if(bindSocket(socket, port))
		{
			_listenSocket = socket;
			_wasWorking = false;
			result = true;
		}
		else
		{
			closesocket(socket);
		}
	}

	return result;
}

void ServerSocket::Stop()
{
	AsyncSocket::Stop();
	Break();
	if(_listenSocket != NULL)
	{
		closesocket(_listenSocket);
		_listenSocket = NULL;
	}
}

bool ServerSocket::Listen(std::wstring& sClientIp)
{
	sClientIp.clear();
	if(_listenSocket != NULL)
	{
		if(listen(_listenSocket, 1) != SOCKET_ERROR)
		{
			SetWorkingState(1);

			TIMEVAL timeout;
			timeout.tv_sec = 1;
			timeout.tv_usec = 0;
			if(WaitFor(_listenSocket, true, &timeout))
			{
				struct sockaddr_in clientaddr = { 0 };
				int iSize = sizeof(clientaddr);
				if(InitializeClientSocket(accept(_listenSocket, (sockaddr *)&clientaddr, &iSize)))
				{
					sClientIp = std::to_wstring(clientaddr.sin_addr.S_un.S_un_b.s_b1) + 
								L"." + 
								std::to_wstring(clientaddr.sin_addr.S_un.S_un_b.s_b2) +
								L"." +
								std::to_wstring(clientaddr.sin_addr.S_un.S_un_b.s_b3) +
								L"." +
								std::to_wstring(clientaddr.sin_addr.S_un.S_un_b.s_b4);
								
					TRACE("ServerSocket::Listen() Accept successfull\n");
					closesocket(_listenSocket);
					_listenSocket = NULL;
					_wasWorking = true;
					return true;
				}
			}
		}
		int error = WSAGetLastError();
		TRACE("ServerSocket::Listen() WSAGetLastError %d\n", error);
	}

	return false;
}

const SOCKET ServerSocket::GetClientSocket() const
{
	return ClientSocket;
}

SOCKET ServerSocket::initializeServerSocket()
{
	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
	{
		int error = WSAGetLastError();
		return NULL;
	}

	return listenSocket;
}

bool ServerSocket::bindSocket(SOCKET socket, u_short port)
{
	sockaddr_in local_addr;
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(port); // не забываем о сетевом пор€дке!!!

	// вызываем bind дл€ св€зывани€
	if (bind(socket, (sockaddr *)&local_addr, sizeof(local_addr)) == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		return false;
	}
	return true;
}

const bool ServerSocket::IsWasWorking() const
{
	return _wasWorking;
}
