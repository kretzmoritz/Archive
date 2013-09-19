////////////////////////////////////////////////////////////
// Philipp Wittershagen
// 2012-07-20
// Implementation of the beat-system
////////////////////////////////////////////////////////////

#ifndef CBEATSYSTEM
#define CBEATSYSTEM

#ifndef PWUSEGLOBALSINGLETONS
#define PWUSEGLOBALSINGLETONS
#endif

#include "..\pwSystem.hpp"
#include "ID.hpp"

#include "CSystem.h"
#include "CMusicSystem.h"
#include "CLevelData.h"

class CBeatSystem
{

public:
    
    CBeatSystem() {}
    ~CBeatSystem() {}

    static void SetBPM(unsigned int BPM);
    static unsigned int GetBPM();

    static float GetPerBeatTime();


private:

    static unsigned int m_uiCurrentBPM;
    static float m_fCurrentPerBeatTime;
};

#endif