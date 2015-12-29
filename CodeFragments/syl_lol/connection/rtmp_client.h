#ifndef RTMP_CLIENT_H
#define RTMP_CLIENT_H

#include <WinSock2.h>
#include <string>

class SSL_SOCKET;

class RtmpClient
{
public:
	RtmpClient(std::string _server, int _port);
	~RtmpClient();

	bool connect_s();
	bool rtmp_handshake();
	void connect_c();

private:
	SOCKET s;
	SSL_SOCKET* sx;

	std::string m_server;
	int m_port;
};

#endif