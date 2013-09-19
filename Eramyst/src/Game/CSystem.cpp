#include "..\..\include\Game\CSystem.h"

//Definition of static variables
std::vector<CSystem::boss_piece*> CSystem::vBoss;
std::vector<CSystem::movement_piece*> CSystem::vMovement;
std::vector<CSystem::weapon_piece*> CSystem::vWeapon;
std::vector<CSystem::weapon_piece_destroyed*> CSystem::vWeaponDestroyed;

float CSystem::m_fSpawnTime;
float CSystem::m_fSpawnTimeRagemode;
float CSystem::m_fMovementSpeed;
float CSystem::m_fRagemodeSpeedFactor;
float CSystem::m_fAnimationSpeedFactor;
float CSystem::m_fDistance;

bool CSystem::m_bNormalMode;

CBossData CSystem::ci_boss_data;

float CSystem::m_fPreviousSpawnTime;
float CSystem::m_fPreviousSpawnTimeRagemode;

void CSystem::init()
{
	m_fSpawnTime = 1.0f;
	m_fSpawnTimeRagemode = 2.0f;
	m_fMovementSpeed = 200.0f;
	m_fRagemodeSpeedFactor = 1.0f;
	m_fAnimationSpeedFactor = 1.0f;
	m_fDistance = 750.0f;

	m_bNormalMode = true;

	m_fPreviousSpawnTime = 0;
	m_fPreviousSpawnTimeRagemode = 0;

	ci_boss_data.init(CLevelData::getCurrentLevelString());
}

void CSystem::reset()
{
	init();
	
	getEnemyVector().clear();
	CProjectile::getProjectileVector().clear();
	
	// Boss reset
	destructBoss();
}

void CSystem::run()
{
	bool bBossStopped = false;
	float fPositionToStop = 2000.0f;

	if(CLevelData::getCurrentLevelString() == "darkage")
	{
		fPositionToStop = 1450.0f;
	}
	else if(CLevelData::getCurrentLevelString() == "future")
	{
		fPositionToStop = 1250.0f;
	}

	for(unsigned int i = 0; i < vBoss.size(); i++)
	{
		if(vBoss[i]->getPosition().x >= fPositionToStop)
		{
			vBoss[i]->setPosition(vBoss[i]->getPosition().x - (200.0f * glGraphics->GetLastFrameTime()), vBoss[i]->getPosition().y);
			vBoss[i]->updateCollisionObjects();
		}
		else
		{
			bBossStopped = true;
		}

		glGraphics->Draw(vBoss[i], vBoss[i]->m_iLayer);
	}

	for(unsigned int i = 0; i < vMovement.size(); i++)
	{
		if(vMovement[i]->getPosition().x >= fPositionToStop && !bBossStopped)
		{
			vMovement[i]->setPosition(vMovement[i]->getPosition().x - (200.0f * glGraphics->GetLastFrameTime()), vMovement[i]->getPosition().y);
			vMovement[i]->rotate(-50.0f * glGraphics->GetLastFrameTime());
		}
		else
		{
			vMovement[i]->rotate(50.0f * glGraphics->GetLastFrameTime());
		}

		glGraphics->Draw(vMovement[i], vMovement[i]->m_iLayer);
	}

	for(unsigned int i = 0; i < vWeapon.size(); i++)
	{
		if(vWeapon[i]->getPosition().x >= fPositionToStop && !bBossStopped)
		{
			vWeapon[i]->setPosition(vWeapon[i]->getPosition().x - (200.0f * glGraphics->GetLastFrameTime()), vWeapon[i]->getPosition().y);
			vWeapon[i]->updateCollisionObjects();
		}

		glGraphics->Draw(vWeapon[i], vWeapon[i]->m_iLayer);
	}
	
	for(unsigned int i = 0; i < vWeaponDestroyed.size(); i++)
	{
		if(vWeaponDestroyed[i]->m_bEnabled)
		{
			glGraphics->Draw(vWeaponDestroyed[i], vWeaponDestroyed[i]->m_iLayer);
		}
	}
}

void CSystem::setSpawnTime(float _value)
{
	m_fSpawnTime = _value;
}

void CSystem::modSpawnTime(float _value)
{
	m_fSpawnTime += _value;
}

float CSystem::getSpawnTime()
{
	return m_fSpawnTime;
}

void CSystem::setSpawnTimeRagemode(float _value)
{
	m_fSpawnTimeRagemode = _value;
}

void CSystem::modSpawnTimeRagemode(float _value)
{
	m_fSpawnTimeRagemode += _value;
}

