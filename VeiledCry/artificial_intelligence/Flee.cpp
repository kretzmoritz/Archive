#include "Flee.h"
#include "../AIListener.h"

#define IDLE 0
#define QUERY 1
#define FLEE 2

int CFlowNode_AIStateFlee::instance_hide = 0;
int CFlowNode_AIStateFlee::instance_grid = 0;

CFlowNode_AIStateFlee::CFlowNode_AIStateFlee(SActivationInfo* pActInfo)
	: CFlowNode_AIStateBase(pActInfo, "hn_flee"), m_QueryHide(""), m_QueryGrid("")
{
}

IFlowNodePtr CFlowNode_AIStateFlee::Clone(SActivationInfo* pActInfo)
{
	CFlowNode_AIStateFlee* pTemp = new CFlowNode_AIStateFlee(pActInfo);
	pTemp->m_State = m_State;
	pTemp->m_QueryHide = m_QueryHide;
	pTemp->m_QueryGrid = m_QueryGrid;
	pTemp->m_TicketHide = m_TicketHide;
	pTemp->m_TicketGrid = m_TicketGrid;
	pTemp->m_Context = m_Context;
	return pTemp;
}

void CFlowNode_AIStateFlee::DerivedMemoryUsage(ICrySizer* pSizer) const
{
	SIZER_COMPONENT_NAME(pSizer, "CFlowNode_AIStateFlee");
	if(!pSizer->Add(this))
		return;

	pSizer->Add(m_State);
	pSizer->Add(m_QueryHide);
	pSizer->Add(m_QueryGrid);
	pSizer->Add(m_TicketHide);
	pSizer->Add(m_TicketGrid);
	pSizer->Add(m_Context);
	
	pSizer->Add(instance_hide);
	pSizer->Add(instance_grid);
}

void CFlowNode_AIStateFlee::GetCustomConfiguration(SFlowNodeConfig& config)
{
	static const SInputPortConfig in_config[] =
	{
		InputPortConfig<EntityId>("Threat", _HELP("Who are we fleeing from?")),
		InputPortConfig<float>("Speed", AISPEED_SPRINT, _HELP("Speed for the AI to flee with."), "Speed", "enum_float:Zero=0.0,Slow=0.21,Walk=0.4,Run=1.0,Sprint=1.4"),
		InputPortConfig<float>("StartDistance", 10.0f, _HELP("How close does our enemy need to be?")),
		InputPortConfig<float>("SearchRadius", 50.0f, _HELP("Area to sweep for hidespots.")),
		InputPortConfig<float>("ActorRadius", 1.0f, _HELP("Actor size (required to figure out valid hidespots).")),
		InputPortConfig<float>("HidespotDistMult", 1.5f, _HELP("StartDistance * HidespotDistMult = How far away do we require our hidespot to be?")),
		InputPortConfig<bool>("HideOnly", false, _HELP("Force HideOnly mode - find spot, then remove.")),
		InputPortConfig<bool>("Visible", false, _HELP("Visible by player - only used in HideOnly mode.")),
		{0}
	};

	static const SOutputPortConfig out_config[] =
	{
		OutputPortConfig<bool>("Flee", _HELP("Am I currently fleeing?")),
		{0}
	};

	config.sDescription = _HELP("AI flees from entity.");
	config.pInputPorts = in_config;
	config.pOutputPorts = out_config;
	config.nFlags = EFLN_TARGET_ENTITY | EFLN_ADVANCED;
}

// Set IDLE state, walk speed and actor radius
// Create queries, enable idle behavior (lookat)
void CFlowNode_AIStateFlee::OnBehaviorStart()
{
	m_State = IDLE;
	m_ActInfo.pEntity->GetAI()->CastToIAIActor()->SetSpeed(GetPortFloat(&m_ActInfo, INPORT_WALK_SPEED));
	m_Context.actorRadius = GetPortFloat(&m_ActInfo, INPORT_ACTOR_RADIUS);
	CreateQuery(m_QueryHide, "hidespots_from_attentionTarget", instance_hide);
	CreateQuery(m_QueryGrid, "grid", instance_grid);
	SetUpdates(true, &m_ActInfo);
	SetIdle();
}

// Disable all outstanding queries
// Disable idle behavior (lookat)
void CFlowNode_AIStateFlee::OnBehaviorEnd()
{
	gEnv->pAISystem->GetTacticalPointSystem()->CancelAsyncQuery(m_TicketHide);
	gEnv->pAISystem->GetTacticalPointSystem()->CancelAsyncQuery(m_TicketGrid);
	SetUpdates(false, &m_ActInfo);
	SetIdle(false);
}

void CFlowNode_AIStateFlee::OnActCancel()
{
	OnEndActGoto();
}

void CFlowNode_AIStateFlee::OnEndActGoto()
{
	ResetGoalpipe();

	m_State = IDLE;
	ActivateOutput(&m_ActInfo, OUTPORT_FLEE, false);
	SetIdle();
}

