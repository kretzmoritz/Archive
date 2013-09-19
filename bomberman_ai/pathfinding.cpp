#include "pathfinding.h"

void Pathfinding::update(CLocalState _local, CGameState _state, std::vector<std::set<int>> _Cost)
{
	m_local = _local;
	m_state = _state;

	m_vCost = _Cost;

	m_Utility.update(_local, _state);
}

std::vector<position> Pathfinding::astar(int _playerid, position _target)
{
	std::vector<position> result;

	m_vOpenList.clear();
	m_vClosedList.clear();

	bool bPathFound = true;
	int iPathIndex;

	if(_target != m_Utility.getPlayerPosition(_playerid)) //No pathfinding needed to position we're currently standing on!
	{
		bPathFound = false;
		position temp;
	
		//Starting position
		m_vOpenList.push_back(m_Utility.getPlayerPosition(_playerid));
	
		//Initialize starting node
		m_vOpenList[m_vOpenList.size() - 1].F = 0.0f;
		m_vOpenList[m_vOpenList.size() - 1].G = 0.0f;

		do
		{
			sort(m_vOpenList);
			m_vClosedList.push_back(m_vOpenList[0]);
			m_vOpenList.erase(m_vOpenList.begin());

			//Check all adjacent nodes
			bool bTop = false;
			bool bDown = false;
			bool bLeft = false;
			bool bRight = false;

			if(m_vClosedList[m_vClosedList.size() - 1].y - 1 >= 0)
			{
				bTop = true;

				if(m_state.m_LevelGrid.Get(m_vClosedList[m_vClosedList.size() - 1].x, m_vClosedList[m_vClosedList.size() - 1].y - 1) == TILE_TYPE_WALL) //Wall = impossible to walk on
				{
					bTop = false;
				}

				temp.x = m_vClosedList[m_vClosedList.size() - 1].x;
				temp.y = m_vClosedList[m_vClosedList.size() - 1].y - 1;

				for(unsigned int i = 0; i < m_vClosedList.size(); i++) //Is it already in the closed list?
				{
					if(m_vClosedList[i] == temp)
					{
						bTop = false;
					}
				}

				for(unsigned int i = 0; i < m_vOpenList.size(); i++) //Is it already in the open list?
				{
					if(m_vOpenList[i] == temp)
					{
						std::set<int>::reverse_iterator rit = m_vCost[m_vOpenList[i].x + m_vOpenList[i].y * m_state.m_LevelGrid.Width()].rbegin();
						
						if(m_vOpenList[i].G > *rit + m_vClosedList[m_vClosedList.size() - 1].G) //Did we find a better path to this tile?
						{
							ParentAndFCost(i, _target);
						}

						bTop = false;
					}
				}
			}

			if(m_vClosedList[m_vClosedList.size() - 1].y + 1 < (int)m_state.m_LevelGrid.Height())
			{
				bDown = true;

				if(m_state.m_LevelGrid.Get(m_vClosedList[m_vClosedList.size() - 1].x, m_vClosedList[m_vClosedList.size() - 1].y + 1) == TILE_TYPE_WALL)
				{
					bDown = false;
				}

				temp.x = m_vClosedList[m_vClosedList.size() - 1].x;
				temp.y = m_vClosedList[m_vClosedList.size() - 1].y + 1;

				for(unsigned int i = 0; i < m_vClosedList.size(); i++)
				{
					if(m_vClosedList[i] == temp)
					{
						bDown = false;
					}
				}

				for(unsigned int i = 0; i < m_vOpenList.size(); i++)
				{
					if(m_vOpenList[i] == temp)
					{
						std::set<int>::reverse_iterator rit = m_vCost[m_vOpenList[i].x + m_vOpenList[i].y * m_state.m_LevelGrid.Width()].rbegin();

						if(m_vOpenList[i].G > *rit + m_vClosedList[m_vClosedList.size() - 1].G)
						{
							ParentAndFCost(i, _target);
						}

						bDown = false;
					}
				}
			}

			if(m_vClosedList[m_vClosedList.size() - 1].x - 1 >= 0)
			{
				bLeft = true;

				if(m_state.m_LevelGrid.Get(m_vClosedList[m_vClosedList.size() - 1].x - 1, m_vClosedList[m_vClosedList.size() - 1].y) == TILE_TYPE_WALL)
				{
					bLeft = false;
				}

				temp.x = m_vClosedList[m_vClosedList.size() - 1].x - 1;
				temp.y = m_vClosedList[m_vClosedList.size() - 1].y;

				for(unsigned int i = 0; i < m_vClosedList.size(); i++)
				{
					if(m_vClosedList[i] == temp)
					{
						bLeft = false;
					}
				}

				for(unsigned int i = 0; i < m_vOpenList.size(); i++)
				{
					if(m_vOpenList[i] == temp)
					{
						std::set<int>::reverse_iterator rit = m_vCost[m_vOpenList[i].x + m_vOpenList[i].y * m_state.m_LevelGrid.Width()].rbegin();

						if(m_vOpenList[i].G > *rit + m_vClosedList[m_vClosedList.size() - 1].G)
						{
							ParentAndFCost(i, _target);
						}

						bLeft = false;
					}
				}
			}

			if(m_vClosedList[m_vClosedList.size() - 1].x + 1 < (int)m_state.m_LevelGrid.Width())
			{
				bRight = true;

				if(m_state.m_LevelGrid.Get(m_vClosedList[m_vClosedList.size() - 1].x + 1, m_vClosedList[m_vClosedList.size() - 1].y) == TILE_TYPE_WALL)
				{
					bRight = false;
				}

				temp.x = m_vClosedList[m_vClosedList.size() - 1].x + 1;
				temp.y = m_vClosedList[m_vClosedList.size() - 1].y;

				for(unsigned int i = 0; i < m_vClosedList.size(); i++)
				{
					if(m_vClosedList[i] == temp)
					{
						bRight = false;
					}
				}

				for(unsigned int i = 0; i < m_vOpenList.size(); i++)
				{
					if(m_vOpenList[i] == temp)
					{
						std::set<int>::reverse_iterator rit = m_vCost[m_vOpenList[i].x + m_vOpenList[i].y * m_state.m_LevelGrid.Width()].rbegin();

						if(m_vOpenList[i].G > *rit + m_vClosedList[m_vClosedList.size() - 1].G)
						{
							ParentAndFCost(i, _target);
						}

						bRight = false;
					}
				}
			}

			//Add viable adjacent nodes to the open list
			if(bTop)
			{
				m_vOpenList.push_back(temp);
				m_vOpenList[m_vOpenList.size() - 1].x = m_vClosedList[m_vClosedList.size() - 1].x;
				m_vOpenList[m_vOpenList.size() - 1].y = m_vClosedList[m_vClosedList.size() - 1].y - 1;

				ParentAndFCost(m_vOpenList.size() - 1, _target);
			}

			if(bDown)
			{
				m_vOpenList.push_back(temp);
				m_vOpenList[m_vOpenList.size() - 1].x = m_vClosedList[m_vClosedList.size() - 1].x;
				m_vOpenList[m_vOpenList.size() - 1].y = m_vClosedList[m_vClosedList.size() - 1].y + 1;

				ParentAndFCost(m_vOpenList.size() - 1, _target);
			}

			if(bLeft)
			{
				m_vOpenList.push_back(temp);
				m_vOpenList[m_vOpenList.size() - 1].x = m_vClosedList[m_vClosedList.size() - 1].x - 1;
				m_vOpenList[m_vOpenList.size() - 1].y = m_vClosedList[m_vClosedList.size() - 1].y;

				ParentAndFCost(m_vOpenList.size() - 1, _target);
			}

			if(bRight)
			{
				m_vOpenList.push_back(temp);
				m_vOpenList[m_vOpenList.size() - 1].x = m_vClosedList[m_vClosedList.size() - 1].x + 1;
				m_vOpenList[m_vOpenList.size() - 1].y = m_vClosedList[m_vClosedList.size() - 1].y;

				ParentAndFCost(m_vOpenList.size() - 1, _target);
			}

			//Check if we found a path
			if(m_vClosedList[m_vClosedList.size() - 1] == _target)
			{
				bPathFound = true;
			}
		}
		while(m_vClosedList[m_vClosedList.size() - 1] != _target && m_vOpenList.size() > 0);

		if(bPathFound)
		{
			iPathIndex = m_vClosedList.size() - 1;
			while(m_vClosedList[iPathIndex].x != m_Utility.getPlayerPosition(_playerid).x || m_vClosedList[iPathIndex].y != m_Utility.getPlayerPosition(_playerid).y)
			{
				for(unsigned int j = 0; j < m_vClosedList.size(); j++)
				{
					if(m_vClosedList[iPathIndex].xParent == m_vClosedList[j].x &&  m_vClosedList[iPathIndex].yParent == m_vClosedList[j].y)
					{
						result.push_back(m_vClosedList[iPathIndex]);
						iPathIndex = j;
					}
				}
			}
		}
	}

	if(bPathFound)
	{
		result.push_back(m_Utility.getPlayerPosition(_playerid));
		std::reverse(result.begin(), result.end());
	}
	else
	{
		result.push_back(position(-1, -1)); //No path found!
	}
	
	return result;
}

void Pathfinding::sort(std::vector<position> &_in)
{
	int id = 0;
	for(unsigned int i = 1; i < _in.size(); i++)
	{
		id = _in[id].F < _in[i].F ? id : i;
	}

	position temp;
	temp = _in[id];
	_in[id] = _in[0];
	_in[0] = temp;
}

void Pathfinding::ParentAndFCost(int _id, position _target)
{
	m_vOpenList[_id].xParent = m_vClosedList[m_vClosedList.size() - 1].x;
	m_vOpenList[_id].yParent = m_vClosedList[m_vClosedList.size() - 1].y;

	std::set<int>::reverse_iterator rit = m_vCost[m_vOpenList[_id].x + m_vOpenList[_id].y * m_state.m_LevelGrid.Width()].rbegin();

	m_vOpenList[_id].H = sqrt(pow((float)(_target.x - m_vOpenList[_id].x), 2) + pow((float)(_target.y - m_vOpenList[_id].y), 2));
	m_vOpenList[_id].G = *rit + m_vClosedList[m_vClosedList.size() - 1].G;
	m_vOpenList[_id].F = m_vOpenList[_id].G + m_vOpenList[_id].H;
}