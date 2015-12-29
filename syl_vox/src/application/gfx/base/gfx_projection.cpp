#include <math.h>

#include "gfx_projection.h"

#define PI_OVER_360 0.0087266f

GfxProjection::GfxProjection()
{
	m_projection(3, 2) = -1.0f;
}

void GfxProjection::operator()(float _aspect, float _fov, float _zNear, float _zFar)
{
	float yMax = _zNear * tanf(_fov * PI_OVER_360);
	float yMin = -yMax;
	float xMax = yMax * _aspect;
	float xMin = yMin * _aspect;

	float width = xMax - xMin;
	float height = yMax - yMin;

	float depth = _zFar - _zNear;

	m_projection(0, 0) = 2.0f * _zNear / width;
	m_projection(1, 1) = 2.0f * _zNear / height;
	m_projection(2, 2) = -(_zFar + _zNear) / depth;
	m_projection(2, 3) = -2.0f * (_zFar * _zNear) / depth;
}

const float* GfxProjection::operator*() const
{
	return *m_projection;
}