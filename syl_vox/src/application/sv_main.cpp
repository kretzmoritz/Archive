#include "sv_main.h"

bool SVMain::attach(HWND _hWnd)
{
	m_inputMapping.registerCB(InputMapping::Action::MOVE_LEFT, &m_camera, &Camera::moveLeft);
	m_inputMapping.registerCB(InputMapping::Action::MOVE_RIGHT, &m_camera, &Camera::moveLeft);
	m_inputMapping.registerCB(InputMapping::Action::MOVE_FWD, &m_camera, &Camera::moveFwd);
	m_inputMapping.registerCB(InputMapping::Action::MOVE_BWD, &m_camera, &Camera::moveFwd);
	m_inputMapping.registerCB(InputMapping::Action::ROTATE_CAMERA, &m_schemeFunction, &SchemeFunction::rotateCamera);
	m_inputMapping.registerCB(InputMapping::Action::BOOST_CAMERA, &m_camera, &Camera::boost);
	m_inputMapping.registerCB(InputMapping::Action::DRAW_OCTREE, static_cast<ChunkManager*>(&m_chunkMgr), &ChunkManager::drawOctree);

	m_schemeFunction.setRawInputHandler(&m_rawInputHandler);
	m_schemeFunction.setCamera(&m_camera);

	m_viewport.setFar(500.0f);
	m_camera.setPosition(Vec3f(0.0f, 0.0f, 250.0f));

	m_light[0].enable();
	m_light[1].setLight(GL_LIGHT1);
	m_light[1].setPosition(Vec3f(50.0f, 200.0f, 50.0f));
	m_light[1].enable();
	
	return true;
}

void SVMain::update(HWND _hWnd)
{
	m_factory.getWindow(_hWnd)->makeCurrent();
	
	m_fps.refresh();

	m_rawInputHandler.refresh(_hWnd);
	m_inputMapping.dispatch("application.xml", &m_rawInputHandler);

	m_renderer.clear();
	m_renderer.textToScreen("FPS: ", Vec3f(1.0f, 1.0f, 0.0f), Vec2i(10, 25), m_factory.getWindow(_hWnd)->getRect());
	m_renderer.textToScreen(m_fps.getFps(), Vec3f(1.0f, 1.0f, 0.0f), Vec2i(55, 25), m_factory.getWindow(_hWnd)->getRect());

	m_viewport.update(_hWnd);
	m_viewport.set();

	m_camera.update(m_fps.getLastFrameTime());
	m_camera.set();

	m_light[0].update();
	m_light[1].update();

	m_chunkMgr.update(&m_renderer);
	
	m_camera.set();
	m_sky.render(&m_camera, &m_renderer);

	m_factory.getWindow(_hWnd)->swapBuffers();
}

void SVMain::release(HWND _hWnd)
{

}

unsigned int SVMain::getSingularTypeId() const
{
	return 2;
}

std::string SVMain::getName() const
{
	return "application";
}