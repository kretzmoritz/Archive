#include <string>

#include "syl_window.h"
#include "syl_window_desc.h"
#include "syl_window_procedure.h"

template<class T> unsigned short SylWindow<T>::m_id = 0;

template<class T>
SylWindow<T>::SylWindow(SylWindowDesc const& _desc)
	: WndProc(new T()), m_thread(&SylWindow::create, this, _desc)
{

}

template<class T>
SylWindow<T>::~SylWindow()
{
	m_thread.join();
	delete WndProc;
}

template<class T>
LRESULT CALLBACK SylWindow<T>::StaticWndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	SylWindow* wnd;

	if (_message == WM_NCCREATE)
	{
		wnd = reinterpret_cast<SylWindow*>(reinterpret_cast<LPCREATESTRUCT>(_lParam)->lpCreateParams);
		SetWindowLongPtr(_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wnd));
	}
	else
	{
		wnd = reinterpret_cast<SylWindow*>(GetWindowLongPtr(_hWnd, GWLP_USERDATA));
	}

	if (wnd)
	{
		return wnd->WndProc->dispatch(_hWnd, _message, _wParam, _lParam);
	}
	
	return DefWindowProc(_hWnd, _message, _wParam, _lParam);
}

template<class T>
void SylWindow<T>::create(SylWindowDesc const& _desc)
{
	std::string className = std::to_string(m_id);
	++m_id;

	WNDCLASSEX WndClsEx;

	WndClsEx.cbSize = sizeof(WNDCLASSEX);
	WndClsEx.style = _desc.style;
	WndClsEx.lpfnWndProc = StaticWndProc;
	WndClsEx.cbClsExtra = 0;
	WndClsEx.cbWndExtra = 0;
	WndClsEx.hInstance = _desc.hInstance;
	WndClsEx.hIcon = _desc.hIcon;
	WndClsEx.hCursor = _desc.hCursor;
	WndClsEx.hbrBackground = _desc.hbrBackground;
	WndClsEx.lpszMenuName = nullptr;
	WndClsEx.lpszClassName = className.c_str();
	WndClsEx.hIconSm = _desc.hIconSm;

	if (!RegisterClassEx(&WndClsEx))
	{
		return;
	}
	
	if (!(hWnd = CreateWindowEx(_desc.dwExStyle, className.c_str(), _desc.lpWindowName, _desc.dwStyle, _desc.x, _desc.y, _desc.nWidth, _desc.nHeight, nullptr, nullptr, _desc.hInstance, this)))
	{
		return;
	}

	ShowWindow(hWnd, _desc.nCmdShow);
	UpdateWindow(hWnd);

	SendMessage(hWnd, SWM_INIT, 0, 0);

	message_loop();
}

template<class T>
void SylWindow<T>::message_loop()
{
	MSG msg;
	bool quit = false;

	while (true)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				quit = true;
			}
		}

		if (quit)
		{
			break;
		}

		SendMessage(hWnd, SWM_UPDATE, 0, 0);
	}
}