#ifndef SV_EVENT_H
#define SV_EVENT_H

#include "../../window/interfaces/custom_event.h"
#include "../../window/creation/factory.h"

class SVEvent : public CustomEvent
{
protected:
	void COMMAND(HWND _hWnd, WPARAM _wParam);

private:
	Factory m_factory;
};

#endif