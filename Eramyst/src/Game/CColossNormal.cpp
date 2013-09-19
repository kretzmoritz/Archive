#include "..\..\include\Game\CColoss.h"
#include "..\..\include\Game\CColossNormal.h"

CColossNormal::CColossNormal()
{
	m_iPreviousFrame = -1;
	
	ci_timer_top = new CTimer(1.0f, 1);
	ci_timer_top->addEventListener(TimerComplete, trcWrapper, this);

	ci_timer_middle_higher = new CTimer(1.0f, 1);
	ci_timer_middle_higher->addEventListener(TimerComplete, mhrcWrapper, this);

	ci_timer_middle_lower = new CTimer(1.0f, 1);
	ci_timer_middle_lower->addEventListener(TimerComplete, mlrcWrapper, this);

	ci_timer_bottom = new CTimer(1.0f, 1);
	ci_timer_bottom->addEventListener(TimerComplete, brcWrapper, this);

	// Button sprites
	pwAnimation temp;
	temp.PlayOnce();
	temp.setOrigin(82.5f, 82.5f);
	temp.SetFramesPerSecond(1.0f);
	temp.SetTotalFrames(1);
	temp.SetFrameSize(165, 165);
	temp.setScale(0.5f, 0.5f);
	temp.setColor(sf::Color(255, 255, 255, 200));
	temp.AddAnimation(C_ANIM_BUTTON_NORMAL, 0, 0);

	for(int i = 0; i < 4; i++)
	{
		m_vButton.push_back(temp);
	}

	m_vButton[0].setPosition(764.0f, 262.5f);
	m_vButton[1].setPosition(764.0f, 462.5f);
	m_vButton[2].setPosition(764.0f, 662.5f);
	m_vButton[3].setPosition(764.0f, 862.5f);

	// Button text
	sf::Text temp_text;
	temp_text.setFont(*glDataBox->GetFont("ahronbd.ttf"));
	temp_text.setColor(sf::Color(0, 0, 0, 255));
	
	for(int i = 0; i < 4; i++)
	{
		m_vButtonText.push_back(temp_text);
	}

	m_vButtonText[0].setPosition(764.0f, 262.5f);
	m_vButtonText[1].setPosition(764.0f, 462.5f);
	m_vButtonText[2].setPosition(764.0f, 662.5f);
	m_vButtonText[3].setPosition(764.0f, 862.5f);

	// Projectile block will be initiated here
	for(int i = 0; i < 4; i++)
	{
		m_vLayerCollision.push_back(m_collision);
	}

	m_vLayerCollision[0].setPosition(764.0f, 262.5f); // Top
	m_vLayerCollision[1].setPosition(764.0f, 462.5f); // Middle higher
	m_vLayerCollision[2].setPosition(764.0f, 662.5f); // Middle lower
	m_vLayerCollision[3].setPosition(764.0f, 862.5f); // Bottom

	// Projectile destruction will happen here
	for(int i = 0; i < 4; i++)
	{
		m_vBlockCollision.push_back(m_collision);
		m_vBlockCollision[i].SetSize(75.0f, 1080.0f);
		m_vBlockCollision[i].setOrigin(m_vBlockCollision[i].GetSize().x/2, 0.0f);
	}

	m_vBlockCollision[0].setPosition(550.0f, 0.0f); // Top
	m_vBlockCollision[1].setPosition(575.0f, 0.0f); // Middle higher
	m_vBlockCollision[2].setPosition(450.0f, 0.0f); // Middle lower
	m_vBlockCollision[3].setPosition(450.0f, 0.0f); // Bottom

	SetRenderCollisionObjects(true);

	// Block sounds
	data = glDataBox->GetIniFile("sounddata.ini");

	// Boss arrows
	m_bossArrow_top.setTexture(*glDataBox->GetTexture("boss_pointer.png"));
	m_bossArrow_top.setPosition(764.0f, 262.5f);
	m_bossArrow_top.setOrigin(60.0f, 52.5f);

	m_bossArrow_middle_higher.setTexture(*glDataBox->GetTexture("boss_pointer.png"));
	m_bossArrow_middle_higher.setPosition(764.0f, 462.5f);
	m_bossArrow_middle_higher.setOrigin(60.0f, 52.5f);

	m_bossArrow_middle_lower.setTexture(*glDataBox->GetTexture("boss_pointer.png"));
	m_bossArrow_middle_lower.setPosition(764.0f, 662.5f);
	m_bossArrow_middle_lower.setOrigin(60.0f, 52.5f);

	m_bossArrow_bottom.setTexture(*glDataBox->GetTexture("boss_pointer.png"));
	m_bossArrow_bottom.setPosition(764.0f, 862.5f);
	m_bossArrow_bottom.setOrigin(60.0f, 52.5f);
}

void CColossNormal::start()
{
	m_bTop_KeyPressed = false;
	m_bMiddleHigher_KeyPressed = false;
	m_bMiddleLower_KeyPressed = false;
	m_bBottom_KeyPressed = false;

	m_bTopBlock_active = false;
	m_bMiddleHigherBlock_active = false;
	m_bMiddleLowerBlock_active = false;
	m_bBottomBlock_active = false;

	// Button sprites
	if(CLevelData::getCurrentLevelString() == "darkage")
	{
		m_vButton[0].setTexture(*glDataBox->GetTexture("btn_da_top.png"));
		m_vButton[1].setTexture(*glDataBox->GetTexture("btn_da_midhigh.png"));
		m_vButton[2].setTexture(*glDataBox->GetTexture("btn_da_midlow.png"));
		m_vButton[3].setTexture(*glDataBox->GetTexture("btn_da_bottom.png"));
	}
	else if(CLevelData::getCurrentLevelString() == "future")
	{
		m_vButton[0].setTexture(*glDataBox->GetTexture("btn_fu_top.png"));
		m_vButton[1].setTexture(*glDataBox->GetTexture("btn_fu_midhigh.png"));
		m_vButton[2].setTexture(*glDataBox->GetTexture("btn_fu_midlow.png"));
		m_vButton[3].setTexture(*glDataBox->GetTexture("btn_fu_bottom.png"));
	}

	// Button text
	m_vButtonText[0].setString(pwHelper::key::getKeyName(CSettings::get()->getLayerKey(0)));
	m_vButtonText[1].setString(pwHelper::key::getKeyName(CSettings::get()->getLayerKey(1)));
	m_vButtonText[2].setString(pwHelper::key::getKeyName(CSettings::get()->getLayerKey(2)));
	m_vButtonText[3].setString(pwHelper::key::getKeyName(CSettings::get()->getLayerKey(3)));

	for(unsigned int i = 0; i < m_vButtonText.size(); i++)
	{
		m_vButtonText[i].setOrigin(m_vButtonText[i].getGlobalBounds().width/2, m_vButtonText[i].getGlobalBounds().height);
	}

	// Projectile block default size
	for(int i = 0; i < 4; i++)
	{
		m_vLayerCollision[i].SetSize(115.0f, 115.0f);
		m_vLayerCollision[i].setOrigin(m_vLayerCollision[i].GetSize().x/2, m_vLayerCollision[i].GetSize().y/2);
	}
}

