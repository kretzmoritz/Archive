#include "..\..\include\Game\CColoss.h"
#include "..\..\include\Game\CColossRagemode.h"

CColossRagemode::CColossRagemode()
{
	ci_timer_switch_key = new CTimer(5.0f);
	ci_timer_switch_key->addEventListener(TimerTick, &sckWrapper, this);

	ci_timer_check_count = new CTimer(1.0f);
	ci_timer_check_count->addEventListener(TimerTick, &cciWrapper, this);

	// Effect sprites
	m_sprite.setTexture(*glDataBox->GetTexture("collider.png"));
	
	for(int i = 0; i < 4; i++)
	{
		m_vEffectSprite.push_back(m_sprite);
		m_vEffectSprite[i].setOrigin(m_vEffectSprite[i].getGlobalBounds().width/2, m_vEffectSprite[i].getGlobalBounds().height/2);
	}

	m_vEffectSprite[0].setPosition(800.0f, 262.5f);
	m_vEffectSprite[1].setPosition(800.0f, 462.5f);
	m_vEffectSprite[2].setPosition(800.0f, 662.5f);
	m_vEffectSprite[3].setPosition(800.0f, 862.5f);

	// Button animation
	pwAnimation temp;
	temp.setTexture(*glDataBox->GetTexture("Button.png"));
	temp.setOrigin(50.0f, 50.0f);
	temp.SetFramesPerSecond(1.0f);
	temp.SetTotalFrames(2);
	temp.SetFrameSize(100, 100);
	temp.setColor(sf::Color(255, 255, 255, 100));
	temp.AddAnimation(C_ANIM_BUTTON_NORMAL, 0, 0);
	temp.AddAnimation(C_ANIM_BUTTON_CLICK, 1, 1);
	
	m_vButton.push_back(temp);
	m_vButton.push_back(temp);
	m_vButton.push_back(temp);
	m_vButton.push_back(temp);

	m_vButton[0].setPosition(800.0f, 262.5f);
	m_vButton[1].setPosition(800.0f, 462.5f);
	m_vButton[2].setPosition(800.0f, 662.5f);
	m_vButton[3].setPosition(800.0f, 862.5f);

	sf::Text temp_text;
	temp_text.setCharacterSize(40);
	temp_text.setFont(*glDataBox->GetFont("ahronbd.ttf"));
	temp_text.setColor(sf::Color(0, 0, 0, 255));
	m_vButtonText.push_back(temp_text);
	m_vButtonText.push_back(temp_text);
	m_vButtonText.push_back(temp_text);
	m_vButtonText.push_back(temp_text);
	m_vButtonText.push_back(temp_text);

	m_vButtonText[0].setPosition(800.0f, 262.5f);
	m_vButtonText[1].setPosition(800.0f, 462.5f);
	m_vButtonText[2].setPosition(800.0f, 662.5f);
	m_vButtonText[3].setPosition(800.0f, 862.5f);
	m_vButtonText[4].setPosition((float)pwEngineOptions::GetInternGraphicsSize().x/2, (float)pwEngineOptions::GetInternGraphicsSize().y - 90.0f);

	// Shield collider
	m_shieldCollider.setPosition(-50.0f, 540.0f);
	m_shieldCollider.SetRadius(750.0f);

	// Shockwave icon
	m_ShockwaveIcon.setTexture(*glDataBox->GetTexture("Spacebar.png"));
	m_ShockwaveIcon.setScale(0.75f, 0.75f);
	m_ShockwaveIcon.setOrigin(312.0f, 50.0f);
	m_ShockwaveIcon.SetFramesPerSecond(2.0f);
	m_ShockwaveIcon.SetTotalFrames(2);
	m_ShockwaveIcon.SetFrameSize(624, 100);
	m_ShockwaveIcon.setPosition((float)pwEngineOptions::GetInternGraphicsSize().x/2, (float)pwEngineOptions::GetInternGraphicsSize().y - 100.0f);

	// Shockwave collider
	m_shockwaveCollider.setPosition(250.0f, 540.0f);
}

