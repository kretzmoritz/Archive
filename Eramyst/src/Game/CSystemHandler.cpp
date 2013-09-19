#include "..\..\include\Game\CSystemHandler.h"

void CSystemHandler::init()
{
	ci_getState = new CColossNormal();
	ci_system_normal = new CSystemNormal();
	ci_system_ragemode = new CSystemRagemode();

	CSystem::init();

	setCurrentState(ci_system_normal); // Used for data access and calling the correct functions based on state
	ci_currentState->start();
	m_iNextState = RAGEMODE_SYSTEM;
}

void CSystemHandler::reset()
{
	m_bColossConstructed = false;
	m_iNextState = NORMAL_SYSTEM;
}

void CSystemHandler::setCurrentState(CSystem *_state)
{
	ci_currentState = _state;
}

void CSystemHandler::update()
{
	decideOnState();
	ci_currentState->run();
}

void CSystemHandler::decideOnState()
{
	if(ci_getState->getHealth() <= 30.0f && m_iNextState == RAGEMODE_SYSTEM)
	{
		ci_currentState->close();
		setCurrentState(ci_system_ragemode);
		ci_currentState->start();
		ci_currentState->setNormalMode(false);
		m_iNextState = NORMAL_SYSTEM;
	}
	else if(ci_getState->getHealth() > 30.0f && m_iNextState == NORMAL_SYSTEM)
	{
		ci_currentState->close();
		setCurrentState(ci_system_normal);
		ci_currentState->start();
		ci_currentState->setNormalMode(true);
		m_iNextState = RAGEMODE_SYSTEM;
	}

	// Boss
	if(ci_system_normal->vBoss.size() == 0 && m_bColossConstructed)
	{
		CColoss::setBoss(false);
		CColoss::setBossFinished(true);
	}

	if(!m_bColossConstructed && CColoss::getBoss())
	{
		CSystem::constructBoss();
		m_bColossConstructed = true;
	}
	else if(m_bColossConstructed && !CColoss::getBoss())
	{
		CSystem::destructBoss();
		m_bColossConstructed = false;
	}
}