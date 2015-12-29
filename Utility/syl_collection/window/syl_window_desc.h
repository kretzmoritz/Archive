#ifndef SYL_WINDOW_DESC_H
#define SYL_WINDOW_DESC_H

#include <Windows.h>

class SylWindowDesc
{
public:
	SylWindowDesc();

	UINT style;
	HICON hIcon;
	HCURSOR hCursor;
	HBRUSH hbrBackground;
	HICON hIconSm;

	DWORD dwExStyle;
	LPCTSTR lpWindowName;
	DWORD dwStyle;
	int x;
	int y;
	int nWidth;
	int nHeight;
	HINSTANCE hInstance;

	int nCmdShow;
};

#endif