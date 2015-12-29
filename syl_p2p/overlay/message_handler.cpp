#include "message_handler.h"
#include "peer.h"

namespace SylP2P
{
	MessageHandler::MessageHandler(Peer& _peer)
		: m_initialized(false), m_requestSpeed(1000), m_connectionTimeout(5000), m_peer(_peer)
	{
		m_udp.acceptPacketsFromUnknownConnections();
		m_udp.registerNetworkable(0, *this);
		m_udp.bind(sf::Socket::AnyPort);

		m_connection.port = m_udp.getLocalPort();
	}

	MessageHandler::~MessageHandler()
	{
		m_udp.unbind();
	}

	void MessageHandler::init(sf::IpAddress _address)
	{
		m_connection.address = _address;
		m_initialized = true;
	}

	SylNet::Connection MessageHandler::getConnection() const
	{
		return m_connection;
	}

	void MessageHandler::setRequestSpeed(float _seconds)
	{
		m_requestSpeed = SylUtil::convertSecondsToMs(_seconds);
	}

	void MessageHandler::setConnectionTimeout(float _seconds)
	{
		m_connectionTimeout = SylUtil::convertSecondsToMs(_seconds);
	}

	void MessageHandler::insert_into_routing_table(SylUtil::Uuid _uuid, SylNet::Connection _c)
	{
		bool inserted = m_peer.m_routingTable.emplace(_uuid, _c).second;

		if (inserted)
		{
			m_lastPongTimes.emplace(_uuid, SylUtil::getCurrentTimeMs());
			send_event(Event::PING, _c, _uuid);
		}
	}

	void MessageHandler::remove_from_routing_table(SylUtil::Uuid _uuid)
	{
		auto i = m_peer.m_routingTable.find(_uuid);

		if (i == m_peer.m_routingTable.end())
		{
			return;
		}

		SylNet::Connection c = i->second;

		for (auto k = m_requests.begin(); k != m_requests.end();)
		{
			auto& connections = k->second;

			connections.erase(c);

			if (connections.empty())
			{
				m_requests.erase(k++);
			}
			else
			{
				++k;
			}
		}

		m_lastPongTimes.erase(_uuid);
		m_peer.m_routingTable.erase(i);
	}

	void MessageHandler::update()
	{
		if (!m_initialized)
		{
			return;
		}

		m_udp.receive();
		send_requests();
		m_udp.send();

		update_pongs();
	}

	void MessageHandler::find_closest(SylUtil::Uuid _distance, SylNet::Connection _c)
	{
		if (!m_initialized)
		{
			return;
		}

		send_event(Event::FIND_CLOSEST, _c, _distance);
	}

	void MessageHandler::try_join(SylUtil::Uuid _distance, SylNet::Connection _c)
	{
		if (!m_initialized)
		{
			return;
		}

		send_event(Event::FIND_JOIN, _c, _distance);
	}

	void MessageHandler::leave_all()
	{
		if (!m_initialized)
		{
			return;
		}

		int const uuidSpace = sizeof(SylUtil::Uuid) * 8;

		for (int i = 0; i < uuidSpace; ++i)
		{
			SylUtil::Uuid distance;
			distance.setBit(i);

			SylUtil::Uuid closest;
			SylUtil::Uuid closest_distance;

			m_peer.find_closest_inverse(m_peer.m_uuid, distance, closest, closest_distance);

			if (closest != m_peer.m_uuid)
			{
				SylNet::Connection c = m_peer.m_routingTable.at(closest);
				send_event(Event::FIND_LEAVE, c, distance);
			}
		}

		while (true)
		{
			update();

			auto j = m_requests.find(Event::FIND_LEAVE);
			auto k = m_requests.find(Event::LEAVE);

			if (j == m_requests.end() && k == m_requests.end()) //Block until task completion
			{
				break;
			}
		}

		m_peer.m_routingTable.clear();
	}

