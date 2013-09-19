////////////////////////////////////////////////////////////
// Moritz Kretz
// 07/14/2012
// System implementation of coloss (player) class
// State: Normal
////////////////////////////////////////////////////////////

#ifndef COLOSS_NORMAL
#define COLOSS_NORMAL

#include "CTimer.h"

#define TOP_COLOSS 0
#define MIDDLE_HIGHER_COLOSS 1
#define MIDDLE_LOWER_COLOSS 2
#define BOTTOM_COLOSS 3

#define C_ANIM_BUTTON_NORMAL 0

class CColossNormal : public CColoss
{
	struct floating_text
	{
		float fTime;
		sf::Text text;
	};

	public:
		CColossNormal();
		~CColossNormal() { delete ci_timer_top; delete ci_timer_middle_higher; delete ci_timer_middle_lower; delete ci_timer_bottom; }

		void start();
		void run();
		void close() { m_vFloatingText.clear(); }

		void getInput();
		bool hasBlockStarted(int _layer);
		void hasBlockFinished();
		void startBlockAnimation(float _fAttackEndX, float _fPressButtonX, float _fProjectileSpeed, int _layer);
		void drawScore();
		void calculateScore(float _distance, int _layer);

		void scaleButtons();
		void drawButtons();
		void tweenBlock();
		void checkForSalve(int _layer);
		void calculateBlockAngle();
		void manageBossInterface();

		void topResetCooldown();
		static void trcWrapper(const void *context);
		void middleHigherResetCooldown();
		static void mhrcWrapper(const void *context);
		void middleLowerResetCooldown();
		static void mlrcWrapper(const void *context);
		void bottomResetCooldown();
		static void brcWrapper(const void *context);

	private:
		bool m_bTop_KeyPressed;
		bool m_bMiddleHigher_KeyPressed;
		bool m_bMiddleLower_KeyPressed;
		bool m_bBottom_KeyPressed;
		
		CTimer *ci_timer_top;
		CTimer *ci_timer_middle_higher;
		CTimer *ci_timer_middle_lower;
		CTimer *ci_timer_bottom;

		pwCollisionRect m_collision;
		std::vector<pwCollisionRect> m_vLayerCollision;
		std::vector<pwCollisionRect> m_vBlockCollision;
		
		bool m_bTopBlock_active;
		bool m_bMiddleHigherBlock_active;
		bool m_bMiddleLowerBlock_active;
		bool m_bBottomBlock_active;

		std::vector<floating_text> m_vFloatingText;
		std::vector<pwAnimation> m_vButton;
		std::vector<sf::Text> m_vButtonText;
		
		int m_iPreviousFrame;

		bool m_bTopSalve;
		bool m_bMiddleHigherSalve;
		bool m_bMiddleLowerSalve;
		bool m_bBottomSalve;

		pwIniFile *data;

		sf::Sprite m_bossArrow_top;
		sf::Sprite m_bossArrow_middle_higher;
		sf::Sprite m_bossArrow_middle_lower;
		sf::Sprite m_bossArrow_bottom;
};

#endif