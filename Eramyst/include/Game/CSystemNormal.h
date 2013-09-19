////////////////////////////////////////////////////////////
// Moritz Kretz
// 07/17/2012
// System implementation of system (enemy creation) class
// State: Normal
////////////////////////////////////////////////////////////

#ifndef SYSTEM_NORMAL
#define SYSTEM_NORMAL

#include "ID.hpp"
#include "CTimer.h"

#include "CSquadData.h"
#include "CBeatSystem.h"
#include "CProjectile.h"

class CSystemNormal : public CSystem
{
	public:
		CSystemNormal();
		~CSystemNormal() { delete ci_timer_spawn; delete ci_squad_data; };

		void start();
		void run();
		void close() {};

		void spawnSquad();
		void bossAttack();
		void wasBossHit();

		void spawn();
		static void spawnWrapper(const void *context);

		std::vector<int> getRandomElements(unsigned int _size, int _removeCount) const;

	private:
		CTimer *ci_timer_spawn;
		CSquadData *ci_squad_data;

		int m_iBossAttack;
};

#endif