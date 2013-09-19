////////////////////////////////////////////////////////////
// Moritz Kretz
// 07/17/2012
// System implementation of system (enemy creation) class
// State: Ragemode
////////////////////////////////////////////////////////////

#ifndef SYSTEM_RAGEMODE
#define SYSTEM_RAGEMODE

#include "ID.hpp"
#include "CTimer.h"

#include "CSquadData.h"

class CSystemRagemode : public CSystem
{
	public:
		CSystemRagemode();
		~CSystemRagemode() { delete ci_timer_spawn; delete ci_squad_data; };

		void start();
		void run();
		void close() {};

		void spawn();
		static void spawnWrapper(const void *context);

	private:
		CTimer *ci_timer_spawn;
		CSquadData *ci_squad_data;
};

#endif