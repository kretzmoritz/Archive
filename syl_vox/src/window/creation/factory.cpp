#include "factory.h"
#include "info.h"

extern "C" IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase) //http://blogs.msdn.com/b/oldnewthing/archive/2004/10/25/247180.aspx; GetModuleHandle(NULL) only returns start adress of process

std::map<std::string, Window> Factory::windows;
System Factory::system;
bool Factory::STATIC_INIT_DONE = false;

LRESULT CALLBACK Factory::WndProcedure(HWND _hWnd, UINT _uMsg, WPARAM _wParam, LPARAM _lParam)
{
	Window* wnd;
	if(_uMsg == WM_NCCREATE)
	{
		wnd = reinterpret_cast<Window*>(LPCREATESTRUCT(_lParam)->lpCreateParams);
		SetWindowLong(_hWnd, GWL_USERDATA, reinterpret_cast<long>(wnd));
		
		wnd->hWnd = _hWnd;
		wnd->hDC = GetDC(_hWnd);
		wnd->hRC = NULL;
	}
	else
		wnd = reinterpret_cast<Window*>(GetWindowLong(_hWnd, GWL_USERDATA));
	
	return system.translate(_hWnd, _uMsg, _wParam, _lParam, wnd, windows);
}

bool Factory::duplicate(const Info& _info)
{
	if(windows.find(_info.name) != windows.end())
	{
		Log<Factory> log;
		log.Out(LogType::LOG_ERR, "Window "+_info.name+" already constructed");

		return true;
	}

	return false;
}

void Factory::construct_internal(Window* _wnd)
{
	if(!STATIC_INIT_DONE)
	{
		ShowWindow(GetConsoleWindow(), SW_HIDE);
		STATIC_INIT_DONE = true;
	}

	HWND       hWnd;
	WNDCLASSEX WndClsEx;

	WndClsEx.cbSize        = sizeof(WNDCLASSEX);
	WndClsEx.style         = _wnd->m_info.style;
	WndClsEx.lpfnWndProc   = WndProcedure;
	WndClsEx.cbClsExtra    = 0;
	WndClsEx.cbWndExtra    = 0;
	WndClsEx.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	WndClsEx.hCursor       = LoadCursor(NULL, IDC_ARROW);
	WndClsEx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClsEx.lpszMenuName  = NULL;
	WndClsEx.lpszClassName = _wnd->m_info.name.c_str();
	WndClsEx.hInstance     = HINST_THISCOMPONENT;
	WndClsEx.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClsEx);

	hWnd = CreateWindow(_wnd->m_info.name.c_str(),
						_wnd->m_info.name.c_str(),
						_wnd->m_info.fullscreen ? Info::WinType::POPUP : _wnd->m_info.type,
						_wnd->m_info.fullscreen ? 0 : _wnd->m_info.rect.left,
						_wnd->m_info.fullscreen ? 0 : _wnd->m_info.rect.top,
						_wnd->m_info.fullscreen ? GetSystemMetrics(SM_CXSCREEN) : _wnd->m_info.rect.right,
						_wnd->m_info.fullscreen ? GetSystemMetrics(SM_CYSCREEN) : _wnd->m_info.rect.bottom,
						NULL,
						NULL,
						HINST_THISCOMPONENT,
						reinterpret_cast<LPVOID>(_wnd));

	ShowWindow(hWnd, _wnd->m_info.show);
	UpdateWindow(hWnd);
}

void Factory::construct(const Info& _info)
{
	if(duplicate(_info))
		return;

	auto it = windows.insert(std::make_pair(_info.name, Window()));
	it.first->second.m_info = _info;
	
	Log<Factory> log;
	log.Out(LogType::LOG_MSG, "Window "+_info.name+" successfully constructed");

	construct_internal(&it.first->second);
}

void Factory::run()
{
	MSG Msg;
	bool bBreak = false;

	while(PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) //Clear message queue
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	while(true)
	{
		while(PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);

			if(Msg.message == WM_QUIT)
			{
				bBreak = true;
				break;
			}
		}
		
		if(bBreak)
			break;
		
		//Update all registered window components
		for(auto w_it = windows.begin(); w_it != windows.end(); ++w_it)
		{
			for(auto c_it = w_it->second.m_components.begin(); c_it != w_it->second.m_components.end(); ++c_it) //Sorted by SingularTypeId (0 to x)
			{
				c_it->second->update(w_it->second.hWnd);
			}
		}
	}
}

Window* Factory::getWindow(HWND _hWnd) const
{
	return reinterpret_cast<Window*>(GetWindowLong(_hWnd, GWL_USERDATA));
}

Window* Factory::getWindow(std::string _name) const
{
	auto it = windows.find(_name);
	
	if(it != windows.end())
		return &it->second;
	else
		return NULL;
}