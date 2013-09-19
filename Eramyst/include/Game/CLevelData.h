////////////////////////////////////////////////////////////
// Philipp Wittershagen
// 2012-07-26
// System implementation of the current level
////////////////////////////////////////////////////////////

#ifndef CLEVELDATA
#define CLEVELDATA

#define PWUSEGLOBALSINGLETONS

#include "..\pwSystem.hpp"
#include "ID.hpp"

class CLevelData : public pwGameState
{
public:
    CLevelData() : m_iMax(30), m_bInitialized(false), pwGameState(ID_GS_LEVELDATA) {};
    ~CLevelData() {};

    bool Start();
    unsigned int Run();

    static void setCurrentLevelString(std::string LevelString, bool PlayOnce = false);
    static std::string getCurrentLevelString();
    static bool getPlayOnce() {return m_bPlayLevelOnce;}

    static void UpdateCurrentLevelTime(float FrameTime);
    static bool LevelToChange();

private:
    static float m_s_fCurrentLevelTime;
    static float m_s_fCurrentLevelEndTime;
    static std::string m_s_ciCurrentLevelString;
    static bool m_bPlayLevelOnce;

    sf::RectangleShape m_ciShape;
    //sf::Sprite         m_ciGameOverSprite;
    sf::Text           m_ciScoreText;
    sf::Text           m_ciScore;
    sf::Text           m_ciInput;
    sf::Text           m_ciContinue;
    sf::Text           m_ciEnterName;

    pwIniFile          m_HighscoreIni;
    std::map<std::string, unsigned int> m_HighscoreMap;

    std::string        m_sInput;

    unsigned int m_iMax;

    bool m_bHighscore;
    bool m_bInitialized;
    bool m_bNewPressKey;
};

#endif