void CColossNormal::run()
{
	getInput();
	scaleButtons();
	drawButtons();

	if(!getBoss())
	{
		drawScore();
	}
	
	tweenBlock();

	// Check for block collision
	hasBlockFinished();

	glGraphics->Draw(this);
	CColoss::run(); // Call to super function

	// Boss interface functionality
	manageBossInterface();
}

void CColossNormal::getInput()
{
	if(sf::Keyboard::isKeyPressed(CSettings::get()->getLayerKey(0)) && !m_bTop_KeyPressed)
	{
		checkForSalve(TOP_COLOSS);

		if(!hasBlockStarted(TOP_COLOSS) && !m_bTopSalve)
		{
			ci_timer_top->start();
			
			m_vButton[0].setColor(sf::Color(255, 0, 0, 200));
			m_vButton[0].SetCurrentAnimation(C_ANIM_BUTTON_NORMAL);
			m_vButton[0].RestartAnimation();
		}

		if(m_bTopSalve)
		{
			getBlockVisuals()[0].hold_animation = true;
		}

		m_bTop_KeyPressed = true;
	}
	else if(!sf::Keyboard::isKeyPressed(CSettings::get()->getLayerKey(0)) && ci_timer_top->getStopped())
	{
		m_bTop_KeyPressed = false;
		getBlockVisuals()[0].hold_animation = false;
	}
	else if(m_bTopSalve)
	{
		m_bTop_KeyPressed = false;
	}

	if(sf::Keyboard::isKeyPressed(CSettings::get()->getLayerKey(1)) && !m_bMiddleHigher_KeyPressed)
	{
		checkForSalve(MIDDLE_HIGHER_COLOSS);

		if(!hasBlockStarted(MIDDLE_HIGHER_COLOSS) && !m_bMiddleHigherSalve)
		{
			ci_timer_middle_higher->start();
			
			m_vButton[1].setColor(sf::Color(255, 0, 0, 200));
			m_vButton[1].SetCurrentAnimation(C_ANIM_BUTTON_NORMAL);
			m_vButton[1].RestartAnimation();
		}

		if(m_bMiddleHigherSalve)
		{
			getBlockVisuals()[1].hold_animation = true;
		}

		m_bMiddleHigher_KeyPressed = true;
	}
	else if(!sf::Keyboard::isKeyPressed(CSettings::get()->getLayerKey(1)) && ci_timer_middle_higher->getStopped())
	{
		m_bMiddleHigher_KeyPressed = false;
		getBlockVisuals()[1].hold_animation = false;
	}
	else if(m_bMiddleHigherSalve)
	{
		m_bMiddleHigher_KeyPressed = false;
	}

	if(sf::Keyboard::isKeyPressed(CSettings::get()->getLayerKey(2)) && !m_bMiddleLower_KeyPressed)
	{
		checkForSalve(MIDDLE_LOWER_COLOSS);

		if(!hasBlockStarted(MIDDLE_LOWER_COLOSS) && !m_bMiddleLowerSalve)
		{
			ci_timer_middle_lower->start();
			
			m_vButton[2].setColor(sf::Color(255, 0, 0, 200));
			m_vButton[2].SetCurrentAnimation(C_ANIM_BUTTON_NORMAL);
			m_vButton[2].RestartAnimation();
		}

		if(m_bMiddleLowerSalve)
		{
			getBlockVisuals()[2].hold_animation = true;
		}

		m_bMiddleLower_KeyPressed = true;
	}
	else if(!sf::Keyboard::isKeyPressed(CSettings::get()->getLayerKey(2)) && ci_timer_middle_lower->getStopped())
	{
		m_bMiddleLower_KeyPressed = false;
		getBlockVisuals()[2].hold_animation = false;
	}
	else if(m_bMiddleLowerSalve)
	{
		m_bMiddleLower_KeyPressed = false;
	}

	if(sf::Keyboard::isKeyPressed(CSettings::get()->getLayerKey(3)) && !m_bBottom_KeyPressed)
	{
		checkForSalve(BOTTOM_COLOSS);

		if(!hasBlockStarted(BOTTOM_COLOSS) && !m_bBottomSalve)
		{
			ci_timer_bottom->start();
			
			m_vButton[3].setColor(sf::Color(255, 0, 0, 200));
			m_vButton[3].SetCurrentAnimation(C_ANIM_BUTTON_NORMAL);
			m_vButton[3].RestartAnimation();
		}

		if(m_bBottomSalve)
		{
			getBlockVisuals()[3].hold_animation = true;
		}

		m_bBottom_KeyPressed = true;
	}
	else if(!sf::Keyboard::isKeyPressed(CSettings::get()->getLayerKey(3)) && ci_timer_bottom->getStopped())
	{
		m_bBottom_KeyPressed = false;
		getBlockVisuals()[3].hold_animation = false;
	}
	else if(m_bBottomSalve)
	{
		m_bBottom_KeyPressed = false;
	}
}

