#ifndef CUSTOM_EVENT_H
#define CUSTOM_EVENT_H

#include <Windows.h>

class CustomEvent
{
	friend class System;

public:
	virtual ~CustomEvent();

protected:
	virtual void COMMAND(HWND _hWnd, WPARAM _wParam) = 0;
};

#endif