void CColossRagemode::start()
{
	setImmortal(true);
	enableFlash();

	// Delete projectiles currently on screen
	for(unsigned int i = 0; i < getProjectileVector().size(); i++)
	{
		getProjectileVector()[i]->setBounced(true);
		getProjectileVector()[i]->bounceOrDestroy();
	}

	ci_timer_switch_key->start();
	ci_timer_check_count->start();

	m_bShieldKeyPressed = false;
	m_bShockwaveKeyPressed = false;

	m_iSpamCount = 0;
	m_iStrengthening = 0;

	m_fAttack = 0.0f;
	m_fShieldStrength = 0.0f;
	m_fPreviousShieldStrength = 0.0f;

	m_fScaleFactor = 0.0f;

	chooseRandomButton();

	// Shockwave
	m_bShockwaveIconShown = false;
	m_ShockwaveIcon.setColor(sf::Color(255, 255, 255, 0));

	m_shockwaveCollider.SetRadius(0.0f);
	CInterface::get()->setCurrentRageWaveProgress(m_shockwaveCollider.GetRadius());

	// Button sprites
	for(unsigned int i = 0; i < m_vButton.size(); i++)
	{
		m_vButton[i].SetCurrentAnimation(C_ANIM_BUTTON_NORMAL);
	}

	// Button text
	m_vButtonText[0].setString(pwHelper::key::getKeyName(CSettings::get()->getLayerKey(0)));
	m_vButtonText[1].setString(pwHelper::key::getKeyName(CSettings::get()->getLayerKey(1)));
	m_vButtonText[2].setString(pwHelper::key::getKeyName(CSettings::get()->getLayerKey(2)));
	m_vButtonText[3].setString(pwHelper::key::getKeyName(CSettings::get()->getLayerKey(3)));
	m_vButtonText[4].setString(pwHelper::key::getKeyName(CSettings::get()->getLayerKey(4)));

	for(unsigned int i = 0; i < m_vButtonText.size(); i++)
	{
		m_vButtonText[i].setOrigin(m_vButtonText[i].getGlobalBounds().width/2, m_vButtonText[i].getGlobalBounds().height);
	}

	// Reset coloss animation to normal state
	for(unsigned int i = 0; i < getBlockVisuals().size(); i++)
	{
		getBlockVisuals()[i].hold_animation = false;
	}

	// Interface ragemode begin
	CInterface::get()->startRage();

	// Switch to ragemode animation
	for(unsigned int i = 0; i < getMoveVisuals().size(); i++)
	{
		getMoveVisuals()[i].animation.setColor(sf::Color(255, 255, 255, 0));
	}

	for(unsigned int i = 0; i < getBlockVisuals().size(); i++)
	{
		getBlockVisuals()[i].animation.SetFramesPerSecond(ci_coloss_data.getFramesPerSecond());
		getBlockVisuals()[i].animation.PlayOnce();
		getBlockVisuals()[i].animation.SetCurrentAnimation(C_ANIM_PULLBACK);
		getBlockVisuals()[i].animation.ResetCurrentAnimation();

		if(i == 0)
		{
			getBlockVisuals()[i].animation.PlayOnce(false);
			getBlockVisuals()[i].animation.SetCurrentAnimation(C_ANIM_IDLE);
		}
		else
		{
			getBlockVisuals()[i].animation.setColor(sf::Color(255, 255, 255, 0));
		}
	}

	for(unsigned int i = 0; i < getRagemodeVisuals().size(); i++)
	{
		getRagemodeVisuals()[i].animation.setColor(sf::Color(255, 255, 255, 255));
	}
}

void CColossRagemode::run()
{
	getInput();
	applySpriteEffect();
	
	drawSpamElements();
	manageShockwave();

	modifyShieldEffect();
	checkShieldCollision();

	applyShockwaveAttack();

	glGraphics->Draw(this);

	CDisplayBar::get()->setRage(m_fAttack / 500.0f);
	CDisplayBar::get()->drawRage();
	CColoss::run(); // Call to super function
}

