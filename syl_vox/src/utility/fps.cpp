#include <Windows.h>

#include "fps.h"

Fps::Fps()
	: m_previousTickCount(GetTickCount()), m_elapsedTime(0), m_previousFps(0), m_fps(0)
{

}

void Fps::refresh()
{
	m_lastFrameTime = GetTickCount() - m_previousTickCount;
	m_previousTickCount = GetTickCount();

	m_elapsedTime += m_lastFrameTime;
	m_fps++;
	
	if(m_elapsedTime >= 1000)
	{
		m_previousFps = m_fps;
		m_elapsedTime = 0;
		m_fps = 0;
	}
}

float Fps::getLastFrameTime() const
{
	return m_lastFrameTime / 1000.0f;
}

unsigned int Fps::getFps() const
{
	return m_previousFps;
}