#include <iostream>

#include "rtmp_client.h"
#include "../libs/ssl_socket/ssl.h"

RtmpClient::RtmpClient(std::string _server, int _port)
	: m_server(_server), m_port(_port), sx(NULL)
{

}

RtmpClient::~RtmpClient()
{
	connect_c();
}

bool RtmpClient::connect_s()
{
	if (sx)
	{
		std::cout << "Connection already open!" << std::endl;
		return false;
	}

	WSADATA w;
	if (int result = WSAStartup(MAKEWORD(2, 2), &w) != 0)
	{
		std::cout << "WinSock2 couldn't be started! Error #" << result << std::endl;
		return false;
	}

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
	{
		std::cout << "Socket couldn't be created!" << std::endl;
		return false;
	}

	hostent* host = gethostbyname(m_server.c_str());
	if (!host)
	{
		std::cout << "Host couldn't be retrieved!" << std::endl;
		return false;
	}

	if (host->h_addrtype != AF_INET)
	{
		std::cout << "Invalid adress type!" << std::endl;
		return false;
	}
	
	if (host->h_length != 4)
	{
		std::cout << "Invalid IP type!" << std::endl;
		return false;
	}

	sockaddr_in addr;
    addr.sin_family = AF_INET;
	addr.sin_port = htons(m_port);

	char** p = host->h_addr_list;
	int result;
	do
	{
		if (!(*p))
		{
			std::cout << "Connection failed!" << std::endl;
			return false;
		}

		memcpy(&addr.sin_addr.S_un.S_addr, *p, host->h_length);

		std::cout << "connect_s: " << inet_ntoa(addr.sin_addr) << ":" << m_port << std::endl;
		result = connect(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));

		++p;
	}
	while (result == SOCKET_ERROR);

	std::cout << "Connection successful!" << std::endl;

	sx = new SSL_SOCKET(s, 0, 0);
	if (sx->ClientInit() != 0)
	{
		std::cout << "SSL failed to initialize!" << std::endl;
		return false;
	}

	std::cout << "SSL initialized!" << std::endl;

	return true;
}

bool RtmpClient::rtmp_handshake()
{
	if (!sx)
	{
		std::cout << "Socket not found. Call connect_s before handshaking!" << std::endl;
		return false;
	}

	char c0_c1[1537];
	ZeroMemory(c0_c1, 1537);

	//C0
	c0_c1[0] = 0x03; //Current RTMP protocol version
	std::cout << "C0: " << c0_c1[0] << std::endl;

	//C1
	std::cout << "C1: " << c0_c1[1] << std::endl; //1536 remaining bytes (0-3 epoch timestamp, rest random); simplification: set all bytes to 0

	//Flush C0_C1
	sx->s_ssend(c0_c1, 1537);

	//S0
	char s0[1];
	ZeroMemory(s0, 1);
	sx->s_rrecv(s0, 1);

	std::cout << "S0: " << s0[0] << std::endl;

	//S1
	char s1[1536];
	ZeroMemory(s1, 1536);
	sx->s_rrecv(s1, 1536);

	std::cout << "S1: " << s1[0] << std::endl;

	//C2
	sx->s_ssend(s1, 1536); //Echo S1

	std::cout << "C2: " << s1[0] << std::endl;

	//S2
	char s2[1536];
	ZeroMemory(s2, 1536);
	sx->s_rrecv(s2, 1536); //Echo C1

	std::cout << "S2: " << s2[0] << std::endl;

	//Validate handshake
	for (int i = 0; i < 1536; ++i)
	{
		if (c0_c1[i + 1] != s2[i])
		{
			return false;
		}
	}

	std::cout << "RTMP handshake validated!" << std::endl;

	return true;
}

void RtmpClient::connect_c()
{
	if (!sx)
	{
		return;
	}

	sx->ClientOff();
	delete sx;
	sx = NULL;

	closesocket(s);
	WSACleanup();

	std::cout << "Connection closed!" << std::endl;
}