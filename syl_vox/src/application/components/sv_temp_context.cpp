#include "sv_temp_context.h"
#include "../gfx/base/gfx_init.h"

unsigned int SVTempContext::getSingularTypeId() const
{
	return 0;
}

std::string SVTempContext::getName() const
{
	return "temp_context_gl";
}

bool SVTempContext::create(HDC _hDC, HGLRC& _hRC)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(_hDC, &pfd);
	if(!nPixelFormat)
		return false;
	SetPixelFormat(_hDC, nPixelFormat, &pfd);

	_hRC = wglCreateContext(_hDC);
	if(!_hRC)
		return false;
	wglMakeCurrent(_hDC, _hRC);

	GfxInit gfxInit;
	return gfxInit.api();
}