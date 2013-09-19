#include "..\..\include\Game\CSystem.h"
#include "..\..\include\Game\CSystemNormal.h"

CSystemNormal::CSystemNormal()
{
	ci_timer_spawn = new CTimer(1.0f); // Standard value
	ci_timer_spawn->addEventListener(TimerTick, &spawnWrapper, this);
	ci_timer_spawn->start();

	ci_squad_data = new CSquadData();
}

void CSystemNormal::start()
{
	m_iBossAttack = 0;

	for(unsigned int i = 0; i < getEnemyVector().size(); i++)
	{
		for(unsigned int j = 0; j < getEnemyVector()[i]->vUnits.size(); j++)
		{
			getEnemyVector()[i]->vUnits[j]->setMove();
			
			getEnemyVector()[i]->vUnits[j]->setScale(-1.0f, 1.0f);
			getEnemyVector()[i]->vUnits[j]->SetStartVelocity(getEnemyVector()[i]->vUnits[j]->getAIData().m_fSpeed, 0.0f); // Units from ragemode state have random speed, apply it again
			getEnemyVector()[i]->vUnits[j]->getAIData().m_iCurrentState = WALK_BACK_ENEMY;
		}
	}
}

void CSystemNormal::run()
{
	if(m_fPreviousSpawnTime != getSpawnTime())
	{
		ci_timer_spawn->changeTimerSettings(getSpawnTime());
		ci_timer_spawn->start();
		m_fPreviousSpawnTime = getSpawnTime();
	}
	
	for(unsigned int i = 0; i < getEnemyVector().size(); i++)
	{
		for(unsigned int j = 0; j < getEnemyVector()[i]->vUnits.size(); j++)
		{
			switch(getEnemyVector()[i]->vUnits[j]->getAIData().m_iCurrentState)
			{
				case WALK_ENEMY:
					{
						float xPosition = getEnemyVector()[i]->vUnits[j]->getEnemyData()->getPosition(getEnemyVector()[i]->vUnits[j]->getType()).x - getDistance();
					
						if(getEnemyVector()[i]->vUnits[j]->getPosition().x <= xPosition)
						{
							for(unsigned int k = 0; k < getEnemyVector()[i]->vUnits.size(); k++)
							{
								getEnemyVector()[i]->vUnits[k]->setPosition(xPosition + k * 100.0f, getEnemyVector()[i]->vUnits[k]->getPosition().y); // Set to correct position
								getEnemyVector()[i]->vUnits[k]->SetStartVelocity(0.0f, 0.0f);
								getEnemyVector()[i]->vUnits[k]->setAnimation(E_ANIM_BACKPEDAL);
								getEnemyVector()[i]->vUnits[k]->setCurrentState(IDLE_ENEMY);
							}
						}
					}
					break;
				case IDLE_ENEMY:
					// Choose correct animation
					for(unsigned int k = 0; k < getEnemyVector()[i]->vUnits.size(); k++)
					{
						if(getEnemyVector()[i]->vUnits[k]->getVisual().WasPlayed())
						{
							getEnemyVector()[i]->vUnits[k]->setAnimation(E_ANIM_BACKPEDAL);
						}
					}

					// Salve
					for(unsigned int k = 0; k < getEnemyVector()[i]->vSalve.size(); k++)
					{
						if(getEnemyVector()[i]->vSalve[k][1] <= getEnemyVector()[i]->iCurrentOrder && getEnemyVector()[i]->vSalve[k][2] >= getEnemyVector()[i]->iCurrentOrder) // Check whether salve is allowed to shoot
						{
							getEnemyVector()[i]->fSalveTime[k] -= glGraphics->GetLastFrameTime();
						
							if(getEnemyVector()[i]->fSalveTime[k] <= 0.0f)
							{
								//getEnemyVector()[i]->vUnits[getEnemyVector()[i]->vSalve[k][0]]->flash();
								getEnemyVector()[i]->vUnits[getEnemyVector()[i]->vSalve[k][0]]->launchAttackSalve();

								getEnemyVector()[i]->fSalveTime[k] = 0.5f;
							}
						}
					}

					// Wait until previous units have shot
					if(getEnemyVector()[i]->vAttackOrder.size() > 0)
					{
						for(unsigned int k = 0; k < getEnemyVector()[i]->vAttackOrder[0].task.size(); k++)
						{
							if(!getEnemyVector()[i]->vUnits[getEnemyVector()[i]->vAttackOrder[0].task[k]]->getAIData().m_bAttackStarted)
							{
								//getEnemyVector()[i]->vUnits[getEnemyVector()[i]->vAttackOrder[0].task[k]]->flash();
								getEnemyVector()[i]->vUnits[getEnemyVector()[i]->vAttackOrder[0].task[k]]->launchAttack();
								getEnemyVector()[i]->vUnits[getEnemyVector()[i]->vAttackOrder[0].task[k]]->setCurrentState(ATTACK_ENEMY);
							}
						}
					}
					else
					{
						for(unsigned int k = 0; k < getEnemyVector()[i]->vUnits.size(); k++)
						{
							getEnemyVector()[i]->vUnits[k]->setScale(-1.0f, 1.0f);
							getEnemyVector()[i]->vUnits[k]->SetStartVelocity(getEnemyVector()[i]->vUnits[k]->getAIData().m_fSpeed, 0.0f); // Continue with starting speed (even though global speed might have changed)
							getEnemyVector()[i]->vUnits[k]->setMove();
							getEnemyVector()[i]->vUnits[k]->setCurrentState(WALK_BACK_ENEMY);
						}
					}
					break;
				case ATTACK_ENEMY:
					{
						// Switch units back into IDLE_ENEMY state
						getEnemyVector()[i]->vAttackOrder[0].fTaskTime -= glGraphics->GetLastFrameTime() / getEnemyVector()[i]->vAttackOrder[0].task.size(); // Account for multiple units

						if(getEnemyVector()[i]->vAttackOrder[0].fTaskTime <= 0.0f)
						{
							for(unsigned int k = 0; k < getEnemyVector()[i]->vAttackOrder[0].task.size(); k++)
							{
								// Reset units to idle state
								getEnemyVector()[i]->vUnits[getEnemyVector()[i]->vAttackOrder[0].task[k]]->getAIData().m_bAttackStarted = false;
								getEnemyVector()[i]->vUnits[getEnemyVector()[i]->vAttackOrder[0].task[k]]->getAIData().m_bAttackFinished = false;
								getEnemyVector()[i]->vUnits[getEnemyVector()[i]->vAttackOrder[0].task[k]]->setCurrentState(IDLE_ENEMY);
							}

							getEnemyVector()[i]->iCurrentOrder++;
							getEnemyVector()[i]->vAttackOrder.erase(getEnemyVector()[i]->vAttackOrder.begin() + 0); // Erase the current attack order
						}
					}
					break;
				case WALK_BACK_ENEMY:
					if(getEnemyVector()[i]->vUnits[j]->getPosition().x > 2000.0f) // Make sure unit is off screen before removal
					{
						getEnemyVector()[i]->vUnits[j]->destroy();
					}
					break;
				case DEATH_ENEMY: // Remove those left over ragemode units that are off screen
					if(getEnemyVector()[i]->vUnits[j]->getPosition().x < 0 || getEnemyVector()[i]->vUnits[j]->getPosition().x > 1920 || getEnemyVector()[i]->vUnits[j]->getPosition().y > 1080 || getEnemyVector()[i]->vUnits[j]->getPosition().y < 0)
					{
						getEnemyVector()[i]->vUnits[j]->destroy();
					}
					break;
			}
		}
	}

	if(vBoss.size() > 0)
	{
		wasBossHit();

		// Show hitable target
		bool bBossProjectileOnScreen = false;
		for(unsigned int i = 0; i < CProjectile::getProjectileVector().size(); i++)
		{
			if(CProjectile::getProjectileVector()[i]->getParent() == 8 || CProjectile::getProjectileVector()[i]->getParent() == 9 || CProjectile::getProjectileVector()[i]->getParent() == 10 || CProjectile::getProjectileVector()[i]->getParent() == 11)
			{
				bBossProjectileOnScreen = true;
				break;
			}
		}

		if(!bBossProjectileOnScreen)
		{
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
	}

	CSystem::run(); // Call to super function
}

void CSystemNormal::spawnSquad()
{
	CSquadData::squad squad_info;
	squad_info = ci_squad_data->getSquad();
	m_squad = new CEnemy::enemy_squad();

	float fRandom;
	std::vector<int> vRandomRemove = getRandomElements(squad_info.vID.size(), CSquadData::getVirtualCount(squad_info.iIndex, CBeatSystem::GetBPM(), 0));

	// Create complete squad
	for(unsigned int i = 0; i < squad_info.vID.size(); i++)
	{
		m_squad->vUnits.push_back(new CEnemy);
		
		// Create normal or virtual unit (based on previous random ids)
		bool bVirtual = false;

		for(unsigned int j = 0; j < vRandomRemove.size(); j++)
		{
			if(i == vRandomRemove[j])
			{
				bVirtual = true;
			}
		}

		if(bVirtual)
		{
			m_squad->vUnits[m_squad->vUnits.size() - 1]->init(squad_info.vID[i], WALK_ENEMY, true);
		}
		else
		{
			m_squad->vUnits[m_squad->vUnits.size() - 1]->init(squad_info.vID[i], WALK_ENEMY, false);
		}

		m_squad->vUnits[m_squad->vUnits.size() - 1]->setPosition(m_squad->vUnits[m_squad->vUnits.size() - 1]->getPosition().x + i * 100.0f, m_squad->vUnits[m_squad->vUnits.size() - 1]->getPosition().y);
		m_squad->vUnits[m_squad->vUnits.size() - 1]->SetStartVelocity(-getMovementSpeed(), 0.0f);
		m_squad->vUnits[m_squad->vUnits.size() - 1]->getAIData().m_fSpeed = getMovementSpeed();
		m_squad->vUnits[m_squad->vUnits.size() - 1]->setAnimationSpeed(getAnimationSpeedFactor());

		// Random attack position in ragemode - might not get used
		fRandom = (float)(rand() % 50001); // 0 - 50000
		fRandom /= 100.0f; // float between 0 - 500
		fRandom += 1000.0f; // float between 1000 - 1500

		m_squad->vUnits[m_squad->vUnits.size() - 1]->getAIData().m_fAttackPosition = fRandom;
	}

	for(unsigned int i = 0; i < squad_info.vAttackOrder.size(); i++)
	{
		squad_info.vAttackOrder[i].fTaskTime = (squad_info.vAttackOrder[i].number_half_beats * 0.5f) * CBeatSystem::GetPerBeatTime();
	}

	m_squad->vSalve = squad_info.vSalve;
	m_squad->iCurrentOrder = 0;
	
	for(unsigned int i = 0; i < squad_info.vSalve.size(); i++)
	{
		m_squad->fSalveTime.push_back(0.5f);
	}

	m_squad->vAttackOrder = squad_info.vAttackOrder;
	getEnemyVector().push_back(m_squad);
}

void CSystemNormal::bossAttack()
{
	for(unsigned int i = 0; i < vBoss.size(); i++)
	{
		vBoss[i]->m_iReturningProjectile = -1;
	}

	for(unsigned int i = 0; i < vWeapon.size(); i++)
	{
		vWeapon[i]->m_iReturningProjectile = -1;
	}

	int iRandomWeapon;

	if(vWeapon.size() > 1)
	{
		iRandomWeapon = rand() % vWeapon.size();
	}
	else
	{
		iRandomWeapon = 0;
	}

	vWeapon[iRandomWeapon]->m_weapon_animation->SetCurrentAnimation(WEAPON_ANIMATION_NORMAL);

	CProjectile *ci_projectile;
	ci_projectile = new CProjectile();
	ci_projectile->init(vWeapon[iRandomWeapon]->getPosition().x + vWeapon[iRandomWeapon]->attack_origin.x, vWeapon[iRandomWeapon]->getPosition().y + vWeapon[iRandomWeapon]->attack_origin.y, vWeapon[iRandomWeapon]->m_iProjectile);

	if(CLevelData::getCurrentLevelString() == "darkage")
	{
		vBoss[0]->m_iReturningProjectile = vWeapon[iRandomWeapon]->m_iProjectile; // Target is highest tower piece
		vBoss[0]->m_boss_sprite->setColor(sf::Color(150, 255, 255, 255));
	}
	else if(CLevelData::getCurrentLevelString() == "future")
	{
		vWeapon[iRandomWeapon]->m_iReturningProjectile = vWeapon[iRandomWeapon]->m_iProjectile; // Target is current weapon
		vWeapon[iRandomWeapon]->m_weapon_animation->setColor(sf::Color(150, 255, 255, 255));
	}
}

void CSystemNormal::wasBossHit()
{
	if(CLevelData::getCurrentLevelString() == "darkage")
	{
		for(unsigned int i = 0; i < CProjectile::getProjectileVector().size(); i++)
		{
			if(CProjectile::getProjectileVector()[i]->getParent() == vBoss[0]->m_iReturningProjectile && CProjectile::getProjectileVector()[i]->getBounced() && !CProjectile::getProjectileVector()[i]->getDead() && (CProjectile::getProjectileVector()[i]->getCollisionObject()->Collides(vBoss[0]->m_boss_collision) || vBoss[0]->m_boss_collision->Collides(CProjectile::getProjectileVector()[i]->getCollisionObject())))
			{
				CProjectile::getProjectileVector()[i]->bounceOrDestroy();
				vWeapon[0]->setPosition(vWeapon[0]->getPosition().x, vWeapon[0]->getPosition().y + vBoss[0]->m_boss_sprite->getTexture()->getSize().y);

				delete vBoss[0];
				vBoss.erase(vBoss.begin() + 0);
				break;
			}
		}
	}
	else if(CLevelData::getCurrentLevelString() == "future")
	{
		bool bWeaponHit = false;

		for(unsigned int i = 0; i < CProjectile::getProjectileVector().size(); i++)
		{
			for(unsigned int j = 0; j < vWeapon.size(); j++)
			{
				if(CProjectile::getProjectileVector()[i]->getParent() == vWeapon[j]->m_iReturningProjectile && CProjectile::getProjectileVector()[i]->getBounced() && !CProjectile::getProjectileVector()[i]->getDead() && (CProjectile::getProjectileVector()[i]->getCollisionObject()->Collides(vWeapon[j]->m_weapon_collision) || vWeapon[j]->m_weapon_collision->Collides(CProjectile::getProjectileVector()[i]->getCollisionObject())))
				{
					CProjectile::getProjectileVector()[i]->bounceOrDestroy();

					vWeaponDestroyed[vWeapon[j]->m_iID]->m_bEnabled = true;

					delete vWeapon[j];
					vWeapon.erase(vWeapon.begin() + j);

					bWeaponHit = true;
					break;
				}
			}

			if(bWeaponHit)
			{
				break;
			}
		}

		if(vWeapon.size() == 0)
		{
			for(unsigned int i = 0; i < vBoss.size(); i++)
			{
				delete vBoss[i];
			}

			vBoss.clear();
		}
	}
}

void CSystemNormal::spawn()
{
	if(vBoss.size() > 0)
	{
		if(m_iBossAttack < 4) // Spawn squads normally
		{
			spawnSquad();
		}
		else if(m_iBossAttack == 6) // Wait 2 squad spawns to clear screen before shooting
		{
			bossAttack();
			spawnSquad();
			m_iBossAttack = 0;
		}

		m_iBossAttack++;
	}
	else
	{
		spawnSquad();
	}
}

void CSystemNormal::spawnWrapper(const void *context)
{
	if(getNormalMode())
	{
		((CSystemNormal*)(context))->spawn();
	}
}

std::vector<int> CSystemNormal::getRandomElements(unsigned int _size, int _removeCount) const
{
	// Random virtual units
	std::vector<int> vRandomIndex;
	std::vector<int> vRandomRemove;

	int id;

	for(unsigned int i = 0; i < _size; i++)
	{
		vRandomIndex.push_back(i);
	}

	for(int i = 0; i < _removeCount; i++)
	{
		id = rand() % vRandomIndex.size();
		vRandomRemove.push_back(vRandomIndex[id]);
		vRandomIndex.erase(vRandomIndex.begin() + id);
	}

	vRandomIndex.clear();
	return vRandomRemove;
}