void CColossRagemode::close()
{
	enableFlash();

	// Delete projectiles currently on screen
	for(unsigned int i = 0; i < getProjectileVector().size(); i++)
	{
		getProjectileVector()[i]->setBounced(true);
		getProjectileVector()[i]->bounceOrDestroy();
	}

	ci_timer_switch_key->reset();
	ci_timer_check_count->reset();

	m_shockwaveCollider.SetRadius(0.0f);
	CInterface::get()->setCurrentRageWaveProgress(m_shockwaveCollider.GetRadius());

	CInterface::get()->setCurrentShieldPercentage(0.0f);

	// Remove 1/8th of score after ragemode
	int iPenalty = CInterface::get()->getCurrentScore() - (CInterface::get()->getCurrentScore() * 7/8);

	if(iPenalty != 0)
	{
		CInterface::get()->setCurrentScore(CInterface::get()->getCurrentScore() * 7/8);

		std::stringstream string_typecast;
		std::string temp;
		string_typecast << iPenalty;
		string_typecast >> temp;
		string_typecast.clear();

		std::string negative_points = "Ragemode penalty: -";
		negative_points += temp;
		text_negative_points.setString(negative_points);
		text_negative_points.setColor(sf::Color(0, 0, 0, 255));
		text_negative_points.setOrigin(text_negative_points.getGlobalBounds().width/2, text_negative_points.getGlobalBounds().height/2);

		m_fFadeOutText = 255.0f;
	}

	// Interface ragemode end
	CInterface::get()->endRage();

	// Switch to move animation
	for(unsigned int i = 0; i < getMoveVisuals().size(); i++)
	{
		getMoveVisuals()[i].animation.setColor(sf::Color(255, 255, 255, 255));
	}

	disableRagemodeColoss();
}

void CColossRagemode::getInput()
{
	if(sf::Keyboard::isKeyPressed(CSettings::get()->getLayerKey(m_iCurrentKey)) && !m_bShieldKeyPressed)
	{
		m_iSpamCount++;
		
		enableCooldown(m_iCurrentKey);
		m_vButton[m_iCurrentKey].SetCurrentAnimation(C_ANIM_BUTTON_CLICK);
		
		m_bShieldKeyPressed = true;
	}
	else if(!sf::Keyboard::isKeyPressed(CSettings::get()->getLayerKey(m_iCurrentKey)))
	{
		disableCooldown(m_iCurrentKey);
		m_vButton[m_iCurrentKey].SetCurrentAnimation(C_ANIM_BUTTON_NORMAL);
		
		m_bShieldKeyPressed = false;
	}

	if(sf::Keyboard::isKeyPressed(CSettings::get()->getLayerKey(vNotAllowedKeys[0])) || sf::Keyboard::isKeyPressed(CSettings::get()->getLayerKey(vNotAllowedKeys[1])) || sf::Keyboard::isKeyPressed(CSettings::get()->getLayerKey(vNotAllowedKeys[2])))
	{
		m_iSpamCount = 0; // Punish the player for pressing multiple keys
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !m_bShockwaveKeyPressed && m_fAttack >= 500.0f)
	{
		m_fAttack = 0.0f;
		m_bShockwaveIconShown = false;
		m_bShockwaveKeyPressed = true;

		CInterface::get()->setSpaceBlur(false);
	}
}

void CColossRagemode::enableCooldown(int _layer)
{
	sf::Color color_cd(255, 0, 0);
	m_vEffectSprite[_layer].setColor(color_cd);
}

void CColossRagemode::disableCooldown(int _layer)
{
	sf::Color color_nocd(255, 255, 255);
	m_vEffectSprite[_layer].setColor(color_nocd);
}

void CColossRagemode::applySpriteEffect()
{
	sf::Color alpha;
	
	alpha = m_vEffectSprite[0].getColor();
	alpha.a = (int)((1.0f - m_fScaleFactor) * 255.0f);
	m_vEffectSprite[0].setScale(m_fScaleFactor, m_fScaleFactor);
	m_vEffectSprite[0].setColor(alpha);

	alpha = m_vEffectSprite[1].getColor();
	alpha.a = (int)((1.0f - m_fScaleFactor) * 255.0f);
	m_vEffectSprite[1].setScale(m_fScaleFactor, m_fScaleFactor);
	m_vEffectSprite[1].setColor(alpha);

	alpha = m_vEffectSprite[2].getColor();
	alpha.a = (int)((1.0f - m_fScaleFactor) * 255.0f);
	m_vEffectSprite[2].setScale(m_fScaleFactor, m_fScaleFactor);
	m_vEffectSprite[2].setColor(alpha);

	alpha = m_vEffectSprite[3].getColor();
	alpha.a = (int)((1.0f - m_fScaleFactor) * 255.0f);
	m_vEffectSprite[3].setScale(m_fScaleFactor, m_fScaleFactor);
	m_vEffectSprite[3].setColor(alpha);
	
	m_fScaleFactor += (2.0f / CBeatSystem::GetPerBeatTime()) * glGraphics->GetLastFrameTime(); // Every 1/4th beat = 2.0f

	if(m_fScaleFactor >= 1.0f)
	{
		m_fScaleFactor -= 1.0f;
	}
}

