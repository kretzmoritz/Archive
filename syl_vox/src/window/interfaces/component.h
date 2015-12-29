#ifndef COMPONENT_H
#define COMPONENT_H

#include <Windows.h>
#include <string>

class Component
{
public:
	virtual ~Component();

	virtual bool attach(HWND _hWnd) = 0;
	virtual void update(HWND _hWnd) = 0;
	virtual void release(HWND _hWnd) = 0;
	
	virtual unsigned int getSingularTypeId() const = 0;
	virtual std::string getName() const = 0;
};

#endif