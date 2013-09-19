#include "rating_system.h"

std::vector<int> RatingSystem::m_vLoopDetection;

RatingSystem::RatingSystem()
{
	m_iFullLifetime = 8;
	m_bLifetimeSet = false;
}

void RatingSystem::update(CLocalState _local, CGameState _state, int _x, int _y)
{
	m_local = _local;
	m_state = _state;

	if(m_vLoopDetection.size() < m_state.m_LevelGrid.Width() * m_state.m_LevelGrid.Height())
	{
		m_vLoopDetection.resize(m_state.m_LevelGrid.Width() * m_state.m_LevelGrid.Height(), 0);
	}

	m_Utility.update(_local, _state);

	calculateFire();
	calculateMovement();
	calculateDanger();
	calculateTraps();
	
	calculateDangerFuture(_x, _y);
}

int RatingSystem::getFullLifetime()
{
	return m_iFullLifetime;
}

std::vector<std::set<int>> RatingSystem::getMovementCostVector()
{
	return m_vMovementCost;
}

std::vector<std::set<int>> RatingSystem::getDangerCostVector()
{
	return m_vDangerCost;
}

std::vector<std::set<int>> RatingSystem::getDangerCostFutureVector()
{
	return m_vDangerCost_Future;
}

std::vector<std::set<int>> RatingSystem::getGCostVector()
{
	std::vector<std::set<int>> result;
	result.resize(m_state.m_LevelGrid.Width() * m_state.m_LevelGrid.Height());

	for(unsigned int i = 0; i < m_state.m_LevelGrid.Width(); i++)
	{
		for(unsigned int j = 0; j < m_state.m_LevelGrid.Height(); j++)
		{
			std::set<int>::reverse_iterator rit_movement = m_vMovementCost[i + j * m_state.m_LevelGrid.Width()].rbegin();
			std::set<int>::reverse_iterator rit_danger = m_vDangerCost[i + j * m_state.m_LevelGrid.Width()].rbegin();

			int iTemp = *rit_movement + *rit_danger;
			result[i + j * m_state.m_LevelGrid.Width()].insert(iTemp);
		}
	}

	return result;
}

std::vector<int> RatingSystem::getLoopDetectionVector()
{
	return m_vLoopDetection;
}

void RatingSystem::modLoopDetection(int _x, int _y, int _value)
{
	if(m_vLoopDetection[_x + _y * m_state.m_LevelGrid.Width()] + _value >= 0)
	{
		m_vLoopDetection[_x + _y * m_state.m_LevelGrid.Width()] += _value;
	}
}

void RatingSystem::calculateFire()
{
	m_vDangerCost.clear();
	m_vDangerCost.resize(m_state.m_LevelGrid.Width() * m_state.m_LevelGrid.Height());

	//Mark fire area
	for(unsigned int i = 0; i < m_state.m_LevelGrid.Width(); i++)
	{
		for(unsigned int j = 0; j < m_state.m_LevelGrid.Height(); j++)
		{
			for(unsigned int k = 0; k < m_state.m_UnitManager.GetAllUnitsOnTile(i, j).size(); k++)
			{
				switch(m_state.m_UnitManager.GetAllUnitsOnTile(i, j)[k].m_Type)
				{
				case UNIT_TYPE_FIRE:
					{
						set(i, j, 2, m_vDangerCost); //Explosion = 2; not very dangerous as already exploded
					}
					break;
				}
			}
		}
	}
}

void RatingSystem::calculateMovement()
{
	m_vMovementCost.clear();
	m_vMovementCost.resize(m_state.m_LevelGrid.Width() * m_state.m_LevelGrid.Height());

	for(unsigned int i = 0; i < m_state.m_LevelGrid.Width(); i++)
	{
		for(unsigned int j = 0; j < m_state.m_LevelGrid.Height(); j++)
		{
			switch(m_state.m_LevelGrid.Get(i, j))
			{
			case TILE_TYPE_FREE:
				set(i, j, 1, m_vMovementCost);
				break;
			case TILE_TYPE_ROCK:
				{
					bool bFire = false;
					std::set<int> temp_set = m_vDangerCost[i + j * m_state.m_LevelGrid.Width()];
					for(std::set<int>::iterator it = temp_set.begin(); it != temp_set.end(); it++)
					{
						if(*it == 2)
						{
							bFire = true;
							break;
						}
					}

					if(!(!isNextToPlayer(position(i, j)) && bFire))
					{
						set(i, j, 5, m_vMovementCost);
					}
					else
					{
						set(i, j, 1, m_vMovementCost);
					}
				}
				break;
			case TILE_TYPE_UPSZ:
				set(i, j, 0, m_vMovementCost);
				break;
			case TILE_TYPE_UPNR:
				set(i, j, 0, m_vMovementCost);
				break;
			case TILE_TYPE_TRAP:
				set(i, j, 1, m_vMovementCost);
				break;
			default:
				set(i, j, 1000, m_vMovementCost); //Everything unwalkable
				break;
			}
		}
	}

	m_Pathfinding.update(m_local, m_state, m_vMovementCost);
}

