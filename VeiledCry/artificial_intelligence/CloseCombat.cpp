#include "CloseCombat.h"

#include "..\..\..\HardNight\Utility\HardNightUtilities.h"

#define START 0
#define FOLLOW 1
#define ATTACK 2

template<class T> int sign(T _val)
{
	return (T(0) < _val) - (_val < T(0));
}

CFlowNode_AIStateCloseCombat::CFlowNode_AIStateCloseCombat(SActivationInfo* pActInfo)
	: CFlowNode_AIStateBase(pActInfo, "hn_closecombat"), m_Animations(6, NULL)
{
}

IFlowNodePtr CFlowNode_AIStateCloseCombat::Clone(SActivationInfo* pActInfo)
{
	CFlowNode_AIStateCloseCombat* pTemp = new CFlowNode_AIStateCloseCombat(pActInfo);
	pTemp->m_State = m_State;
	pTemp->m_InHitReaction = m_InHitReaction;
	pTemp->m_OrientEnabled = m_OrientEnabled;
	pTemp->m_StateChanged = m_StateChanged;
	pTemp->m_CurrentlyActiveAnimation = m_CurrentlyActiveAnimation;
	pTemp->m_HitReactionEndVel = m_HitReactionEndVel;
	pTemp->m_Bones = m_Bones;
	pTemp->already_hit = already_hit;
	pTemp->m_Animations = m_Animations;
	return pTemp;
}

void CFlowNode_AIStateCloseCombat::DerivedMemoryUsage(ICrySizer* pSizer) const
{
	SIZER_COMPONENT_NAME(pSizer, "CFlowNode_AIStateCloseCombat");
	if(!pSizer->Add(this))
		return;

	pSizer->Add(m_State);
	pSizer->Add(m_InHitReaction);
	pSizer->Add(m_OrientEnabled);
	pSizer->Add(m_StateChanged);
	pSizer->Add(m_CurrentlyActiveAnimation);
	pSizer->Add(m_HitReactionEndVel);
	pSizer->AddContainer(m_Bones);
	pSizer->AddContainer(already_hit);
	pSizer->AddContainer(m_Animations);
}

void CFlowNode_AIStateCloseCombat::GetCustomConfiguration(SFlowNodeConfig& config)
{
	static const SInputPortConfig in_config[] =
	{
		InputPortConfig<EntityId>("Target", _HELP("Who to attack?")),
		InputPortConfig<float>("Speed", AISPEED_SPRINT, _HELP("Speed for the AI to chase with."), "Speed", "enum_float:Zero=0.0,Slow=0.21,Walk=0.4,Run=1.0,Sprint=1.4"),
		InputPortConfig<float>("Range", 2.0f, _HELP("Distance in which we enable close combat.")),
		InputPortConfig<float>("DisableRange", 3.0f, _HELP("Distance in which we disable close combat.")),
		InputPortConfig_AnyType("Animation0", _HELP("First animation data. Priority matters!")), //Might need to change this (dynamic allocation) - right now 6 animations will suffice
		InputPortConfig_AnyType("Animation1", _HELP("Second animation data. Priority matters!")),
		InputPortConfig_AnyType("Animation2", _HELP("Third animation data. Priority matters!")),
		InputPortConfig_AnyType("Animation3", _HELP("Fourth animation data. Priority matters!")),
		InputPortConfig_AnyType("Animation4", _HELP("Fifth animation data. Priority matters!")),
		InputPortConfig_AnyType("Animation5", _HELP("Sixth animation data. Priority matters!")),
		InputPortConfig<SFlowSystemVoid>("AnimStart", _HELP("Animation has started.")),
		InputPortConfig<SFlowSystemVoid>("AnimComplete", _HELP("Animation has finished.")),
		InputPortConfig<bool>("DebugSpeed", false, _HELP("CryLog attack speed values to figure out perfect settings.")),
		InputPortConfig<bool>("DebugAngle", false, _HELP("CryLog angles to figure out perfect settings.")),
		InputPortConfig<bool>("DebugHitReaction", false, _HELP("CryLog hit reaction to figure out if everything works properly.")),
		{0}
	};

	static const SOutputPortConfig out_config[] =
	{
		OutputPortConfig<SFlowSystemVoid>("Sync", _HELP("Force animation sync.")),
		OutputPortConfig<string>("Anim", _HELP("Which animation to play.")),
		{0}
	};

	config.sDescription = _HELP("AI melee fighting style.");
	config.pInputPorts = in_config;
	config.pOutputPorts = out_config;
	config.nFlags = EFLN_TARGET_ENTITY | EFLN_ADVANCED;
}

