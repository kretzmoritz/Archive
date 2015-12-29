#include "networkable.h"

namespace SylP2P
{
	template<class T>
	Networkable<T>::Object::Object(T _object, SylUtil::time_ms _sendTime)
		: object(_object), sendTime(_sendTime)
	{
	}

	template<class T>
	Networkable<T>::InterpolationObject::InterpolationObject(T _previous, T _current, SylUtil::time_ms _interpolationStart)
		: previous(_previous), current(_current), interpolationStart(_interpolationStart)
	{
	}

	template<class T>
	Networkable<T>::Networkable(T& _reference)
		: m_reference(_reference), m_peer(nullptr), m_interpolationFunction(nullptr), m_share(false), m_objectTimeout(5000)
	{
	}

	template<class T>
	bool Networkable<T>::registerWith(Peer* _peer, SylNet::Networkable::Id _id, SylUtil::InterpolationFunction<T> _interpolationFunction)
	{
		if (m_peer != nullptr || _peer == nullptr)
		{
			return false;
		}

		SylNet::Networkable::Id id = _id + 1; //We already have our header registered
		bool success = _peer->m_sync.registerNetworkable<Networkable<T>>(id, *this, nullptr, true);

		if (success)
		{
			m_peer = _peer;
			m_id = id;
			m_interpolationFunction = _interpolationFunction;

			++m_peer->m_networkableCount;
		}

		return success;
	}

	template<class T>
	void Networkable<T>::unregister()
	{
		if (m_peer == nullptr)
		{
			return;
		}

		m_peer->m_sync.unregisterNetworkable(m_id);
		m_peer = nullptr;
		m_interpolationFunction = nullptr;

		--m_peer->m_networkableCount;

		m_objects.clear();
		m_receival.clear();
		m_interpolationObjects.clear();
	}

	template<class T>
	void Networkable<T>::share()
	{
		m_share = true;
	}

	template<class T>
	void Networkable<T>::unshare()
	{
		m_share = false;
	}

	template<class T>
	void Networkable<T>::setObjectTimeout(float _seconds)
	{
		m_objectTimeout = SylUtil::convertSecondsToMs(_seconds);
	}

	template<class T>
	std::map<SylUtil::Uuid, T> Networkable<T>::getObjects() const
	{
		std::map<SylUtil::Uuid, T> objects;

		if (m_interpolationFunction)
		{
			for (auto i = m_interpolationObjects.begin(); i != m_interpolationObjects.end(); ++i)
			{
				SylUtil::Uuid uuid = i->first;
				T const& current = i->second.current;
				
				objects.emplace(uuid, current);
			}
		}
		else
		{
			for (auto i = m_objects.begin(); i != m_objects.end(); ++i)
			{
				SylUtil::Uuid uuid = i->first;
				T const& object = i->second.object;

				objects.emplace(uuid, object);
			}
		}

		return objects;
	}

	template<class T>
	SylUtil::Uuid& Networkable<T>::getOwnUuid() const
	{
		return m_peer->m_uuid;
	}

	template<class T>
	SylUtil::Uuid& Networkable<T>::getSyncUuid() const
	{
		return m_peer->m_syncUuid;
	}

	template<class T>
	SylUtil::time_ms& Networkable<T>::getSyncSendTime() const
	{
		return m_peer->m_syncSendTime;
	}

	template<class T>
	SylUtil::time_ms Networkable<T>::getNetworkTime() const
	{
		return SylUtil::getCurrentTimeMs() + m_peer->m_networkTimeOffset;
	}

	template<class T>
	SylUtil::time_ms Networkable<T>::getInterpolationDurationMilliSeconds() const
	{
		return m_peer->getInterpolationDurationMilliSeconds();
	}
};

