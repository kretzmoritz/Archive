#include "database.h"

void Database::update(CLocalState _local, CGameState _state, std::vector<std::set<int>> _MovementCost, std::vector<std::set<int>> _DangerCost, std::vector<std::set<int>> _DangerCostFuture, std::vector<std::set<int>> _GCost, std::vector<int> _LoopDetection, int _fullLifetime)
{
	m_local = _local;
	m_state = _state;

	m_vMovementCost = _MovementCost;
	m_vDangerCost = _DangerCost;
	m_vDangerCostFuture = _DangerCostFuture;
	m_vGCost = _GCost;
	m_vLoopDetection = _LoopDetection;

	m_iFullLifetime = _fullLifetime;

	m_Utility.update(_local, _state);
	m_Pathfinding.update(_local, _state, _GCost);
}

bool Database::isDanger()
{
	bool bDanger = false;
	std::set<int> temp_set = m_vDangerCost[m_Utility.getPlayerPosition(m_local.m_PlayerNr).x + m_Utility.getPlayerPosition(m_local.m_PlayerNr).y * m_state.m_LevelGrid.Width()];

	for(std::set<int>::iterator it = temp_set.begin(); it != temp_set.end(); it++)
	{
		if(*it > 2)
		{
			bDanger = true;
			break;
		}
	}

	return bDanger;
}

bool Database::isSafeToPlaceBomb()
{
	bool bSafe = false;
	position temp = getClosestSafeSpot(1);
	
	if(temp.x != -1 && temp.y != -1)
	{
		bSafe = true;
		std::set<int> temp_set = m_vDangerCostFuture[temp.x + temp.y * m_state.m_LevelGrid.Width()];

		for(std::set<int>::iterator it = temp_set.begin(); it != temp_set.end(); it++)
		{
			if(*it > 2)
			{
				bSafe = false;
				break;
			}
		}
	}
	
	return bSafe;
}

bool Database::isSafeToWalk(position _path)
{
	bool bSafe = true;
	std::set<int> temp_set = m_vDangerCost[_path.x + _path.y * m_state.m_LevelGrid.Width()];

	for(std::set<int>::iterator it = temp_set.begin(); it != temp_set.end(); it++)
	{
		if(m_iFullLifetime - *it == 1)
		{
			bSafe = false;
			break;
		}
	}

	return bSafe;
}

