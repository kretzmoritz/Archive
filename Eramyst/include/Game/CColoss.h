////////////////////////////////////////////////////////////
// Moritz Kretz
// 05/29/2012
// Updated: 07/15/2012
// System implementation of coloss (player) class
////////////////////////////////////////////////////////////

#ifndef COLOSS
#define COLOSS

#define PWUSEGLOBALSINGLETONS

#include "..\pwSystem.hpp"

#include "CInterface.h"
#include "CEnemy.h"
#include "CProjectile.h"
#include "CSystem.h"
#include "CColossData.h"
#include "CBeatSystem.h"
#include "CSettings.h"

#define C_ANIM_IDLE 1
#define C_ANIM_MOVE 2
#define C_ANIM_BLOCK 3
#define C_ANIM_PULLBACK 4

#define getEnemyVector() CEnemy::getEnemyVector()
#define getProjectileVector() CProjectile::getProjectileVector()

class CColoss : public pwGameObject
{
	struct coloss_piece
	{
		int layer_offset;
		bool hold_animation;
		pwAnimation animation;
	};

	struct position
	{
		float x;
		float y;
	};

	public:
		CColoss() { setHealth(100.0f); setScore(0); setModifier(1); setBlocked(0); setSteps(0); setImmortal(false); m_iBPMIncreaseCount = 0; m_bBoss = false; m_bBossFinished = false; }
		~CColoss() {}

		static void init();
		static void constructColoss();
		static position getPosition();
		static void setPosition(float _x, float _y);
		static void reset();

		float getHealth() const { return m_fHealth; }
		void setHealth(float _value) { m_fHealth = _value; CInterface::get()->setCurrentLife(m_fHealth / 100); }
		void modHealth(float _value) { m_fHealth += _value; CInterface::get()->setCurrentLife(m_fHealth / 100); }

		int getScore() const { return CInterface::get()->getCurrentScore(); }
		void setScore(int _value) { CInterface::get()->setCurrentScore(_value); }
		void modScore(int _value) { CInterface::get()->setCurrentScore(CInterface::get()->getCurrentScore() + _value); }

		int getModifier() const { return m_iModifier; }
		void setModifier(int _value) { m_iModifier = _value; CInterface::get()->setCurrentModifier(m_iModifier); }
		void modModifier(int _value) { m_iModifier += _value; CInterface::get()->setCurrentModifier(m_iModifier); }

		int getBlocked() const { return m_iBlockedUnits; }
		void setBlocked(int _value) { m_iBlockedUnits = _value; }
		void modBlocked(int _value) { m_iBlockedUnits += _value; }

		int getSteps() const { return m_iSteps; }
		void setSteps(int _value) { m_iSteps = _value; }
		void modSteps(int _value) { m_iSteps += _value; }

		static bool getImmortal() { return m_bImmortal; }
		static void setImmortal(bool _value) { m_bImmortal = _value; }

		std::vector<coloss_piece>& getMoveVisuals() { return m_vColossMove; }
		std::vector<coloss_piece>& getBlockVisuals() { return m_vColossBlock; }
		std::vector<coloss_piece>& getRagemodeVisuals() { return m_vColossRagemode; }

		static bool getBoss() { return m_bBoss; }
		static void setBoss(bool _value) { m_bBoss = _value; }

		static bool getBossFinished() { return m_bBossFinished; }
		static void setBossFinished(bool _value) { m_bBossFinished = _value; }

		static void disableRagemodeColoss();

		virtual void start() =0;
		virtual void run();
		virtual void close() =0;

		void onHit();
		void increaseBPM();
		void switchToMove();
		void screenShake();
		void drawNegativePointsText();
		void enableFlash();
		void drawFlash();

		static CColossData ci_coloss_data;
		static pwIniFile *modifier_data;
		static int m_iMultiplierDuration;

		static float m_fOriginX;
		static float m_fOriginY;

		static sf::Text text_negative_points;
		static float m_fFadeOutText;

	private:
		static float m_fHealth;
		static bool m_bImmortal;
		static int m_iModifier;
		static int m_iBlockedUnits;
		static int m_iSteps;
		static int m_iBPMIncreaseCount;
		static bool m_bBoss;
		static bool m_bBossFinished;

		static int m_iBPMDuration;
		static float m_fHealthIncrease;

		static pwCollisionRect m_hitableCollision;

		static std::vector<coloss_piece> m_vColossMove;
		static std::vector<coloss_piece> m_vColossBlock;
		static std::vector<coloss_piece> m_vColossRagemode;

		static sf::RectangleShape rectScreenFlash;
		static float m_fFadeOutFlash;
		static bool bInit;
		static bool bFinalPointsAdded;
};

#endif