void CFlowNode_AIStateCloseCombat::OnBehaviorStart()
{
	m_State = START;
	m_InHitReaction = false;
	m_OrientEnabled = false;
	SetUpdates(true, &m_ActInfo);

	m_ActInfo.pEntity->GetAI()->CastToIAIActor()->SetSpeed(GetPortFloat(&m_ActInfo, INPORT_WALK_SPEED));
}

void CFlowNode_AIStateCloseCombat::OnBehaviorEnd()
{
	SetUpdates(false, &m_ActInfo);

	if(m_ActInfo.pEntity && m_ActInfo.pEntity->GetAI())
		if(m_OrientEnabled)
			m_ActInfo.pEntity->GetAI()->CastToIPipeUser()->InsertSubPipe(0, "orient_custom_off");
}

void CFlowNode_AIStateCloseCombat::OnHitReaction(const HitInfo& _hitinfo, float _causedDamage, const SReactionParams& _reactionParams, bool _isTarget)
{
	if(_isTarget)
	{
		if(GetPortBool(&m_ActInfo, INPORT_DEBUG_HIT_REACTION))
			CryLogAlways("Hit reaction started.");

		m_HitReactionEndVel = _reactionParams.endVelocity;
		m_InHitReaction = true;
	}
}

void CFlowNode_AIStateCloseCombat::OnHitReactionEnd()
{
	if(GetPortBool(&m_ActInfo, INPORT_DEBUG_HIT_REACTION))
		CryLogAlways("Hit reaction finished.");

	m_InHitReaction = false;
}

void CFlowNode_AIStateCloseCombat::OnAnimationStart()
{
	IAISignalExtraData* exd = gEnv->pAISystem->CreateSignalExtraData();
	exd->point = m_ActInfo.pEntity->GetAI()->CastToIPipeUser()->GetRefPoint()->GetPos();
	m_ActInfo.pEntity->GetAI()->CastToIAIActor()->SetSignal(AISIGNAL_DEFAULT, "ACT_GOTO", m_ActInfo.pEntity, exd);

	if(!m_OrientEnabled)
	{
		m_ActInfo.pEntity->GetAI()->CastToIPipeUser()->InsertSubPipe(0, "orient_custom");
		m_OrientEnabled = true;
	}
}

void CFlowNode_AIStateCloseCombat::OnAnimationComplete()
{
	m_State = START;
}

void CFlowNode_AIStateCloseCombat::OnUpdate(SActivationInfo* pActInfo)
{
	IEntity* target = gEnv->pEntitySystem->GetEntity(GetPortEntityId(pActInfo, INPORT_TARGET));
	bool isValid = target && target->GetAI() && target->GetAI()->CastToIAIActor()->GetProxy();

	if(isValid && !target->GetAI()->CastToIAIActor()->GetProxy()->IsDead())
	{
		if(pActInfo->pEntity->GetPos().GetDistance(gEnv->pEntitySystem->GetEntity(GetPortEntityId(pActInfo, INPORT_TARGET))->GetPos()) <= GetPortFloat(pActInfo, INPORT_DISABLE_RANGE))
		{
			if(m_State == ATTACK)
			{
				if(m_StateChanged)
				{
					//Initialize bone position information
					m_Bones.resize(m_Animations[m_CurrentlyActiveAnimation]->bones.size());
					for(int i = 0; i < m_Animations[m_CurrentlyActiveAnimation]->bones.size(); i++)
					{
						m_Bones[i].index = pActInfo->pEntity->GetCharacter(0)->GetISkeletonPose()->GetJointIDByName(m_Animations[m_CurrentlyActiveAnimation]->bones[i]);
						m_Bones[i].quat = pActInfo->pEntity->GetCharacter(0)->GetISkeletonPose()->GetAbsJointByID(m_Bones[i].index);
						m_Bones[i].pos_prev = m_Bones[i].quat * Vec3(ZERO);
						m_Bones[i].time = CryGetTicks();
					}

					ActivateOutput(pActInfo, OUTPORT_SYNC, true);
					m_StateChanged = false;
				}
				else if(!m_InHitReaction)
				{
					OnHit(pActInfo);
				}
			}
		}
		else
		{
			if(m_State != FOLLOW)
			{
				ResetGoalpipe();

				if(m_OrientEnabled)
				{
					pActInfo->pEntity->GetAI()->CastToIPipeUser()->InsertSubPipe(0, "orient_custom_off"); //InsertSubPipe = run once
					m_OrientEnabled = false;
				}
				
				pActInfo->pEntity->GetAI()->CastToIPipeUser()->GetCurrentGoalPipe()->PushPipe("goto_custom", true, IGoalPipe::eGT_NOGROUP, GoalParameters()); //PushPipe = run multiple times
				m_State = FOLLOW;
			}
		}

		if(pActInfo->pEntity->GetPos().GetDistance(gEnv->pEntitySystem->GetEntity(GetPortEntityId(pActInfo, INPORT_TARGET))->GetPos()) <= GetPortFloat(pActInfo, INPORT_RANGE))
		{
			if(m_State != ATTACK)
			{
				ResetGoalpipe();

				m_StateChanged = true;
				m_CurrentlyActiveAnimation = -1;

				already_hit.clear();
				ChooseCorrectAnim(pActInfo);

				if(m_CurrentlyActiveAnimation != -1)
				{
					ActivateOutput(pActInfo, OUTPORT_ANIM, m_Animations[m_CurrentlyActiveAnimation]->animation);
					m_State = ATTACK;
				}
			}
		}

		pActInfo->pEntity->GetAI()->CastToIPipeUser()->SetRefPointPos(gEnv->pEntitySystem->GetEntity(GetPortEntityId(pActInfo, INPORT_TARGET))->GetPos());
	}
	else if(m_OrientEnabled)
	{
		m_ActInfo.pEntity->GetAI()->CastToIPipeUser()->InsertSubPipe(0, "orient_custom_off");
		m_OrientEnabled = false;
	}
}

