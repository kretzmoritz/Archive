#include <sstream>

#include "gfx_renderer.h"

void GfxRenderer::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void GfxRenderer::textToScreen(std::string _text, Vec3f _color, Vec2i _pos, RECT _rect)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
		
		gluOrtho2D(_rect.left, _rect.right, _rect.bottom, _rect.top);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
			glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT);
				
				glDisable(GL_LIGHTING);

				glColor3f(_color.x, _color.y, _color.z);
				glRasterPos2i(_pos.x, _pos.y);

				for (unsigned int i = 0; i < _text.size(); ++i)
				{
					glutBitmapCharacter(GLUT_BITMAP_9_BY_15, _text[i]);
				}

			glPopAttrib();
		glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void GfxRenderer::textToScreen(int _i, Vec3f _color, Vec2i _pos, RECT _rect)
{
	std::stringstream ss;
	ss << _i;

	textToScreen(ss.str(), _color, _pos, _rect);
}

void GfxRenderer::boundingBox(Vec3f _position, Vec3f _delta) //Super basic until I have the time to change it
{
	glPushAttrib(GL_LIGHTING_BIT);
		
		glDisable(GL_LIGHTING);

		glBegin(GL_LINES);
			glVertex3f(_position.x - _delta.x, _position.y - _delta.y, _position.z - _delta.z);
			glVertex3f(_position.x + _delta.x, _position.y - _delta.y, _position.z - _delta.z);

			glVertex3f(_position.x - _delta.x, _position.y + _delta.y, _position.z - _delta.z);
			glVertex3f(_position.x + _delta.x, _position.y + _delta.y, _position.z - _delta.z);

			glVertex3f(_position.x - _delta.x, _position.y - _delta.y, _position.z - _delta.z);
			glVertex3f(_position.x - _delta.x, _position.y + _delta.y, _position.z - _delta.z);

			glVertex3f(_position.x + _delta.x, _position.y - _delta.y, _position.z - _delta.z);
			glVertex3f(_position.x + _delta.x, _position.y + _delta.y, _position.z - _delta.z);

			glVertex3f(_position.x - _delta.x, _position.y - _delta.y, _position.z + _delta.z);
			glVertex3f(_position.x + _delta.x, _position.y - _delta.y, _position.z + _delta.z);

			glVertex3f(_position.x - _delta.x, _position.y + _delta.y, _position.z + _delta.z);
			glVertex3f(_position.x + _delta.x, _position.y + _delta.y, _position.z + _delta.z);

			glVertex3f(_position.x - _delta.x, _position.y - _delta.y, _position.z + _delta.z);
			glVertex3f(_position.x - _delta.x, _position.y + _delta.y, _position.z + _delta.z);

			glVertex3f(_position.x + _delta.x, _position.y - _delta.y, _position.z + _delta.z);
			glVertex3f(_position.x + _delta.x, _position.y + _delta.y, _position.z + _delta.z);

			glVertex3f(_position.x - _delta.x, _position.y - _delta.y, _position.z - _delta.z);
			glVertex3f(_position.x - _delta.x, _position.y - _delta.y, _position.z + _delta.z);

			glVertex3f(_position.x + _delta.x, _position.y - _delta.y, _position.z - _delta.z);
			glVertex3f(_position.x + _delta.x, _position.y - _delta.y, _position.z + _delta.z);

			glVertex3f(_position.x - _delta.x, _position.y + _delta.y, _position.z - _delta.z);
			glVertex3f(_position.x - _delta.x, _position.y + _delta.y, _position.z + _delta.z);

			glVertex3f(_position.x + _delta.x, _position.y + _delta.y, _position.z - _delta.z);
			glVertex3f(_position.x + _delta.x, _position.y + _delta.y, _position.z + _delta.z);
		glEnd();

	glPopAttrib();
}

void GfxRenderer::vboToScreen(GLuint _vbo, GLuint _ibo, unsigned int _vertexArraySize, unsigned int _indexCount)
{
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glNormalPointer(GL_FLOAT, 0, (void*)_vertexArraySize);
	glColorPointer(4, GL_FLOAT, 0, (void*)(_vertexArraySize * 2));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

	glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0);
}