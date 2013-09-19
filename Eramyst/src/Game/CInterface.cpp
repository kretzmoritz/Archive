#include "..\..\include\Game\CInterface.h"
#include "..\..\include\Game\CLevelData.h"

CInterface* CInterface::m_s_p_ciInstance = NULL;

void CInterface::setCurrentRageBlurY(float currentRageBlurYPosition)
{
    m_fCurrentYRageBlurPos = currentRageBlurYPosition;
    m_ciRageBlurXShader->setParameter("yPosition", m_fCurrentYRageBlurPos);
    m_ciRageBlurYShader->setParameter("yPosition", m_fCurrentYRageBlurPos);
}
float CInterface::getCurrentRageBlurY()
{
    return m_fCurrentYRageBlurPos;
}

void CInterface::setSpaceBlur(bool useSpaceBlur)
{
    m_bUseSpaceBlur = useSpaceBlur;
    m_ciRageBlurXShader->setParameter("useSpaceF", m_bUseSpaceBlur ? 1.0f : 0.0f);
    m_ciRageBlurYShader->setParameter("useSpaceF", m_bUseSpaceBlur ? 1.0f : 0.0f);
}
bool CInterface::getSpaceBlur()
{
    return m_bUseSpaceBlur;
}

void CInterface::setCurrentLife(float currentLifePercentage)
{
    if (currentLifePercentage < 0.0f)
        currentLifePercentage = 0.0f;
    else if (currentLifePercentage > 1.0f)
        currentLifePercentage = 1.0f;
  
    /*if (m_fCurrentLife > currentLifePercentage && currentLifePercentage != 0.0f)
        m_bLifeLostOpacity = 510.0f;*/

    m_fCurrentLife = currentLifePercentage;
    m_ciLifeFeedbackShader->setParameter("life", m_fCurrentLife);
}
float CInterface::getCurrentLife()
{
    return m_fCurrentLife;
}

void CInterface::setCurrentScore(unsigned int currentScore)
{
    m_iCurrentScore = currentScore;
	
	std::stringstream tmpStream;
    std::string tmpString;
    tmpStream << currentScore;
    tmpStream >> tmpString;

    m_ciScore.setString(tmpString);
    m_ciScore.setOrigin(m_ciScore.getGlobalBounds().width, 0.0f);
}
unsigned int CInterface::getCurrentScore()
{
    return m_iCurrentScore;
}

void CInterface::setCurrentModifier(unsigned int currentModifier)
{
    m_iCurrentModifier = currentModifier;
	
    std::stringstream tmpStream;
    std::string tmpString;
    tmpStream << currentModifier;
    tmpStream >> tmpString;
    tmpString += "x";

    m_ciModifier.setString(tmpString);
    m_ciModifier.setOrigin(m_ciModifier.getGlobalBounds().width, 0.0f);
}
unsigned int CInterface::getCurrentModifier()
{
    return m_iCurrentModifier;
}

void CInterface::setCurrentLevelTime(float currentLevelTime)
{
    m_fCurrentLevelTime = currentLevelTime;
	
    std::stringstream tmpStream;
    std::string tmpString_minutes;
	std::string tmpString_seconds;
	std::string tmpString_final;
    
	int minutes = (int)(m_fCurrentLevelTime / 60.0f);

	tmpStream << std::setw(2) << std::setfill('0') << minutes;
    tmpStream >> tmpString_minutes;
    tmpString_minutes += ":";
	tmpStream.clear();

	tmpStream << std::setw(2) << std::setfill('0') << floor(m_fCurrentLevelTime - (minutes * 60.0f));
    tmpStream >> tmpString_seconds;
	tmpStream.clear();

	tmpString_final = tmpString_minutes;
	tmpString_final += tmpString_seconds;

	m_ciLevelTime.setString(tmpString_final);
    m_ciLevelTime.setOrigin(m_ciLevelTime.getGlobalBounds().width, 0.0f);
}
float CInterface::getCurrentLevelTime()
{
    return m_fCurrentLevelTime;
}

void CInterface::setCurrentRageWaveProgress(float currentWaveProgress)
{
    m_fCurrentRageWaveProgress = currentWaveProgress;
    m_ciRageWaveShader->setParameter("wave", m_fCurrentRageWaveProgress);
}
float CInterface::getCurrentRageWaveProgress()
{
    return m_fCurrentRageWaveProgress;
}