	void MessageHandler::handle_incoming(SylNet::Connection _incoming)
	{
		for (auto i = m_incoming.begin(); i != m_incoming.end(); ++i)
		{
			Event e = i->first;
			std::vector<unsigned char>& data = i->second;

			switch (e)
			{
			case Event::FIND_CLOSEST:
				{
					SylUtil::Uuid distance;
					distance.from(data);

					SylUtil::Uuid uuid;
					uuid.from(data, sizeof(distance));

					SylUtil::Uuid upperLimit = distance;
					upperLimit.setBitsUntilHighest();

					SylUtil::Uuid closest;
					SylUtil::Uuid closest_distance;
					m_peer.find_closest(uuid, distance, upperLimit, closest, closest_distance);

					if (closest == m_peer.m_uuid)
					{
						m_peer.m_uuid.to(data, sizeof(distance));
						add_outgoing(Event::CLOSEST_MATCH, _incoming, data);
					}
					else
					{
						SylNet::Connection c = m_peer.m_routingTable.at(closest);
						sf::Uint32 address = c.address.toInteger();

						data.resize(sizeof(distance) + sizeof(address) + sizeof(c.port));
						memcpy(data.data() + sizeof(distance), &address, sizeof(address));
						memcpy(data.data() + sizeof(distance) + sizeof(address), &c.port, sizeof(c.port));

						add_outgoing(Event::RETURN_CLOSEST, _incoming, data);
					}
				}
				break;
			case Event::RETURN_CLOSEST:
				{
					SylUtil::Uuid distance;
					distance.from(data);

					if (!remove_request(Event::FIND_CLOSEST, _incoming, distance))
					{
						continue;
					}

					sf::Uint32 internal_address;
					memcpy(&internal_address, data.data() + sizeof(distance), sizeof(internal_address));

					unsigned short port;
					memcpy(&port, data.data() + sizeof(distance) + sizeof(internal_address), sizeof(port));

					sf::IpAddress address(internal_address);
					SylNet::Connection c(address, port);

					find_closest(distance, c);
				}
				break;
			case Event::CLOSEST_MATCH:
				{
					SylUtil::Uuid distance;
					distance.from(data);

					if (!remove_request(Event::FIND_CLOSEST, _incoming, distance))
					{
						continue;
					}

					SylUtil::Uuid uuid;
					uuid.from(data, sizeof(distance));

					insert_into_routing_table(uuid, _incoming);

					//Debug log
					m_peer.debug_routing_table();
				}
				break;
			case Event::FIND_JOIN:
				{
					SylUtil::Uuid distance;
					distance.from(data);

					SylUtil::Uuid uuid;
					uuid.from(data, sizeof(distance));

					SylUtil::Uuid closest;
					SylUtil::Uuid closest_distance;
					m_peer.find_closest_inverse(uuid, distance, closest, closest_distance);

					if (closest == m_peer.m_uuid)
					{
						closest_distance.to(data, sizeof(distance));
						add_outgoing(Event::JOIN_MATCH, _incoming, data);
					}
					else
					{
						SylNet::Connection c = m_peer.m_routingTable.at(closest);
						sf::Uint32 address = c.address.toInteger();

						data.resize(sizeof(distance) + sizeof(address) + sizeof(c.port));
						memcpy(data.data() + sizeof(distance), &address, sizeof(address));
						memcpy(data.data() + sizeof(distance) + sizeof(address), &c.port, sizeof(c.port));

						add_outgoing(Event::RETURN_JOIN, _incoming, data);
					}
				}
				break;
			case Event::RETURN_JOIN:
				{
					SylUtil::Uuid distance;
					distance.from(data);

					if (!remove_request(Event::FIND_JOIN, _incoming, distance))
					{
						continue;
					}

					sf::Uint32 internal_address;
					memcpy(&internal_address, data.data() + sizeof(distance), sizeof(internal_address));

					unsigned short port;
					memcpy(&port, data.data() + sizeof(distance) + sizeof(internal_address), sizeof(port));

					sf::IpAddress address(internal_address);
					SylNet::Connection c(address, port);

					try_join(distance, c);
				}
				break;
			case Event::JOIN_MATCH:
				{
					SylUtil::Uuid distance;
					distance.from(data);

					if (!remove_request(Event::FIND_JOIN, _incoming, distance))
					{
						continue;
					}

					SylUtil::Uuid closest_distance;
					closest_distance.from(data, sizeof(distance));

					SylUtil::Uuid upperLimit = distance;
					upperLimit.setBitsUntilHighest();

					if (closest_distance <= upperLimit)
					{
						//Notify our match that we have joined its subnet
						send_event(Event::JOIN, _incoming, distance);
					}
				}
				break;
			case Event::JOIN:
				{
					SylUtil::Uuid distance;
					distance.from(data);
					distance.setOnlyHighestBit();

					SylUtil::Uuid uuid;
					uuid.from(data, sizeof(distance));

					data.resize(sizeof(distance));

					SylUtil::Uuid upperLimit = distance;
					upperLimit.setBitsUntilHighest();

					SylUtil::Uuid closest;
					SylUtil::Uuid closest_distance;
					m_peer.find_closest(m_peer.m_uuid, distance, upperLimit, closest, closest_distance);

					SylUtil::Uuid distanceToJoiningPeer = m_peer.m_uuid.distance(uuid);

					if (distanceToJoiningPeer <= closest_distance || closest == m_peer.m_uuid)
					{
						remove_from_routing_table(closest);
						insert_into_routing_table(uuid, _incoming);

						//Debug log
						m_peer.debug_routing_table();

						++distanceToJoiningPeer;

						if (distanceToJoiningPeer <= upperLimit)
						{
							distanceToJoiningPeer.to(data, sizeof(distance));
						}
					}

					add_outgoing(Event::JOIN_ACK, _incoming, data);
				}
				break;
			case Event::JOIN_ACK:
				{
					SylUtil::Uuid distance;
					distance.from(data);

					if (!remove_request(Event::JOIN, _incoming, distance))
					{
						continue;
					}

					if (data.size() == sizeof(SylUtil::Uuid) * 2)
					{
						SylUtil::Uuid predecessor_distance;
						predecessor_distance.from(data, sizeof(distance));

						SylUtil::Uuid closest;
						SylUtil::Uuid closest_distance;
						m_peer.find_closest_inverse(m_peer.m_uuid, predecessor_distance, closest, closest_distance);

						if (closest != m_peer.m_uuid)
						{
							SylNet::Connection c = m_peer.m_routingTable.at(closest);
							try_join(predecessor_distance, c);
						}
					}
				}
				break;
			case Event::FIND_LEAVE:
				{
					SylUtil::Uuid distance;
					distance.from(data);

					SylUtil::Uuid uuid;
					uuid.from(data, sizeof(distance));

					SylUtil::Uuid closest;
					SylUtil::Uuid closest_distance;
					m_peer.find_closest_inverse(uuid, distance, closest, closest_distance);

					if (closest == m_peer.m_uuid)
					{
						closest_distance.to(data, sizeof(distance));
						add_outgoing(Event::LEAVE_MATCH, _incoming, data);
					}
					else
					{
						SylNet::Connection c = m_peer.m_routingTable.at(closest);
						sf::Uint32 address = c.address.toInteger();

						data.resize(sizeof(distance) + sizeof(address) + sizeof(c.port));
						memcpy(data.data() + sizeof(distance), &address, sizeof(address));
						memcpy(data.data() + sizeof(distance) + sizeof(address), &c.port, sizeof(c.port));

						add_outgoing(Event::RETURN_LEAVE, _incoming, data);
					}
				}
				break;
			case Event::RETURN_LEAVE:
				{
					SylUtil::Uuid distance;
					distance.from(data);

					if (!remove_request(Event::FIND_LEAVE, _incoming, distance))
					{
						continue;
					}

					sf::Uint32 internal_address;
					memcpy(&internal_address, data.data() + sizeof(distance), sizeof(internal_address));

					unsigned short port;
					memcpy(&port, data.data() + sizeof(distance) + sizeof(internal_address), sizeof(port));

					sf::IpAddress address(internal_address);
					SylNet::Connection c(address, port);

					send_event(Event::FIND_LEAVE, c, distance);
				}
				break;
			case LEAVE_MATCH:
				{
					SylUtil::Uuid distance;
					distance.from(data);

					if (!remove_request(Event::FIND_LEAVE, _incoming, distance))
					{
						continue;
					}

					SylUtil::Uuid closest_distance;
					closest_distance.from(data, sizeof(distance));

					SylUtil::Uuid upperLimit = distance;
					upperLimit.setBitsUntilHighest();

					if (closest_distance <= upperLimit)
					{
						//Notify our match that we are leaving its subnet
						send_event(Event::LEAVE, _incoming, distance);
					}
				}
				break;
			case Event::LEAVE:
				{
					SylUtil::Uuid distance;
					distance.from(data);
					distance.setOnlyHighestBit();

					SylUtil::Uuid uuid;
					uuid.from(data, sizeof(distance));

					SylUtil::Uuid upperLimit = distance;
					upperLimit.setBitsUntilHighest();

					SylUtil::Uuid closest;
					SylUtil::Uuid closest_distance;
					m_peer.find_closest(m_peer.m_uuid, distance, upperLimit, closest, closest_distance);

					if (closest == uuid)
					{
						remove_from_routing_table(uuid);

						if (data.size() > sizeof(SylUtil::Uuid) * 2)
						{
							SylUtil::Uuid uuid_successor;
							uuid_successor.from(data, sizeof(distance) + sizeof(uuid));

							if (uuid_successor != m_peer.m_uuid && m_peer.m_uuid.distance(uuid_successor) <= upperLimit)
							{
								sf::Uint32 internal_address;
								memcpy(&internal_address, data.data() + sizeof(distance) + sizeof(uuid) + sizeof(uuid_successor), sizeof(internal_address));

								unsigned short port;
								memcpy(&port, data.data() + sizeof(distance) + sizeof(uuid) + sizeof(uuid_successor) + sizeof(internal_address), sizeof(port));

								sf::IpAddress address(internal_address);
								SylNet::Connection c(address, port);

								insert_into_routing_table(uuid_successor, c);
							}
						}

						//Debug log
						m_peer.debug_routing_table();

						data.resize(sizeof(distance));

						SylUtil::Uuid distanceToLeavingPeer = m_peer.m_uuid.distance(uuid);
						++distanceToLeavingPeer;

						if (distanceToLeavingPeer <= upperLimit)
						{
							distanceToLeavingPeer.to(data, sizeof(distance));
						}
					}
					else
					{
						data.resize(sizeof(distance));
					}

					add_outgoing(Event::LEAVE_ACK, _incoming, data);
				}
				break;
			case Event::LEAVE_ACK:
				{
					SylUtil::Uuid distance;
					distance.from(data);

					if (!remove_request(Event::LEAVE, _incoming, distance))
					{
						continue;
					}

					if (data.size() == sizeof(SylUtil::Uuid) * 2)
					{
						SylUtil::Uuid predecessor_distance;
						predecessor_distance.from(data, sizeof(distance));

						SylUtil::Uuid closest;
						SylUtil::Uuid closest_distance;
						m_peer.find_closest_inverse(m_peer.m_uuid, predecessor_distance, closest, closest_distance);

						if (closest != m_peer.m_uuid)
						{
							SylNet::Connection c = m_peer.m_routingTable.at(closest);
							send_event(Event::FIND_LEAVE, c, predecessor_distance);
						}
					}
				}
				break;
			case Event::PING:
				{
					add_outgoing(Event::PONG, _incoming, data);
				}
				break;
			case Event::PONG:
				{
					SylUtil::Uuid uuid;
					uuid.from(data);

					auto i = m_lastPongTimes.find(uuid);

					if (i != m_lastPongTimes.end())
					{
						SylUtil::time_ms& lastPongReceiveTime = i->second;
						lastPongReceiveTime = SylUtil::getCurrentTimeMs();
					}
				}
				break;
			}
		}

		m_incoming.clear();
	}

