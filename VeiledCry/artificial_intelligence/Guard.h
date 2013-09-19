///////////////////////////////////////////////
// Code by Moritz Kretz
// Description: AI guards a target
///////////////////////////////////////////////

#ifndef GUARD_H
#define GUARD_H

#include "../AIStateBase.h"

class CFlowNode_AIStateGuard : public CFlowNode_AIStateBase
{
public:
	CFlowNode_AIStateGuard(SActivationInfo* pActInfo);

	IFlowNodePtr Clone(SActivationInfo* pActInfo);
	void DerivedMemoryUsage(ICrySizer* pSizer) const;
	void GetCustomConfiguration(SFlowNodeConfig& config);

	void OnBehaviorStart();
	void OnBehaviorEnd();

	void OnActCancel();
	void OnEndActGoto();

protected:
	void OnUpdate(SActivationInfo* pActInfo);

private:
	enum InputPorts
	{
		INPORT_WALK_SPEED,
		INPORT_DELAY,
		INPORT_INSIDE_RADIUS,
		INPORT_OUTSIDE_RADIUS,
		INPORT_GUARDEE, //in my mind this is someone who is being guarded
	};

	void ForceAIGuard();
	void SetIdle(bool _state = true);

	int64 m_State;
};

#endif