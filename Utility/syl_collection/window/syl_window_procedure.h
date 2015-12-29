#ifndef SYL_WINDOW_PROCEDURE_H
#define SYL_WINDOW_PROCEDURE_H

#include <Windows.h>

#define SWM_INIT (WM_USER + 0)
#define SWM_UPDATE (WM_USER + 1)

class SylWindowProcedure
{
public:
	virtual ~SylWindowProcedure() = default;

	virtual LRESULT CALLBACK dispatch(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) = 0;
};

#endif