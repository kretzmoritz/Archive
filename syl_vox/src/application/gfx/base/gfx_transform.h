#ifndef GFX_TRANSFORM_H
#define GFX_TRANSFORM_H

#include "../base/gfx_modelview.h"

class GfxTransform
{
public:
	void translate(float _x, float _y, float _z);

private:
	GfxModelview m_modelview;
};

#endif