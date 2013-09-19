#include "Guard.h"
#include "../AIListener.h"

#define GUARD -1

CFlowNode_AIStateGuard::CFlowNode_AIStateGuard(SActivationInfo* pActInfo)
	: CFlowNode_AIStateBase(pActInfo, "hn_guard")
{
}

IFlowNodePtr CFlowNode_AIStateGuard::Clone(SActivationInfo* pActInfo)
{
	CFlowNode_AIStateGuard* pTemp = new CFlowNode_AIStateGuard(pActInfo);
	pTemp->m_State = m_State;
	return pTemp;
}

void CFlowNode_AIStateGuard::DerivedMemoryUsage(ICrySizer* pSizer) const
{
	SIZER_COMPONENT_NAME(pSizer, "CFlowNode_AIStateRoam");
	if(!pSizer->Add(this))
		return;

	pSizer->Add(m_State);
}

void CFlowNode_AIStateGuard::GetCustomConfiguration(SFlowNodeConfig& config)
{
	static const SInputPortConfig in_config[] =
	{
		InputPortConfig<float>("Speed", AISPEED_WALK, _HELP("Speed for the AI to move with while guarding."), "Speed", "enum_float:Zero=0.0,Slow=0.21,Walk=0.4,Run=1.0,Sprint=1.4"),
		InputPortConfig<float>("Delay", 2.0f, _HELP("How long to idle before moving again.")),
		InputPortConfig<float>("InsideRadius", 2.0f, _HELP("Defines minimum amount our AI needs to move.")),
		InputPortConfig<float>("OutsideRadius", 10.0f, _HELP("Defines area in which our AI can move.")),
		InputPortConfig<EntityId>("Guardee", _HELP("Who is being guarded?")),
		{0}
	};

	static const SOutputPortConfig out_config[] =
	{
		{0}
	};

	config.sDescription = _HELP("AI guards a target.");
	config.pInputPorts = in_config;
	config.pOutputPorts = out_config;
	config.nFlags = EFLN_TARGET_ENTITY | EFLN_ADVANCED;
}

void CFlowNode_AIStateGuard::OnBehaviorStart()
{
	m_State = GUARD;

	SetUpdates(true, &m_ActInfo);
	ForceAIGuard();
}

void CFlowNode_AIStateGuard::OnBehaviorEnd()
{
	SetIdle(false);
	SetUpdates(false, &m_ActInfo);
}

void CFlowNode_AIStateGuard::OnActCancel()
{
	ResetGoalpipe();
	ForceAIGuard();
}

void CFlowNode_AIStateGuard::OnEndActGoto()
{
	ResetGoalpipe();
	m_State = CryGetTicks() + GetPortFloat(&m_ActInfo, INPORT_DELAY) * gEnv->pTimer->GetTicksPerSecond(); //Pause
	SetIdle();
}

void CFlowNode_AIStateGuard::OnUpdate(SActivationInfo* pActInfo)
{
	if(m_State != GUARD && CryGetTicks() >= m_State)
	{
		SetIdle(false);
		ForceAIGuard();
		m_State = GUARD;
	}
}

void CFlowNode_AIStateGuard::ForceAIGuard()
{
	IEntity* pTarget = gEnv->pEntitySystem->GetEntity(GetPortEntityId(&m_ActInfo, INPORT_GUARDEE));

	if(m_ActInfo.pEntity && m_ActInfo.pEntity->GetAI() && pTarget)
	{
		m_ActInfo.pEntity->GetAI()->CastToIAIActor()->SetSpeed(GetPortFloat(&m_ActInfo, INPORT_WALK_SPEED));
		IAISignalExtraData* exd = gEnv->pAISystem->CreateSignalExtraData();
		exd->point = pTarget->GetPos();
		float angle = (rand() / (float)RAND_MAX) * g_PI2; //0-(2PI)
		float range = GetPortFloat(&m_ActInfo, INPORT_INSIDE_RADIUS) + (rand() / (float)RAND_MAX) * GetPortFloat(&m_ActInfo, INPORT_OUTSIDE_RADIUS); //inside_radius-outside_radius
		exd->point.x += sinf(angle) * range;
		exd->point.y += cosf(angle) * range;
		m_ActInfo.pEntity->GetAI()->CastToIAIActor()->SetSignal(AISIGNAL_DEFAULT, "ACT_GOTO", m_ActInfo.pEntity, exd);
	}
}

void CFlowNode_AIStateGuard::SetIdle(bool _state)
{
	CFlowNode_AIStateBase* p = AIListener::GetStateNode("hn_idle", m_ActInfo.pEntity->GetId());
	if(p != NULL)
		p->SetStateActive(_state);

	if(!_state)
		ResetGoalpipe();
}

REGISTER_FLOW_NODE("HardNight:AI:States:Guard", CFlowNode_AIStateGuard);