#ifndef SV_MAIN_H
#define SV_MAIN_H

#include "../window/interfaces/component.h"
#include "../window/creation/factory.h"
#include "../utility/fps.h"
#include "input/raw_input_handler.h"
#include "input/input_mapping.h"
#include "input/scheme_function.h"
#include "camera/viewport.h"
#include "camera/camera.h"
#include "gfx/render/gfx_renderer.h"
#include "gfx/render/gfx_light.h"
#include "gfx/render/gfx_sky.h"
#include "sv_chunkmgr.h"

class SVMain : public Component
{
public:
	bool attach(HWND _hWnd);
	void update(HWND _hWnd);
	void release(HWND _hWnd);
	
	unsigned int getSingularTypeId() const;
	std::string getName() const;

private:
	Factory m_factory;
	Fps m_fps;
	RawInputHandler m_rawInputHandler;
	InputMapping m_inputMapping;
	SchemeFunction m_schemeFunction;
	Viewport m_viewport;
	Camera m_camera;
	GfxRenderer m_renderer;
	GfxLight m_light[2];
	GfxSky m_sky;
	SVChunkMgr m_chunkMgr;
};

#endif