void CFlowNode_AIStateCloseCombat::OnActivate(SActivationInfo* pActInfo)
{
	if(IsPortActive(pActInfo, INPORT_ANIMATION_0))
		m_Animations[0] = hnu::DecodePointer<CFlowNode_AnimData::attack_data>(GetPortVec3(pActInfo, INPORT_ANIMATION_0));
	if(IsPortActive(pActInfo, INPORT_ANIMATION_1))
		m_Animations[1] = hnu::DecodePointer<CFlowNode_AnimData::attack_data>(GetPortVec3(pActInfo, INPORT_ANIMATION_1));
	if(IsPortActive(pActInfo, INPORT_ANIMATION_2))
		m_Animations[2] = hnu::DecodePointer<CFlowNode_AnimData::attack_data>(GetPortVec3(pActInfo, INPORT_ANIMATION_2));
	if(IsPortActive(pActInfo, INPORT_ANIMATION_3))
		m_Animations[3] = hnu::DecodePointer<CFlowNode_AnimData::attack_data>(GetPortVec3(pActInfo, INPORT_ANIMATION_3));
	if(IsPortActive(pActInfo, INPORT_ANIMATION_4))
		m_Animations[4] = hnu::DecodePointer<CFlowNode_AnimData::attack_data>(GetPortVec3(pActInfo, INPORT_ANIMATION_4));
	if(IsPortActive(pActInfo, INPORT_ANIMATION_5))
		m_Animations[5] = hnu::DecodePointer<CFlowNode_AnimData::attack_data>(GetPortVec3(pActInfo, INPORT_ANIMATION_5));
	if(IsPortActive(pActInfo, INPORT_ANIM_START))
		OnAnimationStart();
	if(IsPortActive(pActInfo, INPORT_ANIM_COMPLETE))
		OnAnimationComplete();
}

float CFlowNode_AIStateCloseCombat::CalculateAngle(SActivationInfo* pActInfo)
{
	Vec3 vecToTarget = (gEnv->pEntitySystem->GetEntity(GetPortEntityId(pActInfo, INPORT_TARGET))->GetPos() - pActInfo->pEntity->GetPos()).normalize();
	Vec3 vecFwd = pActInfo->pEntity->GetForwardDir().normalized();
	Vec3 vecUp(0, 0, 1);
	Vec3 vecRight = vecFwd.cross(vecUp);

	float dot_result = vecToTarget.dot(vecFwd); //Returns dot_result = |A|*cos(theta); |A| = 1; A needs to be normalized - cos needs value between -1 and 1
	float radians = cry_acosf(dot_result);
	float angle = radians * 180.0f / g_PI; //0-180 degrees
	float dir = vecRight.dot(vecToTarget);
	dir = sign(dir); // -1, 0 or 1

	if(dir == -1.0f)
		angle = 360.0f - angle;

	if(GetPortBool(pActInfo, INPORT_DEBUG_ANGLE))
		CryLogAlways("%f", angle);

	return angle;
}

