#ifndef SYSTEM_H
#define SYSTEM_H

#include <Windows.h>
#include <string>
#include <map>

class Window;

class System
{
public:
	LRESULT translate(HWND _hWnd, UINT _uMsg, WPARAM _wParam, LPARAM _lParam, Window* _wnd, std::map<std::string, Window>& _windows);

private:
	void console_deallocation(std::map<std::string, Window>& _windows);
};

#endif