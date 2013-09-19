///////////////////////////////////////////////
// Code by Moritz Kretz
// Description: AI melee fighting style
///////////////////////////////////////////////

#ifndef CLOSE_COMBAT_H
#define CLOSE_COMBAT_H

#include "../AIStateBase.h"
#include "../../Anim/AnimData.h"

class CFlowNode_AIStateCloseCombat : public CFlowNode_AIStateBase
{
public:
	CFlowNode_AIStateCloseCombat(SActivationInfo* pActInfo);

	IFlowNodePtr Clone(SActivationInfo* pActInfo);
	void DerivedMemoryUsage(ICrySizer* pSizer) const;
	void GetCustomConfiguration(SFlowNodeConfig& config);

	void OnBehaviorStart();
	void OnBehaviorEnd();

	void OnHitReaction(const HitInfo& _hitinfo, float _causedDamage, const SReactionParams& _reactionParams, bool _isTarget);
	void OnHitReactionEnd();

protected:
	void OnUpdate(SActivationInfo* pActInfo);
	void OnActivate(SActivationInfo* pActInfo);

private:
	struct bone_data
	{
		int index;
		QuatT quat;
		Vec3 pos;
		Vec3 pos_prev;
		int64 time;
	};

	enum InputPorts
	{
		INPORT_TARGET,
		INPORT_WALK_SPEED,
		INPORT_RANGE,
		INPORT_DISABLE_RANGE,
		INPORT_ANIMATION_0,
		INPORT_ANIMATION_1,
		INPORT_ANIMATION_2,
		INPORT_ANIMATION_3,
		INPORT_ANIMATION_4,
		INPORT_ANIMATION_5,
		INPORT_ANIM_START,
		INPORT_ANIM_COMPLETE,
		INPORT_DEBUG_SPEED,
		INPORT_DEBUG_ANGLE,
		INPORT_DEBUG_HIT_REACTION
	};

	enum OutputPorts
	{
		OUTPORT_SYNC,
		OUTPORT_ANIM
	};

	int m_State;
	bool m_InHitReaction;
	bool m_StateChanged;
	bool m_OrientEnabled;
	int m_CurrentlyActiveAnimation;
	Vec3 m_HitReactionEndVel;
	std::vector<bone_data> m_Bones;
	std::set<EntityId> already_hit;
	std::vector<const CFlowNode_AnimData::attack_data*> m_Animations;

	float CalculateAngle(SActivationInfo* pActInfo);
	bool IsAngleValid(int _index, float _degrees);
	void ChooseCorrectAnim(SActivationInfo* pActInfo);
	void OnHit(SActivationInfo* pActInfo);
	void OnAnimationStart();
	void OnAnimationComplete();
};

#endif