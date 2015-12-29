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

#ifndef SYLP2P_MESSAGE_HANDLER_H
#define SYLP2P_MESSAGE_HANDLER_H

#include "../../syl_net/protocol/udp.h"
#include "../../syl_util/uuid/uuid.h"
#include "../../syl_util/time/time.h"

namespace SylP2P
{
	class MessageHandler;
};

namespace SylNet
{
	void serialize(
		SylP2P::MessageHandler& _object, SerializationContext const& _ctx,
		SylUtil::InterpolationFunction<SylP2P::MessageHandler> const& _interpolationFunction = nullptr);
};

namespace SylP2P
{
	class Peer;

	class MessageHandler
	{
		friend void SylNet::serialize(
			MessageHandler& _object, SylNet::SerializationContext const& _ctx,
			SylUtil::InterpolationFunction<MessageHandler> const& _interpolationFunction);

		enum Event
		{
			FIND_CLOSEST,
			RETURN_CLOSEST,
			CLOSEST_MATCH,
			FIND_JOIN,
			RETURN_JOIN,
			JOIN_MATCH,
			JOIN,
			JOIN_ACK,
			FIND_LEAVE,
			RETURN_LEAVE,
			LEAVE_MATCH,
			LEAVE,
			LEAVE_ACK,
			PING,
			PONG,
		};

	public:
		MessageHandler(Peer& _peer);
		~MessageHandler();
		
		/**
		* Initializes the message handler with its own ip address in the network (localhost, LAN, internet).
		*/
		void init(sf::IpAddress _address);
		/**
		* Returns the address and bound UDP port.
		*/
		SylNet::Connection getConnection() const;
		/**
		* The message handler will send requests every timeframe specified, should it not receive an answer (reliability).
		*/
		void setRequestSpeed(float _seconds);
		/**
		* Time until acknowledgement packet has to be received (otherwise connection will close).
		* 0.0f = disable
		*/
		void setConnectionTimeout(float _seconds);

		/**
		* Inserts a uuid/connection pair into the routing table.
		*/
		void insert_into_routing_table(SylUtil::Uuid _uuid, SylNet::Connection _c);
		/**
		* Removes a uuid/connection pair from the routing table.
		*/
		void remove_from_routing_table(SylUtil::Uuid _uuid);

		/**
		* Updates the message handler. Sends, receives and processes packets.
		*/
		void update();

		/**
		* Requests from the peer at the given connection the closest uuid (in reference to our own) above or equal to the specified distance.
		*/
		void find_closest(SylUtil::Uuid _distance, SylNet::Connection _c);
		/**
		* Tries to locate nodes that should hold a reference to our peer.
		*/
		void try_join(SylUtil::Uuid _distance, SylNet::Connection _c);
		/**
		* Clears the routing table. Sends leave messages to all participants.
		*/
		void leave_all();

	private:
		void handle_incoming(SylNet::Connection _incoming);
		void add_outgoing(Event _e, SylNet::Connection _c, std::vector<unsigned char>& _data);
		void send_event(Event _e, SylNet::Connection _c, SylUtil::Uuid _uuid);

		void update_request(Event _e, SylNet::Connection _c, SylUtil::Uuid _uuid);
		bool remove_request(Event _e, SylNet::Connection _c, SylUtil::Uuid _uuid);
		void send_requests();

		void update_pongs();

		bool m_initialized;
		SylNet::Connection m_connection;
		SylUtil::time_ms m_requestSpeed;
		SylUtil::time_ms m_connectionTimeout;

		Peer& m_peer;
		SylNet::Udp m_udp;

		std::map<SylUtil::Uuid, SylUtil::time_ms> m_lastPongTimes;

		std::multimap<Event, std::vector<unsigned char>> m_incoming;
		std::map<SylNet::Connection, std::multimap<Event, std::vector<unsigned char>>> m_outgoing;
		std::multimap<Event, std::vector<unsigned char>>* m_outgoingSubset;
		
		std::map<Event, std::map<SylNet::Connection, std::map<SylUtil::Uuid, SylUtil::time_ms>>> m_requests;
	};
};

#endif