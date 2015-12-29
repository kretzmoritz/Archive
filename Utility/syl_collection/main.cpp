#include <Windows.h>

#include "window/syl_window_desc.h"
#include "application/app_window_procedure.h"
#include "window/syl_window.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SylWindowDesc desc;
	desc.hInstance = hInstance;
	desc.nCmdShow = nCmdShow;

	SylWindow<AppWindowProcedure> wnd(desc);

	desc.lpWindowName = "window2";

	SylWindow<AppWindowProcedure> wnd2(desc);

	return 0;
}