bool CColossNormal::hasBlockStarted(int _layer)
{
	bool blocked = false;
	float fDistance;

	int red;
	int green;
	int blue;
	int alpha;

	for(unsigned int i = 0; i < getProjectileVector().size(); i++)
	{
		switch(_layer)
		{
			case TOP_COLOSS:
				if((m_vLayerCollision[0].Collides(getProjectileVector()[i]->getCollisionObject()) || getProjectileVector()[i]->getCollisionObject()->Collides(&m_vLayerCollision[0])) && !getProjectileVector()[i]->getDead() && !getProjectileVector()[i]->getBounced() && getProjectileVector()[i]->getBlocked() == -1) // Check if small object collides with big one and the other way round
				{
					// Play sound
					if(CLevelData::getCurrentLevelString() == "darkage")
					{
						glAudio->PlaySound(glDataBox->GetSoundBuffer(data->GetString("layer0")), 0);
					}
					else
					{
						glAudio->PlaySound(glDataBox->GetSoundBuffer(data->GetString("layer0_future")), 0);
					}

					// Calculate animation speed so block will be at correct position
					startBlockAnimation(m_vBlockCollision[0].getPosition().x, getProjectileVector()[i]->getPosition().x, getProjectileVector()[i]->GetStartVelocity().x, 0);
					m_bTopBlock_active = true;

					// Distance to center
					fDistance = pwHelper::math::sqrt(pow((getProjectileVector()[i]->getPosition().x - m_vLayerCollision[0].getPosition().x), 2) + pow((getProjectileVector()[i]->getPosition().y - m_vLayerCollision[0].getPosition().y), 2));
					
					// Button color
					red = (int)((fDistance/50 * 255) > 255 ? 255 : (fDistance/50 * 255));
					green = (int)(200 + (55 - (fDistance/150 * 55)));
					blue = 0;
					alpha = 200;

					m_vButton[0].setColor(sf::Color(red, green, blue, alpha));
					m_vButton[0].SetCurrentAnimation(C_ANIM_BUTTON_NORMAL);
					m_vButton[0].RestartAnimation();

					if(!getBoss())
					{
						calculateScore(fDistance, 0);
					}
					else
					{
						if(m_vLayerCollision[0].getPosition().x > getProjectileVector()[i]->getPosition().x)
						{
							getProjectileVector()[i]->setBlockDistance(-fDistance);
						}
						else
						{
							getProjectileVector()[i]->setBlockDistance(fDistance);
						}
					}

					// Projectile blocked
					getProjectileVector()[i]->setBlocked(0);
					blocked = true;

					if(!getBoss())
					{
						modBlocked(1);
						modSteps(1);
					}
				}
				break;
			case MIDDLE_HIGHER_COLOSS:
				if((m_vLayerCollision[1].Collides(getProjectileVector()[i]->getCollisionObject()) || getProjectileVector()[i]->getCollisionObject()->Collides(&m_vLayerCollision[1])) && !getProjectileVector()[i]->getDead() && !getProjectileVector()[i]->getBounced() && getProjectileVector()[i]->getBlocked() == -1)
				{
					if(CLevelData::getCurrentLevelString() == "darkage")
					{
						glAudio->PlaySound(glDataBox->GetSoundBuffer(data->GetString("layer1")), 1);
					}
					else
					{
						glAudio->PlaySound(glDataBox->GetSoundBuffer(data->GetString("layer1_future")), 1);
					}

					startBlockAnimation(m_vBlockCollision[1].getPosition().x, getProjectileVector()[i]->getPosition().x, getProjectileVector()[i]->GetStartVelocity().x, 1);
					m_bMiddleHigherBlock_active = true;

					fDistance = pwHelper::math::sqrt(pow((getProjectileVector()[i]->getPosition().x - m_vLayerCollision[1].getPosition().x), 2) + pow((getProjectileVector()[i]->getPosition().y - m_vLayerCollision[1].getPosition().y), 2));
					
					red = (int)((fDistance/50 * 255) > 255 ? 255 : (fDistance/50 * 255));
					green = (int)(200 + (55 - (fDistance/150 * 55)));
					blue = 0;
					alpha = 200;

					m_vButton[1].setColor(sf::Color(red, green, blue, alpha));
					m_vButton[1].SetCurrentAnimation(C_ANIM_BUTTON_NORMAL);
					m_vButton[1].RestartAnimation();

					if(!getBoss())
					{
						calculateScore(fDistance, 1);
					}
					else
					{
						if(m_vLayerCollision[1].getPosition().x > getProjectileVector()[i]->getPosition().x)
						{
							getProjectileVector()[i]->setBlockDistance(-fDistance);
						}
						else
						{
							getProjectileVector()[i]->setBlockDistance(fDistance);
						}
					}

					getProjectileVector()[i]->setBlocked(1);
					blocked = true;

					if(!getBoss())
					{
						modBlocked(1);
						modSteps(1);
					}
				}
				break;
			case MIDDLE_LOWER_COLOSS:
				if((m_vLayerCollision[2].Collides(getProjectileVector()[i]->getCollisionObject()) || getProjectileVector()[i]->getCollisionObject()->Collides(&m_vLayerCollision[2])) && !getProjectileVector()[i]->getDead() && !getProjectileVector()[i]->getBounced() && getProjectileVector()[i]->getBlocked() == -1)
				{
					if(CLevelData::getCurrentLevelString() == "darkage")
					{
						glAudio->PlaySound(glDataBox->GetSoundBuffer(data->GetString("layer2")), 2);
					}
					else
					{
						glAudio->PlaySound(glDataBox->GetSoundBuffer(data->GetString("layer2_future")), 2);
					}

					startBlockAnimation(m_vBlockCollision[2].getPosition().x, getProjectileVector()[i]->getPosition().x, getProjectileVector()[i]->GetStartVelocity().x, 2);
					m_bMiddleLowerBlock_active = true;

					fDistance = pwHelper::math::sqrt(pow((getProjectileVector()[i]->getPosition().x - m_vLayerCollision[2].getPosition().x), 2) + pow((getProjectileVector()[i]->getPosition().y - m_vLayerCollision[2].getPosition().y), 2));

					red = (int)((fDistance/50 * 255) > 255 ? 255 : (fDistance/50 * 255));
					green = (int)(200 + (55 - (fDistance/150 * 55)));
					blue = 0;
					alpha = 200;

					m_vButton[2].setColor(sf::Color(red, green, blue, alpha));
					m_vButton[2].SetCurrentAnimation(C_ANIM_BUTTON_NORMAL);
					m_vButton[2].RestartAnimation();

					if(!getBoss())
					{
						calculateScore(fDistance, 2);
					}
					else
					{
						if(m_vLayerCollision[2].getPosition().x > getProjectileVector()[i]->getPosition().x)
						{
							getProjectileVector()[i]->setBlockDistance(-fDistance);
						}
						else
						{
							getProjectileVector()[i]->setBlockDistance(fDistance);
						}
					}

					getProjectileVector()[i]->setBlocked(2);
					blocked = true;

					if(!getBoss())
					{
						modBlocked(1);
						modSteps(1);
					}
				}
				break;
			case BOTTOM_COLOSS:
				if((m_vLayerCollision[3].Collides(getProjectileVector()[i]->getCollisionObject()) || getProjectileVector()[i]->getCollisionObject()->Collides(&m_vLayerCollision[3])) && !getProjectileVector()[i]->getDead() && !getProjectileVector()[i]->getBounced() && getProjectileVector()[i]->getBlocked() == -1)
				{
					if(CLevelData::getCurrentLevelString() == "darkage")
					{
						glAudio->PlaySound(glDataBox->GetSoundBuffer(data->GetString("layer3")), 3);
					}
					else
					{
						glAudio->PlaySound(glDataBox->GetSoundBuffer(data->GetString("layer3_future")), 3);
					}

					startBlockAnimation(m_vBlockCollision[3].getPosition().x, getProjectileVector()[i]->getPosition().x, getProjectileVector()[i]->GetStartVelocity().x, 3);
					m_bBottomBlock_active = true;

                    fDistance = pwHelper::math::sqrt(pow((getProjectileVector()[i]->getPosition().x - m_vLayerCollision[3].getPosition().x), 2) + pow((getProjectileVector()[i]->getPosition().y - m_vLayerCollision[3].getPosition().y), 2));

					red = (int)((fDistance/50 * 255) > 255 ? 255 : (fDistance/50 * 255));
					green = (int)(200 + (55 - (fDistance/150 * 55)));
					blue = 0;
					alpha = 200;

					m_vButton[3].setColor(sf::Color(red, green, blue, alpha));
					m_vButton[3].SetCurrentAnimation(C_ANIM_BUTTON_NORMAL);
					m_vButton[3].RestartAnimation();

					if(!getBoss())
					{
						calculateScore(fDistance, 3);
					}
					else
					{
						if(m_vLayerCollision[3].getPosition().x > getProjectileVector()[i]->getPosition().x)
						{
							getProjectileVector()[i]->setBlockDistance(-fDistance);
						}
						else
						{
							getProjectileVector()[i]->setBlockDistance(fDistance);
						}
					}

					getProjectileVector()[i]->setBlocked(3);
					blocked = true;

					if(!getBoss())
					{
						modBlocked(1);
						modSteps(1);
					}
				}
				break;
		}

		// Blocked 15 projectiles in a row? Earn a modifier!
		if(getBlocked() >= m_iMultiplierDuration)
		{
			modModifier(1);
			setBlocked(0);
		}
	}

	return blocked;
}

