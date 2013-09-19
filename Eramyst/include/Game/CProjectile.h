////////////////////////////////////////////////////////////
// Moritz Kretz
// 06/25/2012
// Updated: 07/14/2012
// System implementation of projectile class
////////////////////////////////////////////////////////////

#ifndef PROJECTILE
#define PROJECTILE

#define PWUSEGLOBALSINGLETONS
#define PI_PROJECTILE 3.14159265358979323846f

#include "..\pwSystem.hpp"
#include "ID.hpp"
#include "CProjectileData.h"

#define P_ANIM_NORMAL 1
#define P_ANIM_DEATH 2

class CProjectile : public pwGameObject
{
	typedef struct point
	{
		float x;
		float y;
	};

	typedef struct velocity
	{
		float xVel;
		float yVel;
	};

	public:
		CProjectile() { m_bBounced = false; m_bDead = false; m_iBlocked = -1; m_bShieldIgnore = false; }
		~CProjectile() { delete m_collision; delete ci_projectile_data; }

		void init(float _x, float _y, int _parentID, bool _salve = false);
		void bounceOrDestroy();
		void initiateDestruction();
		
		pwCollisionRect* getCollisionObject() const;
		
		float getDamage() const { return ci_projectile_data->getDamage(m_iParent); }
		bool getSalve() const { return m_bSalve; }
		bool getDead() const { return m_bDead; }
		int getTarget() const { return m_iTarget; }
		int getParent() const { return m_iParent; }

		void setBounced(bool _value);
		bool getBounced() const;

		void setBlocked(int _isBlockedByLayer) { m_iBlocked = _isBlockedByLayer; }
		int getBlocked() const { return m_iBlocked; }
		
		void setShieldIgnore(bool _value) { m_bShieldIgnore = _value; }
		bool getShieldIgnore() const { return m_bShieldIgnore; }

		void setBlockDistance(float _value) { m_fBlockDistance = _value; }
		float getBlockDistance() const { return m_fBlockDistance; }

		static void setFlightTime(float _value);
		static void modFlightTime(float _value);
		static float getFlightTime();

		static void update();
		static std::vector<CProjectile*> &getProjectileVector();

	private:
		velocity calculateForce(int _parentID);
		void rotateToCurrentDirection();
		void destroy();
		static void deleteDeadObjects();

		pwAnimation m_visual;
		pwCollisionRect *m_collision;

		float m_fStartX;
		float m_fStartY;

		int m_iParent;
		bool m_bBounced;
		bool m_bDead;
		int m_iBlocked;
		static float m_fTime;
		bool m_bSalve;
		bool m_bShieldIgnore;
		int m_iTarget;

		float m_fBlockDistance;

		CProjectileData *ci_projectile_data;

		static std::vector<CProjectile*> m_vProjectiles;
};

#endif