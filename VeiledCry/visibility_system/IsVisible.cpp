#include "IsVisible.h"

CFlowNode_IsVisible::CFlowNode_IsVisible(SActivationInfo* pActInfo)
{

}

void CFlowNode_IsVisible::GetMemoryUsage(ICrySizer* pSizer) const
{
	SIZER_COMPONENT_NAME(pSizer, "CFlowNode_IsVisible");
	if(!pSizer->Add(this))
		return;
	
	pSizer->Add(m_id);
	pSizer->Add(m_nextQueryTime);
	pSizer->Add(m_visible);
	pSizer->Add(m_previousVisible);
}

IFlowNodePtr CFlowNode_IsVisible::Clone(SActivationInfo* pActInfo)
{
	CFlowNode_IsVisible* pTemp = new CFlowNode_IsVisible(pActInfo);
	pTemp->m_id = m_id;
	pTemp->m_nextQueryTime = m_nextQueryTime;
	pTemp->m_visible = m_visible;
	pTemp->m_previousVisible = m_previousVisible;
	return pTemp;
}

void CFlowNode_IsVisible::GetConfiguration(SFlowNodeConfig& config)
{
	static const SInputPortConfig in_config[] =
	{
		InputPortConfig<bool>("EnableUpdates", _HELP("Enable node updates.")),
		InputPortConfig<float>("QueryInterval", _HELP("Time spacing between each query.")),
		InputPortConfig<bool>("Raycast", _HELP("Enable occlusion testing for this entity.")),
		SInputPortConfig()
	};

	static const SOutputPortConfig out_config[] =
	{
		OutputPortConfig<bool>("Visible", _HELP("Is this entity visible?")),
		SOutputPortConfig()
	};

	config.sDescription = _HELP("Allows entity to query the visibility system to determine view state.");
	config.pInputPorts = in_config;
	config.pOutputPorts = out_config;
	config.nFlags = EFLN_TARGET_ENTITY | EFLN_ADVANCED;
}

void CFlowNode_IsVisible::ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
{
	switch(event)
	{
	case eFE_Initialize:
		OnInit(pActInfo);
		break;

	case eFE_Update:
		OnUpdate(pActInfo);
		break;

	case eFE_Activate:
		OnActivate(pActInfo);
		break;

	case eFE_SetEntityId:
		OnSetEntityId(pActInfo);
		break;
	};
}

void CFlowNode_IsVisible::OnInit(SActivationInfo* pActInfo)
{
	if(pActInfo->pEntity)
	{
		m_id = pActInfo->pEntity->GetId();

		VeiledCollective::get()->VisibilitySystem.Query(m_id, GetPortBool(pActInfo, INPORT_RAYCAST), NULL, this, &CFlowNode_IsVisible::VisibilityCallback);
		m_nextQueryTime = CryGetTicks() + GetPortFloat(pActInfo, INPORT_QUERY_INTERVAL) * gEnv->pTimer->GetTicksPerSecond();

		m_visible = false;
		m_previousVisible = false;

		SetUpdates(GetPortBool(pActInfo, INPORT_ENABLE_UPDATES), pActInfo);
	}
}

void CFlowNode_IsVisible::OnUpdate(SActivationInfo* pActInfo)
{
	if(m_previousVisible != m_visible)
	{
		ActivateOutput(pActInfo, OUTPORT_VISIBLE, m_visible);
		m_previousVisible = m_visible;
	}

	if(CryGetTicks() >= m_nextQueryTime)
	{
		VeiledCollective::get()->VisibilitySystem.Query(m_id, GetPortBool(pActInfo, INPORT_RAYCAST), NULL, this, &CFlowNode_IsVisible::VisibilityCallback);
		m_nextQueryTime = CryGetTicks() + GetPortFloat(pActInfo, INPORT_QUERY_INTERVAL) * gEnv->pTimer->GetTicksPerSecond();
	}
}

void CFlowNode_IsVisible::OnActivate(SActivationInfo* pActInfo)
{
	if(IsPortActive(pActInfo, INPORT_ENABLE_UPDATES))
		SetUpdates(GetPortBool(pActInfo, INPORT_ENABLE_UPDATES) && pActInfo->pEntity, pActInfo);
}

void CFlowNode_IsVisible::OnSetEntityId(SActivationInfo* pActInfo)
{
	if(m_id != pActInfo->pEntity->GetId())
	{
		SetUpdates(false, pActInfo);
		OnInit(pActInfo);
	}
}

void CFlowNode_IsVisible::SetUpdates(bool _state, SActivationInfo* pActInfo)
{
	pActInfo->pGraph->SetRegularlyUpdated(pActInfo->myID, _state);
}

void CFlowNode_IsVisible::VisibilityCallback(CallbackParam _param)
{
	if(m_id == _param.id)
		m_visible = _param.visible;
}

REGISTER_FLOW_NODE("Veiled:Sensors:IsVisible", CFlowNode_IsVisible);