void CColossNormal::hasBlockFinished()
{
	bool bHasTopCollided = false;
	bool bHasMiddleHigherCollided = false;
	bool bHasMiddleLowerCollided = false;
	bool bHasBottomCollided = false;

	for(unsigned int i = 0; i < getProjectileVector().size(); i++)
	{
		if(m_bTopBlock_active)
		{
			if(getProjectileVector()[i]->getCollisionObject()->Collides(&m_vBlockCollision[0]) && !getProjectileVector()[i]->getBounced() && getProjectileVector()[i]->getBlocked() == 0)
			{
				if(getProjectileVector()[i]->getParent() == 8 || getProjectileVector()[i]->getParent() == 9 || getProjectileVector()[i]->getParent() == 10 || getProjectileVector()[i]->getParent() == 11)
				{
					getProjectileVector()[i]->SetGravityMultiplier(0.0f);
					getProjectileVector()[i]->SetStartVelocity(300.0f + (m_bossArrow_top.getPosition().x - m_vBlockCollision[0].getPosition().x), getProjectileVector()[i]->getBlockDistance() * 2.0f);
					getProjectileVector()[i]->setBounced(true);
				}
				else
				{
					getProjectileVector()[i]->bounceOrDestroy();
				}

				bHasTopCollided = true;

				for(unsigned int j = 0; j < getProjectileVector().size(); j++)
				{
					if(i != j && !getProjectileVector()[j]->getBounced() && getProjectileVector()[j]->getBlocked() == 0)
					{
						bHasTopCollided = false; // Another projectile has been blocked and is on its way
					}
				}
			}
		}

		if(m_bMiddleHigherBlock_active)
		{
			if(getProjectileVector()[i]->getCollisionObject()->Collides(&m_vBlockCollision[1]) && !getProjectileVector()[i]->getBounced() && getProjectileVector()[i]->getBlocked() == 1)
			{
				if(getProjectileVector()[i]->getParent() == 8 || getProjectileVector()[i]->getParent() == 9 || getProjectileVector()[i]->getParent() == 10 || getProjectileVector()[i]->getParent() == 11)
				{
					getProjectileVector()[i]->SetGravityMultiplier(0.0f);
					getProjectileVector()[i]->SetStartVelocity(300.0f + (m_bossArrow_middle_higher.getPosition().x - m_vBlockCollision[1].getPosition().x), getProjectileVector()[i]->getBlockDistance() * 2.0f);
					getProjectileVector()[i]->setBounced(true);
				}
				else
				{
					getProjectileVector()[i]->bounceOrDestroy();
				}

				bHasMiddleHigherCollided = true;

				for(unsigned int j = 0; j < getProjectileVector().size(); j++)
				{
					if(i != j && !getProjectileVector()[j]->getBounced() && getProjectileVector()[j]->getBlocked() == 1)
					{
						bHasMiddleHigherCollided = false;
					}
				}
			}
		}

		if(m_bMiddleLowerBlock_active)
		{
			if(getProjectileVector()[i]->getCollisionObject()->Collides(&m_vBlockCollision[2]) && !getProjectileVector()[i]->getBounced() && getProjectileVector()[i]->getBlocked() == 2)
			{
				if(getProjectileVector()[i]->getParent() == 8 || getProjectileVector()[i]->getParent() == 9 || getProjectileVector()[i]->getParent() == 10 || getProjectileVector()[i]->getParent() == 11)
				{
					getProjectileVector()[i]->SetGravityMultiplier(0.0f);
					getProjectileVector()[i]->SetStartVelocity(300.0f + (m_bossArrow_middle_lower.getPosition().x - m_vBlockCollision[2].getPosition().x), getProjectileVector()[i]->getBlockDistance() * 2.0f);
					getProjectileVector()[i]->setBounced(true);
				}
				else
				{
					getProjectileVector()[i]->bounceOrDestroy();
				}

				bHasMiddleLowerCollided = true;

				for(unsigned int j = 0; j < getProjectileVector().size(); j++)
				{
					if(i != j && !getProjectileVector()[j]->getBounced() && getProjectileVector()[j]->getBlocked() == 2)
					{
						bHasMiddleLowerCollided = false;
					}
				}
			}
		}

		if(m_bBottomBlock_active)
		{
			if(getProjectileVector()[i]->getCollisionObject()->Collides(&m_vBlockCollision[3]) && !getProjectileVector()[i]->getBounced() && getProjectileVector()[i]->getBlocked() == 3)
			{
				if(getProjectileVector()[i]->getParent() == 8 || getProjectileVector()[i]->getParent() == 9 || getProjectileVector()[i]->getParent() == 10 || getProjectileVector()[i]->getParent() == 11)
				{
					getProjectileVector()[i]->SetGravityMultiplier(0.0f);
					getProjectileVector()[i]->SetStartVelocity(300.0f + (m_bossArrow_bottom.getPosition().x - m_vBlockCollision[3].getPosition().x), getProjectileVector()[i]->getBlockDistance() * 2.0f);
					getProjectileVector()[i]->setBounced(true);
				}
				else
				{
					getProjectileVector()[i]->bounceOrDestroy();
				}

				bHasBottomCollided = true;

				for(unsigned int j = 0; j < getProjectileVector().size(); j++)
				{	
					if(i != j && !getProjectileVector()[j]->getBounced() && getProjectileVector()[j]->getBlocked() == 3)
					{
						bHasBottomCollided = false;
					}
				}
			}
		}
	}

	if(bHasTopCollided)
	{
		m_bTopBlock_active = false;
	}

	if(bHasMiddleHigherCollided)
	{
		m_bMiddleHigherBlock_active = false;
	}

	if(bHasMiddleLowerCollided)
	{
		m_bMiddleLowerBlock_active = false;
	}

	if(bHasBottomCollided)
	{
		m_bBottomBlock_active = false;
	}
}

