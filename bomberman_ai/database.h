#ifndef DATABASE_H
#define DATABASE_H

#include "../gameplay/game_state.h"
#include "../gameplay/game_state_io.h"

#include "utility.h"
#include "pathfinding.h"

class Database
{
public:
	void update(CLocalState _local, CGameState _state, std::vector<std::set<int>> _MovementCost, std::vector<std::set<int>> _DangerCost, std::vector<std::set<int>> _DangerCostFuture, std::vector<std::set<int>> _GCost, std::vector<int> _LoopDetection, int _fullLifetime);
	
	bool isDanger();
	bool isSafeToPlaceBomb();
	bool isSafeToWalk(position _path);

	position getClosestSafeSpot(int _preview = 0);
	position getClosestUpgrade();
	position getClosestEnemy();

private:
	CLocalState m_local;
	CGameState m_state;

	Utility m_Utility;
	Pathfinding m_Pathfinding;

	std::vector<std::set<int>> m_vMovementCost;
	std::vector<std::set<int>> m_vDangerCost;
	std::vector<std::set<int>> m_vDangerCostFuture;
	std::vector<std::set<int>> m_vGCost;
	std::vector<int> m_vLoopDetection;

	int m_iFullLifetime;

	void sortByDanger(std::vector<position> &_in, int _preview);
	void sortByDistance(std::vector<position> &_in);
	bool isNextToPlayer(position _tile);
};

#endif