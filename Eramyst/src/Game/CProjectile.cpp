#include "..\..\include\Game\CProjectile.h"
#include "..\..\include\Game\CBeatSystem.h"

//Definition of static variable and vector
float CProjectile::m_fTime;
std::vector<CProjectile*> CProjectile::m_vProjectiles;

void CProjectile::init(float _x, float _y, int _parentID, bool _salve)
{
	ci_projectile_data = new CProjectileData();
	
	std::string sProjectileSound;
	sProjectileSound = ci_projectile_data->getSound(_parentID);

	if(!_salve)
	{
		sProjectileSound += ".wav";
		PlaySound(glDataBox->GetSoundBuffer(sProjectileSound), _parentID + 4); // Make sure sounds get played on different layers
	}
	else
	{
		sProjectileSound += "_salve.wav";
		PlaySound(glDataBox->GetSoundBuffer(sProjectileSound), _parentID + 4);
	}

	m_fStartX = _x;
	m_fStartY = _y;

	m_iParent = _parentID;
	m_bSalve = _salve;
	setBlocked(-1);
	
	setScale(-1.0f, 1.0f);
	setPosition(_x, _y);
	setOrigin(ci_projectile_data->getOrigin(m_iParent).x, ci_projectile_data->getOrigin(m_iParent).y);

	m_collision = new pwCollisionRect();
	m_collision->SetSize((float)ci_projectile_data->getSize(m_iParent).width, (float)ci_projectile_data->getSize(m_iParent).height);
	AddCollisionObject(ID_CO_PROJECTILE, m_collision);

	SetGravityMultiplier(ci_projectile_data->getGravity(m_iParent) * (CBeatSystem::GetBPM() * 1.5f) / 80.0f); // Use BMP to modify gravity to a certain extent -> projectile needs to hit coloss
	m_visual.SetFramesPerSecond(ci_projectile_data->getAnimationSpeed(m_iParent));
	
	velocity temp = calculateForce(m_iParent);
	SetStartVelocity(temp.xVel, temp.yVel);

	m_visual.setTexture(*glDataBox->GetTexture(ci_projectile_data->getTexture(m_iParent)));
	m_visual.SetFrameSize(ci_projectile_data->getSize(m_iParent).width, ci_projectile_data->getSize(m_iParent).height);
	m_visual.SetTotalFrames(ci_projectile_data->getFrames(m_iParent));
	
	m_visual.AddAnimation(P_ANIM_NORMAL, ci_projectile_data->getAnimation(m_iParent).normalStart, ci_projectile_data->getAnimation(m_iParent).normalEnd);
	m_visual.AddAnimation(P_ANIM_DEATH, ci_projectile_data->getAnimation(m_iParent).deathStart, ci_projectile_data->getAnimation(m_iParent).deathEnd);
	m_visual.SetCurrentAnimation(P_ANIM_NORMAL);

	AddDrawableObject(ID_DO_PROJECTILE, &m_visual);

	m_vProjectiles.push_back(this);
	glLogics->RegisterGameObject(this, ID_GS_PLAY);
}

void CProjectile::bounceOrDestroy()
{
	float fRandom = (float)(rand() % 10001); // 0 - 10000
	fRandom /= 100.0f; // float between 0 - 100

	if(!getBounced() && fRandom < ci_projectile_data->getBounceChance(m_iParent))
	{
		if(CLevelData::getCurrentLevelString() == "darkage")
		{
			SetStartVelocity(100.0f, myVelocity.y);
		}
		else if(CLevelData::getCurrentLevelString() == "future")
		{
			SetStartVelocity(300.0f, myVelocity.y);
		}

		setBounced(true);
	}
	else
	{
		initiateDestruction();
	}
}

void CProjectile::setBounced(bool _value)
{
	m_bBounced = _value;
}

bool CProjectile::getBounced() const
{
	return m_bBounced;
}

pwCollisionRect* CProjectile::getCollisionObject() const
{
	return m_collision;
}

void CProjectile::initiateDestruction()
{
	float fCurrentDir = (myVelocity.x / abs(myVelocity.x)) * -1.0f;

	m_bDead = true;

	SetStartVelocity(25.0f * fCurrentDir, 0.0f);
	m_visual.SetCurrentAnimation(P_ANIM_DEATH);
}

void CProjectile::setFlightTime(float _value)
{
	m_fTime = _value;
}