void RatingSystem::calculateDanger()
{
	std::vector<CUnit> vUnitsOnTile;

	//Mark bomb effect area; mark enemy danger zone
	for(unsigned int i = 0; i < m_state.m_LevelGrid.Width(); i++)
	{
		for(unsigned int j = 0; j < m_state.m_LevelGrid.Height(); j++)
		{
			vUnitsOnTile = m_state.m_UnitManager.GetAllUnitsOnTile(i, j);

			for(unsigned int k = 0; k < vUnitsOnTile.size(); k++)
			{
				switch(vUnitsOnTile[k].m_Type)
				{
				case UNIT_TYPE_BOMB:
					{
						if(vUnitsOnTile[k].m_LifeTime > 0) //No traps
						{
							bool bTopBroken = false;
							bool bDownBroken = false;
							bool bLeftBroken = false;
							bool bRightBroken = false;

							if(!m_bLifetimeSet)
							{
								m_iFullLifetime = vUnitsOnTile[k].m_LifeTime + 4; //Bomb scaling 4-7; Explosion = UNIT_TYPE_FIRE
								m_bLifetimeSet = true;
							}

							set(i, j, m_iFullLifetime - vUnitsOnTile[k].m_LifeTime, m_vDangerCost);

							unsigned int iExplosionSize = m_state.m_Player[vUnitsOnTile[k].m_Owner].m_ExplosionSize;
							if(vUnitsOnTile[k].m_Owner != m_local.m_PlayerNr && getSizeUpgradeReachable(vUnitsOnTile[k].m_Owner, position(i, j), k)) //Upgrade reachable by enemy player?
							{
								iExplosionSize++;
							}

							for(unsigned int h = 1; h <= iExplosionSize; h++)
							{
								if(!bTopBroken)
								{
									set(i, j - h, m_iFullLifetime - vUnitsOnTile[k].m_LifeTime, m_vDangerCost);

									if(m_state.m_LevelGrid.Get(i, j - h) == TILE_TYPE_ROCK || m_state.m_LevelGrid.Get(i, j - h) == TILE_TYPE_WALL)
									{
										bTopBroken = true;
									}
								}

								if(!bDownBroken)
								{
									set(i, j + h, m_iFullLifetime - vUnitsOnTile[k].m_LifeTime, m_vDangerCost);

									if(m_state.m_LevelGrid.Get(i, j + h) == TILE_TYPE_ROCK || m_state.m_LevelGrid.Get(i, j + h) == TILE_TYPE_WALL)
									{
										bDownBroken = true;
									}
								}

								if(!bLeftBroken)
								{
									set(i - h, j, m_iFullLifetime - vUnitsOnTile[k].m_LifeTime, m_vDangerCost);

									if(m_state.m_LevelGrid.Get(i - h, j) == TILE_TYPE_ROCK || m_state.m_LevelGrid.Get(i - h, j) == TILE_TYPE_WALL)
									{
										bLeftBroken = true;
									}
								}

								if(!bRightBroken)
								{
									set(i + h, j, m_iFullLifetime - vUnitsOnTile[k].m_LifeTime, m_vDangerCost);

									if(m_state.m_LevelGrid.Get(i + h, j) == TILE_TYPE_ROCK || m_state.m_LevelGrid.Get(i + h, j) == TILE_TYPE_WALL)
									{
										bRightBroken = true;
									}
								}

								if(bTopBroken && bDownBroken && bLeftBroken && bRightBroken)
								{
									break;
								}
							}
						}
					}
					break;
				case UNIT_TYPE_HERO:
					if(vUnitsOnTile[k].m_Owner != m_local.m_PlayerNr)
					{
						bool bTopBroken = false;
						bool bDownBroken = false;
						bool bLeftBroken = false;
						bool bRightBroken = false;

						set(i, j, 1, m_vDangerCost);

						for(unsigned int h = 1; h <= m_state.m_Player[vUnitsOnTile[k].m_Owner].m_ExplosionSize; h++)
						{
							if(!bTopBroken)
							{
								set(i, j - h, 1, m_vDangerCost);

								if(m_state.m_LevelGrid.Get(i, j - h) == TILE_TYPE_ROCK || m_state.m_LevelGrid.Get(i, j - h) == TILE_TYPE_WALL)
								{
									bTopBroken = true;
								}
							}

							if(!bDownBroken)
							{
								set(i, j + h, 1, m_vDangerCost);

								if(m_state.m_LevelGrid.Get(i, j + h) == TILE_TYPE_ROCK || m_state.m_LevelGrid.Get(i, j + h) == TILE_TYPE_WALL)
								{
									bDownBroken = true;
								}
							}

							if(!bLeftBroken)
							{
								set(i - h, j, 1, m_vDangerCost);

								if(m_state.m_LevelGrid.Get(i - h, j) == TILE_TYPE_ROCK || m_state.m_LevelGrid.Get(i - h, j) == TILE_TYPE_WALL)
								{
									bLeftBroken = true;
								}
							}

							if(!bRightBroken)
							{
								set(i + h, j, 1, m_vDangerCost);

								if(m_state.m_LevelGrid.Get(i + h, j) == TILE_TYPE_ROCK || m_state.m_LevelGrid.Get(i + h, j) == TILE_TYPE_WALL)
								{
									bRightBroken = true;
								}
							}

							if(bTopBroken && bDownBroken && bLeftBroken && bRightBroken)
							{
								break;
							}
						}
					}
					break;
				}
			}
		}
	}

	for(unsigned int i = 0; i < m_vDangerCost.size(); i++)
	{
		if(m_vDangerCost[i].empty())
		{
			m_vDangerCost[i].insert(0); //Standard value
		}
	}
}

