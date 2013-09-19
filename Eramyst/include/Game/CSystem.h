////////////////////////////////////////////////////////////
// Moritz Kretz
// 05/29/2012
// Updated: 07/15/2012
// System implementation of coloss (player) class
////////////////////////////////////////////////////////////

#ifndef SYSTEM
#define SYSTEM

#define PWUSEGLOBALSINGLETONS

#include "..\pwSystem.hpp"
#include "CEnemy.h"
#include "CLevelData.h"
#include "CBossData.h"

#define getEnemyVector() CEnemy::getEnemyVector()

#define WALK_ENEMY 0
#define IDLE_ENEMY 1
#define ATTACK_ENEMY 2
#define WALK_BACK_ENEMY 3

#define WALK_ENEMY_RAGEMODE 4
#define ATTACK_ENEMY_RAGEMODE 5
#define WALK_BACK_ENEMY_RAGEMODE 6

#define DEATH_ENEMY 7

#define MOVEMENT_ANIMATION_IDLE 0
#define MOVEMENT_ANIMATION_NORMAL 1

#define WEAPON_ANIMATION_IDLE 0
#define WEAPON_ANIMATION_NORMAL 1

class CSystem
{
	struct position
	{
		float x;
		float y;
	};

	class boss_piece : public pwGameObject
	{
		public:
			boss_piece()
			{
				m_iReturningProjectile = -1;

				m_boss_sprite = new sf::Sprite();
				AddDrawableObject(0, m_boss_sprite);
				
				m_boss_collision = new pwCollisionRect();
				AddCollisionObject(0, m_boss_collision);
				
				//SetRenderCollisionObjects(true);
			}

			~boss_piece()
			{
				delete m_boss_sprite;
				delete m_boss_collision;
			}

			int m_iLayer;
			int m_iReturningProjectile;
			sf::Sprite *m_boss_sprite;
			pwCollisionRect *m_boss_collision;
	};

	class movement_piece : public pwGameObject
	{
		public:
			movement_piece()
			{
				m_movement_animation = new pwAnimation();
				AddDrawableObject(0, m_movement_animation);
			}

			~movement_piece()
			{
				delete m_movement_animation;
			}

			int m_iLayer;
			pwAnimation *m_movement_animation;
	};

	class weapon_piece : public pwGameObject
	{
		public:
			weapon_piece()
			{
				m_iReturningProjectile = -1;

				m_weapon_animation = new pwAnimation();
				AddDrawableObject(0, m_weapon_animation);
				
				m_weapon_collision = new pwCollisionRect();
				AddCollisionObject(0, m_weapon_collision);
				
				//SetRenderCollisionObjects(true);
			}

			~weapon_piece()
			{
				delete m_weapon_animation;
				delete m_weapon_collision;
			}

			int m_iLayer;
			int m_iID;
			int m_iProjectile;
			int m_iReturningProjectile;
			position attack_origin;
			pwAnimation *m_weapon_animation;
			pwCollisionRect *m_weapon_collision;
	};

	class weapon_piece_destroyed : public pwGameObject
	{
		public:
			weapon_piece_destroyed()
			{
				m_bEnabled = false;

				m_weapon_destroyed_animation = new pwAnimation();
				AddDrawableObject(0, m_weapon_destroyed_animation);
			}

			~weapon_piece_destroyed()
			{
				delete m_weapon_destroyed_animation;
			}

			int m_iLayer;
			bool m_bEnabled;
			pwAnimation *m_weapon_destroyed_animation;
	};

	public:
		CSystem() {}
		~CSystem() { delete m_squad; }

		static void init();
		static void reset();

		virtual void start() =0;
		virtual void run();
		virtual void close() =0;

		static void setSpawnTime(float _value); // In seconds, don't touch, set by beat
		static void modSpawnTime(float _value);
		static float getSpawnTime();

		static void setSpawnTimeRagemode(float _value); // In seconds
		static void modSpawnTimeRagemode(float _value);
		static float getSpawnTimeRagemode();

		static void setMovementSpeed(float _value); // Don't touch, set by beat
		static void modMovementSpeed(float _value);
		static float getMovementSpeed();

		static void setRagemodeSpeedFactor(float _value);
		static void modRagemodeSpeedFactor(float _value);
		static float getRagemodeSpeedFactor();

		static void setAnimationSpeedFactor(float _value);
		static void modAnimationSpeedFactor(float _value);
		static float getAnimationSpeedFactor();

		static void setDistance(float _value);
		static void modDistance(float _value);
		static float getDistance();

		static void setNormalMode(bool _value) { m_bNormalMode = _value; }
		static bool getNormalMode() { return m_bNormalMode; }

		static void constructBoss();
		static void destructBoss();

		static float m_fPreviousSpawnTime;
		static float m_fPreviousSpawnTimeRagemode;

		CEnemy::enemy_squad *m_squad;
		
		static std::vector<boss_piece*> vBoss;
		static std::vector<movement_piece*> vMovement;
		static std::vector<weapon_piece*> vWeapon;
		static std::vector<weapon_piece_destroyed*> vWeaponDestroyed;

	private:
		static float m_fSpawnTime;
		static float m_fSpawnTimeRagemode;
		static float m_fMovementSpeed;
		static float m_fRagemodeSpeedFactor;
		static float m_fAnimationSpeedFactor;
		static float m_fDistance;

		static bool m_bNormalMode;

		static CBossData ci_boss_data;
};

#endif