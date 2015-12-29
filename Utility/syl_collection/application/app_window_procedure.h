#ifndef APP_WINDOW_PROCEDURE_H
#define APP_WINDOW_PROCEDURE_H

#include "../window/syl_window_procedure.h"
#include "app_main.h"

class AppWindowProcedure : public SylWindowProcedure
{
public:
	LRESULT CALLBACK dispatch(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);

private:
	AppMain m_application;
};

#endif