namespace SylNet
{
	template<class T> void serialize(
		SylP2P::Networkable<T>& _object, SerializationContext const& _ctx,
		SylUtil::InterpolationFunction<SylP2P::Networkable<T>> const& _interpolationFunction)
	{
		if (_ctx.state == Networkable::State::READ)
		{
			T reference;
			serialize(reference, _ctx);

			auto i = _object.m_objects.find(_object.getSyncUuid());

			if (i != _object.m_objects.end())
			{
				auto& previous = i->second;

				if (_object.getSyncSendTime() > previous.sendTime)
				{
					previous.object = reference;
					previous.sendTime = _object.getSyncSendTime();

					if (_object.m_interpolationFunction)
					{
						auto& j = _object.m_interpolationObjects.at(_object.getSyncUuid());
						
						j.previous = j.current;
						j.interpolationStart = SylUtil::getCurrentTimeMs();
					}
				}
			}
			else
			{
				_object.m_objects.emplace(
					_object.getSyncUuid(), SylP2P::Networkable<T>::Object(reference, _object.getSyncSendTime()));

				if (_object.m_interpolationFunction)
				{
					_object.m_interpolationObjects.emplace(
						_object.getSyncUuid(), SylP2P::Networkable<T>::InterpolationObject(reference, reference, SylUtil::getCurrentTimeMs()));
				}
			}

			bool share;
			serialize(share, _ctx);

			if (share)
			{
				std::map<SylUtil::Uuid, SylP2P::Networkable<T>::Object> objects;
				serialize(objects, _ctx);

				for (i = objects.begin(); i != objects.end(); ++i)
				{
					SylUtil::Uuid uuid = i->first;

					if (!(uuid == _object.getOwnUuid()))
					{
						auto& received = i->second;
						auto j = _object.m_objects.find(uuid);

						if (j != _object.m_objects.end())
						{
							auto& previous = j->second;

							if (received.sendTime > previous.sendTime)
							{
								previous.object = received.object;
								previous.sendTime = received.sendTime;

								if (_object.m_interpolationFunction)
								{
									auto& k = _object.m_interpolationObjects.at(uuid);
									
									k.previous = k.current;
									k.interpolationStart = SylUtil::getCurrentTimeMs();
								}
							}
						}
						else
						{
							_object.m_objects.emplace(
								uuid, SylP2P::Networkable<T>::Object(received.object, received.sendTime));

							if (_object.m_interpolationFunction)
							{
								_object.m_interpolationObjects.emplace(
									uuid, SylP2P::Networkable<T>::InterpolationObject(received.object, received.object, SylUtil::getCurrentTimeMs()));
							}
						}
					}
				}
			}
		}
		else if (_ctx.state == Networkable::State::WRITE)
		{
			serialize(_object.m_reference, _ctx);
			serialize(_object.m_share, _ctx);

			if (_object.m_share)
			{
				serialize(_object.m_objects, _ctx);
			}
		}
		else if (_ctx.state == Networkable::State::INTERPOLATE)
		{
			for (auto i = _object.m_objects.begin(); i != _object.m_objects.end();)
			{
				SylUtil::time_ms sendTime = i->second.sendTime;

				if (_object.m_objectTimeout == 0 || _object.getNetworkTime() - sendTime < _object.m_objectTimeout)
				{
					++i;
					continue;
				}

				SylUtil::Uuid uuid = i->first;

				_object.m_objects.erase(i++);
				_object.m_interpolationObjects.erase(uuid);
			}

			if (!_object.m_interpolationFunction)
			{
				return;
			}

			SylUtil::time_ms interpolationDuration = _object.getInterpolationDurationMilliSeconds();

			for (auto i = _object.m_interpolationObjects.begin(); i != _object.m_interpolationObjects.end(); ++i)
			{
				SylUtil::Uuid uuid = i->first;
				SylUtil::time_ms interpolationStart = i->second.interpolationStart;

				float interpolationProgress = interpolationDuration > 0 ?
					(SylUtil::getCurrentTimeMs() - interpolationStart) / static_cast<float>(interpolationDuration) : 1.0f;

				T& object = i->second.current;
				T& previousObject = i->second.previous;
				T& nextObject = _object.m_objects.at(uuid).object;

				_object.m_interpolationFunction(object, previousObject, nextObject, interpolationProgress);
			}
		}
	}
};