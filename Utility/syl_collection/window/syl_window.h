#ifndef SYL_WINDOW_H
#define SYL_WINDOW_H

#include <Windows.h>
#include <thread>

class SylWindowDesc;
class SylWindowProcedure;

template<class T>
class SylWindow
{
public:
	SylWindow(SylWindowDesc const& _desc);
	~SylWindow();

private:
	static LRESULT CALLBACK StaticWndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);
	void create(SylWindowDesc const& _desc);
	void message_loop();

	SylWindowProcedure* WndProc;
	std::thread m_thread;
	static unsigned short m_id;
	HWND hWnd;
};

#include "syl_window.tcc"

#endif