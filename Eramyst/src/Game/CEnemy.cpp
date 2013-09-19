#include "..\..\include\Game\CEnemy.h"

//Definition of static vector
std::vector<CEnemy::enemy_squad*> CEnemy::m_vSquads;

void CEnemy::init(int _type, int _state, bool _virtual)
{
	ci_timer = new CTimer(0.5f, 1);
	ci_timer->addEventListener(TimerComplete, &hasFlashFinishedWrapper, this);
	
	ci_enemy_data = new CEnemyData();
	
	ai_data.m_iCurrentState = _state;
	ai_data.m_bAttackStarted = false;
	ai_data.m_bAttackFinished = false;

	m_iType = _type;
	m_bVirtual = _virtual;
	m_color = m_visual.getColor();

	setAnimationSpeed(1.0f);

	setPosition((float)ci_enemy_data->getPosition(m_iType).x, (float)ci_enemy_data->getPosition(m_iType).y);

	m_collision.SetSize((float)ci_enemy_data->getSize(m_iType).width/2, (float)ci_enemy_data->getSize(m_iType).height); // width too big for collision box, divide by 2
	m_collision.setOrigin((float)ci_enemy_data->getSize(m_iType).width/4, (float)ci_enemy_data->getSize(m_iType).height/2);
	AddCollisionObject(ID_CO_ENEMY, &m_collision);
	
	m_visual.setTexture(*glDataBox->GetTexture(ci_enemy_data->getTexture(m_iType)));
	m_visual.SetFrameSize(ci_enemy_data->getSize(m_iType).width, ci_enemy_data->getSize(m_iType).height);
	m_visual.SetTotalFrames(ci_enemy_data->getFrames(m_iType));
	
	m_visual.AddAnimation(E_ANIM_MOVE, ci_enemy_data->getAnimation(m_iType).moveStart, ci_enemy_data->getAnimation(m_iType).moveEnd);
	m_visual.AddAnimation(E_ANIM_ATTACK, ci_enemy_data->getAnimation(m_iType).attackStart, ci_enemy_data->getAnimation(m_iType).attackEnd);
	m_visual.AddAnimation(E_ANIM_BACKPEDAL, ci_enemy_data->getAnimation(m_iType).backpedalStart, ci_enemy_data->getAnimation(m_iType).backpedalEnd);
	m_visual.AddAnimation(E_ANIM_WOUNDED, ci_enemy_data->getAnimation(m_iType).woundedStart, ci_enemy_data->getAnimation(m_iType).woundedEnd);
	m_visual.AddAnimation(E_ANIM_DEATH, ci_enemy_data->getAnimation(m_iType).deathStart, ci_enemy_data->getAnimation(m_iType).deathEnd);
	setAnimation(E_ANIM_MOVE, ci_enemy_data->getAnimation(m_iType).bPlayMoveOnce);
	m_bPlayMoveOnce = ci_enemy_data->getAnimation(m_iType).bPlayMoveOnce;
	
	m_visual.setScale(-1.0f, 1.0f);
	m_visual.setOrigin((float)ci_enemy_data->getSize(m_iType).width/2, (float)ci_enemy_data->getSize(m_iType).height/2);
	
	AddDrawableObject(ID_DO_ENEMY, &m_visual);
	
	glLogics->RegisterGameObject(this, ID_GS_PLAY);
}

void CEnemy::setCurrentState(int _state)
{
	ai_data.m_iCurrentState = _state;
}

CEnemy::enemy_ai &CEnemy::getAIData()
{
	return ai_data;
}

pwCollisionRect CEnemy::getCollisionObject() const
{
	return m_collision;
}

void CEnemy::setMove()
{
	if(!m_bPlayMoveOnce)
	{
		setAnimation(E_ANIM_MOVE);
	}
	else
	{
		setAnimation(E_ANIM_MOVE, true);
		m_visual.SetCurrentFrame((float)m_visual.GetCurrentAnimationFrames() - 1);
	}
}

void CEnemy::setAnimation(unsigned int _animation, bool _playOnce)
{
	m_visual.PlayOnce(_playOnce);
	m_visual.SetCurrentAnimation(_animation);
}

void CEnemy::setAnimationSpeed(float _value)
{
	m_visual.SetFramesPerSecond(_value * ci_enemy_data->getAnimationSpeed(m_iType));
}

void CEnemy::flash()
{
	if(ci_timer->getStopped())
	{
		sf::Color flash(ci_enemy_data->getFlashColor(m_iType).r, ci_enemy_data->getFlashColor(m_iType).g, ci_enemy_data->getFlashColor(m_iType).b);
		m_visual.setColor(flash);

		ci_timer->start();
	}
}

void CEnemy::launchAttack()
{
	ai_data.m_bAttackStarted = true;
	setAnimation(E_ANIM_ATTACK, true);

	if(!m_bVirtual)
	{
		CProjectile *ci_projectile;
		ci_projectile = new CProjectile();
		ci_projectile->init(getPosition().x, getPosition().y, m_iType);
	}
}

void CEnemy::launchAttackSalve()
{
	ai_data.m_bAttackStarted = true;
	setAnimation(E_ANIM_ATTACK, true);

	if(!m_bVirtual)
	{
		CProjectile *ci_projectile;
		ci_projectile = new CProjectile();
		ci_projectile->init(getPosition().x, getPosition().y, m_iType, true);
	}
}

void CEnemy::destroy()
{
	Kill();
}

void CEnemy::update()
{
	for(unsigned int i = 0; i < m_vSquads.size(); i++)
	{
		for(unsigned int j = 0; j < m_vSquads[i]->vUnits.size(); j++)
		{
			if(!m_vSquads[i]->vUnits[j]->m_bVirtual)
			{
				glGraphics->Draw(m_vSquads[i]->vUnits[j], (int)(m_vSquads[i]->vUnits[j]->getPosition().y + m_vSquads[i]->vUnits[j]->getEnemyData()->getSize(m_vSquads[i]->vUnits[j]->getType()).height/2)); // Z-Sort by yPos, only add half height because origin is at center
			}
			else // Hidden unit
			{
				m_vSquads[i]->vUnits[j]->m_visual.update();
			}

			m_vSquads[i]->vUnits[j]->updateCollisionObjects(); // Update collision object position and rotation
		}
	}

	deleteDeadObjects();
}

std::vector<CEnemy::enemy_squad*> &CEnemy::getEnemyVector()
{
	return m_vSquads;
}

void CEnemy::hasFlashFinished()
{
	m_visual.setColor(m_color);
}

void CEnemy::hasFlashFinishedWrapper(const void *context)
{
	((CEnemy*)(context))->hasFlashFinished();
}

void CEnemy::deleteDeadObjects()
{
	for(unsigned int i = 0; i < m_vSquads.size(); i++)
	{
		for(unsigned int j = 0; j < m_vSquads[i]->vUnits.size(); j++)
		{
			if(!m_vSquads[i]->vUnits[j]->GetAlive())
			{
				delete m_vSquads[i]->vUnits[j];
				m_vSquads[i]->vUnits.erase(m_vSquads[i]->vUnits.begin() + j);
				j--;
			}
		}
	}

	for(unsigned int i = 0; i < m_vSquads.size(); i++)
	{
		if(m_vSquads[i]->vUnits.size() == 0)
		{
			delete m_vSquads[i];
			m_vSquads.erase(m_vSquads.begin() + i);
			i--;
		}
	}
}