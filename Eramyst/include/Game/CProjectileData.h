////////////////////////////////////////////////////////////
// Moritz Kretz
// 07/14/2012
// Updated: 07/20/2012
// System implementation of projectile data return class
////////////////////////////////////////////////////////////

#ifndef PROJECTILEDATA
#define PROJECTILEDATA

#define PWUSEGLOBALSINGLETONS

#include "../pwDataBox.hpp"
#include <string>
#include <vector>

#define TEXTURE_PROJECTILE 0
#define SOUND_PROJECTILE 1
#define TARGET_PROJECTILE 2
#define DAMAGE_PROJECTILE 3
#define BOUNCE_CHANCE_PROJECTILE 4
#define SIZE_PROJECTILE 5
#define FRAMES_PROJECTILE 6
#define ANIMATION_PROJECTILE 7
#define ANIMATION_SPEED_PROJECTILE 8
#define GRAVITY_PROJECTILE 9
#define ROTATION_PROJECTILE 10
#define ORIGIN_PROJECTILE 11

class CProjectileData
{
	typedef struct size
	{
		int width;
		int height;
	};

	typedef struct animation
	{
		int normalStart;
		int normalEnd;
		int deathStart;
		int deathEnd;
	};

	typedef struct position
	{
		float x;
		float y;
	};

	public:
		CProjectileData() {};
		~CProjectileData() {};

		static void init();

		std::string getTexture(int _type) const;
		std::string getSound(int _type) const;
		int getTarget(int _type) const;
		float getDamage(int _type) const;
		float getBounceChance(int _type) const;
		size getSize(int _type) const;
		int getFrames(int _type) const;
		animation getAnimation(int _type) const;
		float getAnimationSpeed(int _type) const;
		float getGravity(int _type) const;
		float getRotation(int _type) const;
		position getOrigin(int _type) const;

	private:
		static pwIniFile *data;
		void* dataHandler(int _type, int _question) const;

		static std::vector<std::string> m_vTexture;
		static std::vector<std::string> m_vSound;
		static std::vector<int> m_vTarget;
		static std::vector<float> m_vDamage;
		static std::vector<float> m_vBounceChance;
		static std::vector<size> m_vSize;
		static std::vector<int> m_vFrames;
		static std::vector<animation> m_vAnimation;
		static std::vector<float> m_vAnimationSpeed;
		static std::vector<float> m_vGravity;
		static std::vector<float> m_vRotation;
		static std::vector<position> m_vOrigin;
};

#endif