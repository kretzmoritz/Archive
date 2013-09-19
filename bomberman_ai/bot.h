#ifndef BOT_H
#define BOT_H

#include "../gameplay/game_play.h"
#include "../gameplay/game_state.h"
#include "../gameplay/game_state_io.h"

#include "higher_logic.h"

class Bot
{
public:
	Bot(CLocalState* _local, CGameState* _state);
	~Bot();

	PlayerControlType getAction();

private:
	CLocalState* m_local;
	CGameState* m_state;

	HigherLogic m_HigherLogic;
};

#endif