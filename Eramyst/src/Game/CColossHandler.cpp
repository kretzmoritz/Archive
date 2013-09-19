#include "..\..\include\Game\CColossHandler.h"

void CColossHandler::init()
{
	ci_coloss_normal = new CColossNormal();
	ci_coloss_ragemode = new CColossRagemode();

	CColoss::init();

	setCurrentState(ci_coloss_normal); // Used for data access and calling the correct functions based on state
	ci_currentState->start();
	m_iNextState = RAGEMODE_COLOSS;
}

void CColossHandler::reset()
{
	m_iNextState = NORMAL_COLOSS;
}

void CColossHandler::setCurrentState(CColoss *_state)
{
	ci_currentState = _state;
}

void CColossHandler::update()
{
	decideOnState();
	ci_currentState->run();
}

void CColossHandler::decideOnState()
{
	if(ci_currentState->getHealth() <= 30.0f && m_iNextState == RAGEMODE_COLOSS)
	{
		ci_currentState->close();
		setCurrentState(ci_coloss_ragemode);
		ci_currentState->start();
		m_iNextState = NORMAL_COLOSS;
	}
	else if(ci_currentState->getHealth() > 30.0f && m_iNextState == NORMAL_COLOSS)
	{
		ci_currentState->close();
		setCurrentState(ci_coloss_normal);
		ci_currentState->start();
		m_iNextState = RAGEMODE_COLOSS;
	}
}