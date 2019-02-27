#pragma once

#include "AsyncSocket.h"
#include <string>

class ServerSocket : public AsyncSocket
{
public:
	ServerSocket(unsigned int signature);
	~ServerSocket();

	bool Start(u_short port);
	virtual void Stop() override;
	bool Listen(std::wstring& sClientIp);

	const SOCKET GetClientSocket() const;

	const bool IsWasWorking() const;
private:
	SOCKET _listenSocket;
	bool _wasWorking;

	SOCKET initializeServerSocket();
	bool bindSocket(SOCKET socket, u_short port);
};