////////////////////////////////////////////////////////////
// Philipp Wittershagen
// 2012-07-17
// Implementation of the trailer before the game is started : inherits from pwGameState
////////////////////////////////////////////////////////////

#ifndef CINSTANT
#define CINSTANT

#ifndef PWUSEGLOBALSINGLETONS
#define PWUSEGLOBALSINGLETONS
#endif

#include "..\pwSystem.hpp"
#include "ID.hpp"

class CInstant : public pwGameState
{

public:
    
    CInstant();
    ~CInstant();

    bool Start ();
    unsigned int Run();

private:

    sf::Sprite m_ciInstantEntertainmentLogo;

    bool m_bDown;
    float m_fAlpha;

};

#endif