#ifndef RATING_SYSTEM_H
#define RATING_SYSTEM_H

#include "../gameplay/game_state.h"
#include "../gameplay/game_state_io.h"

#include "utility.h"
#include "pathfinding.h"

#include <vector>
#include <set>
#include <iostream>

class RatingSystem
{
public:
	RatingSystem();
	void update(CLocalState _local, CGameState _state, int _x = -1, int _y = -1);
	
	int getFullLifetime();

	std::vector<std::set<int>> getMovementCostVector();
	std::vector<std::set<int>> getDangerCostVector();
	std::vector<std::set<int>> getDangerCostFutureVector();
	std::vector<std::set<int>> getGCostVector();
	std::vector<int> getLoopDetectionVector();

	void modLoopDetection(int _x, int _y, int _value);

private:
	CLocalState m_local;
	CGameState m_state;

	Utility m_Utility;
	Pathfinding m_Pathfinding;

	static std::vector<int> m_vLoopDetection;

	std::vector<std::set<int>> m_vMovementCost;
	std::vector<std::set<int>> m_vDangerCost;
	std::vector<std::set<int>> m_vDangerCost_Future;
	std::vector<position> m_vSizeUpgrades;

	int m_iFullLifetime;
	bool m_bLifetimeSet;

	void calculateFire();
	void calculateMovement();
	void calculateDanger();
	void calculateTraps();

	void calculateDangerFuture(int _x, int _y);
	
	bool getSizeUpgradeReachable(int _playerid, position _bomb, int _index);

	void set(int _x, int _y, int _value, std::vector<std::set<int>> &_vector);
	int get(int _x, int _y, std::vector<std::set<int>> &_vector);

	void sort(std::vector<position> &_in);
	bool isNextToPlayer(position _tile);
};

#endif