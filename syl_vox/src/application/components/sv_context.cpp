#include <gl/glew.h>
#include <gl/wglew.h>

#include "sv_context.h"
#include "../gfx/base/gfx_init.h"
#include "../../utility/log.h"

unsigned int SVContext::getSingularTypeId() const
{
	return 0;
}

std::string SVContext::getName() const
{
	return "context_gl";
}

bool SVContext::create(HDC _hDC, HGLRC& _hRC)
{
	GfxInit gfxInit;
	if(!gfxInit.api_init_done())
		return false;

	const int attribListP[] =
	{
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		WGL_SAMPLE_BUFFERS_ARB, 1,
		WGL_SAMPLES_ARB, 8, //8x MSAA
		0
	};

	int nPixelFormat;
	UINT nNumFormats;

	if(!wglChoosePixelFormatARB(_hDC, attribListP, NULL, 1, &nPixelFormat, &nNumFormats))
		return false;
	SetPixelFormat(_hDC, nPixelFormat, NULL);

	const int attribListC[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
		0
	};

	_hRC = wglCreateContextAttribsARB(_hDC, NULL, attribListC);
	if(!_hRC)
	{
		_hRC = wglCreateContext(_hDC);
		if(!_hRC)
			return false;
	}
	wglMakeCurrent(_hDC, _hRC);

	Log<SVContext> log;
	log.Out(LogType::LOG_MSG, "Current OpenGL version: "+std::string((const char*)glGetString(GL_VERSION)));

	gfxInit.glut();
	gfxInit.settings();

	return true;
}