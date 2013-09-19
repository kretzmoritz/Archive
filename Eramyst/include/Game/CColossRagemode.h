////////////////////////////////////////////////////////////
// Moritz Kretz
// 07/23/2012
// System implementation of coloss (player) class
// State: Ragemode
////////////////////////////////////////////////////////////

#ifndef COLOSS_RAGEMODE
#define COLOSS_RAGEMODE

#include "CTimer.h"
#include "CDisplayBar.h"

#define C_ANIM_BUTTON_NORMAL 0
#define C_ANIM_BUTTON_CLICK 1

#define C_ANIM_SHIELD_NORMAL 0

class CColossRagemode : public CColoss
{
	public:
		CColossRagemode();
		~CColossRagemode() { delete ci_timer_switch_key; }

		void start();
		void run();
		void close();

		void getInput();
		
		void enableCooldown(int _layer);
		void disableCooldown(int _layer);
		void applySpriteEffect();

		void drawSpamElements();
		void manageShockwave();

		void modifyShieldEffect();
		void checkShieldCollision();

		void applyShockwaveAttack();
		void chooseRandomButton();

	private:
		void switchCurrentKey();
		static void sckWrapper(const void *context);

		void checkCountIncrease();
		static void cciWrapper(const void *context);

		CTimer *ci_timer_switch_key;
		CTimer *ci_timer_check_count;

		bool m_bShieldKeyPressed;
		bool m_bShockwaveKeyPressed;
		
		int m_iSpamCount;
		int m_iStrengthening;
		std::vector<int> vNotAllowedKeys;

		float m_fAttack;
		float m_fShieldStrength;
		float m_fPreviousShieldStrength;
		
		float m_fScaleFactor;

		sf::Sprite m_sprite;
		std::vector<sf::Sprite> m_vEffectSprite;

		int m_iCurrentKey;
		std::vector<pwAnimation> m_vButton;
		std::vector<sf::Text> m_vButtonText;
		
		pwCollisionCircle m_shieldCollider;

		bool m_bShockwaveIconShown;
		pwAnimation m_ShockwaveIcon;
		pwCollisionCircle m_shockwaveCollider;
};

#endif