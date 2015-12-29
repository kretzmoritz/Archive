#ifndef CAMERA_H
#define CAMERA_H

#include "../../math/vector.h"
#include "../gfx/base/gfx_modelview.h"

class Camera
{
public:
	Camera(Vec3f _position = Vec3f(0.0f, 0.0f, 0.0f), Vec3f _rotation = Vec3f(0.0f, 0.0f, 0.0f), float _speed = 17.5f);

	void update(float _lastFrameTime);
	void set();

	void moveLeft(float _direction);
	void moveUp(float _direction);
	void moveFwd(float _direction);
	void setPosition(Vec3f _position);
	Vec3f getPosition() const;

	void pitch(float _amount);
	void yaw(float _amount);
	void roll(float _amount);
	void setRotation(Vec3f _angle);
	void lookat(Vec3f _position);

	void setSpeed(float _speed);
	void boost(float _boost);

private:
	Vec3f m_translation;
	Vec3f m_rotation;

	float m_speed;
	float m_boost;

	GfxModelview m_modelview;
};

#endif