void CColossRagemode::drawSpamElements()
{
	for(unsigned int i = 0; i < m_vButton.size(); i++)
	{
		if(m_vButton[i].WasPlayed())
		{
			m_vButton[i].SetCurrentAnimation(C_ANIM_BUTTON_NORMAL);
		}
	}

	glGraphics->Draw(&m_vButton[m_iCurrentKey]);
	glGraphics->Draw(&m_vButtonText[m_iCurrentKey]);
	glGraphics->Draw(&m_vEffectSprite[m_iCurrentKey]);
}

void CColossRagemode::manageShockwave()
{
	if(m_fAttack >= 500.0f && !m_bShockwaveIconShown)
	{
		m_ShockwaveIcon.setColor(sf::Color(255, 255, 255, 255));
		m_bShockwaveIconShown = true;

		CInterface::get()->setSpaceBlur(true);
	}

	if(m_bShockwaveIconShown)
	{
		glGraphics->Draw(&m_ShockwaveIcon, 3000);
		glGraphics->Draw(&m_vButtonText[4], 3001);
	}

	if(m_bShockwaveKeyPressed)
	{
		m_shockwaveCollider.SetRadius(m_shockwaveCollider.GetRadius() + 650.0f * glGraphics->GetLastFrameTime());
		CInterface::get()->setCurrentRageWaveProgress(m_shockwaveCollider.GetRadius());

		if(m_shockwaveCollider.GetRadius() > 2000.0f)
		{
			m_bShockwaveKeyPressed = false;
		}
	}
	else
	{
		m_shockwaveCollider.SetRadius(0.0f);
		CInterface::get()->setCurrentRageWaveProgress(m_shockwaveCollider.GetRadius());
	}
}

void CColossRagemode::modifyShieldEffect()
{
	float fPercentageChange = 75.0f * glGraphics->GetLastFrameTime();

	if(m_iStrengthening == -1)
	{
		CInterface::get()->setCurrentShieldPercentage(CInterface::get()->getCurrentShieldPercentage() - fPercentageChange);

		if(CInterface::get()->getCurrentShieldPercentage() < 0.0f)
		{
			CInterface::get()->setCurrentShieldPercentage(0.0f);
		}
	}
	else if(m_iStrengthening == 1)
	{
		CInterface::get()->setCurrentShieldPercentage(CInterface::get()->getCurrentShieldPercentage() + fPercentageChange);

		if(CInterface::get()->getCurrentShieldPercentage() > 100.0f)
		{
			CInterface::get()->setCurrentShieldPercentage(100.0f);
		}
	}
	else if(m_iStrengthening == 0 && m_fShieldStrength == 0.0f)
	{
		CInterface::get()->setCurrentShieldPercentage(CInterface::get()->getCurrentShieldPercentage() - fPercentageChange);

		if(CInterface::get()->getCurrentShieldPercentage() < 0.0f)
		{
			CInterface::get()->setCurrentShieldPercentage(0.0f);
		}
	}

	if(getHealth() == 0.0f)
	{
		CInterface::get()->setCurrentShieldPercentage(0.0f);
	}
}

void CColossRagemode::checkShieldCollision()
{
	for(unsigned int i = 0; i < getProjectileVector().size(); i++)
	{
		if(getProjectileVector()[i]->getCollisionObject()->Collides(&m_shieldCollider) && !getProjectileVector()[i]->getShieldIgnore())
		{
			float fRandom = (float)(rand() % 10001); // 0 - 10000
			fRandom /= 100.0f; // float between 0 - 100

			if(fRandom < m_fShieldStrength)
			{
				getProjectileVector()[i]->bounceOrDestroy();
			}
			else
			{
				getProjectileVector()[i]->setShieldIgnore(true);
			}
		}
	}
}

