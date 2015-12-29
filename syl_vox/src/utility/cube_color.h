#ifndef CUBE_COLOR_H
#define CUBE_COLOR_H

#include "../math/vector.h"

struct CubeColor
{
	void setGradientTopBottom(Vec4f _tColor, Vec4f _bColor);

	Vec4f front_top_left;
	Vec4f front_top_right;
	Vec4f front_bottom_left;
	Vec4f front_bottom_right;

	Vec4f back_top_left;
	Vec4f back_top_right;
	Vec4f back_bottom_left;
	Vec4f back_bottom_right;

	Vec4f left_top_front;
	Vec4f left_top_back;
	Vec4f left_bottom_front;
	Vec4f left_bottom_back;

	Vec4f right_top_front;
	Vec4f right_top_back;
	Vec4f right_bottom_front;
	Vec4f right_bottom_back;

	Vec4f top_front_left;
	Vec4f top_front_right;
	Vec4f top_back_left;
	Vec4f top_back_right;

	Vec4f bottom_front_left;
	Vec4f bottom_front_right;
	Vec4f bottom_back_left;
	Vec4f bottom_back_right;
};

#endif