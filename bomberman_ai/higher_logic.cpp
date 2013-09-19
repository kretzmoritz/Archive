#include "higher_logic.h"
#include <Windows.h>

void HigherLogic::update(CLocalState _local, CGameState _state)
{
	std::cout << std::endl << "Updating all systems..." << std::endl;

	m_local = _local;
	m_state = _state;

	std::cout << "Utility..." << std::endl;
	m_Utility.update(_local, _state);

	std::cout << "Rating..." << std::endl;
	m_RatingSystem.update(_local, _state);

	std::cout << "Pathfinding..." << std::endl;
	m_Pathfinding.update(_local, _state, m_RatingSystem.getGCostVector());

	std::cout << "Database..." << std::endl;
	m_Database.update(_local, _state, m_RatingSystem.getMovementCostVector(), m_RatingSystem.getDangerCostVector(), m_RatingSystem.getDangerCostFutureVector(), m_RatingSystem.getGCostVector(), m_RatingSystem.getLoopDetectionVector(), m_RatingSystem.getFullLifetime());
}

PlayerControlType HigherLogic::getAction()
{
	//Sleep(250);
	std::cout << std::endl << "New Action: " << std::endl;

	PlayerControlType result = PLAYER_CONTROL_NONE;
	bool bActionInitialized = false;
	bool bActionChosen = false;
	bool bDodge = false;
	
	position target;
	std::vector<position> path;
	
	int iThreshold = 3;
	int iCount = 1;

	if(m_Database.isDanger())
	{
		std::cout << "Danger; Dodge initiated..." << std::endl;

		target = m_Database.getClosestSafeSpot();
		bActionInitialized = true;
		bDodge = true;

		std::cout << target.x << ' ' << target.y << std::endl;
	}
	else if(m_Database.getClosestEnemy().iDistanceToPlayer <= iThreshold)
	{
		if(foreseeBomb() && m_Database.isSafeToPlaceBomb())
		{
			std::cout << "Enemy bot in range; Attack initiated..." << std::endl;

			result = PLAYER_CONTROL_BOMB;
			bActionInitialized = true;
			bActionChosen = true;
		}

		revert();
	}
	
	if(!bActionInitialized)
	{
		if(!(m_Database.getClosestUpgrade().x == -1 && m_Database.getClosestUpgrade().y == -1))
		{
			std::cout << "Upgrade in range; Fetching initiated..." << std::endl;

			target = m_Database.getClosestUpgrade();

			std::cout << target.x << ' ' << target.y << std::endl;
		}
		else if(!(m_Database.getClosestEnemy().x == -1 && m_Database.getClosestEnemy().y == -1))
		{
			std::cout << "No more upgrades; Finding enemy bot..." << std::endl;

			target = m_Database.getClosestEnemy();

			std::cout << target.x << ' ' << target.y << std::endl;
		}
		else
		{
			bActionChosen = true; //No action
		}
	}
	
	if(!bActionChosen)
	{
		if(bDodge)
		{
			m_Pathfinding.update(m_local, m_state, m_RatingSystem.getMovementCostVector()); //Switch to MovementCost so danger can't interfere
		}

		path = m_Pathfinding.astar(m_local.m_PlayerNr, target);
		m_Pathfinding.update(m_local, m_state, m_RatingSystem.getGCostVector()); //Revert to GCost

		if(path.size() > 1)
		{
			m_RatingSystem.modLoopDetection(path[1].x, path[1].y, 1);

			if(m_state.m_LevelGrid.Get(path[1].x, path[1].y) == TILE_TYPE_ROCK)
			{
				if(foreseeBomb() && m_Database.isSafeToPlaceBomb())
				{
					std::cout << "Rock in path; Bomb initiated..." << std::endl;

					result = PLAYER_CONTROL_BOMB;
				}

				revert();
			}
			else
			{
				if(m_Database.isSafeToWalk(path[1]))
				{
					if(m_Utility.getPlayerPosition(m_local.m_PlayerNr).x == path[1].x && m_Utility.getPlayerPosition(m_local.m_PlayerNr).y > path[1].y)
					{
						std::cout << "Moving up..." << std::endl;

						result = PLAYER_CONTROL_MOVE_UP;
					}
					else if(m_Utility.getPlayerPosition(m_local.m_PlayerNr).x == path[1].x && m_Utility.getPlayerPosition(m_local.m_PlayerNr).y < path[1].y)
					{
						std::cout << "Moving down..." << std::endl;

						result = PLAYER_CONTROL_MOVE_DOWN;
					}
					else if(m_Utility.getPlayerPosition(m_local.m_PlayerNr).x > path[1].x && m_Utility.getPlayerPosition(m_local.m_PlayerNr).y == path[1].y)
					{
						std::cout << "Moving left..." << std::endl;

						result = PLAYER_CONTROL_MOVE_LEFT;
					}
					else if(m_Utility.getPlayerPosition(m_local.m_PlayerNr).x < path[1].x && m_Utility.getPlayerPosition(m_local.m_PlayerNr).y == path[1].y)
					{
						std::cout << "Moving right..." << std::endl;

						result = PLAYER_CONTROL_MOVE_RIGHT;
					}
				}
			}
		}
	}

	if(iCount == 4) //Update this every 4 rounds
	{
		for(unsigned int i = 0; i < m_state.m_LevelGrid.Width(); i++)
		{
			for(unsigned int j = 0; j < m_state.m_LevelGrid.Height(); j++)
			{
				m_RatingSystem.modLoopDetection(i, j, -1);
			}
		}

		iCount = 0;
	}

	iCount++;

	std::cout << "Sending action..." << std::endl;
	return result;
}

bool HigherLogic::foreseeBomb()
{
	m_RatingSystem.update(m_local, m_state, m_Utility.getPlayerPosition(m_local.m_PlayerNr).x, m_Utility.getPlayerPosition(m_local.m_PlayerNr).y); //Bomb that we might place
	m_Database.update(m_local, m_state, m_RatingSystem.getMovementCostVector(), m_RatingSystem.getDangerCostVector(), m_RatingSystem.getDangerCostFutureVector(), m_RatingSystem.getGCostVector(), m_RatingSystem.getLoopDetectionVector(), m_RatingSystem.getFullLifetime());

	return true;
}

void HigherLogic::revert()
{
	m_RatingSystem.update(m_local, m_state); //Actual data
	m_Database.update(m_local, m_state, m_RatingSystem.getMovementCostVector(), m_RatingSystem.getDangerCostVector(), m_RatingSystem.getDangerCostFutureVector(), m_RatingSystem.getGCostVector(), m_RatingSystem.getLoopDetectionVector(), m_RatingSystem.getFullLifetime());
}