void CColossRagemode::applyShockwaveAttack()
{
	for(unsigned int i = 0; i < getProjectileVector().size(); i++)
	{
		if(getProjectileVector()[i]->getCollisionObject()->Collides(&m_shockwaveCollider))
		{
			getProjectileVector()[i]->setBounced(true);
			getProjectileVector()[i]->bounceOrDestroy();
		}
	}

	for(unsigned int i = 0; i < getEnemyVector().size(); i++)
	{
		for(unsigned int j = 0; j < getEnemyVector()[i]->vUnits.size(); j++)
		{
            if (!getEnemyVector()[i]->vUnits[j]->getWasHit() && getEnemyVector()[i]->vUnits[j]->getCollisionObject().Collides(&m_shockwaveCollider))
            {
				glLogics->RegisterGameObject(getEnemyVector()[i]->vUnits[j], ID_GS_PLAY);
				
				getEnemyVector()[i]->vUnits[j]->getAIData().m_iCurrentState = DEATH_ENEMY;
				getEnemyVector()[i]->vUnits[j]->SetGravityMultiplier(2.0f);
                getEnemyVector()[i]->vUnits[j]->SetStartVelocity(600.0f, -400.0f);
                getEnemyVector()[i]->vUnits[j]->ApplyForce(600.0f, -400.0f);

                getEnemyVector()[i]->vUnits[j]->setWasHit();
			}
            else if (getEnemyVector()[i]->vUnits[j]->getWasHit())
            {
                getEnemyVector()[i]->vUnits[j]->setRotation(-atan2(getEnemyVector()[i]->vUnits[j]->GetStartVelocity().y, getEnemyVector()[i]->vUnits[j]->GetStartVelocity().x)*180.0f / pwHelper::math::PI);
            }
		}
	}
}

void CColossRagemode::chooseRandomButton()
{
	m_iCurrentKey = rand() % 4;

	vNotAllowedKeys.clear();
	switch(m_iCurrentKey)
	{
		case 0:
			vNotAllowedKeys.push_back(1);
			vNotAllowedKeys.push_back(2);
			vNotAllowedKeys.push_back(3);
			break;
		case 1:
			vNotAllowedKeys.push_back(0);
			vNotAllowedKeys.push_back(2);
			vNotAllowedKeys.push_back(3);
			break;
		case 2:
			vNotAllowedKeys.push_back(0);
			vNotAllowedKeys.push_back(1);
			vNotAllowedKeys.push_back(3);
			break;
		case 3:
			vNotAllowedKeys.push_back(0);
			vNotAllowedKeys.push_back(1);
			vNotAllowedKeys.push_back(2);
			break;
	}

	// Move interface blur to y button position
	CInterface::get()->setCurrentRageBlurY(m_vButton[m_iCurrentKey].getPosition().y);
}

void CColossRagemode::switchCurrentKey()
{
	setImmortal(false);

	if(ci_timer_switch_key->getTickCount() > 3)
	{
		if(getHealth() > 0.0f) // So coloss can't gain health while dying
		{
			modHealth(60.0f);
		}
	}

	chooseRandomButton();
}

void CColossRagemode::sckWrapper(const void *context)
{
	((CColossRagemode*)(context))->switchCurrentKey();
}

void CColossRagemode::checkCountIncrease()
{
	float m_fNeededCountForFullStrength = 4.0f;
	m_fShieldStrength = ((float)m_iSpamCount / m_fNeededCountForFullStrength) * 100.0f;
	
	if(m_fShieldStrength > 100.0f)
	{
		m_fAttack += m_fShieldStrength - 100.0f;
		m_fShieldStrength = 100.0f;
	}

	if(m_fPreviousShieldStrength > m_fShieldStrength)
	{
		m_iStrengthening = -1;
	}
	else if(m_fPreviousShieldStrength < m_fShieldStrength)
	{
		m_iStrengthening = 1;
	}
	else
	{
		m_iStrengthening = 0;
	}

	m_fPreviousShieldStrength = m_fShieldStrength;
	m_iSpamCount = 0;
}

void CColossRagemode::cciWrapper(const void *context)
{
	((CColossRagemode*)(context))->checkCountIncrease();
}