	void MessageHandler::add_outgoing(Event _e, SylNet::Connection _c, std::vector<unsigned char>& _data)
	{
		auto i = m_outgoing.find(_c);

		if (i == m_outgoing.end())
		{
			i = m_outgoing.emplace(_c, std::multimap<Event, std::vector<unsigned char>>()).first;
		}

		auto& events = i->second;
		events.emplace(_e, _data);
	}

	void MessageHandler::send_event(Event _e, SylNet::Connection _c, SylUtil::Uuid _uuid)
	{
		std::vector<unsigned char> data;
		_uuid.to(data);

		switch (_e)
		{
		case FIND_CLOSEST:
		case FIND_JOIN:
		case JOIN:
		case FIND_LEAVE:
			{
				m_peer.m_uuid.to(data, data.size());
			}
			break;
		case LEAVE:
			{
				m_peer.m_uuid.to(data, data.size());

				//Find successor
				SylUtil::Uuid distance;
				distance.setBit(0);

				SylUtil::Uuid upperLimit;
				upperLimit.setAllBits();

				SylUtil::Uuid closest;
				SylUtil::Uuid closest_distance;
				m_peer.find_closest(m_peer.m_uuid, distance, upperLimit, closest, closest_distance);

				if (closest != m_peer.m_uuid)
				{
					closest.to(data, data.size());

					SylNet::Connection c = m_peer.m_routingTable.at(closest);
					sf::Uint32 address = c.address.toInteger();

					data.resize(data.size() + sizeof(address) + sizeof(c.port));
					memcpy(data.data() + sizeof(distance) + sizeof(m_peer.m_uuid) + sizeof(closest), &address, sizeof(address));
					memcpy(data.data() + sizeof(distance) + sizeof(m_peer.m_uuid) + sizeof(closest) + sizeof(address), &c.port, sizeof(c.port));
				}
			}
			break;
		}

		add_outgoing(_e, _c, data);
		update_request(_e, _c, _uuid); //This is so udp transmits events reliably
	}

