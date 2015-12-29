///////////////////////////////////////
//  (             (                
//  )\ )     (    )\ )             
// (()/((    )\  (()/(    (   )    
//  /(_))\ )((_)  /(_))  ))\ /((   
// (_))(()/( _   (_))_  /((_|_))\  
// / __|)(_)) |   |   \(_)) _)((_) 
// \__ \ || | |   | |) / -_)\ V /  
// |___/\_, |_|   |___/\___| \_/   
//      |__/                       
//
// 2014 Moritz Kretz
///////////////////////////////////////

#ifndef SYLP2P_PEER_H
#define SYLP2P_PEER_H

#include <map>

#include "../../syl_net/protocol/tcp_client.h"
#include "../../syl_net/protocol/tcp_server.h"
#include "../../syl_net/protocol/udp.h"
#include "../../syl_util/uuid/uuid.h"
#include "message_handler.h"
#include "../com/networkable.h"

namespace SylP2P
{
	class Peer;
};

namespace SylNet
{
	void serialize(
		SylP2P::Peer& _object, SerializationContext const& _ctx,
		SylUtil::InterpolationFunction<SylP2P::Peer> const& _interpolationFunction = nullptr);
};

namespace SylP2P
{
	class Peer
	{
		friend class MessageHandler;
		template<class T> friend class Networkable;

		friend void SylNet::serialize(
			Peer& _object, SylNet::SerializationContext const& _ctx,
			SylUtil::InterpolationFunction<Peer> const& _interpolationFunction);

		enum Port
		{
			Sync = 1,
		};

	public:
		Peer();
		~Peer();

		/**
		* Initiates the bootstrap procedure.
		* This will either leave you connected to the specified peer-to-peer network or start your own.
		*/
		void bootstrap();
		/**
		* Updates the peer. Sends, receives and processes packets.
		*/
		void update();

		/**
		* Limits sending over network to one packet per timeframe specified.
		*/
		void setSendDelay(float _seconds);
		/**
		* Returns the send delay used in ms.
		*/
		SylUtil::time_ms getSendDelayMilliSeconds() const;
		/**
		* Returns the send delay used in seconds.
		*/
		float getSendDelaySeconds() const;
		/**
		* Limits receiving over network (per frame) to packet limit specified.
		* 0 = unlimited
		*/
		void setReceivePacketLimit(unsigned int _packetLimit);

		/**
		* Default interpolation time will be given as (sendDelay * 2).
		* This is due to Udp sometimes loosing packets in transfer.
		*/
		void setDefaultInterpolationDuration();
		/**
		* USE WITH CARE: This allows to set a custom interpolation time.
		* Effects might vary.
		*/
		void setCustomInterpolationDuration(float _seconds);
		/**
		* Returns the interpolation time used in ms.
		*/
		SylUtil::time_ms getInterpolationDurationMilliSeconds() const;
		/**
		* Returns the interpolation time used in seconds.
		*/
		float getInterpolationDurationSeconds() const;

		/**
		* Returns the current connection count.
		*/
		unsigned int getConnectionCount() const;
		/**
		* Get the port to which the socket is bound locally.
		*/
		unsigned short getLocalPort() const;

	private:
		void create();
		void integrate(SylNet::Protocol& _caller, SylNet::ClientId _clientId, unsigned short _localPort, SylNet::Connection _connection);
		
		void build_routing_table();
		void debug_routing_table();
		void find_closest(SylUtil::Uuid _uuid, SylUtil::Uuid _distance, SylUtil::Uuid _upperLimit, SylUtil::Uuid& _closest, SylUtil::Uuid& _closest_distance);
		void find_closest_inverse(SylUtil::Uuid _uuid, SylUtil::Uuid _distance, SylUtil::Uuid& _closest, SylUtil::Uuid& _closest_distance);

		void sync();

		bool m_isBootstrappingNode;

		SylNet::TcpClient m_peer;
		SylNet::TcpServer m_server;

		MessageHandler m_handler;
		SylNet::Udp m_sync;

		SylUtil::Uuid m_uuid;
		std::map<SylUtil::Uuid, SylNet::Connection> m_routingTable;
		SylUtil::time_ms m_networkTimeOffset;

		unsigned int m_networkableCount;
		SylUtil::Uuid m_syncUuid;
		SylUtil::time_ms m_syncSendTime;
	};
};

#endif