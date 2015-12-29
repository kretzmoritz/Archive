#include "app_window_procedure.h"

LRESULT AppWindowProcedure::dispatch(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	switch (_message)
	{
	case SWM_INIT:
		m_application.init();
		break;
	case SWM_UPDATE:
		m_application.update();
		break;
	case WM_DESTROY:
		m_application.release();
		PostQuitMessage(WM_QUIT);
		break;
	}

	return DefWindowProc(_hWnd, _message, _wParam, _lParam);
}