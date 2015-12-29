#ifndef SV_MENU_H
#define SV_MENU_H

#include "../../window/interfaces/component.h"

class SVMenu : public Component
{
public:
	enum MenuItem
	{
		IDM_FULLSCREEN,
		IDM_EXIT
	};

	bool attach(HWND _hWnd);
	void update(HWND _hWnd);
	void release(HWND _hWnd);
	
	unsigned int getSingularTypeId() const;
	std::string getName() const;

private:
	HMENU hMenu;

	HMENU AddPopupMenu(HMENU _hMenu, std::string _name);
	void AddMenuItem(HMENU _hMenu, MenuItem _id, std::string _name);
};

#endif