	void MessageHandler::update_request(Event _e, SylNet::Connection _c, SylUtil::Uuid _uuid)
	{
		auto i = m_requests.find(_e);

		if (i == m_requests.end())
		{
			i = m_requests.emplace(_e, std::map<SylNet::Connection, std::map<SylUtil::Uuid, SylUtil::time_ms>>()).first;
		}

		auto& connections = i->second;
		auto j = connections.find(_c);

		if (j == connections.end())
		{
			j = connections.emplace(_c, std::map<SylUtil::Uuid, SylUtil::time_ms>()).first;
		}

		auto& uuids = j->second;
		auto k = uuids.find(_uuid);

		if (k == uuids.end())
		{
			uuids.emplace(_uuid, SylUtil::getCurrentTimeMs() + m_requestSpeed);
		}
		else
		{
			SylUtil::time_ms& nextRequestTime = k->second;
			nextRequestTime = SylUtil::getCurrentTimeMs() + m_requestSpeed;
		}
	}

	bool MessageHandler::remove_request(Event _e, SylNet::Connection _c, SylUtil::Uuid _uuid)
	{
		bool removed = false;
		auto i = m_requests.find(_e);

		if (i != m_requests.end())
		{
			auto& connections = i->second;
			auto j = connections.find(_c);

			if (j != connections.end())
			{
				auto& uuids = j->second;
				removed = uuids.erase(_uuid) > 0 ? true : false;

				if (uuids.empty())
				{
					connections.erase(_c);

					if (connections.empty())
					{
						m_requests.erase(_e);
					}
				}
			}
		}

		return removed;
	}

