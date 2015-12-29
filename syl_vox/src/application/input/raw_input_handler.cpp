#include "raw_input_handler.h"

RawInputHandler::RawInputHandler()
	: m_initialized(false), m_focus(false)
{

}

void RawInputHandler::refresh(HWND _hWnd)
{
	//Init - needs hWnd
	if(!m_initialized)
	{
		init(_hWnd);
		m_initialized = true;
	}

	//Check for focus, else reset values
	if(GetFocus() != _hWnd)
	{
		if(m_focus)
		{
			init(_hWnd);
			m_focus = false;
		}

		return;
	}

	//Pressing keys is not continuous - wait for up event
	memset(m_pressed, 0, KEY_COUNT);

	//Figure out key states
	for(int i = 0; i < KEY_COUNT; ++i)
	{
		if(HIWORD(GetKeyState(i)))
		{
			m_down[i] = true;

			if(m_released[i])
			{
				m_pressed[i] = true;
				m_released[i] = false;
			}
		}
		else
		{
			m_down[i] = false;
			m_released[i] = true;
		}
	}

	//Mouse movement
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(_hWnd, &point);

	if(m_focus)
	{
		m_range[0].x = m_position[0].x - point.x;
		m_range[0].y = m_position[0].y - point.y;
	}

	m_position[0](float(point.x), float(point.y));

	//Enable window focus
	if(!m_focus)
		m_focus = true;
}

bool RawInputHandler::isPressed(unsigned int _id) const
{
	if(_id < 0 || _id >= KEY_COUNT)
		return false;

	return m_pressed[_id];
}

bool RawInputHandler::isDown(unsigned int _id) const
{
	if(_id < 0 || _id >= KEY_COUNT)
		return false;

	return m_down[_id];
}

Vec2f RawInputHandler::getRange(unsigned int _id) const
{
	if(_id < 0 || _id >= RANGE_COUNT)
		return Vec2f(0.0f, 0.0f);

	return m_range[_id];
}

Vec2f RawInputHandler::getPosition(unsigned int _id) const
{
	if(_id < 0 || _id >= RANGE_COUNT)
		return Vec2f(0.0f, 0.0f);

	return m_position[_id];
}

void RawInputHandler::init(HWND _hWnd)
{
	memset(m_pressed, 0, KEY_COUNT);
	memset(m_released, 1, KEY_COUNT);
	memset(m_down, 0, KEY_COUNT);

	for(int i = 0; i < RANGE_COUNT; ++i)
	{
		m_range[i](0.0f, 0.0f);
		m_position[i](0.0f, 0.0f);
	}
}