void CColossNormal::startBlockAnimation(float _fAttackEndX, float _fPressButtonX, float _fProjectileSpeed, int _layer)
{	
	if(getBlockVisuals()[_layer].animation.GetCurrentAnimation() != C_ANIM_BLOCK)
	{
		getBlockVisuals()[_layer].animation.SetCurrentAnimation(C_ANIM_BLOCK); // So the correct number of frames can be read
	}
	
	float fDistance;
	float fTime;

	float fStandardAnimationFrames = (float)getBlockVisuals()[_layer].animation.GetCurrentAnimationFrames();
	float fDesiredAnimationSpeed;

	// Projectile time
	fDistance = _fPressButtonX - _fAttackEndX;
	fTime = fDistance / abs(_fProjectileSpeed);

	// Animation time
	fDesiredAnimationSpeed = fStandardAnimationFrames * 1.0f / fTime;

	if(_layer == 3)
	{
		fDesiredAnimationSpeed *= 1.3f; // Fix/workaround so bottom block works perfectly
	}

	// Apply speed and start the animation
	getBlockVisuals()[_layer].animation.SetFramesPerSecond(fDesiredAnimationSpeed);
	getBlockVisuals()[_layer].animation.PlayOnce();
	getBlockVisuals()[_layer].animation.setColor(sf::Color(255, 255, 255, 255));

	if(getBlockVisuals()[_layer].hold_animation == false)
	{
		getBlockVisuals()[_layer].animation.RestartAnimation();
	}

	// Move block to correct position and disable connected move animation
	int iCurrentFrame = (int)getMoveVisuals()[0].animation.GetCurrentFrame();
	
	switch(_layer)
	{
		case 1: // Front arm
			getBlockVisuals()[_layer].animation.setPosition(m_fOriginX + ci_coloss_data.getFrontArmTween()[iCurrentFrame].x, m_fOriginY + ci_coloss_data.getFrontArmTween()[iCurrentFrame].y);
			getMoveVisuals()[2].animation.setColor(sf::Color(255, 255, 255, 0));
			break;
		case 2: // Back arm
			getBlockVisuals()[_layer].animation.setPosition(m_fOriginX + ci_coloss_data.getBackArmTween()[iCurrentFrame].x, m_fOriginY + ci_coloss_data.getBackArmTween()[iCurrentFrame].y);
			getMoveVisuals()[1].animation.setColor(sf::Color(255, 255, 255, 0));
			break;
		case 3: // Tail
			getBlockVisuals()[_layer].animation.setPosition(m_fOriginX + ci_coloss_data.getTailTween()[iCurrentFrame].x, m_fOriginY + ci_coloss_data.getTailTween()[iCurrentFrame].y);
			getMoveVisuals()[5].animation.setColor(sf::Color(255, 255, 255, 0));
			break;
	}

	m_iPreviousFrame = iCurrentFrame;
}

void CColossNormal::drawScore()
{
	// Draw floating text and fade it until removal
	for(unsigned int i = 0; i < m_vFloatingText.size(); i++)
	{
		m_vFloatingText[i].fTime -= glGraphics->GetLastFrameTime();
		m_vFloatingText[i].text.setColor(sf::Color(0, 0, 0, (int)(m_vFloatingText[i].fTime * 255)));
		m_vFloatingText[i].text.setPosition(m_vFloatingText[i].text.getPosition().x + 12.0f * glGraphics->GetLastFrameTime(), m_vFloatingText[i].text.getPosition().y - 12.0f * glGraphics->GetLastFrameTime());

		if(m_vFloatingText[i].fTime <= 0.0f)
		{
			m_vFloatingText.erase(m_vFloatingText.begin() + i);
		}
		else
		{
			glGraphics->Draw(&m_vFloatingText[i].text);
		}
	}
}

void CColossNormal::calculateScore(float _distance, int _layer)
{
	floating_text temp;
	temp.text.setFont(*glDataBox->GetFont("pecita.otf"));
	temp.text.setCharacterSize(40);
	temp.text.setColor(sf::Color(0, 0, 0, 255));
	
	int iScore;
	std::string sScore;
	std::string sModifier;
	std::string sFinal;
	std::stringstream string_typecast;
	
	if(_distance < 1.0f)
	{
		_distance = 1.0f;
	}

	// Calculate score
	iScore = (int)(1.0f / _distance * 10000.0f);
	string_typecast << iScore;
	string_typecast >> sScore;
	string_typecast.clear();
	iScore *= getModifier();

	// Show modifier if it is bigger than 1
	if(getModifier() != 1)
	{
		string_typecast << getModifier();
		string_typecast >> sModifier;
		string_typecast.clear();

		sFinal += sModifier;
		sFinal += "x ";
	}

	// Create the final string
	sFinal += sScore;

	// Show floating text (score)
	m_vFloatingText.push_back(temp);
	m_vFloatingText[m_vFloatingText.size() - 1].fTime = 1.0f;
	m_vFloatingText[m_vFloatingText.size() - 1].text.setString(sFinal);
	m_vFloatingText[m_vFloatingText.size() - 1].text.setOrigin(m_vFloatingText[m_vFloatingText.size() - 1].text.getGlobalBounds().width / 2, m_vFloatingText[m_vFloatingText.size() - 1].text.getGlobalBounds().height / 2);
	m_vFloatingText[m_vFloatingText.size() - 1].text.setPosition(m_vLayerCollision[_layer].getPosition().x, m_vLayerCollision[_layer].getPosition().y - 82);

	// Mod internal score
	modScore(iScore);

	sScore.clear();
	sModifier.clear();
	sFinal.clear();
	string_typecast.clear();
}

