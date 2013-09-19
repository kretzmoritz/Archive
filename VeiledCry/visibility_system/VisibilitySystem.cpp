#include "VisibilitySystem.h"
#include "IEntitySystem.h"
#include "IGameFramework.h"
#include "IActorSystem.h"

VisibilitySystem::VisibilitySystem(float _saveTime, float _updateTime, unsigned _queryThreshold)
	: m_saveTime(_saveTime), m_updateTime(_updateTime), m_queryThreshold(_queryThreshold)
{

}

void VisibilitySystem::SetSaveTime(float _saveTime)
{
	m_saveTime = _saveTime;
}

void VisibilitySystem::SetUpdateTime(float _updateTime)
{
	m_updateTime = _updateTime;
}

void VisibilitySystem::SetQueryThreshold(unsigned _queryThreshold)
{
	m_queryThreshold = _queryThreshold;
}

void VisibilitySystem::Update()
{
	tick_t endTime = CryGetTicks() + m_updateTime * gEnv->pTimer->GetTicksPerSecond();

	while(!m_queries.empty())
	{
		auto it = m_data.find(m_queries.front().first);
		
		if(it != m_data.end()) //Do we still have usable data?
		{
			(*m_queries.front().second.cb)(CallbackParam(m_queries.front().first.id, it->second.visible, m_queries.front().second.userData));
			delete m_queries.front().second.cb;
			m_queries.pop();
			
			continue;
		}

		bool inView = false;
		
		IEntity* pEntity = gEnv->pEntitySystem->GetEntity(m_queries.front().first.id);
		if(pEntity)
		{
			AABB aabb;
			pEntity->GetWorldBounds(aabb);
			inView = GetISystem()->GetViewCamera().IsAABBVisible_F(aabb);
		}

		if(!m_queries.front().first.raycast || m_queries.size() > m_queryThreshold) //Let's only use AABB testing
		{
			m_data.insert(std::make_pair(m_queries.front().first, VisibilityInfo(inView, CryGetTicks() + m_saveTime * gEnv->pTimer->GetTicksPerSecond())));

			(*m_queries.front().second.cb)(CallbackParam(m_queries.front().first.id, inView, m_queries.front().second.userData));
			delete m_queries.front().second.cb;
			m_queries.pop();
			
			continue;
		}

		if(inView)
		{
			inView = false;

			if(gEnv->pGameFramework->GetClientActor() && gEnv->pGameFramework->GetClientActor()->GetEntity())
			{
				IPhysicalEntity* pActor = gEnv->pGameFramework->GetClientActor()->GetEntity()->GetPhysics();
				if(pActor)
				{
					IEntity* pEntity = gEnv->pEntitySystem->GetEntity(m_queries.front().first.id);
					if(pEntity)
					{
						AABB aabb;
						pEntity->GetWorldBounds(aabb);
						Vec3 dir = aabb.GetCenter() - GetISystem()->GetViewCamera().GetPosition();
					
						ray_hit rayhit; //Finally we test for obstruction using a raycast
						if(gEnv->pPhysicalWorld->RayWorldIntersection(GetISystem()->GetViewCamera().GetPosition(), dir, ent_all, rwi_stop_at_pierceable | rwi_colltype_any, &rayhit, 1, &pActor, 1))
						{
							if(rayhit.pCollider)
							{
								IEntity* pObjectHit = (IEntity*)rayhit.pCollider->GetForeignData(PHYS_FOREIGN_ID_ENTITY);
								if(pObjectHit == pEntity)
								{
									inView = true;
								}
							}
						}
						else
						{
							inView = true;
						}
					}
				}
			}
		}

		m_data.insert(std::make_pair(m_queries.front().first, VisibilityInfo(inView, CryGetTicks() + m_saveTime * gEnv->pTimer->GetTicksPerSecond())));

		(*m_queries.front().second.cb)(CallbackParam(m_queries.front().first.id, inView, m_queries.front().second.userData));
		delete m_queries.front().second.cb;
		m_queries.pop();

		if(m_updateTime >= 0.0f && CryGetTicks() >= endTime)
			break;
	}

	for(auto it = m_data.begin(); it != m_data.end();)
	{
		if(CryGetTicks() >= it->second.valid)
			m_data.erase(it++);
		else
			++it;
	}
}