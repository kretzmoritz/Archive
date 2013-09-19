////////////////////////////////////////////////////////////
// Philipp Wittershagen
// 2012-07-17
// Implementation of the level-select-menu : inherits from pwGameState
////////////////////////////////////////////////////////////

#ifndef CMENUCREDITS
#define CMENUCREDITS

#ifndef PWUSEGLOBALSINGLETONS
#define PWUSEGLOBALSINGLETONS
#endif

#include "..\pwSystem.hpp"
#include "ID.hpp"

class CMenuCredits : public pwGameState
{

public:
    
    CMenuCredits();
    ~CMenuCredits();

    bool Start ();
    unsigned int Run();

private:

    sf::Sprite m_ciBackground;
    sf::Sprite m_ciTitle;
    sf::Sprite m_ciCredits;
    pwGUI::pwButton m_ciBackButton;

    sf::Sprite m_ciBacklIdle;
    sf::Sprite m_ciBackHover;
    pwCollisionRect m_ciBackCollision;

    sf::Text m_ciBackText;

    bool Initialized;
};

#endif