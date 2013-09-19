////////////////////////////////////////////////////////////
// Philipp Wittershagen
// 2012-07-16
// System implementation of the interface 
// Uses pwGraphicsSystem-Layer 2
////////////////////////////////////////////////////////////

#ifndef CSETTINGS
#define CSETTINGS

#ifndef PWUSEGLOBALSINGLETONS
#define PWUSEGLOBALSINGLETONS
#endif

#include "..\pwSystem.hpp"

class CSettings
{
public:
    CSettings();
    ~CSettings();

    void setSoundVolume(float Volume);
    float getSoundVolume();

    void setMusicVolume(float Volume);
    float getMusicVolume();

    void setLayerKey(unsigned int Layer, sf::Keyboard::Key Key);
    sf::Keyboard::Key getLayerKey(unsigned int Layer);

    const std::string& getHighscoreFileName();

    void save();
    void init();

    static CSettings* get();
    bool Changed()              {return m_bChanged;}

private:
    float m_fSoundVolume;
    float m_fMusicVolume;

    float m_aSoundVolumes[32];
    
    sf::Keyboard::Key m_eLayer00;
	sf::Keyboard::Key m_eLayer01;
    sf::Keyboard::Key m_eLayer02;
    sf::Keyboard::Key m_eLayer03;
    sf::Keyboard::Key m_eLayer04;

    bool m_bChanged;
    bool m_bInitialized;

    std::string m_sHighscoreFileName;

    static CSettings* m_s_p_ciInstance;
};

#endif