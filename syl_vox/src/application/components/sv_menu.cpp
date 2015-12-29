#include "sv_menu.h"

bool SVMenu::attach(HWND _hWnd)
{
	HMENU hMenu = CreateMenu();
	if(!hMenu)
		return false;
	
	HMENU hFile = AddPopupMenu(hMenu, "File");
	if(!hFile)
		return false;
	AddMenuItem(hFile, IDM_FULLSCREEN, "Fullscreen");
	AddMenuItem(hFile, IDM_EXIT, "Exit");

	SetMenu(_hWnd, hMenu);

	return true;
}

void SVMenu::update(HWND _hWnd)
{

}

void SVMenu::release(HWND _hWnd)
{
	SetMenu(_hWnd, NULL);
	DestroyMenu(hMenu);
}

unsigned int SVMenu::getSingularTypeId() const
{
	return 1;
}

std::string SVMenu::getName() const
{
	return "menu";
}

HMENU SVMenu::AddPopupMenu(HMENU _hMenu, std::string _name)
{
	HMENU hSubMenu = CreatePopupMenu();
	AppendMenu(_hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, _name.c_str());
	
	return hSubMenu;
}

void SVMenu::AddMenuItem(HMENU _hMenu, MenuItem _id, std::string _name)
{
	AppendMenu(_hMenu, MF_STRING, _id, _name.c_str());
}