	void MessageHandler::send_requests()
	{
		for (auto i = m_requests.begin(); i != m_requests.end(); ++i)
		{
			Event e = i->first;
			auto& connections = i->second;

			for (auto j = connections.begin(); j != connections.end(); ++j)
			{
				SylNet::Connection c = j->first;
				auto& uuids = j->second;

				for (auto k = uuids.begin(); k != uuids.end(); ++k)
				{
					SylUtil::time_ms nextRequestTime = k->second;

					if (SylUtil::getCurrentTimeMs() < nextRequestTime)
					{
						continue;
					}

					SylUtil::Uuid uuid = k->first;
					send_event(e, c, uuid);
				}
			}
		}
	}

	void MessageHandler::update_pongs()
	{
		std::vector<SylUtil::Uuid> remove;

		for (auto i = m_lastPongTimes.begin(); i != m_lastPongTimes.end(); ++i)
		{
			SylUtil::Uuid uuid = i->first;
			SylUtil::time_ms lastPongReceiveTime = i->second;
				
			if (m_connectionTimeout == 0 || SylUtil::getCurrentTimeMs() - lastPongReceiveTime < m_connectionTimeout)
			{
				continue;
			}
			
			remove.push_back(uuid);
		}

		for (auto i = remove.begin(); i != remove.end(); ++i)
		{
			SylUtil::Uuid uuid = *i;
			remove_from_routing_table(uuid);
		}

		if (!remove.empty())
		{
			//Debug log
			m_peer.debug_routing_table();
		}
	}
};

namespace SylNet
{
	void serialize(
		SylP2P::MessageHandler& _object, SerializationContext const& _ctx,
		SylUtil::InterpolationFunction<SylP2P::MessageHandler> const& _interpolationFunction)
	{
		if (_ctx.state == Networkable::State::READ)
		{
			SylNet::Connection c;

			serialize(c, _ctx);
			serialize(_object.m_incoming, _ctx);

			_object.handle_incoming(c);
		}
		else if (_ctx.state == Networkable::State::WRITE)
		{
			if (_object.m_udp.getConnectionCount() > 0)
			{
				serialize(_object.m_connection, _ctx);
				serialize(*_object.m_outgoingSubset, _ctx);
			}
			else
			{
				float sendDelay = _object.m_udp.getSendDelaySeconds();
				_object.m_udp.setSendDelay(0.0f);

				for (auto i = _object.m_outgoing.begin(); i != _object.m_outgoing.end(); ++i)
				{
					SylNet::Connection c = i->first;
					_object.m_outgoingSubset = &i->second;
					
					_object.m_udp.addConnection(0, c);
					_object.m_udp.send();
					_object.m_udp.clearConnections();
				}

				_object.m_outgoing.clear();
				_object.m_udp.setSendDelay(sendDelay);
			}
		}
	}
};