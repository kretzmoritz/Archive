#include "syl_gl.h"






void sOpenGL::bindContext(HWND hWnd, HDC & hDC, HGLRC hRC)
{
    PIXELFORMATDESCRIPTOR pixelInfo;
    int pixelSetting;

    hDC = GetDC(hWnd);

    /* Different graphic choices and settings - including single- or doubleBuffer */

    memset(&pixelInfo, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pixelInfo.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pixelInfo.nVersion = 1;
    pixelInfo.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pixelInfo.iPixelType = PFD_TYPE_RGBA;
    pixelInfo.cColorBits = 32;
    pixelInfo.cDepthBits = 24;
    pixelInfo.cStencilBits = 1;
    pixelInfo.iLayerType = PFD_MAIN_PLANE;
    pixelSetting = ChoosePixelFormat(hDC, &pixelInfo);
    SetPixelFormat(hDC, pixelSetting, &pixelInfo);

    /* Connects to the OpenGL context */

    hRC = wglCreateContext(hDC);

    /* Binds this context to our window */

    wglMakeCurrent(hDC, hRC);
}






void sOpenGL::destroyContext(HWND hWnd, HDC hDC, HGLRC hRC)
{

    /* Sets OpenGL to NULL and releases our device context */

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hWnd, hDC);
}
