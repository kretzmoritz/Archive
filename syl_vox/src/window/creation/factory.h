#ifndef FACTORY_H
#define FACTORY_H

#include <Windows.h>
#include <string>
#include <map>

#include "../window.h"
#include "../msg_exec/system.h"
#include "../../utility/log.h"

struct Info;

class Factory
{
public:
	void construct(const Info& _info);
	template<class T> void construct(const Info& _info);
	void run();

	Window* getWindow(HWND _hWnd) const;
	Window* getWindow(std::string _name) const;

private:
	static std::map<std::string, Window> windows; //Removal or insertion of objects will NOT invalidate pointers to existing elements
	static System system;
	static bool STATIC_INIT_DONE;

	static LRESULT CALLBACK WndProcedure(HWND _hWnd, UINT _uMsg, WPARAM _wParam, LPARAM _lParam);
	
	bool duplicate(const Info& _info);
	void construct_internal(Window* _wnd);
};

template<class T>
void Factory::construct(const Info& _info)
{
	if(duplicate(_info))
		return;

	auto it = windows.insert(std::make_pair(_info.name, Window()));
	it.first->second.m_info = _info;
	
	Log<Factory> log;
	log.Out(LogType::LOG_MSG, "Window "+_info.name+" successfully constructed");
	
	it.first->second.registerCustomEvent<T>();

	construct_internal(&it.first->second);
}

#endif