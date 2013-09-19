#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "../gameplay/game_state.h"
#include "../gameplay/game_state_io.h"

#include "utility.h"

#include <vector>
#include <set>
#include <algorithm>

class Pathfinding
{
public:
	void update(CLocalState _local, CGameState _state, std::vector<std::set<int>> _Cost);
	std::vector<position> astar(int _playerid, position _target);

private:
	CLocalState m_local;
	CGameState m_state;
	
	Utility m_Utility;

	std::vector<position> m_vOpenList;
	std::vector<position> m_vClosedList;
	std::vector<std::set<int>> m_vCost;

	void sort(std::vector<position> &_in);
	void ParentAndFCost(int _id, position _target);
};

#endif