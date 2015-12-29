#include <gl/glew.h>

#include "viewport.h"

const Viewport::ViewportAlign Viewport::m_align[] = {	Viewport::ViewportAlign(Vec2f(0.0f, 0.0f), Vec2f(1.0f, 1.0f)), 
														Viewport::ViewportAlign(Vec2f(0.0f, 0.0f), Vec2f(1.0f, 0.5f)), 
														Viewport::ViewportAlign(Vec2f(0.0f, 0.5f), Vec2f(1.0f, 0.5f)), 
														Viewport::ViewportAlign(Vec2f(0.0f, 0.0f), Vec2f(0.5f, 1.0f)), 
														Viewport::ViewportAlign(Vec2f(0.5f, 0.0f), Vec2f(0.5f, 1.0f)), 
														Viewport::ViewportAlign(Vec2f(0.0f, 0.0f), Vec2f(0.5f, 0.5f)), 
														Viewport::ViewportAlign(Vec2f(0.5f, 0.0f), Vec2f(0.5f, 0.5f)), 
														Viewport::ViewportAlign(Vec2f(0.0f, 0.5f), Vec2f(0.5f, 0.5f)), 
														Viewport::ViewportAlign(Vec2f(0.5f, 0.5f), Vec2f(0.5f, 0.5f))	};

Viewport::Viewport(float _fov, float _zNear, float _zFar)
	: m_fov(_fov), m_zNear(_zNear), m_zFar(_zFar)
{

}

void Viewport::update(HWND _hWnd, Type _type)
{
	RECT rect = m_factory.getWindow(_hWnd)->getRect();
	m_current.position.x = (rect.right - rect.left) * m_align[_type].position.x;
	m_current.position.y = (rect.bottom - rect.top) * m_align[_type].position.y;
	m_current.size.x = (rect.right - rect.left) * m_align[_type].size.x;
	m_current.size.y = (rect.bottom - rect.top) > 0 ? (rect.bottom - rect.top) * m_align[_type].size.y : 1.0f;

	m_projection(m_current.size.x / m_current.size.y, m_fov, m_zNear, m_zFar);
}

void Viewport::set()
{
	glViewport(GLint(m_current.position.x), GLint(m_current.position.y), GLint(m_current.size.x), GLint(m_current.size.y));

	glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(*m_projection);
}

void Viewport::setFov(float _fov)
{
	m_fov = _fov;
}

void Viewport::setNear(float _zNear)
{
	m_zNear = _zNear;
}

void Viewport::setFar(float _zFar)
{
	m_zFar = _zFar;
}