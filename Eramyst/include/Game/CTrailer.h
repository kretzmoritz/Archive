////////////////////////////////////////////////////////////
// Philipp Wittershagen
// 2012-07-17
// Implementation of the trailer before the game is started : inherits from pwGameState
////////////////////////////////////////////////////////////

#ifndef CTRAILER
#define CTRAILER

#ifndef PWUSEGLOBALSINGLETONS
#define PWUSEGLOBALSINGLETONS
#endif

#include "..\pwSystem.hpp"
#include "..\sfeMovie\Movie.hpp"
#include "ID.hpp"

class CTrailer : public pwGameState
{

public:
    
    CTrailer();
    ~CTrailer();

    bool Start ();
    unsigned int Run();

private:

    sfe::Movie m_ciTrailer;

};

#endif