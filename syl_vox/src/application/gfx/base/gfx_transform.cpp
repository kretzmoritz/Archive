#include <gl/glew.h>

#include "gfx_transform.h"

void GfxTransform::translate(float _x, float _y, float _z)
{
	m_modelview.setTranslation(Vec3f(_x, _y, _z));
	glMultMatrixf(*m_modelview);
}