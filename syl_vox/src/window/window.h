#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include <string>
#include <map>
#include <sstream>

#include "creation/info.h"
#include "interfaces/custom_event.h"
#include "interfaces/component.h"
#include "../utility/log.h"

class Window
{
	friend class Factory;
	friend class Context;

public:
	Window();
	~Window();
	
	std::string getName() const;
	bool getDebug() const;
	int getShow() const;
	RECT getRect() const;

	void setShow(int _show = Info::WinShow::SHOW);
	void setFullscreen();
	void setSize(unsigned int _width, unsigned int _height);
	void setPos(unsigned int _x, unsigned int _y);
	void setType(unsigned int _type);

	int makeCurrent();
	void swapBuffers();
	void destruct();

	template<class T> CustomEvent* registerCustomEvent();
	void unregisterCustomEvent();
	CustomEvent* getCustomEvent() const;

	template<class T> void registerComponent();
	template<class T> void unregisterComponent();

private:
	HWND hWnd;
	HDC hDC;
	HGLRC hRC;

	Info m_info;
	CustomEvent* m_event;
	std::map<unsigned int, Component*> m_components;
};

template<class T>
CustomEvent* Window::registerCustomEvent()
{
	delete m_event;
	m_event = new T();

	Log<Window> log;
	log.Out(LogType::LOG_MSG, m_info.name+" registered custom events");

	return m_event;
}

template<class T>
void Window::registerComponent()
{
	Log<Window> log;
	Component* comp = new T();

	if(m_components.find(comp->getSingularTypeId()) != m_components.end())
	{
		std::stringstream ss;
		ss << comp->getSingularTypeId();
		log.Out(LogType::LOG_ERR, m_info.name+" - component with SingularTypeId "+ss.str()+" already registered");
		
		delete comp;
		return;
	}

	if(comp->attach(hWnd))
	{
		log.Out(LogType::LOG_MSG, m_info.name+" registered component "+comp->getName());
		m_components.insert(std::make_pair(comp->getSingularTypeId(), comp));
	}
	else
	{
		log.Out(LogType::LOG_ERR, m_info.name+" - component "+comp->getName()+" failed to attach");
		
		comp->release(hWnd);
		delete comp;
	}
}

template<class T>
void Window::unregisterComponent()
{
	Log<Window> log;
	Component* comp = new T();

	auto it = m_components.find(comp->getSingularTypeId());
	if(it == m_components.end())
	{
		std::stringstream ss;
		ss << comp->getSingularTypeId();
		log.Out(LogType::LOG_ERR, m_info.name+" - component with SingularTypeId "+ss.str()+" not found");
		log.Out(LogType::LOG_ERR, "<unregister canceled>");
		
		delete comp;
		return;
	}

	if(comp->getName() != it->second->getName())
	{
		log.Out(LogType::LOG_ERR, m_info.name+" - component "+comp->getName()+" not found");
		log.Out(LogType::LOG_ERR, "<unregister canceled>");
		
		delete comp;
		return;
	}

	it->second->release(hWnd);
	delete it->second;
	m_components.erase(it);

	log.Out(LogType::LOG_MSG, m_info.name+" unregistered component "+comp->getName());
	
	delete comp;
}

#endif