void CColossNormal::scaleButtons()
{
	bool bNoCollisionTop = true;
	bool bNoCollisionMiddleHigher = true;
	bool bNoCollisionMiddleLower = true;
	bool bNoCollisionBottom = true;

	for(unsigned int i = 0; i < getProjectileVector().size(); i++)
	{
		if((getProjectileVector()[i]->getCollisionObject()->Collides(&m_vLayerCollision[0]) || m_vLayerCollision[0].Collides(getProjectileVector()[i]->getCollisionObject())) && !getProjectileVector()[i]->getBounced() && getProjectileVector()[i]->getBlocked() == -1) // Check if small object collides with big one
		{
			bNoCollisionTop = false;
		}

		if((getProjectileVector()[i]->getCollisionObject()->Collides(&m_vLayerCollision[1]) || m_vLayerCollision[1].Collides(getProjectileVector()[i]->getCollisionObject())) && !getProjectileVector()[i]->getBounced() && getProjectileVector()[i]->getBlocked() == -1)
		{
			bNoCollisionMiddleHigher = false;
		}

		if((getProjectileVector()[i]->getCollisionObject()->Collides(&m_vLayerCollision[2]) || m_vLayerCollision[2].Collides(getProjectileVector()[i]->getCollisionObject())) && !getProjectileVector()[i]->getBounced() && getProjectileVector()[i]->getBlocked() == -1)
		{
			bNoCollisionMiddleLower = false;
		}

		if((getProjectileVector()[i]->getCollisionObject()->Collides(&m_vLayerCollision[3]) || m_vLayerCollision[3].Collides(getProjectileVector()[i]->getCollisionObject())) && !getProjectileVector()[i]->getBounced() && getProjectileVector()[i]->getBlocked() == -1)
		{
			bNoCollisionBottom = false;
		}
	}

	if(!bNoCollisionTop)
	{
		if(m_vButton[0].getScale().x < 0.7f && m_vButton[0].getScale().y < 0.7f)
		{
			m_vButton[0].setScale(m_vButton[0].getScale().x + 0.01f, m_vButton[0].getScale().y + 0.01f);
		}
	}
	else
	{
		m_vButton[0].setScale(0.55f, 0.55f);
	}

	if(!bNoCollisionMiddleHigher)
	{
		if(m_vButton[1].getScale().x < 0.7f && m_vButton[1].getScale().y < 0.7f)
		{
			m_vButton[1].setScale(m_vButton[1].getScale().x + 0.01f, m_vButton[1].getScale().y + 0.01f);
		}
	}
	else
	{
		m_vButton[1].setScale(0.55f, 0.55f);
	}

	if(!bNoCollisionMiddleLower)
	{
		if(m_vButton[2].getScale().x < 0.7f && m_vButton[2].getScale().y < 0.7f)
		{
			m_vButton[2].setScale(m_vButton[2].getScale().x + 0.01f, m_vButton[2].getScale().y + 0.01f);
		}
	}
	else
	{
		m_vButton[2].setScale(0.55f, 0.55f);
	}

	if(!bNoCollisionBottom)
	{
		if(m_vButton[3].getScale().x < 0.7f && m_vButton[3].getScale().y < 0.7f)
		{
			m_vButton[3].setScale(m_vButton[3].getScale().x + 0.01f, m_vButton[3].getScale().y + 0.01f);
		}
	}
	else
	{
		m_vButton[3].setScale(0.55f, 0.55f);
	}
}

void CColossNormal::drawButtons()
{
	for(unsigned int i = 0; i < m_vButton.size(); i++)
	{
		if(m_vButton[i].WasPlayed())
		{
			m_vButton[i].setColor(sf::Color(255, 255, 255, 200));
		}

		glGraphics->Draw(&m_vButton[i]);
	}

	for(unsigned int i = 0; i < m_vButtonText.size(); i++)
	{
		glGraphics->Draw(&m_vButtonText[i]);
	}
}

void CColossNormal::tweenBlock()
{
	int iCurrentFrame = (int)getMoveVisuals()[0].animation.GetCurrentFrame();
	
	if(m_iPreviousFrame != iCurrentFrame)
	{
		if(getBlockVisuals()[1].animation.GetCurrentAnimation() == C_ANIM_BLOCK || getBlockVisuals()[1].animation.GetCurrentAnimation() == C_ANIM_PULLBACK)
		{
			getBlockVisuals()[1].animation.setPosition(m_fOriginX + ci_coloss_data.getFrontArmTween()[iCurrentFrame].x, m_fOriginY + ci_coloss_data.getFrontArmTween()[iCurrentFrame].y);
		}

		if(getBlockVisuals()[2].animation.GetCurrentAnimation() == C_ANIM_BLOCK || getBlockVisuals()[2].animation.GetCurrentAnimation() == C_ANIM_PULLBACK)
		{
			getBlockVisuals()[2].animation.setPosition(m_fOriginX + ci_coloss_data.getBackArmTween()[iCurrentFrame].x, m_fOriginY + ci_coloss_data.getBackArmTween()[iCurrentFrame].y);
		}

		if(getBlockVisuals()[3].animation.GetCurrentAnimation() == C_ANIM_BLOCK || getBlockVisuals()[3].animation.GetCurrentAnimation() == C_ANIM_PULLBACK)
		{
			getBlockVisuals()[3].animation.setPosition(m_fOriginX + ci_coloss_data.getTailTween()[iCurrentFrame].x, m_fOriginY + ci_coloss_data.getTailTween()[iCurrentFrame].y);
		}

		m_iPreviousFrame = iCurrentFrame;
	}
}

void CColossNormal::checkForSalve(int _layer)
{
	bool bSalveTop = false;
	bool bSalveMiddleHigher = false;
	bool bSalveMiddleLower = false;
	bool bSalveBottom = false;

	for(unsigned int i = 0; i < getProjectileVector().size(); i++)
	{
		if(_layer == 0)
		{
			if(getProjectileVector()[i]->getSalve() && getProjectileVector()[i]->getTarget() == 0 && !getProjectileVector()[i]->getDead() && !getProjectileVector()[i]->getBounced())
			{
				bSalveTop = true;
			}
		}
		
		if(_layer == 1)
		{
			if(getProjectileVector()[i]->getSalve() && getProjectileVector()[i]->getTarget() == 1 && !getProjectileVector()[i]->getDead() && !getProjectileVector()[i]->getBounced())
			{
				bSalveMiddleHigher = true;
			}
		}
		
		if(_layer == 2)
		{
			if(getProjectileVector()[i]->getSalve() && getProjectileVector()[i]->getTarget() == 2 && !getProjectileVector()[i]->getDead() && !getProjectileVector()[i]->getBounced())
			{
				bSalveMiddleLower = true;
			}
		}
		
		if(_layer == 3)
		{
			if(getProjectileVector()[i]->getSalve() && getProjectileVector()[i]->getTarget() == 3 && !getProjectileVector()[i]->getDead() && !getProjectileVector()[i]->getBounced())
			{
				bSalveBottom = true;
			}
		}
	}

	if(_layer == 0)
	{
		if(bSalveTop)
		{
			m_bTopSalve = true;
		}
		else
		{
			m_bTopSalve = false;
			getBlockVisuals()[0].hold_animation = false;
		}
	}

	if(_layer == 1)
	{
		if(bSalveMiddleHigher)
		{
			m_bMiddleHigherSalve = true;
		}
		else
		{
			m_bMiddleHigherSalve = false;
			getBlockVisuals()[1].hold_animation = false;
		}
	}

	if(_layer == 2)
	{
		if(bSalveMiddleLower)
		{
			m_bMiddleLowerSalve = true;
		}
		else
		{
			m_bMiddleLowerSalve = false;
			getBlockVisuals()[2].hold_animation = false;
		}
	}

	if(_layer == 3)
	{
		if(bSalveBottom)
		{
			m_bBottomSalve = true;
		}
		else
		{
			m_bBottomSalve = false;
			getBlockVisuals()[3].hold_animation = false;
		}
	}
}

