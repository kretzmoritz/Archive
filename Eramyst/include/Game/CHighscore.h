////////////////////////////////////////////////////////////
// Philipp Wittershagen
// 2012-07-17
// Implementation of the level-select-menu : inherits from pwGameState
////////////////////////////////////////////////////////////

#ifndef CHIGHSCORE
#define CHIGHSCORE

#ifndef PWUSEGLOBALSINGLETONS
#define PWUSEGLOBALSINGLETONS
#endif

#include "..\pwSystem.hpp"
#include "ID.hpp"

class CHighscore : public pwGameState
{

public:
    
    CHighscore();
    ~CHighscore();

    bool Start ();
    unsigned int Run();

private:

    sf::Sprite m_ciBackground;
    sf::Sprite m_ciTitle;
    pwGUI::pwButton m_ciBackButton;
    pwGUI::pwButton m_ciScrollSlider;
    pwGUI::pwScroll m_ciScrollBar;

    std::list<pwHelper::split_data<unsigned int, pwHelper::split_data<std::string, std::string>>> m_HighscoreEntryList;
    pwHelper::split_data<sf::Text, sf::Text> m_ciHighscoreEntries[10];

    sf::Sprite m_ciBacklIdle;
    sf::Sprite m_ciBackHover;
    sf::Sprite m_ciScrollIdle;
    sf::Sprite m_ciScrollHover;
    sf::Sprite m_ciScroll;
    pwCollisionRect m_ciScrollCollision;
    pwCollisionRect m_ciBackCollision;

    sf::Text m_ciBackText;

    bool Initialized;
};

#endif