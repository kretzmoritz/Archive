////////////////////////////////////////////////////////////
// Philipp Wittershagen
// 2012-07-20
// Implementation of the music-system
////////////////////////////////////////////////////////////

#ifndef CMUSICSYSTEM
#define CMUSICSYSTEM

#ifndef PWUSEGLOBALSINGLETONS
#define PWUSEGLOBALSINGLETONS
#endif

#include <SFML\Audio.hpp>
#include "..\pwSystem.hpp"
#include "ID.hpp"

#include "CSystem.h"
#include "CTimer.h"
#include "..\pwGraphicsLog.hpp"

class CMusicSystem
{

public:
    
    CMusicSystem() {m_ciBPM = "80bpm"; m_ciSound = new sf::Sound;}
    ~CMusicSystem() {if (m_ciSound != NULL) {delete m_ciSound; m_ciSound = NULL;}}

    void SetBPM(unsigned int BPM);
    void init();
    void reset();
    void shutdown();

    sf::Sound* getSound()   {return m_ciSound;}
    void setSound(sf::Sound* Sound) {m_ciSound = Sound;}

    static void CorrectSound();

    static CMusicSystem* get();

private:

    static std::string m_ciFileName;
    static std::string m_ciBPM;
    unsigned int m_uiBPM;
    static unsigned int m_uiBeats;
    static bool m_bChangeBPM;
    sf::Sound *m_ciSound;
    CTimer m_ciTimer;

    static CMusicSystem* m_s_p_ciInstance;
};

#endif