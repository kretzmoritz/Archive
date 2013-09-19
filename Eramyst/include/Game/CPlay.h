#ifndef CPLAY
#define CPLAY

#include "..\pwSystem.hpp"
#include "ID.hpp"

#include "CTimer.h"
#include "CBackground.h"
#include "CInterface.h"
#include "CEnemy.h"
#include "CProjectile.h"
#include "CColossHandler.h"
#include "CSystemHandler.h"
#include "CBeatSystem.h"
#include "CMusicSystem.h"
#include "CDisplayBar.h"

class CPlay : public pwGameState 
{
public:
    CPlay();
    ~CPlay();

	bool Start();
	unsigned int Run();

    static void SetMenu(bool Menu) {m_bMenu = Menu;}

private:
	CBackground *m_ciBackground;
	CColossHandler *m_ciColossHandler;
	CSystemHandler *m_ciSystemHandler;

    // pwAnimation m_ciColossDeathBottomLeft;
    // pwAnimation m_ciColossDeathBottomRight;
    // pwAnimation m_ciColossDeathTop;

    pwStreamAnimation m_ciColossDeath;
    
    sf::RectangleShape m_ciShape;

    bool m_bInitialized;
    float m_fFade;

    static bool m_bMenu;
    bool bButtonPressed;
};

#endif