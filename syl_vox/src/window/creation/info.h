#ifndef INFO_H
#define INFO_H

#include <Windows.h>
#include <string>

struct Info
{
public:
	enum WinShow
	{
		SHOW = SW_SHOW,
		HIDE = SW_HIDE
	};

	enum WinType
	{
		POPUP = WS_POPUP,
		OVERLAPPED_WINDOW = WS_OVERLAPPEDWINDOW
	};

	enum WinStyle
	{
		V_REDRAW = CS_VREDRAW,	//1 << 0
		H_REDRAW = CS_HREDRAW	//1 << 1
	};

	std::string name;
	bool debug;
	int show;
	bool fullscreen;
	RECT rect;
	unsigned int type;
	unsigned int style;

	Info(std::string _name = "wnd", bool _debug = false, int _show = SHOW, bool _fullscreen = false, unsigned int _width = 640, unsigned int _height = 480, 
		unsigned int _x = 0, unsigned int _y = 0, unsigned int _type = OVERLAPPED_WINDOW, unsigned int _style = V_REDRAW | H_REDRAW);
	void operator()(std::string _name = "wnd", bool _debug = false, int _show = SHOW, bool _fullscreen = false, unsigned int _width = 640, unsigned int _height = 480, 
		unsigned int _x = 0, unsigned int _y = 0, unsigned int _type = OVERLAPPED_WINDOW, unsigned int _style = V_REDRAW | H_REDRAW);

private:
	void init(std::string _name, bool _debug, int _show, bool _fullscreen, unsigned int _width, unsigned int _height, unsigned int _x, unsigned int _y, unsigned int _type, unsigned int _style);
};

#endif