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

#ifndef SYLP2P_NETWORKABLE_H
#define SYLP2P_NETWORKABLE_H

#include <map>

#include "../../syl_net/util/serialization.h"
#include "../../syl_util/uuid/uuid.h"
#include "../../syl_util/time/time.h"

namespace SylP2P
{
	template<class T>
	class Networkable;
};

namespace SylNet
{
	template<class T> void serialize(
		SylP2P::Networkable<T>& _object, SerializationContext const& _ctx,
		SylUtil::InterpolationFunction<SylP2P::Networkable<T>> const& _interpolationFunction = nullptr);
};

namespace SylP2P
{
	class Peer;

	template<class T>
	class Networkable
	{
		friend void SylNet::serialize<T>(
			SylP2P::Networkable<T>& _object, SylNet::SerializationContext const& _ctx,
			SylUtil::InterpolationFunction<SylP2P::Networkable<T>> const& _interpolationFunction);

		struct Object
		{
			Object() = default;
			Object(T _object, SylUtil::time_ms _sendTime);

			T object;
			SylUtil::time_ms sendTime;
		};

		struct InterpolationObject
		{
			InterpolationObject() = default;
			InterpolationObject(T _previous, T _current, SylUtil::time_ms _interpolationStart);

			T previous;
			T current;
			SylUtil::time_ms interpolationStart;
		};

	public:
		Networkable(T& _reference);

		/**
		* Register this networkable with the specified peer. It will then be serialized.
		* An interpolation function can be given in order to achieve smooth transition.
		*/
		bool registerWith(Peer* _peer, SylNet::Networkable::Id _id, SylUtil::InterpolationFunction<T> _interpolationFunction = nullptr);
		/**
		* Unregister this networkable. It will no longer be serialized.
		*/
		void unregister();

		/**
		* Serialize received instances (objects from other peers).
		*/
		void share();
		/**
		* Don't serialize received instances (objects from other peers).
		*/
		void unshare();

		/**
		* Time until new data has to be sent by other peers (otherwise object removal will take place).
		* 0.0f = disable
		*/
		void setObjectTimeout(float _seconds);
		/**
		* Returns all received instances (objects from other peers).
		*/
		std::map<SylUtil::Uuid, T> getObjects() const;

	private:
		SylUtil::Uuid& getOwnUuid() const;
		SylUtil::Uuid& getSyncUuid() const;
		SylUtil::time_ms& getSyncSendTime() const;
		SylUtil::time_ms getNetworkTime() const;
		SylUtil::time_ms getInterpolationDurationMilliSeconds() const;

		T& m_reference;
		std::map<SylUtil::Uuid, Object> m_objects;
		std::map<SylUtil::Uuid, InterpolationObject> m_interpolationObjects;

		Peer* m_peer;
		SylNet::Networkable::Id m_id;
		SylUtil::InterpolationFunction<T> m_interpolationFunction;

		bool m_share;

		SylUtil::time_ms m_objectTimeout;
	};
};

#include "networkable.tcc"

#endif