void CInterface::setCurrentShieldPercentage(float currentShieldPercentage)
{
    m_fCurrentShieldPercentage = currentShieldPercentage;
    m_ciShieldShader->setParameter("percentage", m_fCurrentShieldPercentage);
}
float CInterface::getCurrentShieldPercentage()
{
    return m_fCurrentShieldPercentage;
}

void CInterface::init()
{
    if (m_bInitialized)
        return;

    m_ciRageOverlay.setTexture(*glDataBox->GetTexture("rage_overlay.png"));
    m_bRage = false;

    m_ciScore.setFont(*glDataBox->GetFont("pecita.otf"));
    m_ciScore.setCharacterSize(48);
    m_ciScore.setPosition((float)pwEngineOptions::GetInternGraphicsSize().x - 50.0f, 30.0f);
    m_ciScore.setOrigin(m_ciScore.getGlobalBounds().width, 0.0f);
    m_ciScore.setString(sf::String("0"));

	m_ciModifier.setFont(*glDataBox->GetFont("pecita.otf"));
    m_ciModifier.setCharacterSize(48);
    m_ciModifier.setPosition((float)pwEngineOptions::GetInternGraphicsSize().x - 50.0f, 80.0f);
    m_ciModifier.setOrigin(m_ciModifier.getGlobalBounds().width, 0.0f);
    m_ciModifier.setString(sf::String("1x"));

	m_ciLevelTime.setFont(*glDataBox->GetFont("pecita.otf"));
    m_ciLevelTime.setCharacterSize(48);
    m_ciLevelTime.setOrigin(m_ciModifier.getGlobalBounds().width/2, 0.0f);
    m_ciLevelTime.setPosition(960.0f, 30.0f);
    m_ciLevelTime.setString(sf::String("00:00"));

    m_fCurrentLife = 1.0f;
    m_fCurrentRageWaveProgress = 0.0f;
    m_fCurrentShieldPercentage = 0.0f;

    m_ciLifeFeedbackShader = glDataBox->GetPixelShader("shader01.sfx");
	glGraphics->AddGlobalShader(m_ciLifeFeedbackShader, 1);
    m_ciLifeFeedbackShader->setParameter("life", m_fCurrentLife);
    m_ciLifeFeedbackShader->setParameter("overlay", *glDataBox->GetTexture(glDataBox->GetIniFile("background_" + CLevelData::getCurrentLevelString() + ".ini")->GetString("over")));

    m_ciRageWaveShader = glDataBox->GetPixelShader("shader02.sfx");
    glGraphics->AddGlobalShader(m_ciRageWaveShader, 2);
    m_ciRageWaveShader->setParameter("wave", m_fCurrentRageWaveProgress);

    //------------------------------------------------

    // Shield effect
	m_ciShieldAnimation.setTexture(*glDataBox->GetTexture("shield.png"));
	//m_ciShieldAnimation.setPosition(400.0f, 0.0f);
	m_ciShieldAnimation.SetFrameSize(309, 1109);
	m_ciShieldAnimation.SetTotalFrames(10);
    m_ciShieldAnimation.SetFramesPerSecond(10);

    m_ciShieldShader = glDataBox->GetPixelShader("shield.sfx");
    glGraphics->AddGlobalShader(m_ciShieldShader, 3);
    m_ciShieldShader->setParameter("frame", m_ciShieldAnimation.GetCurrentFrame());
    m_ciShieldShader->setParameter("percentage", m_fCurrentShieldPercentage);
    m_ciShieldShader->setParameter("shield", *glDataBox->GetTexture("shield.png"));

    //

    m_ciRageBlurXShader = glDataBox->GetPixelShader("blur_horizontal.sfx");
    m_ciRageBlurYShader = glDataBox->GetPixelShader("blur_vertical.sfx");

    m_preShutdowned = false;

    m_bInitialized = true;
}

