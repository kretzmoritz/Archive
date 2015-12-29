#include "gfx_modelview.h"

GfxModelview::GfxModelview()
{
	MatHelper::make_identity(m_scale);
	MatHelper::make_identity(m_rotate);
	MatHelper::make_identity(m_translate);
}

void GfxModelview::setTranslation(Vec3f _position)
{
	m_translate(0, 3) = _position.x;
	m_translate(1, 3) = _position.y;
	m_translate(2, 3) = _position.z;
}

void GfxModelview::modTranslation(Vec3f _direction)
{
	m_translate(0, 3) += _direction.x;
	m_translate(1, 3) += _direction.y;
	m_translate(2, 3) += _direction.z;
}

Vec3f GfxModelview::getTranslation() const
{
	return Vec3f(m_translate(0, 3), m_translate(1, 3), m_translate(2, 3));
}

void GfxModelview::setRotation(Vec3f _angle)
{
	anglesToAxes(_angle, m_rotate);
}

void GfxModelview::setRotation(Vec3f _left, Vec3f _up, Vec3f _fwd)
{
	m_rotate(0, 0) = _left.x;
	m_rotate(1, 0) = _left.y;
	m_rotate(2, 0) = _left.z;

	m_rotate(0, 1) = _up.x;
	m_rotate(1, 1) = _up.y;
	m_rotate(2, 1) = _up.z;

	m_rotate(0, 2) = _fwd.x;
	m_rotate(1, 2) = _fwd.y;
	m_rotate(2, 2) = _fwd.z;
}

void GfxModelview::modRotation(Vec3f _angle)
{
	Matf<4, 4>::CM m;
	m(3, 3) = 1.0f;
	
	anglesToAxes(_angle, m);
	m_rotate = m_rotate * m;
}

Vec3f GfxModelview::getLeft() const
{
	return Vec3f(m_rotate(0, 0), m_rotate(1, 0), m_rotate(2, 0));
}

Vec3f GfxModelview::getUp() const
{
	return Vec3f(m_rotate(0, 1), m_rotate(1, 1), m_rotate(2, 1));
}

Vec3f GfxModelview::getFwd() const
{
	return Vec3f(m_rotate(0, 2), m_rotate(1, 2), m_rotate(2, 2));
}

void GfxModelview::setScale(Vec3f _scale)
{
	m_scale(0, 0) = _scale.x;
	m_scale(1, 1) = _scale.y;
	m_scale(2, 2) = _scale.z;
}

void GfxModelview::modScale(Vec3f _scale)
{
	m_scale(0, 0) += _scale.x;
	m_scale(1, 1) += _scale.y;
	m_scale(2, 2) += _scale.z;
}

Vec3f GfxModelview::getScale() const
{
	return Vec3f(m_scale(0, 0), m_scale(1, 1), m_scale(2, 2));
}

const float* GfxModelview::operator*()
{
	m_modelview = m_translate * m_rotate * m_scale;
	
	return *m_modelview;
}

const float* GfxModelview::inverse()
{
	//Translation = negate the translation components in the matrix
	Matf<4, 4>::CM translateInverse = m_translate;
	translateInverse(0, 3) *= -1.0f;
	translateInverse(1, 3) *= -1.0f;
	translateInverse(2, 3) *= -1.0f;

	//Scaling = invert (1/x) the first three diagonal elements of the matrix
	Matf<4, 4>::CM scaleInverse = m_scale;
	scaleInverse(0, 0) = 1.0f / scaleInverse(0, 0);
	scaleInverse(1, 1) = 1.0f / scaleInverse(1, 1);
	scaleInverse(2, 2) = 1.0f / scaleInverse(2, 2);
	
	//Rotation = take the transpose of the matrix
	m_modelview = scaleInverse * m_rotate.transposed() * translateInverse; //Reverse multiplication order

	return *m_modelview;
}

void GfxModelview::anglesToAxes(Vec3f _angle, Matf<4, 4>::CM& _m) //Roll(Z) -> Yaw(Y) -> Pitch(X)
{
	static const float DEG_TO_RADIAN = 0.01745329444444444444444444444444f;
	float sx, sy, sz, cx, cy, cz, theta;

	//Pitch angle
	theta = _angle.x * DEG_TO_RADIAN;
	sx = sinf(theta);
	cx = cosf(theta);

	//Yaw angle
	theta = _angle.y * DEG_TO_RADIAN;
	sy = sinf(theta);
	cy = cosf(theta);

	//Roll angle
	theta = _angle.z * DEG_TO_RADIAN;
	sz = sinf(theta);
	cz = cosf(theta);

	//Left axis
	_m(0, 0) = cz * cy;
	_m(1, 0) = sz * cy;
	_m(2, 0) = -sy;

	//Up axis
	_m(0, 1) = -sz * cx + cz * sy * sx;
	_m(1, 1) = cz * cx + sz * sy * sx;
	_m(2, 1) = cy * sx;

	//Forward axis
	_m(0, 2) = sz * sx + cz * sy * cx;
	_m(1, 2) = -cz * sx + sz * sy * cx;
	_m(2, 2) = cy * cx;
}