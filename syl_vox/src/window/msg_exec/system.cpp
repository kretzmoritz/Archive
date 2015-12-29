#include "system.h"
#include "../window.h"
#include "../interfaces/custom_event.h"
#include "../creation/info.h"
#include "../../utility/log.h"

LRESULT System::translate(HWND _hWnd, UINT _uMsg, WPARAM _wParam, LPARAM _lParam, Window* _wnd, std::map<std::string, Window>& _windows)
{
	switch(_uMsg)
    {
	case WM_COMMAND:
		if(_wnd->getCustomEvent())
			_wnd->getCustomEvent()->COMMAND(_hWnd, _wParam);
		break;
	case WM_SHOWWINDOW:
		if(_wnd->getDebug())
		{
			if(_wParam)
				ShowWindow(GetConsoleWindow(), Info::WinShow::SHOW);
			else
				console_deallocation(_windows);
		}
		break;
    case WM_DESTROY:
		{
		std::string name = _wnd->getName();
		bool debug = _wnd->getDebug();
		int show = _wnd->getShow();

		_windows.erase(name);

		if(debug && show)
			console_deallocation(_windows);

		Log<System> log;
		log.Out(LogType::LOG_MSG, "Window "+name+" successfully destructed");

		if(_windows.empty())
			PostQuitMessage(WM_QUIT);
		}
        break;
    default:
        return DefWindowProc(_hWnd, _uMsg, _wParam, _lParam);
    }

	return 0;
}

void System::console_deallocation(std::map<std::string, Window>& _windows)
{
	bool console = false;
	
	for(auto it = _windows.begin(); it != _windows.end(); ++it)
	{
		if(it->second.getDebug() && it->second.getShow())
		{
			console = true;
			break;
		}
	}

	if(!console)
		ShowWindow(GetConsoleWindow(), Info::WinShow::HIDE);
}