position Database::getClosestSafeSpot(int _preview)
{
	m_Pathfinding.update(m_local, m_state, m_vMovementCost); //Switch to MovementCost so danger can't interfere

	std::vector<position> safespots;

	for(unsigned int i = 0; i < m_state.m_LevelGrid.Width(); i++)
	{
		for(unsigned int j = 0; j < m_state.m_LevelGrid.Height(); j++)
		{
			if(m_state.m_LevelGrid.Get(i, j) != TILE_TYPE_ROCK && m_state.m_LevelGrid.Get(i, j) != TILE_TYPE_WALL)
			{
				std::vector<position> path = m_Pathfinding.astar(m_local.m_PlayerNr, position(i, j));
				
				if(path[0].x != -1 && path[0].y != -1) //Account for inaccessible locations
				{
					bool bUnwalkable = false;
					for(unsigned int k = 0; k < path.size(); k++)
					{
						bool bMovingThroughDanger = false;
						std::set<int> temp_set = _preview > 0 ? m_vDangerCostFuture[path[k].x + path[k].y * m_state.m_LevelGrid.Width()] : m_vDangerCost[path[k].x + path[k].y * m_state.m_LevelGrid.Width()];

						for(std::set<int>::iterator it = temp_set.begin(); it != temp_set.end(); it++)
						{
							if(*it > 2 && m_iFullLifetime - *it - k == 0) //Is the path to safety deadly? Will I die while dodging from bombs?
							{
								bMovingThroughDanger = true;
								break;
							}
						}

						bool bFire = false;
						temp_set.clear();
						temp_set = m_vDangerCost[path[k].x + path[k].y * m_state.m_LevelGrid.Width()];
						for(std::set<int>::iterator it = temp_set.begin(); it != temp_set.end(); it++)
						{
							if(*it == 2)
							{
								bFire = true;
								break;
							}
						}

						bool bAlive_0 = !(!isNextToPlayer(path[k]) && bFire); //Only walkable if NOT next to player and AFFECTED by fire; Explosion = 2; Rock has already been removed
						bool bAlive_FutureException = !bFire;

						bFire = false;
						temp_set = m_vDangerCostFuture[path[k].x + path[k].y * m_state.m_LevelGrid.Width()];
						for(std::set<int>::iterator it = temp_set.begin(); it != temp_set.end(); it++)
						{
							if(*it == 2)
							{
								bFire = true;
								break;
							}
						}

						bool bAlive_1 = !(!isNextToPlayer(path[k]) && bFire);

						if(_preview == 0 && ((m_state.m_LevelGrid.Get(path[k].x, path[k].y) == TILE_TYPE_ROCK && bAlive_0) || bMovingThroughDanger))
						{
							bUnwalkable = true;
							break;
						}
						else if(_preview == 1 && ((m_state.m_LevelGrid.Get(path[k].x, path[k].y) == TILE_TYPE_ROCK && bAlive_FutureException && bAlive_1) || bMovingThroughDanger))
						{
							bUnwalkable = true;
							break;
						}
					}

					std::set<int>::reverse_iterator rit = _preview > 0 ? m_vDangerCostFuture[m_Utility.getPlayerPosition(m_local.m_PlayerNr).x + m_Utility.getPlayerPosition(m_local.m_PlayerNr).y * m_state.m_LevelGrid.Width()].rbegin() : m_vDangerCost[m_Utility.getPlayerPosition(m_local.m_PlayerNr).x + m_Utility.getPlayerPosition(m_local.m_PlayerNr).y * m_state.m_LevelGrid.Width()].rbegin();
					
					if(!bUnwalkable && (int)(path.size() - 1) <= (m_iFullLifetime - *rit))
					{
						safespots.push_back(position(i, j));
						safespots[safespots.size() - 1].iDistanceToPlayer = path.size() - 1;
					}
				}
			}
		}
	}

	if(safespots.size() > 0)
	{
		sortByDanger(safespots, _preview);

		std::vector<position> safespots_danger;
		for(unsigned int i = 0; i < safespots.size(); i++)
		{
			std::set<int>::reverse_iterator rit_zero = _preview > 0 ? m_vDangerCostFuture[safespots[0].x + safespots[0].y * m_state.m_LevelGrid.Width()].rbegin() : m_vDangerCost[safespots[0].x + safespots[0].y * m_state.m_LevelGrid.Width()].rbegin();
			std::set<int>::reverse_iterator rit_i = _preview > 0 ? m_vDangerCostFuture[safespots[i].x + safespots[i].y * m_state.m_LevelGrid.Width()].rbegin() : m_vDangerCost[safespots[i].x + safespots[i].y * m_state.m_LevelGrid.Width()].rbegin();

			if(*rit_zero == *rit_i)
			{
				safespots_danger.push_back(safespots[i]);
			}
		}

		sortByDistance(safespots_danger);

		std::vector<position> safespots_final;
		for(unsigned int i = 0; i < safespots_danger.size(); i++)
		{
			if(safespots_danger[0].iDistanceToPlayer == safespots_danger[i].iDistanceToPlayer)
			{
				safespots_final.push_back(safespots_danger[i]);
			}
		}

		m_Pathfinding.update(m_local, m_state, m_vGCost); //Revert to GCost

		for(unsigned int i = 0; i < safespots_final.size(); i++) //Dodge towards closest upgrade
		{
			safespots_final[i].iDistanceToPlayer = (int)sqrt(pow((float)(safespots_final[i].x - getClosestUpgrade().x), 2) + pow((float)(safespots_final[i].y - getClosestUpgrade().y), 2));
		}

		sortByDistance(safespots_final);

		return safespots_final[0];
	}

	m_Pathfinding.update(m_local, m_state, m_vGCost); //Revert to GCost
	return position(-1, -1);
}

position Database::getClosestUpgrade()
{
	std::vector<position> upgrades;

	for(unsigned int i = 0; i < m_state.m_LevelGrid.Width(); i++)
	{
		for(unsigned int j = 0; j < m_state.m_LevelGrid.Height(); j++)
		{
			if(m_state.m_LevelGrid.Get(i, j) == TILE_TYPE_UPNR || m_state.m_LevelGrid.Get(i, j) == TILE_TYPE_UPSZ)
			{
				std::vector<position> path = m_Pathfinding.astar(m_local.m_PlayerNr, position(i, j));
				
				if(path.size() > 1) //Account for removal delay and inaccessible upgrades
				{
					upgrades.push_back(position(i, j));
					upgrades[upgrades.size() - 1].iDistanceToPlayer = path.size() - 1;

					for(unsigned int k = 1; k < path.size(); k++) //Start path at first waypoint - NOT where we're currently standing
					{
						upgrades[upgrades.size() - 1].iDistanceToPlayer += m_vLoopDetection[path[k].x + path[k].y * m_state.m_LevelGrid.Width()];

						bool bFire = false;
						std::set<int> temp_set = m_vDangerCost[path[k].x + path[k].y * m_state.m_LevelGrid.Width()];
						for(std::set<int>::iterator it = temp_set.begin(); it != temp_set.end(); it++)
						{
							if(*it == 2)
							{
								bFire = true;
								break;
							}
						}

						if(m_state.m_LevelGrid.Get(path[k].x, path[k].y) == TILE_TYPE_ROCK && !(!isNextToPlayer(path[k]) && bFire))
						{
							upgrades[upgrades.size() - 1].iDistanceToPlayer += 4;
						}
						else if(m_state.m_LevelGrid.Get(path[k].x, path[k].y) == TILE_TYPE_UPSZ || m_state.m_LevelGrid.Get(path[k].x, path[k].y) == TILE_TYPE_UPNR) //Is there an upgrade directly in our path? Take it into account...
						{
							upgrades[upgrades.size() - 1].iDistanceToPlayer--;
						}
					}
				}
			}
		}
	}

	if(upgrades.size() > 0)
	{
		sortByDistance(upgrades);
		return upgrades[0];
	}

	return position(-1, -1);
}

