#include "Roam.h"
#include "../AIListener.h"

#define ROAM -1

CFlowNode_AIStateRoam::CFlowNode_AIStateRoam(SActivationInfo* pActInfo)
	: CFlowNode_AIStateBase(pActInfo, "hn_roam")
{
}

IFlowNodePtr CFlowNode_AIStateRoam::Clone(SActivationInfo* pActInfo)
{
	CFlowNode_AIStateRoam* pTemp = new CFlowNode_AIStateRoam(pActInfo);
	pTemp->m_State = m_State;
	pTemp->m_StartPos = m_StartPos;
	return pTemp;
}

void CFlowNode_AIStateRoam::DerivedMemoryUsage(ICrySizer* pSizer) const
{
	SIZER_COMPONENT_NAME(pSizer, "CFlowNode_AIStateRoam");
	if(!pSizer->Add(this))
		return;

	pSizer->Add(m_State);
	pSizer->Add(m_StartPos);
}

void CFlowNode_AIStateRoam::GetCustomConfiguration(SFlowNodeConfig& config)
{
	static const SInputPortConfig in_config[] =
	{
		InputPortConfig<float>("Speed", AISPEED_WALK, _HELP("Speed for the AI to roam with."), "Speed", "enum_float:Zero=0.0,Slow=0.21,Walk=0.4,Run=1.0,Sprint=1.4"),
		InputPortConfig<float>("Delay", 2.0f, _HELP("How long to idle before moving again.")),
		InputPortConfig<float>("InsideRadius", 2.0f, _HELP("Defines minimum amount our AI needs to move.")),
		InputPortConfig<float>("OutsideRadius", 10.0f, _HELP("Defines area in which our AI can roam.")),
		InputPortConfig<bool>("Guard", true, _HELP("Forces the AI to guard its starting position. Will roam freely otherwise.")),
		{0}
	};

	static const SOutputPortConfig out_config[] =
	{
		{0}
	};

	config.sDescription = _HELP("AI wanders around aimlessly.");
	config.pInputPorts = in_config;
	config.pOutputPorts = out_config;
	config.nFlags = EFLN_TARGET_ENTITY | EFLN_ADVANCED;
}

void CFlowNode_AIStateRoam::OnBehaviorStart()
{
	m_State = ROAM;
	m_StartPos = m_ActInfo.pEntity->GetPos();

	SetUpdates(true, &m_ActInfo);
	ForceAIRoam();
}

void CFlowNode_AIStateRoam::OnBehaviorEnd()
{
	SetIdle(false);
	SetUpdates(false, &m_ActInfo);
}

void CFlowNode_AIStateRoam::OnActCancel()
{
	ResetGoalpipe();
	ForceAIRoam();
}

void CFlowNode_AIStateRoam::OnEndActGoto()
{
	ResetGoalpipe();
	m_State = CryGetTicks() + GetPortFloat(&m_ActInfo, INPORT_DELAY) * gEnv->pTimer->GetTicksPerSecond(); //Pause
	SetIdle();
}

void CFlowNode_AIStateRoam::OnUpdate(SActivationInfo* pActInfo)
{
	if(m_State != ROAM && CryGetTicks() >= m_State)
	{
		SetIdle(false);
		ForceAIRoam();
		m_State = ROAM;
	}
}

void CFlowNode_AIStateRoam::ForceAIRoam()
{
	if(m_ActInfo.pEntity && m_ActInfo.pEntity->GetAI())
	{
		m_ActInfo.pEntity->GetAI()->CastToIAIActor()->SetSpeed(GetPortFloat(&m_ActInfo, INPORT_WALK_SPEED));
		IAISignalExtraData* exd = gEnv->pAISystem->CreateSignalExtraData();
		exd->point = GetPortBool(&m_ActInfo, INPORT_GUARD) ? m_StartPos : m_ActInfo.pEntity->GetPos();
		float angle = (rand() / (float)RAND_MAX) * g_PI2; //0-(2PI)
		float range = GetPortFloat(&m_ActInfo, INPORT_INSIDE_RADIUS) + (rand() / (float)RAND_MAX) * GetPortFloat(&m_ActInfo, INPORT_OUTSIDE_RADIUS); //inside_radius-outside_radius
		exd->point.x += sinf(angle) * range;
		exd->point.y += cosf(angle) * range;
		m_ActInfo.pEntity->GetAI()->CastToIAIActor()->SetSignal(AISIGNAL_DEFAULT, "ACT_GOTO", m_ActInfo.pEntity, exd);
	}
}

void CFlowNode_AIStateRoam::SetIdle(bool _state)
{
	CFlowNode_AIStateBase* p = AIListener::GetStateNode("hn_idle", m_ActInfo.pEntity->GetId());
	if(p)
		p->SetStateActive(_state);

	if(!_state)
		ResetGoalpipe();
}

REGISTER_FLOW_NODE("HardNight:AI:States:Roam", CFlowNode_AIStateRoam);