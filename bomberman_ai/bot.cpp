#include "bot.h"

Bot::Bot(CLocalState* _local, CGameState* _state) :
m_local(_local), m_state(_state)
{
}

Bot::~Bot()
{
	delete m_local;
	delete m_state;
}

PlayerControlType Bot::getAction()
{
	m_HigherLogic.update(*m_local, *m_state);
	
	return m_HigherLogic.getAction();
}