position Database::getClosestEnemy()
{
	std::vector<position> enemies;

	for(unsigned int i = 0; i < m_state.m_LevelGrid.Width(); i++)
	{
		for(unsigned int j = 0; j < m_state.m_LevelGrid.Height(); j++)
		{
			for(unsigned int k = 0; k < m_state.m_UnitManager.GetAllUnitsOnTile(i, j).size(); k++)
			{
				if(m_state.m_UnitManager.GetAllUnitsOnTile(i, j)[k].m_Type == UNIT_TYPE_HERO && m_state.m_UnitManager.GetAllUnitsOnTile(i, j)[k].m_Owner != m_local.m_PlayerNr)
				{
					std::vector<position> path = m_Pathfinding.astar(m_local.m_PlayerNr, position(i, j));
					enemies.push_back(position(i, j));
					enemies[enemies.size() - 1].iDistanceToPlayer = path.size() - 1;

					for(unsigned int h = 1; h < path.size(); h++)
					{
						enemies[enemies.size() - 1].iDistanceToPlayer += m_vLoopDetection[path[h].x + path[h].y * m_state.m_LevelGrid.Width()];

						bool bFire = false;
						std::set<int> temp_set = m_vDangerCost[path[h].x + path[h].y * m_state.m_LevelGrid.Width()];
						for(std::set<int>::iterator it = temp_set.begin(); it != temp_set.end(); it++)
						{
							if(*it == 2)
							{
								bFire = true;
								break;
							}
						}

						if(m_state.m_LevelGrid.Get(path[h].x, path[h].y) == TILE_TYPE_ROCK && !(!isNextToPlayer(path[h]) && bFire))
						{
							enemies[enemies.size() - 1].iDistanceToPlayer += 4;
						}
						else if(m_state.m_LevelGrid.Get(path[h].x, path[h].y) == TILE_TYPE_UPSZ || m_state.m_LevelGrid.Get(path[h].x, path[h].y) == TILE_TYPE_UPNR) //Is there an upgrade directly in our path? Take it into account...
						{
							enemies[enemies.size() - 1].iDistanceToPlayer--;
						}
					}
				}
			}
		}
	}

	if(enemies.size() > 0)
	{
		sortByDistance(enemies);
		return enemies[0];
	}

	return position(-1, -1);
}

void Database::sortByDanger(std::vector<position> &_in, int _preview)
{
	int id = 0;
	for(unsigned int i = 1; i < _in.size(); i++)
	{
		std::set<int>::reverse_iterator rit_id = _preview > 0 ? m_vDangerCostFuture[_in[id].x + _in[id].y * m_state.m_LevelGrid.Width()].rbegin() : m_vDangerCost[_in[id].x + _in[id].y * m_state.m_LevelGrid.Width()].rbegin();
		std::set<int>::reverse_iterator rit_i = _preview > 0 ? m_vDangerCostFuture[_in[i].x + _in[i].y * m_state.m_LevelGrid.Width()].rbegin() : m_vDangerCost[_in[i].x + _in[i].y * m_state.m_LevelGrid.Width()].rbegin();

		id = *rit_id < *rit_i ? id : i;
	}

	position temp;
	temp = _in[id];
	_in[id] = _in[0];
	_in[0] = temp;
}

void Database::sortByDistance(std::vector<position> &_in)
{
	int id = 0;
	for(unsigned int i = 1; i < _in.size(); i++)
	{
		id = _in[id].iDistanceToPlayer < _in[i].iDistanceToPlayer ? id : i;
	}

	position temp;
	temp = _in[id];
	_in[id] = _in[0];
	_in[0] = temp;
}

bool Database::isNextToPlayer(position _tile)
{
	if(_tile.x == m_Utility.getPlayerPosition(m_local.m_PlayerNr).x && _tile.y + 1 == m_Utility.getPlayerPosition(m_local.m_PlayerNr).y) //Top
	{
		return true;
	}
	else if(_tile.x == m_Utility.getPlayerPosition(m_local.m_PlayerNr).x && _tile.y - 1 == m_Utility.getPlayerPosition(m_local.m_PlayerNr).y) //Down
	{
		return true;
	}
	else if(_tile.x + 1 == m_Utility.getPlayerPosition(m_local.m_PlayerNr).x && _tile.y == m_Utility.getPlayerPosition(m_local.m_PlayerNr).y) //Left
	{
		return true;
	}
	else if(_tile.x - 1 == m_Utility.getPlayerPosition(m_local.m_PlayerNr).x && _tile.y == m_Utility.getPlayerPosition(m_local.m_PlayerNr).y) //Right
	{
		return true;
	}

	return false;
}