void CProjectile::modFlightTime(float _value)
{
	m_fTime += _value;
}

float CProjectile::getFlightTime()
{
	return m_fTime;
}

void CProjectile::update()
{
	for(unsigned int i = 0; i < m_vProjectiles.size(); i++)
	{
		if(m_vProjectiles[i]->m_bDead) // If projectile is dead -> fade out
		{
			m_vProjectiles[i]->m_visual.setColor(sf::Color(255, 255, 255, m_vProjectiles[i]->m_visual.getColor().a - 2));
		}

		if((m_vProjectiles[i]->m_visual.getColor().a <= 10.0f && m_vProjectiles[i]->m_bDead) || (m_vProjectiles[i]->getPosition().x < 0 || m_vProjectiles[i]->getPosition().x > 1920 || m_vProjectiles[i]->getPosition().y > 1080)) // If fade out complete or projectile off screen -> destroy it
		{
			m_vProjectiles[i]->destroy();
		}

		m_vProjectiles[i]->rotateToCurrentDirection();
		glGraphics->Draw(m_vProjectiles[i], 1);
			
		m_vProjectiles[i]->updateCollisionObjects(); // Update collision object position and rotation
	}

	deleteDeadObjects();
}

std::vector<CProjectile*> &CProjectile::getProjectileVector()
{
	return m_vProjectiles;
}

CProjectile::velocity CProjectile::calculateForce(int _parentID)
{
	velocity result;
    float dX;
    float dY;

	m_iTarget = ci_projectile_data->getTarget(_parentID);
	
	// Boss projectiles have random target
	if(_parentID == 8)
	{
		if(CSystem::vBoss.size() > 2)
		{
			m_iTarget = rand() % 2;
		}
		else
		{
			m_iTarget = (rand() % 2) + 2;
		}
	}
	else if(_parentID == 9)
	{
		m_iTarget = rand() % 3;
	}
	else if(_parentID == 10 || _parentID == 11)
	{
		m_iTarget = rand() % 4;
	}

	// Top to bottom
    switch(m_iTarget)
	{
		case 0:
	        dX = m_fStartX - 764.0f;
	        dY = m_fStartY - 262.5f;
			break;
		case 1:
	        dX = m_fStartX - 764.0f;
	        dY = m_fStartY - 462.5f;
			break;
		case 2:
	        dX = m_fStartX - 764.0f;
	        dY = m_fStartY - 662.5f;
			break;
		case 3:
	        dX = m_fStartX - 764.0f;
	        dY = m_fStartY - 862.5f;
			break;
	}

	float dT = m_fTime; // Brauche eine bestimmte Sekundenanzahl für den Schuss

	if(_parentID == 8 || _parentID == 9 || _parentID == 10 || _parentID == 11) // Boss projectile is slow
	{
		dT = 2.0f;
	}

	float inv_dT = 1.0f / dT; // Beim verwendeten dT relativ sinnlos ... bei anderen dT sinnvoll

    result.xVel = -(dX * inv_dT);
	result.yVel = -((dY + (0.5f * glLogics->GetGravity() * GetGravityMultiplier() * dT*dT)) * inv_dT);

	return result;
}

void CProjectile::rotateToCurrentDirection()
{
	if(!m_bDead)
	{
		if(ci_projectile_data->getRotation(m_iParent) != 0.0f) // Axtwurf
		{
			rotate(ci_projectile_data->getRotation(m_iParent) * glGraphics->GetLastFrameTime() * 360.0f);
		}
		else
		{
			point start;
			start.x = getPosition().x;
			start.y = getPosition().y;
	
			point target;
			target.x = getPosition().x + myVelocity.x;
			target.y = getPosition().y + myVelocity.y;

			point direction;
			direction.x = target.x - start.x;
			direction.y = target.y - start.y;

			// atan2 returns radians, convert this to degrees
			setRotation(atan2(direction.y, direction.x) * 180 / PI_PROJECTILE);
		}
	}
}

void CProjectile::destroy()
{
	Kill();
}

void CProjectile::deleteDeadObjects()
{
	for(unsigned int i = 0; i < m_vProjectiles.size(); i++)
	{
		if(!m_vProjectiles[i]->GetAlive())
		{
			delete m_vProjectiles[i];
			m_vProjectiles.erase(m_vProjectiles.begin() + i);
			i--;
		}
	}
}