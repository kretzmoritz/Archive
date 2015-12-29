#include "gfx_vbo.h"

BufferObjects GfxVBO::request()
{
	BufferObjects bo;

	glGenBuffers(1, &bo.vbo);
	glGenBuffers(1, &bo.ibo);

	return bo;
}

void GfxVBO::release(BufferObjects _bo)
{
	if (!wglGetCurrentContext())
	{
		return;
	}

	glDeleteBuffers(1, &_bo.vbo);
	glDeleteBuffers(1, &_bo.ibo);
}