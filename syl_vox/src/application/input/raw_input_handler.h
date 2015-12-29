#ifndef RAW_INPUT_HANDLER_H
#define RAW_INPUT_HANDLER_H

#include <Windows.h>

#include "../../math/vector.h"

#define KEY_COUNT 256
#define RANGE_COUNT 1

class RawInputHandler
{
public:
	RawInputHandler();
	
	void refresh(HWND _hWnd);

	bool isPressed(unsigned int _id) const;
	bool isDown(unsigned int _id) const;
	Vec2f getRange(unsigned int _id) const;
	Vec2f getPosition(unsigned int _id) const;

private:
	bool m_initialized;
	bool m_focus;

	bool m_pressed[KEY_COUNT];
	bool m_released[KEY_COUNT];
	bool m_down[KEY_COUNT];

	Vec2f m_range[RANGE_COUNT];
	Vec2f m_position[RANGE_COUNT];

	void init(HWND _hWnd);
};

#endif