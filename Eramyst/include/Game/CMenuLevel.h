////////////////////////////////////////////////////////////
// Philipp Wittershagen
// 2012-07-17
// Implementation of the level-select-menu : inherits from pwGameState
////////////////////////////////////////////////////////////

#ifndef CMENULEVEL
#define CMENULEVEL

#ifndef PWUSEGLOBALSINGLETONS
#define PWUSEGLOBALSINGLETONS
#endif

#include "..\pwSystem.hpp"
#include "ID.hpp"
#include "CLevelData.h"

class CMenuLevel : public pwGameState
{

public:
    
    CMenuLevel();
    ~CMenuLevel();

    bool Start ();
    unsigned int Run();
    bool Close();


private:

    sf::Sprite m_ciBackground;
    sf::Sprite m_ciTitle;
    pwGUI::pwButton m_ciDarkButton;
    pwGUI::pwButton m_ciFutureButton;
    pwGUI::pwButton m_ciBackButton;

    sf::Sprite m_ciDarkIdle;
    sf::Sprite m_ciDarkHover;
    sf::Sprite m_ciFutureIdle;
    sf::Sprite m_ciFutureHover;
    sf::Sprite m_ciBacklIdle;
    sf::Sprite m_ciBackHover;
    pwCollisionRect m_ciDarkCollision;
    pwCollisionRect m_ciFutureCollision;
    pwCollisionRect m_ciBackCollision;

    sf::Text m_ciDarkText;
    sf::Text m_ciFutureText;
    sf::Text m_ciBackText;

    bool Initialized;
};

#endif