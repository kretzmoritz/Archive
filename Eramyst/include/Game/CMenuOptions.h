////////////////////////////////////////////////////////////
// Philipp Wittershagen
// 2012-07-17
// Implementation of the options-menu : inherits from pwGameState
////////////////////////////////////////////////////////////

#ifndef CMENUOPTIONS
#define CMENUOPTIONS

#ifndef PWUSEGLOBALSINGLETONS
#define PWUSEGLOBALSINGLETONS
#endif

#include "..\pwSystem.hpp"
#include "CSettings.h"
#include "ID.hpp"

class CMenuOptions : public pwGameState
{

public:
    
    CMenuOptions();
    ~CMenuOptions();

    bool Start ();
    unsigned int Run();
    bool Close();


private:

    sf::Sprite m_ciBackground;
    sf::Sprite m_ciTitle;

    sf::RectangleShape m_ciKeyAssignBGShape;
    sf::Sprite m_ciKeyAssignBackground;
    
    pwGUI::pwButton m_ciCancelButton;
    pwGUI::pwButton m_ciOkButton;
    pwGUI::pwButton m_ciControls;
    pwGUI::pwSlider m_ciAudio;
    pwGUI::pwSlider m_ciMusic;
    pwGUI::pwSnapSlider m_ciGraphics;
    pwGUI::pwCheckBox m_ciFullscreen;

    sf::Text m_ciAudioText;
    sf::Text m_ciMusicText;
    sf::Text m_ciGraphicsText;
    sf::Text m_ciFullscreenText;
    sf::Text m_ciControlsLabel;
    sf::Text m_ciControlsText;

    sf::Text m_ciAudioValue;
    sf::Text m_ciMusicValue;
    sf::Text m_ciGraphicsValue;

    sf::Sprite m_ciMusicSlider;
    sf::Sprite m_ciAudioSlider;
    sf::Sprite m_ciGraphicsSlider;
    
    sf::Sprite m_ciCancelIdle;
    sf::Sprite m_ciCancelHover;
    sf::Sprite m_ciOkIdle;
    sf::Sprite m_ciOkHover;
    sf::Sprite m_ciFullscreenIdle;
    sf::Sprite m_ciFullscreenHover;
    sf::Sprite m_ciControlsIdle;
    sf::Sprite m_ciControlsHover;
    
    sf::Text m_ciCancelText;
    sf::Text m_ciOkText;

    pwCollisionRect m_ciCancelCollision;
    pwCollisionRect m_ciOkCollision;
    pwCollisionRect m_ciFullscreenCollision;
    pwCollisionRect m_ciControlsCollision;

    unsigned int m_uiState;

    std::vector<sf::VideoMode> m_aVideoModes;

    sf::Keyboard::Key m_aKeys[5];
    sf::Keyboard::Key m_aOldKeys[5];
    sf::Text m_aKeyNames[5];
    sf::Text m_aLayerNames[5];
    std::string m_aHelpString[5];
    sf::Text m_ciHelpText;

    unsigned int m_uiVideoHeight;
    unsigned int m_uiVideoWidth;

    float m_fOldAudioVolume;
    float m_fOldMusicVolume;

    bool Initialized;
};

#endif