float CSystem::getSpawnTimeRagemode()
{
	return m_fSpawnTimeRagemode;
}

void CSystem::setMovementSpeed(float _value)
{
	m_fMovementSpeed = _value;
}

void CSystem::modMovementSpeed(float _value)
{
	m_fMovementSpeed += _value;
}

float CSystem::getMovementSpeed()
{
	return m_fMovementSpeed;
}

void CSystem::setRagemodeSpeedFactor(float _value)
{
	m_fRagemodeSpeedFactor = _value;
}

void CSystem::modRagemodeSpeedFactor(float _value)
{
	m_fRagemodeSpeedFactor += _value;
}

float CSystem::getRagemodeSpeedFactor()
{
	return m_fRagemodeSpeedFactor;
}

void CSystem::setAnimationSpeedFactor(float _value)
{
	m_fAnimationSpeedFactor = _value;
}

void CSystem::modAnimationSpeedFactor(float _value)
{
	m_fAnimationSpeedFactor += _value;
}

float CSystem::getAnimationSpeedFactor()
{
	return m_fAnimationSpeedFactor;
}

void CSystem::setDistance(float _value)
{
	m_fDistance = _value;
}

void CSystem::modDistance(float _value)
{
	m_fDistance += _value;
}

float CSystem::getDistance()
{
	return m_fDistance;
}

