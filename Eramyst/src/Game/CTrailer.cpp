#include "..\..\include\Game\CTrailer.h"

CTrailer::CTrailer() : pwGameState(ID_GS_TRAILER)
{
}
CTrailer::~CTrailer()
{
}

bool CTrailer::Start ()
{
#ifdef NDEBUG
    m_ciTrailer.openFromFile("data\\movies\\trailer.theora.ogv");
    m_ciTrailer.resizeToFrame(0, 0, pwEngineOptions::GetInternGraphicsSize().x, pwEngineOptions::GetInternGraphicsSize().y);
    m_ciTrailer.play();
#endif
    return true;
}
unsigned int CTrailer::Run()
{
#ifdef NDEBUG
    if (m_ciTrailer.getStatus() != sfe::Movie::Playing || 
        sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || 
        sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        m_ciTrailer.stop();
        return ID_GS_PLAY;
    }

    glGraphics->Draw(&m_ciTrailer);

    return myID;
#else
    return ID_GS_PLAY;
#endif
}