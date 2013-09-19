///////////////////////////////////////////////
// Code by Moritz Kretz
// Description: AI wanders around aimlessly
///////////////////////////////////////////////

#ifndef ROAM_H
#define ROAM_H

#include "../AIStateBase.h"

class CFlowNode_AIStateRoam : public CFlowNode_AIStateBase
{
public:
	CFlowNode_AIStateRoam(SActivationInfo* pActInfo);

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
		INPORT_GUARD
	};

	void ForceAIRoam();
	void SetIdle(bool _state = true);

	int64 m_State;
	Vec3 m_StartPos;
};

#endif