void RatingSystem::calculateTraps()
{
	std::vector<CUnit> vUnitsOnTile;

	//Mark trap effect area
	for(unsigned int i = 0; i < m_state.m_LevelGrid.Width(); i++)
	{
		for(unsigned int j = 0; j < m_state.m_LevelGrid.Height(); j++)
		{
			vUnitsOnTile = m_state.m_UnitManager.GetAllUnitsOnTile(i, j);

			for(unsigned int k = 0; k < vUnitsOnTile.size(); k++)
			{
				switch(vUnitsOnTile[k].m_Type)
				{
				case UNIT_TYPE_BOMB:
					{
						if(vUnitsOnTile[k].m_LifeTime == 0 && *m_vDangerCost[i + j * m_state.m_LevelGrid.Width()].rbegin() >= 2)
						{
							bool bTopBroken = false;
							bool bDownBroken = false;
							bool bLeftBroken = false;
							bool bRightBroken = false;

							int iTrapLifetime = m_iFullLifetime - *(m_vDangerCost[i + j * m_state.m_LevelGrid.Width()].rbegin()) + 1;
							bool bFire = false;
							std::set<int> temp_set = m_vDangerCost[i + j * m_state.m_LevelGrid.Width()];
							for(std::set<int>::iterator it = temp_set.begin(); it != temp_set.end(); it++)
							{
								if(*it == 2)
								{
									bFire = true;
									break;
								}
							}
							
							set(i, j, bFire ? 7 : m_iFullLifetime - iTrapLifetime, m_vDangerCost);

							unsigned int iExplosionSize = m_state.m_Player[vUnitsOnTile[k].m_Owner].m_ExplosionSize;

							for(unsigned int h = 1; h <= iExplosionSize; h++)
							{
								if(!bTopBroken)
								{
									set(i, j - h, bFire ? 7 : m_iFullLifetime - iTrapLifetime, m_vDangerCost);

									if(m_state.m_LevelGrid.Get(i, j - h) == TILE_TYPE_ROCK || m_state.m_LevelGrid.Get(i, j - h) == TILE_TYPE_WALL)
									{
										bTopBroken = true;
									}
								}

								if(!bDownBroken)
								{
									set(i, j + h, bFire ? 7 : m_iFullLifetime - iTrapLifetime, m_vDangerCost);

									if(m_state.m_LevelGrid.Get(i, j + h) == TILE_TYPE_ROCK || m_state.m_LevelGrid.Get(i, j + h) == TILE_TYPE_WALL)
									{
										bDownBroken = true;
									}
								}

								if(!bLeftBroken)
								{
									set(i - h, j, bFire ? 7 : m_iFullLifetime - iTrapLifetime, m_vDangerCost);

									if(m_state.m_LevelGrid.Get(i - h, j) == TILE_TYPE_ROCK || m_state.m_LevelGrid.Get(i - h, j) == TILE_TYPE_WALL)
									{
										bLeftBroken = true;
									}
								}

								if(!bRightBroken)
								{
									set(i + h, j, bFire ? 7 : m_iFullLifetime - iTrapLifetime, m_vDangerCost);

									if(m_state.m_LevelGrid.Get(i + h, j) == TILE_TYPE_ROCK || m_state.m_LevelGrid.Get(i + h, j) == TILE_TYPE_WALL)
									{
										bRightBroken = true;
									}
								}

								if(bTopBroken && bDownBroken && bLeftBroken && bRightBroken)
								{
									break;
								}
							}
						}
					}
					break;
				}
			}
		}
	}
}

