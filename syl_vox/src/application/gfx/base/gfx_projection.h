#ifndef GFX_PROJECTION_H
#define GFX_PROJECTION_H

#include "../../../math/matrix.h"

class GfxProjection
{
public:
	GfxProjection();
	void operator()(float _aspect, float _fov, float _zNear, float _zFar);

	const float* operator*() const;
	
private:
	Matf<4, 4>::CM m_projection;
};

#endif