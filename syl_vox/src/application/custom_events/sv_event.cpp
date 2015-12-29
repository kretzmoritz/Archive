#include "sv_event.h"
#include "../components/sv_menu.h"

void SVEvent::COMMAND(HWND _hWnd, WPARAM _wParam)
{
	switch(LOWORD(_wParam))
	{
	case SVMenu::MenuItem::IDM_FULLSCREEN:
		m_factory.getWindow(_hWnd)->setFullscreen();
		break;
	case SVMenu::MenuItem::IDM_EXIT:
		m_factory.getWindow(_hWnd)->destruct();
		break;
	}
}