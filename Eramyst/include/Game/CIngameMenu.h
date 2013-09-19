////////////////////////////////////////////////////////////
// Philipp Wittershagen
// 2012-07-17
// Implementation of the main-menu : inherits from pwGameState
////////////////////////////////////////////////////////////

#ifndef CINGAME
#define CINGAME

#ifndef PWUSEGLOBALSINGLETONS
#define PWUSEGLOBALSINGLETONS
#endif

#include "..\pwSystem.hpp"
#include "ID.hpp"
#include "CLevelData.h"

class CIngame : public pwGameState
{

public:
    
    CIngame();
    ~CIngame();

    bool Start ();
    unsigned int Run();

private:

    sf::Sprite m_ciBackground;
    sf::Sprite m_ciBackgroundObject00;
    sf::Sprite m_ciBackgroundObject01;
    sf::Sprite m_ciTitle;
    float m_fBackground00Rotation;
    float m_fBackground01Rotation;

    pwGUI::pwButton m_ciBackToMenuButton;
    pwGUI::pwButton m_ciPlayButton;
    pwGUI::pwButton m_ciReplayButton;

    sf::Text m_ciBackToMenuText;

    sf::Sprite m_ciPlayIdle;
    sf::Sprite m_ciPlayHover;
    sf::Sprite m_ciBackToMenuIdle;
    sf::Sprite m_ciBackToMenuHover;
    sf::Sprite m_ciReplayIdle;
    sf::Sprite m_ciReplayHover;
    pwCollisionRect m_ciPlayCollision;
    pwCollisionRect m_ciBackToMenuCollision;
    pwCollisionRect m_ciReplayCollision;

    bool Initialized;
};

#endif