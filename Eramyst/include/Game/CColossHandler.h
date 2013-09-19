////////////////////////////////////////////////////////////
// Moritz Kretz
// 07/14/2012
// System implementation of coloss handler (decides on state) class
////////////////////////////////////////////////////////////

#ifndef COLOSS_HANDLER
#define COLOSS_HANDLER

#define PWUSEGLOBALSINGLETONS

#include "..\pwSystem.hpp"
#include "CColoss.h"
#include "CColossNormal.h"
#include "CColossRagemode.h"

#define NORMAL_COLOSS 0
#define RAGEMODE_COLOSS 1

class CColossHandler : public pwGameObject
{
	public:
		CColossHandler() {}
		~CColossHandler() { delete ci_currentState; delete ci_coloss_normal; delete ci_coloss_ragemode; }

		void init();
		void reset();

		void setCurrentState(CColoss *_state);

		void update();
		void decideOnState();

	private:
		CColoss *ci_currentState;
		CColoss *ci_coloss_normal;
		CColoss *ci_coloss_ragemode;

		int m_iNextState;
};

#endif