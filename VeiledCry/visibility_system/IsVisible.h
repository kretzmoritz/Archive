///////////////////////////////////////////////
// Code by Moritz Kretz
// Description: Allows entities to query the visibility system
// Outputs current view state
///////////////////////////////////////////////

#ifndef IS_VISIBLE_H
#define IS_VISIBLE_H

#include "stdafx.h"
#include "../../../Game/GameDll/Nodes/G2FlowBaseNode.h"
#include "../../Systems/VeiledCollective.h"
 
class CFlowNode_IsVisible : public CFlowBaseNode<eNCT_Instanced>
{
public:
	CFlowNode_IsVisible(SActivationInfo* pActInfo);
	void GetMemoryUsage(ICrySizer* pSizer) const;
	
	IFlowNodePtr Clone(SActivationInfo* pActInfo);
	
	void GetConfiguration(SFlowNodeConfig& config);
	void ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo);

private:
	enum InputPorts
	{
		INPORT_ENABLE_UPDATES,
		INPORT_QUERY_INTERVAL,
		INPORT_RAYCAST
	};

	enum OutputPorts
	{
		OUTPORT_VISIBLE
	};

	EntityId m_id;
	tick_t m_nextQueryTime;
	bool m_visible;
	bool m_previousVisible;

	void OnInit(SActivationInfo* pActInfo);
	void OnUpdate(SActivationInfo* pActInfo);
	void OnActivate(SActivationInfo* pActInfo);
	void OnSetEntityId(SActivationInfo* pActInfo);

	void SetUpdates(bool _state, SActivationInfo* pActInfo);
	void VisibilityCallback(CallbackParam _param);
};

#endif