void CInterface::reset()
{
    if (!m_bInitialized)
        init();

    m_ciModifier.setString(sf::String("1x"));
    m_iCurrentModifier = 1;

    m_fCurrentLife = 1.0f;
    m_fCurrentRageWaveProgress = 0.0f;
    m_fCurrentShieldPercentage = 0.0f;

    m_ciLifeFeedbackShader = glDataBox->GetPixelShader("shader01.sfx");
	glGraphics->AddGlobalShader(m_ciLifeFeedbackShader, 1);
    m_ciLifeFeedbackShader->setParameter("life", m_fCurrentLife);
    m_ciLifeFeedbackShader->setParameter("overlay", *glDataBox->GetTexture(glDataBox->GetIniFile("background_" + CLevelData::getCurrentLevelString() + ".ini")->GetString("over")));


    m_ciRageWaveShader = glDataBox->GetPixelShader("shader02.sfx");
    glGraphics->AddGlobalShader(m_ciRageWaveShader, 2);
    m_ciRageWaveShader->setParameter("wave", m_fCurrentRageWaveProgress);

    m_ciShieldShader = glDataBox->GetPixelShader("shield.sfx");
    glGraphics->AddGlobalShader(m_ciShieldShader, 3);
    m_ciShieldShader->setParameter("percentage", m_fCurrentShieldPercentage);

    m_preShutdowned = false;
}

void CInterface::restart()
{
    m_ciLifeFeedbackShader = glDataBox->GetPixelShader("shader01.sfx");
	glGraphics->AddGlobalShader(m_ciLifeFeedbackShader, 1);

    m_ciRageWaveShader = glDataBox->GetPixelShader("shader02.sfx");
    glGraphics->AddGlobalShader(m_ciRageWaveShader, 2);

    m_ciShieldShader = glDataBox->GetPixelShader("shield.sfx");
    glGraphics->AddGlobalShader(m_ciShieldShader, 3);

    m_preShutdowned = false;

    if (m_bRage)
        startRage();
}
void CInterface::shutdown()
{
    glGraphics->RemoveGlobalShader(1);
}

void CInterface::pre_shutdown()
{
    glGraphics->RemoveGlobalShader(2);
    glGraphics->RemoveGlobalShader(3);
    glGraphics->RemoveGlobalShader(4);
    glGraphics->RemoveGlobalShader(5);
    m_preShutdowned = true;
}

void CInterface::draw()
{
    m_ciShieldAnimation.update();
    m_ciShieldShader->setParameter("frame", m_ciShieldAnimation.GetCurrentFrame());

    if (!m_preShutdowned)
    {
        glGraphics->DrawOverlay(&m_ciScore, 2);
	    glGraphics->DrawOverlay(&m_ciModifier, 2);
	    glGraphics->DrawOverlay(&m_ciLevelTime, 2);
    }

    if (m_bRage)
    {
        //m_ciRageOverlay.setColor(sf::Color(255, 255, 255, 255));
        glGraphics->Draw(&m_ciRageOverlay, 5000, sf::BlendMultiply);
    }
    //else if (m_bLifeLostOpacity != 0.0f)
    //{
    //    m_bLifeLostOpacity -= 255.0f*glGraphics->GetLastFrameTime();

    //    if ((int)(m_bLifeLostOpacity / 255.0f) == 0)
    //        m_ciRageOverlay.setColor(sf::Color(255, 255, 255, ((int)m_bLifeLostOpacity) % 256));
    //    else
    //        m_ciRageOverlay.setColor(sf::Color(255, 255, 255, 255 -((int)m_bLifeLostOpacity) % 256));

    //    glGraphics->Draw(&m_ciRageOverlay, 5000/*, sf::BlendMultiply*/);

    //    //glGLog << m_bLifeLostOpacity << pwGraphicsLog::flush;

    //    if (m_bLifeLostOpacity < 0.0f)
    //        m_bLifeLostOpacity = 0.0f;
    //}
}

CInterface* CInterface::get()
{
    if (m_s_p_ciInstance == NULL)
    {
        m_s_p_ciInstance = new CInterface;
    }

    return m_s_p_ciInstance;
}

void CInterface::startRage()
{
    glGraphics->AddGlobalShader(m_ciRageBlurXShader, 4);
    glGraphics->AddGlobalShader(m_ciRageBlurYShader, 5);
    m_bRage = true;
}
void CInterface::endRage()
{
    glGraphics->RemoveGlobalShader(4);
    glGraphics->RemoveGlobalShader(5);
    m_bRage = false;
}