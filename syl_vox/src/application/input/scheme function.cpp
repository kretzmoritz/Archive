#include "scheme_function.h"
#include "raw_input_handler.h"
#include "../camera/camera.h"

SchemeFunction::SchemeFunction()
	: m_rawInputHandler(NULL), m_camera(NULL)
{

}

void SchemeFunction::setRawInputHandler(RawInputHandler* _rawInputHandler)
{
	m_rawInputHandler = _rawInputHandler;
}

void SchemeFunction::unsetRawInputHandler()
{
	m_rawInputHandler = NULL;
}

void SchemeFunction::setCamera(Camera* _camera)
{
	m_camera = _camera;
}

void SchemeFunction::unsetCamera()
{
	m_camera = NULL;
}

void SchemeFunction::rotateCamera(float _sensitivity)
{
	if(m_rawInputHandler && m_camera)
	{
		m_camera->yaw(m_rawInputHandler->getRange(0).x * _sensitivity);
		m_camera->pitch(m_rawInputHandler->getRange(0).y * _sensitivity);
	}
}