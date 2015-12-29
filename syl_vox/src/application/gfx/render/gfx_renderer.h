#ifndef GFX_RENDERER_H
#define GFX_RENDERER_H

#include <gl/glew.h>
#include <gl/freeglut.h>

#include <Windows.h>
#include <string>

#include "../../../math/vector.h"

class GfxRenderer
{
public:
	void clear();

	void textToScreen(std::string _text, Vec3f _color, Vec2i _pos, RECT _rect);
	void textToScreen(int _i, Vec3f _color, Vec2i _pos, RECT _rect);

	void boundingBox(Vec3f _position, Vec3f _delta);

	void vboToScreen(GLuint _vbo, GLuint _ibo, unsigned int _vertexArraySize, unsigned int _indexCount);
};

#endif