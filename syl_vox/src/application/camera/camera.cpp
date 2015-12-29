#include <gl/glew.h>

#include "camera.h"

Camera::Camera(Vec3f _position, Vec3f _rotation, float _speed)
	: m_rotation(_rotation), m_speed(_speed), m_boost(0.0f)
{
	m_modelview.setTranslation(_position);
	m_modelview.setRotation(_rotation);
}

void Camera::update(float _lastFrameTime)
{
	if(m_translation.length_sqr() > 0.0f)
	{
		m_modelview.modTranslation(m_translation.normalized() * (m_speed + m_boost) * _lastFrameTime);
		m_translation(0.0f, 0.0f, 0.0f);
	}

	m_modelview.setRotation(m_rotation);
	m_boost = 0.0f;
}

void Camera::set()
{
	//A matrix M which transforms from the space A to the space B has the basis vectors of space A, but expressed relative to space B.
	//The camera matrix transforms from world space to camera space. Thus, the basis vectors of the camera matrix are the basis vectors of world space, as seen from camera space. 
	//The orientation of the camera, relative to world space, is the inverse of this transformation.

	glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(m_modelview.inverse());
}

void Camera::moveLeft(float _direction)
{
	m_translation += (m_modelview.getLeft() * _direction).normalized();
}

void Camera::moveUp(float _direction)
{
	m_translation += (m_modelview.getUp() * _direction).normalized();
}

void Camera::moveFwd(float _direction)
{
	m_translation += (m_modelview.getFwd() * _direction).normalized();
}

void Camera::setPosition(Vec3f _position)
{
	m_modelview.setTranslation(_position);
}

Vec3f Camera::getPosition() const
{
	return m_modelview.getTranslation();
}

void Camera::pitch(float _amount)
{
	m_rotation.x += _amount;
}

void Camera::yaw(float _amount)
{
	m_rotation.y += _amount;
}

void Camera::roll(float _amount)
{
	m_rotation.z += _amount;
}

void Camera::setRotation(Vec3f _rotation)
{
	m_modelview.setRotation(_rotation);
}

void Camera::lookat(Vec3f _position)
{
	Vec3f fwd = -(_position - m_modelview.getTranslation()).normalized();
	Vec3f left = -(fwd.cross(Vec3f(0.0f, 1.0f, 0.0f)));
	Vec3f up = fwd.cross(left);

	m_modelview.setRotation(left, up, fwd);
}

void Camera::setSpeed(float _speed)
{
	m_speed = _speed;
}

void Camera::boost(float _boost)
{
	m_boost += _boost;
}