void CSystem::constructBoss()
{
	float positionX = ci_boss_data.getPosition().x;
	float positionY = ci_boss_data.getPosition().y;

	for(unsigned int i = 0; i < ci_boss_data.getBossPieces().size(); i++)
	{
		vBoss.push_back(new boss_piece());
		vBoss[vBoss.size() -1]->m_boss_sprite->setTexture(*glDataBox->GetTexture(ci_boss_data.getBossPieces()[i].texture));
		vBoss[vBoss.size() -1]->m_iLayer = ci_boss_data.getBossPieces()[i].layer;
		vBoss[vBoss.size() -1]->setPosition(positionX + ci_boss_data.getBossPieces()[i].offset.x, positionY + ci_boss_data.getBossPieces()[i].offset.y);
		
		if(CLevelData::getCurrentLevelString() == "darkage")
		{
			vBoss[vBoss.size() -1]->m_boss_collision->SetSize((float)vBoss[vBoss.size() -1]->m_boss_sprite->getTexture()->getSize().x, (float)vBoss[vBoss.size() -1]->m_boss_sprite->getTexture()->getSize().y);
		}
	}

	for(unsigned int i = 0; i < ci_boss_data.getMovementPieces().size(); i++)
	{
		vMovement.push_back(new movement_piece());
		vMovement[vMovement.size() - 1]->m_movement_animation->setTexture(*glDataBox->GetTexture(ci_boss_data.getMovementPieces()[i].texture));
		vMovement[vMovement.size() - 1]->m_movement_animation->SetTotalFrames(ci_boss_data.getMovementPieces()[i].frames);
		vMovement[vMovement.size() - 1]->m_movement_animation->SetFramesPerSecond(15.0f);
		vMovement[vMovement.size() - 1]->m_movement_animation->SetFrameSize((int)ci_boss_data.getMovementPieces()[i].size.width, (int)ci_boss_data.getMovementPieces()[i].size.height);
		vMovement[vMovement.size() - 1]->m_movement_animation->AddAnimation(MOVEMENT_ANIMATION_IDLE, 0, 0);
		vMovement[vMovement.size() - 1]->m_movement_animation->AddAnimation(MOVEMENT_ANIMATION_NORMAL, 0, ci_boss_data.getMovementPieces()[i].frames - 1);
		vMovement[vMovement.size() - 1]->m_movement_animation->SetCurrentAnimation(MOVEMENT_ANIMATION_IDLE);
		vMovement[vMovement.size() - 1]->m_iLayer = ci_boss_data.getMovementPieces()[i].layer;
		vMovement[vMovement.size() - 1]->setOrigin(vMovement[vMovement.size() - 1]->m_movement_animation->getGlobalBounds().width/2, vMovement[vMovement.size() - 1]->m_movement_animation->getGlobalBounds().height/2);
		vMovement[vMovement.size() - 1]->setPosition(positionX + ci_boss_data.getMovementPieces()[i].offset.x, positionY + ci_boss_data.getMovementPieces()[i].offset.y);
	}

	for(unsigned int i = 0; i < ci_boss_data.getWeaponPieces().size(); i++)
	{
		vWeapon.push_back(new weapon_piece());
		vWeapon[vWeapon.size() - 1]->m_weapon_animation->setTexture(*glDataBox->GetTexture(ci_boss_data.getWeaponPieces()[i].texture));
		vWeapon[vWeapon.size() - 1]->m_weapon_animation->SetTotalFrames(ci_boss_data.getWeaponPieces()[i].frames);
		vWeapon[vWeapon.size() - 1]->m_weapon_animation->SetFramesPerSecond(15.0f);
		vWeapon[vWeapon.size() - 1]->m_weapon_animation->SetFrameSize((int)ci_boss_data.getWeaponPieces()[i].size.width, (int)ci_boss_data.getWeaponPieces()[i].size.height);
		vWeapon[vWeapon.size() - 1]->m_weapon_animation->PlayOnce();
		vWeapon[vWeapon.size() - 1]->m_weapon_animation->AddAnimation(WEAPON_ANIMATION_IDLE, 0, 0);
		vWeapon[vWeapon.size() - 1]->m_weapon_animation->AddAnimation(WEAPON_ANIMATION_NORMAL, 0, ci_boss_data.getWeaponPieces()[i].frames - 1);
		vWeapon[vWeapon.size() - 1]->m_weapon_animation->SetCurrentAnimation(WEAPON_ANIMATION_IDLE);
		vWeapon[vWeapon.size() - 1]->m_iLayer = ci_boss_data.getWeaponPieces()[i].layer;
		vWeapon[vWeapon.size() - 1]->attack_origin.x = ci_boss_data.getWeaponPieces()[i].attack_origin.x;
		vWeapon[vWeapon.size() - 1]->attack_origin.y = ci_boss_data.getWeaponPieces()[i].attack_origin.y;
		vWeapon[vWeapon.size() - 1]->m_iProjectile = ci_boss_data.getWeaponPieces()[i].projectile;
		vWeapon[vWeapon.size() - 1]->m_iID = i;
		vWeapon[vWeapon.size() - 1]->setPosition(positionX + ci_boss_data.getWeaponPieces()[i].offset.x, positionY + ci_boss_data.getWeaponPieces()[i].offset.y);
		
		if(CLevelData::getCurrentLevelString() == "future")
		{
			vWeapon[vWeapon.size() -1]->m_weapon_collision->SetSize(ci_boss_data.getWeaponPieces()[i].size.width, ci_boss_data.getWeaponPieces()[i].size.height);
		}
	}

	for(unsigned int i = 0; i < ci_boss_data.getWeaponDestroyedPieces().size(); i++)
	{
		vWeaponDestroyed.push_back(new weapon_piece_destroyed());
		vWeaponDestroyed[vWeaponDestroyed.size() - 1]->m_weapon_destroyed_animation->setTexture(*glDataBox->GetTexture(ci_boss_data.getWeaponDestroyedPieces()[i].texture));
		vWeaponDestroyed[vWeaponDestroyed.size() - 1]->m_weapon_destroyed_animation->SetTotalFrames(ci_boss_data.getWeaponDestroyedPieces()[i].frames);
		vWeaponDestroyed[vWeaponDestroyed.size() - 1]->m_weapon_destroyed_animation->SetFramesPerSecond(15.0f);
		vWeaponDestroyed[vWeaponDestroyed.size() - 1]->m_weapon_destroyed_animation->SetFrameSize((int)ci_boss_data.getWeaponDestroyedPieces()[i].size.width, (int)ci_boss_data.getWeaponDestroyedPieces()[i].size.height);
		vWeaponDestroyed[vWeaponDestroyed.size() - 1]->m_iLayer = ci_boss_data.getWeaponDestroyedPieces()[i].layer;
		vWeaponDestroyed[vWeaponDestroyed.size() - 1]->setPosition(1250.0f + ci_boss_data.getWeaponDestroyedPieces()[i].offset.x, positionY + ci_boss_data.getWeaponDestroyedPieces()[i].offset.y);
	}
}

void CSystem::destructBoss()
{
	for(unsigned int i = 0; i < vBoss.size(); i++)
	{
		delete vBoss[i];
	}

	for(unsigned int i = 0; i < vMovement.size(); i++)
	{
		delete vMovement[i];
	}

	for(unsigned int i = 0; i < vWeapon.size(); i++)
	{
		delete vWeapon[i];
	}

	for(unsigned int i = 0; i < vWeaponDestroyed.size(); i++)
	{
		delete vWeaponDestroyed[i];
	}

	vBoss.clear();
	vMovement.clear();
	vWeapon.clear();
	vWeaponDestroyed.clear();
}