bool CFlowNode_AIStateCloseCombat::IsAngleValid(int _index, float _degrees)
{
	bool valid = false;
				
	if(m_Animations[_index]->s_angle > m_Animations[_index]->e_angle)
		valid = _degrees >= m_Animations[_index]->s_angle || _degrees <= m_Animations[_index]->e_angle;
	else
		valid = _degrees >= m_Animations[_index]->s_angle && _degrees <= m_Animations[_index]->e_angle;

	return valid;
}

void CFlowNode_AIStateCloseCombat::ChooseCorrectAnim(SActivationInfo* pActInfo)
{
	for(int i = 0; i < m_Animations.size(); i++)
	{
		if(m_Animations[i] != NULL)
		{
			if(m_Animations[i]->distance != -1)
			{
				float dist = pActInfo->pEntity->GetPos().GetDistance(gEnv->pEntitySystem->GetEntity(GetPortEntityId(pActInfo, INPORT_TARGET))->GetPos());
				if(dist >= m_Animations[i]->distance)
				{
					if(IsAngleValid(i, CalculateAngle(pActInfo)))
						m_CurrentlyActiveAnimation = i;
				}
			}
			else
			{
				if(IsAngleValid(i, CalculateAngle(pActInfo)))
					m_CurrentlyActiveAnimation = i;
			}
		}
	}
}

void CFlowNode_AIStateCloseCombat::OnHit(SActivationInfo* pActInfo)
{
	for(int i = 0; i < m_Animations[m_CurrentlyActiveAnimation]->bones.size(); i++)
	{
		ray_hit rayhit;
		rayhit.pCollider = NULL;
		rayhit.next = NULL;

		m_Bones[i].index = pActInfo->pEntity->GetCharacter(0)->GetISkeletonPose()->GetJointIDByName(m_Animations[m_CurrentlyActiveAnimation]->bones[i]);
		m_Bones[i].quat = pActInfo->pEntity->GetCharacter(0)->GetISkeletonPose()->GetAbsJointByID(m_Bones[i].index);
		m_Bones[i].pos = pActInfo->pEntity->GetWorldTM() * (m_Bones[i].quat * Vec3(ZERO));

		Vec3 current_pos = m_Bones[i].quat * Vec3(ZERO);
		float dist = current_pos.GetDistance(m_Bones[i].pos_prev);
		float time = (CryGetTicks() - m_Bones[i].time) / (float)gEnv->pTimer->GetTicksPerSecond();
		float speed = dist / time;
		m_Bones[i].pos_prev = current_pos;
		m_Bones[i].time = CryGetTicks();

		if(speed >= m_Animations[m_CurrentlyActiveAnimation]->speed_threshold)
		{
			if(GetPortBool(pActInfo, INPORT_DEBUG_SPEED))
				CryLogAlways("%f", speed);

			IPhysicalEntity** entities;
			int numEntities = gEnv->pEntitySystem->GetPhysicalEntitiesInBox(m_Bones[i].pos, 0.05f, entities); //Might need to make size dynamic

			for(int j = 0; j < numEntities; j++)
			{
				IEntity* pPotentialTarget = gEnv->pEntitySystem->GetEntityFromPhysics(entities[j]);

				if(pPotentialTarget && pPotentialTarget != pActInfo->pEntity)
				{
					if(already_hit.find(pPotentialTarget->GetId()) == already_hit.end())
					{
						HitInfo hit;
						hit.damage = m_Animations[m_CurrentlyActiveAnimation]->damage;
						hit.impulse = m_Animations[m_CurrentlyActiveAnimation]->impact_force;
						hit.dir = pPotentialTarget->GetPos() - pActInfo->pEntity->GetPos(); //Shooter to target
						hit.targetId = pPotentialTarget->GetId();
						hit.shooterId = pActInfo->pEntity->GetId();

						already_hit.insert(hit.targetId);

						if(gEnv->bServer)
							g_pGame->GetGameRules()->ServerHit(hit);
					}
				}
			}
		}
	}
}

REGISTER_FLOW_NODE("HardNight:AI:States:CloseCombat", CFlowNode_AIStateCloseCombat);