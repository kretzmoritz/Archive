#ifndef _WINDOW
#define _WINDOW

#define IDM_FILE_OPEN_MODEL 1
#define IDM_FILE_OPEN_TEXTURE 2
#define IDM_FILE_OPEN_SHADER 3

#define IDM_OTHERSETTINGS_WIREFRAME 4
#define IDM_OTHERSETTINGS_DISABLE_TEXTURE 5
#define IDM_OTHERSETTINGS_DISABLE_SHADER 6

#include "glConnect.h"
#include "view.h"

#include <iostream>

class cWindow
{
    private:

    HINSTANCE hInstance;
    HWND hWnd;
    HDC hDC;
    HGLRC hRC;

    WNDCLASSEX WndClsEx;

    MSG msg;
    bool quit;

    public:

    cWindow();
    void create(int width, int height);
};

#endif
