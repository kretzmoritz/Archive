#include "cube_color.h"

void CubeColor::setGradientTopBottom(Vec4f _tColor, Vec4f _bColor)
{
	front_top_left = _tColor;
	front_top_right = _tColor;
	front_bottom_left = _bColor;
	front_bottom_right = _bColor;

	back_top_left = _tColor;
	back_top_right = _tColor;
	back_bottom_left = _bColor;
	back_bottom_right = _bColor;

	left_top_front = _tColor;
	left_top_back = _tColor;
	left_bottom_front = _bColor;
	left_bottom_back = _bColor;

	right_top_front = _tColor;
	right_top_back = _tColor;
	right_bottom_front = _bColor;
	right_bottom_back = _bColor;

	top_front_left = _tColor;
	top_front_right = _tColor;
	top_back_left = _tColor;
	top_back_right = _tColor;

	bottom_front_left = _bColor;
	bottom_front_right = _bColor;
	bottom_back_left = _bColor;
	bottom_back_right = _bColor;
}