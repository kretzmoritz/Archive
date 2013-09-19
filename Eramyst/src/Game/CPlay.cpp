#define PWUSEGLOBALSINGLETONS
#include "..\..\include\Game\CPlay.h"

bool CPlay::m_bMenu;

CPlay::CPlay() : pwGameState(ID_GS_PLAY)
{
     m_bInitialized = false;
     m_bMenu = false;
}

CPlay::~CPlay() {}

bool CPlay::Start()
{
    if (m_bMenu)
    {
        CInterface::get()->restart();
        CMusicSystem::get()->getSound()->play();
        glAudio->Resume();
        return true;
    }

    std::string file = "squaddata_" + CLevelData::getCurrentLevelString() + ".ini";
	
	if (!m_bInitialized)
    {
        glLogics->SetGravity(100.0f);
			
	    // Initialize random numbers
	    srand( (unsigned int)time(NULL) );

        if (!glSystem->LoadDataList("game", "game_load.pws", "default", ID_GS_LOAD))
            return 0;

        m_ciShape.setSize(sf::Vector2f((float)pwEngineOptions::GetInternGraphicsSize().x, (float)pwEngineOptions::GetInternGraphicsSize().y));
        m_ciShape.setFillColor(sf::Color(29, 29, 29, 255));

        CEnemyData::init();
	    CProjectileData::init();
	    CSquadData::init(file);
	    CColossData::init();

	    m_ciBackground = new CBackground();
	    m_ciBackground->init();
			
        CInterface::get()->init();

	    m_ciColossHandler = new CColossHandler();
	    m_ciColossHandler->init();
		CColoss::reset();

        // Coloss death animation

        /*m_ciColossDeathBottomLeft.setTexture(*glDataBox->GetTexture("coloss_death_bottomleft.png"));
        m_ciColossDeathBottomLeft.setPosition(0.0f, 372.0f);
	    m_ciColossDeathBottomLeft.SetFramesPerSecond(15.0f);
	    m_ciColossDeathBottomLeft.SetTotalFrames(63);
	    m_ciColossDeathBottomLeft.SetFrameSize(687, 522);
	    m_ciColossDeathBottomLeft.setColor(sf::Color(255, 255, 255, 0));
        m_ciColossDeathBottomLeft.PlayOnce();
        m_ciColossDeathBottomRight.setTexture(*glDataBox->GetTexture("coloss_death_bottomright.png"));
        m_ciColossDeathBottomRight.setPosition(0.0f, 372.0f);
	    m_ciColossDeathBottomRight.SetFramesPerSecond(15.0f);
	    m_ciColossDeathBottomRight.SetTotalFrames(63);
	    m_ciColossDeathBottomRight.SetFrameSize(1087, 522);
	    m_ciColossDeathBottomRight.setColor(sf::Color(255, 255, 255, 0));
        m_ciColossDeathBottomRight.PlayOnce();
        m_ciColossDeathTop.setTexture(*glDataBox->GetTexture("coloss_death_topright.png"));
        m_ciColossDeathTop.setPosition(687.0f, 0.0f);
	    m_ciColossDeathTop.SetFramesPerSecond(15.0f);
	    m_ciColossDeathTop.SetTotalFrames(47);
	    m_ciColossDeathTop.SetFrameSize(612, 372);
	    m_ciColossDeathTop.setColor(sf::Color(255, 255, 255, 0));
        m_ciColossDeathTop.PlayOnce();*/

        m_ciColossDeath.setAnimation("animation\\coloss_death\\cd");
        m_ciColossDeath.setPosition(-550.0f, 200.0f);
        m_ciColossDeath.SetTotalFrames(63);
        m_ciColossDeath.SetFramesPerSecond(15.0f);
        m_ciColossDeath.PlayOnce();
        m_ciColossDeath.Load();
    

        //

	    m_ciSystemHandler = new CSystemHandler();
	    m_ciSystemHandler->init();

        CMusicSystem::get()->init();

		if(CLevelData::getCurrentLevelString() == "darkage")
		{
			CBeatSystem::SetBPM(80);
		}
		else if(CLevelData::getCurrentLevelString() == "future")
		{
			CBeatSystem::SetBPM(100);
		}
        
        m_bInitialized = true;
    }
    else
    {
		CSquadData::init(file);
        CColoss::reset();
		CSystem::reset();

        m_ciBackground->init();
        
        CMusicSystem::get()->init();

		if(CLevelData::getCurrentLevelString() == "darkage")
		{
			CBeatSystem::SetBPM(80);
		}
		else if(CLevelData::getCurrentLevelString() == "future")
		{
			CBeatSystem::SetBPM(100);
		}

        CInterface::get()->reset();

		m_ciColossHandler->reset();
		m_ciSystemHandler->reset();

        //m_ciColossDeathTop.setColor(sf::Color(255, 255, 255, 0));
        //m_ciColossDeathBottomRight.setColor(sf::Color(255, 255, 255, 0));
        //m_ciColossDeathBottomLeft.setColor(sf::Color(255, 255, 255, 0));
        m_ciColossDeath.RestartAnimation();
        m_fFade = 255.0f;
    }


	return true;
}