// Creates hide and grid queries based on input parameters
void CFlowNode_AIStateFlee::CreateQuery(std::string &_query, std::string _name, int &_instance)
{
	ITacticalPointSystem* pTPS = gEnv->pAISystem->GetTacticalPointSystem();

	if(_query == "")
	{
		char buffer[16];
		itoa(_instance, buffer, 10);
		_query = _name;
		_query += "_location_search";
		_query += buffer;
		
		int id = pTPS->GetQueryID(_query.c_str());
		while(id != 0)
		{
			_instance++;
			itoa(_instance, buffer, 10);
			_query = _name;
			_query += "_location_search";
			_query += buffer;
			id = pTPS->GetQueryID(_query.c_str());
		}

		_instance++;
	}
	else
	{
		int id = pTPS->GetQueryID(_query.c_str());
		pTPS->DestroyQueryID(id);
	}

	int id = pTPS->CreateQueryID(_query.c_str());
	pTPS->AddToGeneration(id, (_name + "_around_puppet").c_str(), GetPortFloat(&m_ActInfo, INPORT_SEARCH_RADIUS));
	pTPS->AddToWeights(id, "distance_from_puppet", -1.0f);
	pTPS->AddToConditions(id, "min_distance_to_attentionTarget", GetPortFloat(&m_ActInfo, INPORT_START_DISTANCE) * GetPortFloat(&m_ActInfo, INPORT_HIDESPOT_DIST_MULT));
	pTPS->AddToConditions(id, "canReachBefore_the_attentionTarget", true);
}

// Receives completed queries, if any results are returned - move AI to best position
// Was the query of type hide and no results found? Try to find a spot on the grid to flee towards.
// Should none of the above apply return to idle state
void CFlowNode_AIStateFlee::AcceptResults(bool bError, TPSQueryTicket nQueryTicket, STacticalPointResult* vResults, int nResults, int nOptionUsed)
{
	m_State = IDLE;

	if(nResults > 0)
	{
		IAISignalExtraData* exd = gEnv->pAISystem->CreateSignalExtraData();
		exd->point = vResults[0].vPos;
 		m_ActInfo.pEntity->GetAI()->CastToIAIActor()->SetSignal(AISIGNAL_DEFAULT, "ACT_GOTO", m_ActInfo.pEntity, exd);

		m_State = FLEE;
		ActivateOutput(&m_ActInfo, OUTPORT_FLEE, true);

		SetIdle(false);
	}
	else if(nQueryTicket == m_TicketHide)
		ForceAIFlee(m_QueryGrid, m_TicketGrid);
}

// Force the AI to flee if its threat is in a certain distance
void CFlowNode_AIStateFlee::OnUpdate(SActivationInfo* pActInfo)
{
	if(GetPortBool(&m_ActInfo, INPORT_HIDE_ONLY) && !GetPortBool(&m_ActInfo, INPORT_VISIBLE))
	{
		//gEnv->pEntitySystem->RemoveEntity(m_ActInfo.pEntity->GetId()); //Complete removal
		m_ActInfo.pEntity->Hide(true);
		OnBehaviorEnd();
	}

	if(m_State == IDLE && gEnv->pEntitySystem->GetEntity(GetPortEntityId(&m_ActInfo, INPORT_THREAT)))
	{
		if(gEnv->pEntitySystem->GetEntity(GetPortEntityId(&m_ActInfo, INPORT_THREAT))->GetPos().GetDistance(m_ActInfo.pEntity->GetPos()) <= GetPortFloat(pActInfo, INPORT_START_DISTANCE))
		{
			ForceAIFlee(m_QueryHide, m_TicketHide);
		}
	}
}

// Carry out the queries created above (using the correct AI and attention target position)
void CFlowNode_AIStateFlee::ForceAIFlee(std::string &_query, TPSQueryTicket &_ticket)
{
	ITacticalPointSystem* pTPS = gEnv->pAISystem->GetTacticalPointSystem();

	m_Context.pAIActor = m_ActInfo.pEntity->GetAI()->CastToIAIActor();
	m_Context.actorPos = m_ActInfo.pEntity->GetPos();

	m_Context.attentionTarget = gEnv->pEntitySystem->GetEntity(GetPortEntityId(&m_ActInfo, INPORT_THREAT))->GetPos();

	_ticket = pTPS->AsyncQuery(pTPS->GetQueryID(_query.c_str()), m_Context, eTPQT_GENERATOR, 15, this); //Maximum of 15 points are generated

	m_State = QUERY;
}

void CFlowNode_AIStateFlee::SetIdle(bool _state)
{
	CFlowNode_AIStateBase* p = AIListener::GetStateNode("hn_idle", m_ActInfo.pEntity->GetId());
	if(p != NULL)
		p->SetStateActive(_state);

	if(!_state)
		ResetGoalpipe();
}

REGISTER_FLOW_NODE("HardNight:AI:States:Flee", CFlowNode_AIStateFlee);