///////////////////////////////////////////////
// Code by Moritz Kretz
// Description: AI flees from entity
///////////////////////////////////////////////

#ifndef FLEE_H
#define FLEE_H

#include "../AIStateBase.h"
#include "ITacticalPointSystem.h"

class CFlowNode_AIStateFlee : public CFlowNode_AIStateBase, public ITacticalPointResultsReceiver
{
public:
	CFlowNode_AIStateFlee(SActivationInfo* pActInfo);

	IFlowNodePtr Clone(SActivationInfo* pActInfo);
	void DerivedMemoryUsage(ICrySizer* pSizer) const;
	void GetCustomConfiguration(SFlowNodeConfig& config);

	void OnBehaviorStart();
	void OnBehaviorEnd();

	void OnActCancel();
	void OnEndActGoto();

	void AcceptResults(bool bError, TPSQueryTicket nQueryTicket, STacticalPointResult* vResults, int nResults, int nOptionUsed);

protected:
	void OnUpdate(SActivationInfo* pActInfo);

private:
	enum InputPorts
	{
		INPORT_THREAT,
		INPORT_WALK_SPEED,
		INPORT_START_DISTANCE,
		INPORT_SEARCH_RADIUS,
		INPORT_ACTOR_RADIUS,
		INPORT_HIDESPOT_DIST_MULT,
		INPORT_HIDE_ONLY,
		INPORT_VISIBLE
	};

	enum OutputPort
	{
		OUTPORT_FLEE
	};

	void CreateQuery(std::string &_query, std::string _name, int &_instance);
	void ForceAIFlee(std::string &_query, TPSQueryTicket &_ticket);
	void SetIdle(bool _state = true);

	int m_State;
	std::string m_QueryHide;
	std::string m_QueryGrid;
	TPSQueryTicket m_TicketHide;
	TPSQueryTicket m_TicketGrid;
	QueryContext m_Context;
	
	static int instance_hide;
	static int instance_grid;
};

#endif