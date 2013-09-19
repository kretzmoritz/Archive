#include "..\..\include\Game\CSystem.h"
#include "..\..\include\Game\CSystemRagemode.h"

CSystemRagemode::CSystemRagemode()
{
	ci_timer_spawn = new CTimer(1.0f); // Standard value
	ci_timer_spawn->addEventListener(TimerTick, &spawnWrapper, this);
	ci_timer_spawn->start();

	ci_squad_data = new CSquadData();
}

void CSystemRagemode::start()
{
	float fRandom;

	for(unsigned int i = 0; i < getEnemyVector().size(); i++)
	{
		for(unsigned int j = 0; j < getEnemyVector()[i]->vUnits.size(); j++)
		{
			getEnemyVector()[i]->vUnits[j]->setMove();

			// Random speed
			fRandom = (float)(rand() % 15001); // 0 - 15000
			fRandom /= 100.0f; // float between 0 - 150
			fRandom += 200.0f; // float between 200 - 350

			getEnemyVector()[i]->vUnits[j]->getAIData().m_fSpeed = fRandom * getRagemodeSpeedFactor(); // Units switched to ragemode need new random speed to create chaos in their ranks
			getEnemyVector()[i]->vUnits[j]->setAnimationSpeed(getAnimationSpeedFactor());
			
			getEnemyVector()[i]->vUnits[j]->setScale(-1.0f, 1.0f);
			getEnemyVector()[i]->vUnits[j]->SetStartVelocity(getEnemyVector()[i]->vUnits[j]->getAIData().m_fSpeed, 0.0f);
			getEnemyVector()[i]->vUnits[j]->getAIData().m_iCurrentState = WALK_BACK_ENEMY_RAGEMODE;
		}
	}

	// Reset boss and weapon marker
	if(CLevelData::getCurrentLevelString() == "darkage")
	{
		for(unsigned int i = 0; i < vBoss.size(); i++)
		{
			vBoss[i]->m_boss_sprite->setColor(sf::Color(255, 255, 255, 255));
		}
	}
	else if(CLevelData::getCurrentLevelString() == "future")
	{
		for(unsigned int i = 0; i < vWeapon.size(); i++)
		{
			vWeapon[i]->m_weapon_animation->setColor(sf::Color(255, 255, 255, 255));
		}
	}
}