void CColossNormal::calculateBlockAngle()
{
	float fDistance;

	for(unsigned int i = 0; i < getProjectileVector().size(); i++)
	{
		if((m_vLayerCollision[0].Collides(getProjectileVector()[i]->getCollisionObject()) || getProjectileVector()[i]->getCollisionObject()->Collides(&m_vLayerCollision[0])) && getProjectileVector()[i]->getBlocked() == -1 && (getProjectileVector()[i]->getParent() == 8 || getProjectileVector()[i]->getParent() == 9 || getProjectileVector()[i]->getParent() == 10 || getProjectileVector()[i]->getParent() == 11) && getProjectileVector()[i]->getTarget() == 0)
		{
			fDistance = pwHelper::math::sqrt(pow((getProjectileVector()[i]->getPosition().x - m_vLayerCollision[0].getPosition().x), 2) + pow((getProjectileVector()[i]->getPosition().y - m_vLayerCollision[0].getPosition().y), 2));

			if(m_vLayerCollision[0].getPosition().x > getProjectileVector()[i]->getPosition().x)
			{
				m_bossArrow_top.setRotation(atan2(-(fDistance * 2.0f), 300.0f) * 180 / PI_PROJECTILE);
			}
			else
			{
				m_bossArrow_top.setRotation(atan2((fDistance * 2.0f), 300.0f) * 180 / PI_PROJECTILE);
			}

			// Fade out unused arrows
			if(m_bossArrow_middle_higher.getColor().a > 0 && m_bossArrow_middle_lower.getColor().a > 0 && m_bossArrow_bottom.getColor().a > 0)
			{
				m_bossArrow_middle_higher.setColor(sf::Color(255, 255, 255, m_bossArrow_middle_higher.getColor().a - 1));
				m_bossArrow_middle_lower.setColor(sf::Color(255, 255, 255, m_bossArrow_middle_lower.getColor().a - 1));
				m_bossArrow_bottom.setColor(sf::Color(255, 255, 255, m_bossArrow_bottom.getColor().a - 1));
			}
			else
			{
				m_bossArrow_middle_higher.setColor(sf::Color(255, 255, 255, 0));
				m_bossArrow_middle_lower.setColor(sf::Color(255, 255, 255, 0));
				m_bossArrow_bottom.setColor(sf::Color(255, 255, 255, 0));
			}
		}
		else if((m_vLayerCollision[1].Collides(getProjectileVector()[i]->getCollisionObject()) || getProjectileVector()[i]->getCollisionObject()->Collides(&m_vLayerCollision[1])) && getProjectileVector()[i]->getBlocked() == -1 && (getProjectileVector()[i]->getParent() == 8 || getProjectileVector()[i]->getParent() == 9 || getProjectileVector()[i]->getParent() == 10 || getProjectileVector()[i]->getParent() == 11) && getProjectileVector()[i]->getTarget() == 1)
		{
			fDistance = pwHelper::math::sqrt(pow((getProjectileVector()[i]->getPosition().x - m_vLayerCollision[1].getPosition().x), 2) + pow((getProjectileVector()[i]->getPosition().y - m_vLayerCollision[1].getPosition().y), 2));

			if(m_vLayerCollision[1].getPosition().x > getProjectileVector()[i]->getPosition().x)
			{
				m_bossArrow_middle_higher.setRotation(atan2(-(fDistance * 2.0f), 300.0f) * 180 / PI_PROJECTILE);
			}
			else
			{
				m_bossArrow_middle_higher.setRotation(atan2((fDistance * 2.0f), 300.0f) * 180 / PI_PROJECTILE);
			}

			// Fade out unused arrows
			if(m_bossArrow_top.getColor().a > 0 && m_bossArrow_middle_lower.getColor().a > 0 && m_bossArrow_bottom.getColor().a > 0)
			{
				m_bossArrow_top.setColor(sf::Color(255, 255, 255, m_bossArrow_top.getColor().a - 1));
				m_bossArrow_middle_lower.setColor(sf::Color(255, 255, 255, m_bossArrow_middle_lower.getColor().a - 1));
				m_bossArrow_bottom.setColor(sf::Color(255, 255, 255, m_bossArrow_bottom.getColor().a - 1));
			}
			else
			{
				m_bossArrow_top.setColor(sf::Color(255, 255, 255, 0));
				m_bossArrow_middle_lower.setColor(sf::Color(255, 255, 255, 0));
				m_bossArrow_bottom.setColor(sf::Color(255, 255, 255, 0));
			}
		}
		else if((m_vLayerCollision[2].Collides(getProjectileVector()[i]->getCollisionObject()) || getProjectileVector()[i]->getCollisionObject()->Collides(&m_vLayerCollision[2])) && getProjectileVector()[i]->getBlocked() == -1 && (getProjectileVector()[i]->getParent() == 8 || getProjectileVector()[i]->getParent() == 9 || getProjectileVector()[i]->getParent() == 10 || getProjectileVector()[i]->getParent() == 11) && getProjectileVector()[i]->getTarget() == 2)
		{
			fDistance = pwHelper::math::sqrt(pow((getProjectileVector()[i]->getPosition().x - m_vLayerCollision[2].getPosition().x), 2) + pow((getProjectileVector()[i]->getPosition().y - m_vLayerCollision[2].getPosition().y), 2));

			if(m_vLayerCollision[2].getPosition().x > getProjectileVector()[i]->getPosition().x)
			{
				m_bossArrow_middle_lower.setRotation(atan2(-(fDistance * 2.0f), 300.0f) * 180 / PI_PROJECTILE);
			}
			else
			{
				m_bossArrow_middle_lower.setRotation(atan2((fDistance * 2.0f), 300.0f) * 180 / PI_PROJECTILE);
			}

			// Fade out unused arrows
			if(m_bossArrow_top.getColor().a > 0 && m_bossArrow_middle_higher.getColor().a > 0 && m_bossArrow_bottom.getColor().a > 0)
			{
				m_bossArrow_top.setColor(sf::Color(255, 255, 255, m_bossArrow_top.getColor().a - 1));
				m_bossArrow_middle_higher.setColor(sf::Color(255, 255, 255, m_bossArrow_middle_higher.getColor().a - 1));
				m_bossArrow_bottom.setColor(sf::Color(255, 255, 255, m_bossArrow_bottom.getColor().a - 1));
			}
			else
			{
				m_bossArrow_top.setColor(sf::Color(255, 255, 255, 0));
				m_bossArrow_middle_higher.setColor(sf::Color(255, 255, 255, 0));
				m_bossArrow_bottom.setColor(sf::Color(255, 255, 255, 0));
			}
		}
		else if((m_vLayerCollision[3].Collides(getProjectileVector()[i]->getCollisionObject()) || getProjectileVector()[i]->getCollisionObject()->Collides(&m_vLayerCollision[3])) && getProjectileVector()[i]->getBlocked() == -1 && (getProjectileVector()[i]->getParent() == 8 || getProjectileVector()[i]->getParent() == 9 || getProjectileVector()[i]->getParent() == 10 || getProjectileVector()[i]->getParent() == 11) && getProjectileVector()[i]->getTarget() == 3)
		{
			fDistance = pwHelper::math::sqrt(pow((getProjectileVector()[i]->getPosition().x - m_vLayerCollision[3].getPosition().x), 2) + pow((getProjectileVector()[i]->getPosition().y - m_vLayerCollision[3].getPosition().y), 2));

			if(m_vLayerCollision[3].getPosition().x > getProjectileVector()[i]->getPosition().x)
			{
				m_bossArrow_bottom.setRotation(atan2(-(fDistance * 2.0f), 300.0f) * 180 / PI_PROJECTILE);
			}
			else
			{
				m_bossArrow_bottom.setRotation(atan2((fDistance * 2.0f), 300.0f) * 180 / PI_PROJECTILE);
			}

			// Fade out unused arrows
			if(m_bossArrow_top.getColor().a > 0 && m_bossArrow_middle_higher.getColor().a > 0 && m_bossArrow_middle_lower.getColor().a > 0)
			{
				m_bossArrow_top.setColor(sf::Color(255, 255, 255, m_bossArrow_top.getColor().a - 1));
				m_bossArrow_middle_higher.setColor(sf::Color(255, 255, 255, m_bossArrow_middle_higher.getColor().a - 1));
				m_bossArrow_middle_lower.setColor(sf::Color(255, 255, 255, m_bossArrow_middle_lower.getColor().a - 1));
			}
			else
			{
				m_bossArrow_top.setColor(sf::Color(255, 255, 255, 0));
				m_bossArrow_middle_higher.setColor(sf::Color(255, 255, 255, 0));
				m_bossArrow_middle_lower.setColor(sf::Color(255, 255, 255, 0));
			}
		}
	}
}

