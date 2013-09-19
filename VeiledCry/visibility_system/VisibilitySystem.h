///////////////////////////////////////////////
// Code by Moritz Kretz
// Description: Frustum AABB intersection check to determine object visibility
// Raycast check in order to figure out occlusion
///////////////////////////////////////////////

#ifndef VISIBILITY_SYSTEM_H
#define VISIBILITY_SYSTEM_H

#include "stdafx.h"
#include "../Utility/callback.h"

///////////////////////////////////////////////
// Helper structs
///////////////////////////////////////////////

struct EntityInfo
{
	EntityId id;
	bool raycast;

	EntityInfo(EntityId _id, bool _raycast)
		: id(_id), raycast(_raycast) {}

	bool operator<(const EntityInfo& _rhs) const
	{
		if(id != _rhs.id)
			return id < _rhs.id;
		else
			return raycast < _rhs.raycast;
	}
};

struct VisibilityInfo
{
	bool visible;
	tick_t valid;

	VisibilityInfo(bool _visible, tick_t _valid)
		: visible(_visible), valid(_valid) {}
};

struct CallbackParam
{
	EntityId id;
	bool visible;
	void* userData;

	CallbackParam(EntityId _id, bool _visible, void* _userData)
		: id(_id), visible(_visible), userData(_userData) {}
};

struct CallbackInfo
{
	BaseCallback<CallbackParam>* cb;
	void* userData;

	CallbackInfo(BaseCallback<CallbackParam>* _cb, void* _userData)
		: cb(_cb), userData(_userData) {}
};

///////////////////////////////////////////////
///////////////////////////////////////////////

class VisibilitySystem
{
public:
	VisibilitySystem(float _saveTime = 0.5f, float _updateTime = 0.005f, unsigned _queryThreshold = 100); //Time in seconds (e.g. 0.005f = 5ms)

	template<class T> void Query(EntityId _id, bool _raycast, void* _userData, T* _class, void (T::*FunctionPtr)(CallbackParam))
	{
		m_queries.push(std::make_pair(EntityInfo(_id, _raycast), CallbackInfo(new Callback<T, CallbackParam>(_class, FunctionPtr), _userData)));
	}

	void SetSaveTime(float _saveTime);
	void SetUpdateTime(float _updateTime);
	void SetQueryThreshold(unsigned _queryThreshold);
	void Update();

private:
	float m_saveTime;
	float m_updateTime;
	unsigned m_queryThreshold;
	
	std::queue<std::pair<EntityInfo, CallbackInfo>> m_queries;
	std::map<EntityInfo, VisibilityInfo> m_data;
};

#endif