void CSystemRagemode::run()
{
	if(m_fPreviousSpawnTimeRagemode != getSpawnTimeRagemode())
	{
		ci_timer_spawn->changeTimerSettings(getSpawnTimeRagemode());
		ci_timer_spawn->start();
		m_fPreviousSpawnTimeRagemode = getSpawnTimeRagemode();
	}
	
	for(unsigned int i = 0; i < getEnemyVector().size(); i++)
	{
		for(unsigned int j = 0; j < getEnemyVector()[i]->vUnits.size(); j++)
		{
			switch(getEnemyVector()[i]->vUnits[j]->getAIData().m_iCurrentState)
			{
				case WALK_ENEMY_RAGEMODE:
					if(getEnemyVector()[i]->vUnits[j]->getPosition().x <= getEnemyVector()[i]->vUnits[j]->getAIData().m_fAttackPosition && !getEnemyVector()[i]->vUnits[j]->getAIData().m_bAttackStarted)
					{
						getEnemyVector()[i]->vUnits[j]->setPosition(getEnemyVector()[i]->vUnits[j]->getAIData().m_fAttackPosition, getEnemyVector()[i]->vUnits[j]->getPosition().y); // Set to correct position
						getEnemyVector()[i]->vUnits[j]->SetStartVelocity(0.0f, 0.0f);
						//getEnemyVector()[i]->vUnits[j]->flash();
						getEnemyVector()[i]->vUnits[j]->launchAttack();
						getEnemyVector()[i]->vUnits[j]->setCurrentState(ATTACK_ENEMY_RAGEMODE);
					}

					if(getEnemyVector()[i]->vUnits[j]->getPosition().x < 0.0f)
					{
						getEnemyVector()[i]->vUnits[j]->destroy();
					}
					break;
				case ATTACK_ENEMY_RAGEMODE:
					if(getEnemyVector()[i]->vUnits[j]->getVisual().WasPlayed())
					{
						getEnemyVector()[i]->vUnits[j]->getAIData().m_bAttackFinished = true;
						getEnemyVector()[i]->vUnits[j]->setMove();
						getEnemyVector()[i]->vUnits[j]->setScale(-1.0f, 1.0f);
						getEnemyVector()[i]->vUnits[j]->SetStartVelocity(getEnemyVector()[i]->vUnits[j]->getAIData().m_fSpeed, 0.0f);
						getEnemyVector()[i]->vUnits[j]->setCurrentState(WALK_BACK_ENEMY_RAGEMODE);
					}
					break;
				case WALK_BACK_ENEMY_RAGEMODE:
					if(getEnemyVector()[i]->vUnits[j]->getPosition().x > 2000.0f)
					{
						getEnemyVector()[i]->vUnits[j]->destroy();
					}
					break;
				case DEATH_ENEMY: // Remove dead units that are off screen
					if(getEnemyVector()[i]->vUnits[j]->getPosition().x < 0 || getEnemyVector()[i]->vUnits[j]->getPosition().x > 1920 || getEnemyVector()[i]->vUnits[j]->getPosition().y > 1080 || getEnemyVector()[i]->vUnits[j]->getPosition().y < 0)
					{
						getEnemyVector()[i]->vUnits[j]->destroy();
					}
					break;
			}
		}
	}

	CSystem::run(); // Call to super function
}

void CSystemRagemode::spawn()
{
	CSquadData::squad squad_info;
	squad_info = ci_squad_data->getSquad();
	m_squad = new CEnemy::enemy_squad();

	float fRandom;
	
	for(unsigned int i = 0; i < squad_info.vID.size(); i++)
	{
		m_squad->vUnits.push_back(new CEnemy);
		m_squad->vUnits[m_squad->vUnits.size() - 1]->init(squad_info.vID[i], WALK_ENEMY_RAGEMODE, false);
		m_squad->vUnits[m_squad->vUnits.size() - 1]->setPosition(m_squad->vUnits[m_squad->vUnits.size() - 1]->getPosition().x + i * 100.0f, m_squad->vUnits[m_squad->vUnits.size() - 1]->getPosition().y + i * 25.0f); // Different layers, chaos, no real attack order
		m_squad->vUnits[m_squad->vUnits.size() - 1]->setAnimationSpeed(getAnimationSpeedFactor());

		// Random speed
		fRandom = (float)(rand() % 15001); // 0 - 15000
		fRandom /= 100.0f; // float between 0 - 150
		fRandom += 200.0f; // float between 200 - 350
		
		m_squad->vUnits[m_squad->vUnits.size() - 1]->getAIData().m_fSpeed = fRandom * getRagemodeSpeedFactor();
		m_squad->vUnits[m_squad->vUnits.size() - 1]->SetStartVelocity(-m_squad->vUnits[m_squad->vUnits.size() - 1]->getAIData().m_fSpeed, 0.0f);

		// Random attack position in ragemode - might not get used
		fRandom = (float)(rand() % 50001); // 0 - 50000
		fRandom /= 100.0f; // float between 0 - 500
		fRandom += 1000.0f; // float between 1000 - 1500

		m_squad->vUnits[m_squad->vUnits.size() - 1]->getAIData().m_fAttackPosition = fRandom;
	}
	
	getEnemyVector().push_back(m_squad);
}

void CSystemRagemode::spawnWrapper(const void *context)
{
	if(!getNormalMode())
	{
		((CSystemRagemode*)(context))->spawn();
	}
}