void CColossNormal::manageBossInterface()
{
	if(getBoss())
	{
		bool bBossProjectileOnScreen = false;

		for(unsigned int i = 0; i < getProjectileVector().size(); i++)
		{
			if(getProjectileVector()[i]->getParent() == 8 || getProjectileVector()[i]->getParent() == 9 || getProjectileVector()[i]->getParent() == 10 || getProjectileVector()[i]->getParent() == 11)
			{
				bBossProjectileOnScreen = true;
				break;
			}
		}

		if(bBossProjectileOnScreen)
		{
			calculateBlockAngle();

			glGraphics->Draw(&m_bossArrow_top);
			glGraphics->Draw(&m_bossArrow_middle_higher);
			glGraphics->Draw(&m_bossArrow_middle_lower);
			glGraphics->Draw(&m_bossArrow_bottom);

			for(int i = 0; i < 4; i++)
			{
				m_vLayerCollision[i].SetSize(615.0f, 115.0f);
				m_vLayerCollision[i].setOrigin(m_vLayerCollision[i].GetSize().x/2, m_vLayerCollision[i].GetSize().y/2);
			}
		}
		else
		{
			m_bossArrow_top.setRotation(atan2(615.0f, 300.0f) * 180 / PI_PROJECTILE);
			m_bossArrow_top.setColor(sf::Color(255, 255, 255, 255));

			m_bossArrow_middle_higher.setRotation(atan2(615.0f, 300.0f) * 180 / PI_PROJECTILE);
			m_bossArrow_middle_higher.setColor(sf::Color(255, 255, 255, 255));

			m_bossArrow_middle_lower.setRotation(atan2(615.0f, 300.0f) * 180 / PI_PROJECTILE);
			m_bossArrow_middle_lower.setColor(sf::Color(255, 255, 255, 255));

			m_bossArrow_bottom.setRotation(atan2(615.0f, 300.0f) * 180 / PI_PROJECTILE);
			m_bossArrow_bottom.setColor(sf::Color(255, 255, 255, 255));

			for(int i = 0; i < 4; i++)
			{
				m_vLayerCollision[i].SetSize(115.0f, 115.0f);
				m_vLayerCollision[i].setOrigin(m_vLayerCollision[i].GetSize().x/2, m_vLayerCollision[i].GetSize().y/2);
			}
		}
	}
}

// Cooldown reset using the CTimer class
void CColossNormal::topResetCooldown()
{
	if(!sf::Keyboard::isKeyPressed(CSettings::get()->getLayerKey(0)))
	{
		m_bTop_KeyPressed = false;
	}
}

void CColossNormal::trcWrapper(const void *context)
{
	((CColossNormal*)(context))->topResetCooldown();
}

void CColossNormal::middleHigherResetCooldown()
{
	if(!sf::Keyboard::isKeyPressed(CSettings::get()->getLayerKey(1)))
	{
		m_bMiddleHigher_KeyPressed = false;
	}
}

void CColossNormal::mhrcWrapper(const void *context)
{
	((CColossNormal*)(context))->middleHigherResetCooldown();
}

void CColossNormal::middleLowerResetCooldown()
{
	if(!sf::Keyboard::isKeyPressed(CSettings::get()->getLayerKey(2)))
	{
		m_bMiddleLower_KeyPressed = false;
	}
}

void CColossNormal::mlrcWrapper(const void *context)
{
	((CColossNormal*)(context))->middleLowerResetCooldown();
}

void CColossNormal::bottomResetCooldown()
{
	if(!sf::Keyboard::isKeyPressed(CSettings::get()->getLayerKey(3)))
	{
		m_bBottom_KeyPressed = false;
	}
}

void CColossNormal::brcWrapper(const void *context)
{
	((CColossNormal*)(context))->bottomResetCooldown();
}