unsigned int CPlay::Run()
{
    m_bMenu = false;

	CDisplayBar::get()->drawHealth();

    CTimer::update();
	CEnemy::update();
	m_ciBackground->update();
	CInterface::get()->draw();
	m_ciColossHandler->update();
	m_ciSystemHandler->update();
	CProjectile::update();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && glSystem->GetKeyPressed().first)
    {
        m_bMenu = true;
        glAudio->Pause();
        CMusicSystem::get()->getSound()->pause();
        CInterface::get()->pre_shutdown();
        CInterface::get()->shutdown();
        return ID_GS_INGAME;
    }

    CLevelData::UpdateCurrentLevelTime(glGraphics->GetLastFrameTime());

    if (CLevelData::LevelToChange() || CInterface::get()->getCurrentLife() == 0.0f)
    {
        if (m_ciShape.getFillColor().a < 255)// || glAudio->GetSoundLayerSettings()->GetVolume() > 0) && CColoss::getPosition().x < 1200.0f)
        {
            CInterface::get()->pre_shutdown();

            m_fFade += 48.0f*glGraphics->GetLastFrameTime();
            if (m_fFade > 255.0f)
                m_fFade = 255.0f;
 
            m_ciShape.setFillColor(sf::Color(m_ciShape.getFillColor().r, m_ciShape.getFillColor().g, m_ciShape.getFillColor().b, (sf::Uint8)m_fFade)); 
            
            if (CMusicSystem::get()->getSound()->getVolume() - 20.0f*glGraphics->GetLastFrameTime() > 0.0f)
                CMusicSystem::get()->getSound()->setVolume(CMusicSystem::get()->getSound()->getVolume() - 20.0f*glGraphics->GetLastFrameTime());
            else
                CMusicSystem::get()->getSound()->setVolume(0.0f);

            if (CInterface::get()->getCurrentLife() != 0.0f)
            {
                CColoss::setPosition(CColoss::getPosition().x + 300.0f*glGraphics->GetLastFrameTime(), CColoss::getPosition().y);
			    CColoss::setImmortal(true);
            }
            else
            {
                CColoss::disableRagemodeColoss();
                glGraphics->Draw(&m_ciColossDeath);

                /*m_ciColossDeathBottomLeft.setColor(sf::Color(255, 255, 255, 255));
                m_ciColossDeathBottomRight.setColor(sf::Color(255, 255, 255, 255));
                m_ciColossDeathTop.setColor(sf::Color(255, 255, 255, 255));
                m_ciColossDeathTop.RestartAnimation();
                m_ciColossDeathBottomRight.RestartAnimation();
                m_ciColossDeathBottomLeft.RestartAnimation();*/
            }
        }
        else
        {
            glGraphics->Draw(&m_ciShape, INT_MAX -1);
            CMusicSystem::get()->shutdown();
            return ID_GS_LEVELDATA;
        }
    }
    else if (m_ciShape.getFillColor().a > 0)
    {
        m_fFade -= 48.0f*glGraphics->GetLastFrameTime();
        if (m_fFade < 0)
            m_fFade = 0;

        m_ciShape.setFillColor(sf::Color(m_ciShape.getFillColor().r, m_ciShape.getFillColor().g, m_ciShape.getFillColor().b, (sf::Uint8)m_fFade));
    }

    /*glGraphics->Draw(&m_ciColossDeathTop, 5);
    glGraphics->Draw(&m_ciColossDeathBottomLeft, 5);
    glGraphics->Draw(&m_ciColossDeathBottomRight, 5);*/

    if (m_ciShape.getFillColor().a != 0)
    {
        glGraphics->Draw(&m_ciShape, INT_MAX -1);
    }

	return myID;
}