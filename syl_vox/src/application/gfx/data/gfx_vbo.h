#ifndef GFX_VBO_H
#define GFX_VBO_H

#include <gl/glew.h>
#include <gl/wglew.h>

struct BufferObjects
{
	GLuint vbo;
	GLuint ibo;
};

class GfxVBO
{
public:
	BufferObjects request();
	void release(BufferObjects _bo);
};

#endif