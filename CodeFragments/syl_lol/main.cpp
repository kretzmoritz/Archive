#include "connection/rtmp_client.h"

int main()
{
	RtmpClient rtmpClient("prod.eu.lol.riotgames.com", 2099);
	rtmpClient.connect_s();
	rtmpClient.rtmp_handshake();
	rtmpClient.connect_c();

	std::getchar();

	return 0;
}