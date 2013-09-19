#include "..\..\include\Game\CInstant.h"

CInstant::CInstant() : pwGameState(ID_GS_INSTANTENTERTAINMENT)
{
}
CInstant::~CInstant()
{
}

bool CInstant::Start ()
{
    m_bDown = false;
    m_ciInstantEntertainmentLogo.setTexture(*glDataBox->GetTexture("instantentertainment.png"));
    m_ciInstantEntertainmentLogo.setOrigin((float)m_ciInstantEntertainmentLogo.getTexture()->getSize().x / 2.0f, (float)m_ciInstantEntertainmentLogo.getTexture()->getSize().y / 2.0f);
    m_ciInstantEntertainmentLogo.setPosition((float)pwEngineOptions::GetInternGraphicsSize().x / 2.0f, (float)pwEngineOptions::GetInternGraphicsSize().y / 2.0f);
    m_ciInstantEntertainmentLogo.setColor(sf::Color(255, 255, 255, 0));
    m_fAlpha = 0.0f;
    return true;
}
unsigned int CInstant::Run()
{
    if (m_bDown)
    {
        m_fAlpha = m_fAlpha - 128.0f*glGraphics->GetLastFrameTime();
        m_ciInstantEntertainmentLogo.setColor(sf::Color(255, 255, 255, (m_fAlpha<0.0f)?0:(sf::Uint8)m_fAlpha));
    }
    else
    {
        m_fAlpha = m_fAlpha + 128.0f*glGraphics->GetLastFrameTime();

        if (m_fAlpha >= 255.0f)
        {
            m_bDown = true;
        }
        m_ciInstantEntertainmentLogo.setColor(sf::Color(255, 255, 255, (m_fAlpha>255.0f)?255:(sf::Uint8)m_fAlpha));
    }
    glGraphics->Draw(&m_ciInstantEntertainmentLogo);

    if ((m_bDown && m_ciInstantEntertainmentLogo.getColor().a == 0)  || 
        sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || 
        sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        return ID_GS_MENU_MAIN;
    }

    return myID;
}