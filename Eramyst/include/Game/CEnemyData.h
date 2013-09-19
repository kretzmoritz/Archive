////////////////////////////////////////////////////////////
// Moritz Kretz
// 05/29/2012
// Updated: 07/20/2012
// System implementation of enemy data return class
////////////////////////////////////////////////////////////

#ifndef ENEMYDATA
#define ENEMYDATA

#define PWUSEGLOBALSINGLETONS

#include "../pwDataBox.hpp"
#include <string>
#include <vector>

#define TEXTURE_ENEMY 0
#define POSITION_ENEMY 1
#define SIZE_ENEMY 2
#define FRAMES_ENEMY 3
#define COLOR_ENEMY 4
#define ANIMATION_ENEMY 5
#define ANIMATION_SPEED_ENEMY 6

class CEnemyData
{
	typedef struct position
	{
		float x;
		float y;
	};
	
	typedef struct size
	{
		int width;
		int height;
	};

	typedef struct color
	{
		int r;
		int g;
		int b;
	};

	typedef struct animation
	{
		bool bPlayMoveOnce;
		int moveStart;
		int moveEnd;
		int attackStart;
		int attackEnd;
		int backpedalStart;
		int backpedalEnd;
		int woundedStart;
		int woundedEnd;
		int deathStart;
		int deathEnd;
	};

	public:
		CEnemyData() {};
		~CEnemyData() {};

		static void init();

		std::string getTexture(int _type) const;
		position getPosition(int _type) const;
		size getSize(int _type) const;
		int getFrames(int _type) const;
		color getFlashColor(int _type) const;
		animation getAnimation(int _type) const;
		float getAnimationSpeed(int _type) const;

	private:
		static pwIniFile *data;
		void* dataHandler(int _type, int _question) const;

		static std::vector<std::string> m_vTexture;
		static std::vector<position> m_vPosition;
		static std::vector<size> m_vSize;
		static std::vector<int> m_vFrames;
		static std::vector<color> m_vFlashColor;
		static std::vector<animation> m_vAnimation;
		static std::vector<float> m_vAnimationSpeed;
};

#endif