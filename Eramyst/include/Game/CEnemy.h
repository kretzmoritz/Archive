////////////////////////////////////////////////////////////
// Moritz Kretz
// 05/29/2012
// Updated: 07/14/2012
// System implementation of enemy (attacking units) class
////////////////////////////////////////////////////////////

#ifndef ENEMY
#define ENEMY

#define PWUSEGLOBALSINGLETONS

#include <SFML\Graphics.hpp>

#include "ID.hpp"
#include "CTimer.h"

#include "CSquadData.h"
#include "CEnemyData.h"
#include "CProjectile.h"

#define E_ANIM_MOVE 1
#define E_ANIM_ATTACK 2
#define E_ANIM_BACKPEDAL 3
#define E_ANIM_WOUNDED 4
#define E_ANIM_DEATH 5

class CEnemy : public pwGameObject
{
	struct enemy_ai
	{
		int m_iCurrentState;
		bool m_bAttackStarted;
		bool m_bAttackFinished;
		float m_fSpeed;
		float m_fAttackPosition;
	} ai_data;

	public:
		struct enemy_squad
		{
			std::vector<CEnemy*> vUnits;
			std::vector< std::vector<int> > vSalve;
			std::vector<float> fSalveTime;
			int iCurrentOrder;
			std::vector<CSquadData::single_order> vAttackOrder;
		};

		CEnemy() : m_bWasHit(false) {};
		~CEnemy() { delete ci_timer; delete ci_enemy_data; };

		void init(int _type, int _state, bool _virtual);
		void setCurrentState(int _value);
		
		enemy_ai &getAIData();
		int getType() const { return m_iType; }
		CEnemyData* getEnemyData() const { return ci_enemy_data; }

		pwAnimation &getVisual() { return m_visual; }
		pwCollisionRect getCollisionObject() const;

        void setWasHit()       {m_bWasHit = true;}
        bool getWasHit() const {return m_bWasHit;}
		
		void setMove();
		void setAnimation(unsigned int _animation, bool _playOnce = false);
		void setAnimationSpeed(float _value);

		void flash(); // Puts emphasis on the enemy using a colored flash
		void launchAttack(); // Attacks the coloss
		void launchAttackSalve();
		void destroy();

		static void update();
		static std::vector<enemy_squad*> &getEnemyVector();

	private:
		void hasFlashFinished();
		static void hasFlashFinishedWrapper(const void *context);
		static void deleteDeadObjects();

        bool m_bWasHit;
		int m_iType;
		bool m_bVirtual;
		bool m_bPlayMoveOnce;
		sf::Color m_color;

		pwAnimation m_visual;
		pwCollisionRect m_collision;

		CTimer *ci_timer;
		CEnemyData *ci_enemy_data;

		static std::vector<enemy_squad*> m_vSquads;
};

#endif