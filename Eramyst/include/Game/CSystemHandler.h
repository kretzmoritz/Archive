////////////////////////////////////////////////////////////
// Moritz Kretz
// 07/17/2012
// System implementation of system handler (decides on state) class
////////////////////////////////////////////////////////////

#ifndef SYSTEM_HANDLER
#define SYSTEM_HANDLER

#define PWUSEGLOBALSINGLETONS

#include "..\pwSystem.hpp"
#include "CColoss.h"
#include "CColossNormal.h"
#include "CSystem.h"
#include "CSystemNormal.h"
#include "CSystemRagemode.h"

#define NORMAL_SYSTEM 0
#define RAGEMODE_SYSTEM 1

class CSystemHandler : public pwGameObject
{
	public:
		CSystemHandler() { m_bColossConstructed = false; };
		~CSystemHandler() { delete ci_currentState; delete ci_system_normal; delete ci_system_ragemode; delete ci_getState; };

		void init();
		void reset();
		
		void setCurrentState(CSystem *_state);

		void update();
		void decideOnState();

	private:
		CSystem *ci_currentState;
		CSystem *ci_system_normal;
		CSystem *ci_system_ragemode;

		CColossNormal *ci_getState; // Used for state changes as we can get static health from any member of CColoss class

		int m_iNextState;
		bool m_bColossConstructed;
};

#endif