void RatingSystem::calculateDangerFuture(int _x, int _y)
{
	m_vDangerCost_Future.clear();
	m_vDangerCost_Future.resize(m_state.m_LevelGrid.Width() * m_state.m_LevelGrid.Height());

	std::vector<CUnit> vUnitsOnTile;

	//Mark bomb effect area; mark fire area
	for(unsigned int i = 0; i < m_state.m_LevelGrid.Width(); i++)
	{
		for(unsigned int j = 0; j < m_state.m_LevelGrid.Height(); j++)
		{
			vUnitsOnTile = m_state.m_UnitManager.GetAllUnitsOnTile(i, j);

			if(i == _x && j == _y)
			{
				CUnit temp;
				temp.m_Type = UNIT_TYPE_BOMB;
				temp.m_Owner = m_local.m_PlayerNr;
				temp.m_LifeTime = (m_iFullLifetime - 4) + 1; //Offset by 1 (fresh bomb)
				vUnitsOnTile.push_back(temp);
			}

			for(unsigned int k = 0; k < vUnitsOnTile.size(); k++)
			{
				switch(vUnitsOnTile[k].m_Type)
				{
				case UNIT_TYPE_BOMB:
					{
						bool bTopBroken = false;
						bool bDownBroken = false;
						bool bLeftBroken = false;
						bool bRightBroken = false;

						set(i, j, m_iFullLifetime - (vUnitsOnTile[k].m_LifeTime - 1) < 8 ? m_iFullLifetime - (vUnitsOnTile[k].m_LifeTime - 1) : 2, m_vDangerCost_Future);

						unsigned int iExplosionSize = m_state.m_Player[vUnitsOnTile[k].m_Owner].m_ExplosionSize;

						for(unsigned int h = 1; h <= iExplosionSize; h++)
						{
							if(!bTopBroken)
							{
								set(i, j - h, m_iFullLifetime - (vUnitsOnTile[k].m_LifeTime - 1) < 8 ? m_iFullLifetime - (vUnitsOnTile[k].m_LifeTime - 1) : 2, m_vDangerCost_Future);

								bool bFire = false;
								std::set<int> temp_set = m_vDangerCost[i + (j - h) * m_state.m_LevelGrid.Width()];
								for(std::set<int>::iterator it = temp_set.begin(); it != temp_set.end(); it++)
								{
									if(*it == 2)
									{
										bFire = true;
										break;
									}
								}

								if((m_state.m_LevelGrid.Get(i, j - h) == TILE_TYPE_ROCK && !bFire) || m_state.m_LevelGrid.Get(i, j - h) == TILE_TYPE_WALL)
								{
									bTopBroken = true;
								}
							}

							if(!bDownBroken)
							{
								set(i, j + h, m_iFullLifetime - (vUnitsOnTile[k].m_LifeTime - 1) < 8 ? m_iFullLifetime - (vUnitsOnTile[k].m_LifeTime - 1) : 2, m_vDangerCost_Future);

								bool bFire = false;
								std::set<int> temp_set = m_vDangerCost[i + (j + h) * m_state.m_LevelGrid.Width()];
								for(std::set<int>::iterator it = temp_set.begin(); it != temp_set.end(); it++)
								{
									if(*it == 2)
									{
										bFire = true;
										break;
									}
								}

								if((m_state.m_LevelGrid.Get(i, j + h) == TILE_TYPE_ROCK && !bFire) || m_state.m_LevelGrid.Get(i, j + h) == TILE_TYPE_WALL)
								{
									bDownBroken = true;
								}
							}

							if(!bLeftBroken)
							{
								set(i - h, j, m_iFullLifetime - (vUnitsOnTile[k].m_LifeTime - 1) < 8 ? m_iFullLifetime - (vUnitsOnTile[k].m_LifeTime - 1) : 2, m_vDangerCost_Future);

								bool bFire = false;
								std::set<int> temp_set = m_vDangerCost[(i - h) + j * m_state.m_LevelGrid.Width()];
								for(std::set<int>::iterator it = temp_set.begin(); it != temp_set.end(); it++)
								{
									if(*it == 2)
									{
										bFire = true;
										break;
									}
								}

								if((m_state.m_LevelGrid.Get(i - h, j) == TILE_TYPE_ROCK && !bFire) || m_state.m_LevelGrid.Get(i - h, j) == TILE_TYPE_WALL)
								{
									bLeftBroken = true;
								}
							}

							if(!bRightBroken)
							{
								set(i + h, j, m_iFullLifetime - (vUnitsOnTile[k].m_LifeTime - 1) < 8 ? m_iFullLifetime - (vUnitsOnTile[k].m_LifeTime - 1) : 2, m_vDangerCost_Future);

								bool bFire = false;
								std::set<int> temp_set = m_vDangerCost[(i + h) + j * m_state.m_LevelGrid.Width()];
								for(std::set<int>::iterator it = temp_set.begin(); it != temp_set.end(); it++)
								{
									if(*it == 2)
									{
										bFire = true;
										break;
									}
								}

								if((m_state.m_LevelGrid.Get(i + h, j) == TILE_TYPE_ROCK && !bFire) || m_state.m_LevelGrid.Get(i + h, j) == TILE_TYPE_WALL)
								{
									bRightBroken = true;
								}
							}

							if(bTopBroken && bDownBroken && bLeftBroken && bRightBroken)
							{
								break;
							}
						}
					}
					break;
				}
			}
		}
	}

	for(unsigned int i = 0; i < m_vDangerCost_Future.size(); i++)
	{
		if(m_vDangerCost_Future[i].empty())
		{
			m_vDangerCost_Future[i].insert(0); //Standard value
		}
	}
}

