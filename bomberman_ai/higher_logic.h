#ifndef HIGHER_LOGIC_H
#define HIGHER_LOGIC_H

#include "../gameplay/game_play.h"

#include "utility.h"
#include "rating_system.h"
#include "pathfinding.h"
#include "database.h"

#include <iostream>
#include <vector>

class HigherLogic
{
public:
	void update(CLocalState _local, CGameState _state);
	PlayerControlType getAction();

private:
	CLocalState m_local;
	CGameState m_state;

	Utility m_Utility;
	RatingSystem m_RatingSystem;
	Pathfinding m_Pathfinding;
	Database m_Database;

	bool foreseeBomb();
	void revert();
};

#endif