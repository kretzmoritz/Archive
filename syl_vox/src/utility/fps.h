#ifndef FPS_H
#define FPS_H

class Fps
{
public:
	Fps();
	void refresh();

	float getLastFrameTime() const;
	unsigned int getFps() const;

private:
	unsigned int m_previousTickCount;
	unsigned int m_lastFrameTime;
	unsigned int m_elapsedTime;
	unsigned int m_previousFps;
	unsigned int m_fps;
};

#endif