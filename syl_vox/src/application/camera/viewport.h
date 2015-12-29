#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <Windows.h>

#include "../../math/vector.h"
#include "../gfx/base/gfx_projection.h"
#include "../../window/creation/factory.h"

class Viewport
{
public:
	enum Type
	{
		FULL,
		BOTTOM,
		TOP,
		LEFT,
		RIGHT,
		BOTTOM_LEFT,
		BOTTOM_RIGHT,
		TOP_LEFT,
		TOP_RIGHT
	};

	Viewport(float _fov = 45.0f, float _zNear = 0.1f, float _zFar = 100.0f);

	void update(HWND _hWnd, Type _type = FULL);
	void set();

	void setFov(float _fov);
	void setNear(float _zNear);
	void setFar(float _zFar);

private:
	struct ViewportAlign
	{
		ViewportAlign() {}

		ViewportAlign(Vec2f _position, Vec2f _size)
			: position(_position), size(_size) {}

		Vec2f position;
		Vec2f size;
	};

	ViewportAlign m_current;
	static const ViewportAlign m_align[];

	float m_fov;
	float m_zNear;
	float m_zFar;

	GfxProjection m_projection;

	Factory m_factory;
};

#endif