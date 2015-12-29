#ifndef CONTEXT_H
#define CONTEXT_H

#include "component.h"

class Window;

class Context : public Component
{
public:
	virtual ~Context();

	bool attach(HWND _hWnd);
	void update(HWND _hWnd);
	void release(HWND _hWnd);

protected:
	virtual bool create(HDC _hDC, HGLRC& _hRC) = 0;

private:
	Window* m_window;
};

#endif