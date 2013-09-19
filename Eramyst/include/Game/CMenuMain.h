////////////////////////////////////////////////////////////
// Philipp Wittershagen
// 2012-07-17
// Implementation of the main-menu : inherits from pwGameState
////////////////////////////////////////////////////////////

#ifndef CMENUMAIN
#define CMENUMAIN

#ifndef PWUSEGLOBALSINGLETONS
#define PWUSEGLOBALSINGLETONS
#endif

#include "..\pwSystem.hpp"
#include "ID.hpp"
#include "CLevelData.h"

class CMenuMain : public pwGameState
{

public:
    
    CMenuMain();
    ~CMenuMain();

    bool Start ();
    unsigned int Run();
    bool Close();


private:

    sf::Sprite m_ciBackground;
    sf::Sprite m_ciBackgroundObject00;
    sf::Sprite m_ciBackgroundObject01;
    sf::Sprite m_ciTitle;
    float m_fBackground00Rotation;
    float m_fBackground01Rotation;

    pwGUI::pwButton m_ciPlayButton;
    pwGUI::pwButton m_ciLevelButton;
    pwGUI::pwButton m_ciTutorialButton;
    pwGUI::pwButton m_ciHighscoreButton;
    pwGUI::pwButton m_ciOptionsButton;
    pwGUI::pwButton m_ciCreditsButton;
    pwGUI::pwButton m_ciQuitButton;

    sf::Text m_ciPlayText;
    sf::Text m_ciLevelText;
    sf::Text m_ciTutorialText;
    sf::Text m_ciHighscoreText;
    sf::Text m_ciOptionsText;
    sf::Text m_ciQuitText;
    sf::Text m_ciCreditsText;

    sf::Sprite m_ciPlayIdle;
    sf::Sprite m_ciPlayHover;
    sf::Sprite m_ciLevelIdle;
    sf::Sprite m_ciLevelHover;
    sf::Sprite m_ciTutorialIdle;
    sf::Sprite m_ciTutorialHover;
    sf::Sprite m_ciHighscoreIdle;
    sf::Sprite m_ciHighscoreHover;
    sf::Sprite m_ciOptionsIdle;
    sf::Sprite m_ciOptionsHover;
    sf::Sprite m_ciQuitIdle;
    sf::Sprite m_ciQuitHover;
    sf::Sprite m_ciCreditsIdle;
    sf::Sprite m_ciCreditsHover;
    pwCollisionRect m_ciPlayCollision;
    pwCollisionRect m_ciLevelCollision;
    pwCollisionRect m_ciTutorialCollision;
    pwCollisionRect m_ciHighscoreCollision;
    pwCollisionRect m_ciOptionsCollision;
    pwCollisionRect m_ciQuitCollision;
    pwCollisionRect m_ciCreditsCollision;

    pwStreamAnimation tmp;

    pwGameObject m_ciMenuWrapper;

    bool Initialized;
};

#endif