bool RatingSystem::getSizeUpgradeReachable(int _playerid, position _bomb, int _index)
{
	/*std::cout << "TEST_IN" << std::endl;

	if(_playerid != m_state.m_Player.size() - 1) //Fake player can put traps - fix so bot doesn't crash
	{
		std::cout << "TEST" << std::endl;

		m_vSizeUpgrades.clear();

		for(unsigned int i = 0; i < m_state.m_LevelGrid.Width(); i++)
		{
			for(unsigned int j = 0; j < m_state.m_LevelGrid.Height(); j++)
			{
				switch(m_state.m_LevelGrid.Get(i, j))
				{
					case TILE_TYPE_UPSZ:
						std::cout << "TEST1" << std::endl;
						m_vSizeUpgrades.push_back(position(i, j));
						std::vector<position> path = m_Pathfinding.astar(_playerid, m_vSizeUpgrades[m_vSizeUpgrades.size() - 1]);
						m_vSizeUpgrades[m_vSizeUpgrades.size() - 1].iDistanceToPlayer = path.size() - 1;
						std::cout << "TES2T" << std::endl;

						for(unsigned int k = 0; k < path.size(); k++)
						{
							if(m_state.m_LevelGrid.Get(path[k].x, path[k].y) == TILE_TYPE_ROCK)
							{
								return false;
							}
						}
						break;
				}
			}
		}

		if(m_vSizeUpgrades.size() > 0)
		{
			sort(m_vSizeUpgrades);

			if(m_vSizeUpgrades[0].iDistanceToPlayer < (int)m_state.m_UnitManager.GetAllUnitsOnTile(_bomb.x, _bomb.y)[_index].m_LifeTime)
			{
				return true;
			}
		}
	}*/

	return false;
}

void RatingSystem::set(int _x, int _y, int _value, std::vector<std::set<int>> &_vector)
{
	if(_x < (int)m_state.m_LevelGrid.Width() && _x >= 0 && _y < (int)m_state.m_LevelGrid.Height() && _y >= 0)
	{
		_vector[_x + _y * m_state.m_LevelGrid.Width()].insert(_value);
	}
}

int RatingSystem::get(int _x, int _y, std::vector<std::set<int>> &_vector)
{
	if(_x < (int)m_state.m_LevelGrid.Width() && _x >= 0 && _y < (int)m_state.m_LevelGrid.Height() && _y >= 0)
	{
		return *(_vector[_x + _y * m_state.m_LevelGrid.Width()].rbegin());
	}

	return -1;
}

void RatingSystem::sort(std::vector<position> &_in)
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

bool RatingSystem::isNextToPlayer(position _tile)
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