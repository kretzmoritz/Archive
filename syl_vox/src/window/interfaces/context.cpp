#include "context.h"
#include "../window.h"

Context::~Context()
{

}

bool Context::attach(HWND _hWnd)
{
	m_window = reinterpret_cast<Window*>(GetWindowLong(_hWnd, GWL_USERDATA));

	return create(m_window->hDC, m_window->hRC);
}

void Context::update(HWND _hWnd)
{

}

void Context::release(HWND _hWnd)
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(m_window->hRC);
	m_window->hRC = NULL;
}