#include <iostream>

#include "peer.h"

namespace SylP2P
{
	Peer::Peer()
		: m_isBootstrappingNode(false), m_handler(*this), m_networkTimeOffset(0), m_networkableCount(0)
	{
		m_server.setSendDelay(0.0f);
		
		m_sync.acceptPacketsFromUnknownConnections();
		m_sync.registerNetworkable(0, *this);
		m_sync.bind(m_handler.getConnection().port + Port::Sync);
	}

	Peer::~Peer()
	{
		m_handler.leave_all();
		m_sync.unbind();
	}

	void Peer::bootstrap()
	{
		SylNet::ConnectionCallback cb = std::bind(&Peer::integrate, this,
			std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		m_peer.setConnectCallback(cb);

		sf::IpAddress adress("127.0.0.1");
		unsigned short port = 5000;

		if (!m_peer.open(adress, port))
		{
			create();
		}
	}

	void Peer::update()
	{
		if (m_isBootstrappingNode)
		{
			m_server.accept();
		}

		m_handler.update();
		sync();
	}

	void Peer::setSendDelay(float _seconds)
	{
		m_sync.setSendDelay(_seconds);
	}

	SylUtil::time_ms Peer::getSendDelayMilliSeconds() const
	{
		return m_sync.getSendDelayMilliSeconds();
	}

	float Peer::getSendDelaySeconds() const
	{
		return m_sync.getSendDelaySeconds();
	}

	void Peer::setReceivePacketLimit(unsigned int _packetLimit)
	{
		m_sync.setReceivePacketLimit(_packetLimit);
	}

	void Peer::setDefaultInterpolationDuration()
	{
		m_sync.setDefaultInterpolationDuration();
	}

	void Peer::setCustomInterpolationDuration(float _seconds)
	{
		m_sync.setCustomInterpolationDuration(_seconds);
	}

	SylUtil::time_ms Peer::getInterpolationDurationMilliSeconds() const
	{
		return m_sync.getInterpolationDurationMilliSeconds();
	}

	float Peer::getInterpolationDurationSeconds() const
	{
		return m_sync.getInterpolationDurationSeconds();
	}

	unsigned int Peer::getConnectionCount() const
	{
		return m_sync.getConnectionCount();
	}

	unsigned short Peer::getLocalPort() const
	{
		return m_sync.getLocalPort();
	}

	void Peer::create()
	{
		SylNet::ConnectionCallback cb = std::bind(&Peer::integrate, this,
			std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		m_server.setConnectCallback(cb);
		m_server.listen_start(5000);

		m_handler.init("127.0.0.1");

		m_isBootstrappingNode = true;
	}

	void Peer::integrate(SylNet::Protocol& _caller, SylNet::ClientId _clientId, unsigned short _localPort, SylNet::Connection _connection)
	{
		SylNet::Protocol* caller = &_caller;

		if (caller == &m_peer)
		{
			sf::IpAddress address;
			SylUtil::time_ms bootstrapTime = 0;

			m_peer.registerNetworkable(0, m_uuid);
			m_peer.registerNetworkable(1, m_routingTable);
			m_peer.registerNetworkable(2, address);
			m_peer.registerNetworkable(3, bootstrapTime);

			SylUtil::time_ms latency;

			while (m_uuid.isZero() || m_routingTable.empty() || address == sf::IpAddress::None || bootstrapTime == 0)
			{
				m_peer.receive();
				latency = m_peer.getLastLatency();
			}

			m_peer.close();

			m_handler.init(address);
			m_networkTimeOffset = (bootstrapTime + latency) - SylUtil::getCurrentTimeMs();

			build_routing_table();
		}
		else if (caller == &m_server)
		{
			std::string str = _connection.address.toString() + std::to_string(_connection.port);

			SylUtil::Uuid uuid;
			uuid.sha1(str);

			std::map<SylUtil::Uuid, SylNet::Connection> routingTable;
			routingTable = m_routingTable;
			routingTable.emplace(m_uuid, m_handler.getConnection());

			SylUtil::time_ms networkTime = SylUtil::getCurrentTimeMs() + m_networkTimeOffset;

			m_server.registerNetworkable(0, uuid);
			m_server.registerNetworkable(1, routingTable);
			m_server.registerNetworkable(2, _connection.address);
			m_server.registerNetworkable(3, networkTime);
			
			m_server.send();

			m_server.clearNetworkables();

			while (m_server.getConnectionCount() > 0)
			{
				m_server.receive();
			}

			//Debug log
			debug_routing_table();
		}
	}

	void Peer::build_routing_table()
	{
		int const uuidSpace = sizeof(SylUtil::Uuid) * 8; //128-bit uuid space

		SylUtil::Uuid upperLimit;

		for (int i = 0; i < uuidSpace; ++i)
		{
			upperLimit.setBit(i);

			SylUtil::Uuid distance;
			distance.setBit(i);

			SylUtil::Uuid closest;
			SylUtil::Uuid closest_distance;
			find_closest(m_uuid, distance, upperLimit, closest, closest_distance);

			if (closest != m_uuid)
			{
				SylNet::Connection c = m_routingTable.at(closest);
				m_handler.find_closest(distance, c);
			}

			find_closest_inverse(m_uuid, distance, closest, closest_distance);

			if (closest != m_uuid)
			{
				SylNet::Connection c = m_routingTable.at(closest);
				m_handler.try_join(distance, c); //Update the routing tables of other nodes in the overlay
			}
		}

		m_routingTable.clear();
	}

	void Peer::debug_routing_table()
	{
		system("cls");

		std::cout << "My uuid: " << m_uuid.toString() << std::endl;
		std::cout << std::endl;
		std::cout << "Peer-to-peer subnet:" << std::endl;

		for (auto i = m_routingTable.begin(); i != m_routingTable.end(); ++i)
		{
			SylUtil::Uuid uuid = i->first;
			SylNet::Connection c = i->second;

			std::cout << "index: " << m_uuid.distance(uuid).exponentToString() << " uuid: " << uuid.toString() << " address: " << c.address.toString() << ':' << c.port << std::endl;
		}
	}

	void Peer::find_closest(SylUtil::Uuid _uuid, SylUtil::Uuid _distance, SylUtil::Uuid _upperLimit, SylUtil::Uuid& _closest, SylUtil::Uuid& _closest_distance)
	{
		_closest = m_uuid;
		_closest_distance = _uuid.distance(m_uuid);

		for (auto i = m_routingTable.begin(); i != m_routingTable.end(); ++i) //Not optimized! Brute force search the whole routing table.
		{
			SylUtil::Uuid uuid = i->first;
			SylUtil::Uuid distance = _uuid.distance(uuid);

			if (distance >= _distance && distance <= _upperLimit && (distance < _closest_distance || _closest_distance.isZero()))
			{
				_closest = uuid;
				_closest_distance = distance;
			}
		}
	}

	void Peer::find_closest_inverse(SylUtil::Uuid _uuid, SylUtil::Uuid _distance, SylUtil::Uuid& _closest, SylUtil::Uuid& _closest_distance)
	{
		_closest = m_uuid;
		_closest_distance = m_uuid.distance(_uuid);

		for (auto i = m_routingTable.begin(); i != m_routingTable.end(); ++i)
		{
			SylUtil::Uuid uuid = i->first;
			SylUtil::Uuid distance = uuid.distance(_uuid);

			if (distance >= _distance && (distance < _closest_distance || _closest_distance.isZero()))
			{
				_closest = uuid;
				_closest_distance = distance;
			}
		}
	}

	void Peer::sync()
	{
		m_sync.clearConnections();

		unsigned int index = 0;
		
		for (auto i = m_routingTable.begin(); i != m_routingTable.end(); ++i)
		{
			SylNet::Connection c = i->second;
			c.port += Port::Sync;

			m_sync.addConnection(index, c);

			++index;
		}

		m_sync.receive();
		m_sync.send();
	}
};

namespace SylNet
{
	void serialize(
		SylP2P::Peer& _object, SerializationContext const& _ctx,
		SylUtil::InterpolationFunction<SylP2P::Peer> const& _interpolationFunction)
	{
		if (_ctx.state == Networkable::State::READ)
		{
			serialize(_object.m_syncUuid, _ctx);
			serialize(_object.m_syncSendTime, _ctx);
		}
		else if (_ctx.state == Networkable::State::WRITE)
		{
			if (_object.m_networkableCount == 0)
			{
				return;
			}

			serialize(_object.m_uuid, _ctx);
			
			SylUtil::time_ms networkTime = SylUtil::getCurrentTimeMs() + _object.m_networkTimeOffset;
			serialize(networkTime, _ctx);
		}
	}
};

//client connects to dyndns adress
//if connection established -> inclusion in overlay
//if ip resolve fails -> register own ip with dyndns adress; setup overlay
//if connection fails -> dyndns not reachable; try again later