#ifndef _GLCONNECT
#define _GLCONNECT

#include <windows.h>

class cGlConnect
{
    private:

    int format;
    PIXELFORMATDESCRIPTOR pfd;

    public:

    void bind(HWND hWnd, HDC & hDC, HGLRC hRC);
    void destroy(HWND hWnd, HDC hDC, HGLRC hRC);
};

#endif
