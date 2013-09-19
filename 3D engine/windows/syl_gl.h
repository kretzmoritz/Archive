#ifndef _SYL_GL
#define _SYL_GL

#include <windows.h>
#include <stdio.h>

class sOpenGL
{
public:

    /* Function declaration */

    void bindContext(HWND hWnd, HDC & hDC, HGLRC hRC);
    void destroyContext(HWND hWnd, HDC hDC, HGLRC hRC);
};

#endif
