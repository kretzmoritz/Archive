#include "info.h"

Info::Info(std::string _name, bool _debug, int _show, bool _fullscreen, unsigned int _width, unsigned int _height, unsigned int _x, unsigned int _y, unsigned int _type, unsigned int _style)
{
	init(_name, _debug, _show, _fullscreen, _width, _height, _x, _y, _type, _style);
}

void Info::operator()(std::string _name, bool _debug, int _show, bool _fullscreen, unsigned int _width, unsigned int _height, unsigned int _x, unsigned int _y, unsigned int _type, unsigned int _style)
{
	init(_name, _debug, _show, _fullscreen, _width, _height, _x, _y, _type, _style);
}

void Info::init(std::string _name, bool _debug, int _show, bool _fullscreen, unsigned int _width, unsigned int _height, unsigned int _x, unsigned int _y, unsigned int _type, unsigned int _style)
{
	name = _name;
	debug = _debug;
	show = _show;
	fullscreen = _fullscreen;
	rect.right = _width;
	rect.bottom = _height;
	rect.left = _x;
	rect.top = _y;
	type = _type;
	style = _style;
}