#include "window.h"

Window::Window()
	: m_event(NULL)
{

}

Window::~Window()
{
	delete m_event;
	m_event = NULL;

	makeCurrent();

	for(auto it = m_components.rbegin(); it != m_components.rend(); ++it)
	{
		it->second->release(hWnd);
		delete it->second; //Free component memory
	}
	m_components.clear();

	ReleaseDC(hWnd, hDC);
}

std::string Window::getName() const
{
	return m_info.name;
}

bool Window::getDebug() const
{
	return m_info.debug;
}

int Window::getShow() const
{
	return m_info.show;
}

RECT Window::getRect() const
{
	RECT rect;
	GetClientRect(hWnd, &rect);

	return rect;
}

void Window::setShow(int _show)
{
	m_info.show = _show;
	ShowWindow(hWnd, _show);
}

void Window::setFullscreen()
{
	if(!m_info.fullscreen)
	{
		GetWindowRect(hWnd, &m_info.rect);
		m_info.type = GetWindowLong(hWnd, GWL_STYLE);

		MONITORINFO mi;
		mi.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &mi);

		setType(Info::WinType::POPUP);
		setPos(mi.rcMonitor.left, mi.rcMonitor.top);
		setSize(mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top);
	}
	else
	{
		setType(m_info.type);
		setPos(m_info.rect.left, m_info.rect.top);
		setSize(m_info.rect.right - m_info.rect.left, m_info.rect.bottom - m_info.rect.top);
	}

	m_info.fullscreen = !m_info.fullscreen;
}

void Window::setSize(unsigned int _width, unsigned int _height)
{
	SetWindowPos(hWnd, NULL, 0, 0, _width, _height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void Window::setPos(unsigned int _x, unsigned int _y)
{
	SetWindowPos(hWnd, NULL, _x, _y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void Window::setType(unsigned int _type)
{
	SetWindowLong(hWnd, GWL_STYLE, _type);
	ShowWindow(hWnd, m_info.show);
}

int Window::makeCurrent()
{
	return wglMakeCurrent(hDC, hRC);
}

void Window::swapBuffers()
{
	SwapBuffers(hDC);
}

void Window::destruct()
{
	DestroyWindow(hWnd);
}

void Window::unregisterCustomEvent()
{
	delete m_event;
	m_event = NULL;

	Log<Window> log;
	log.Out(LogType::LOG_MSG, m_info.name+" unregistered custom events");
}

CustomEvent* Window::getCustomEvent() const
{
	return m_event;
}