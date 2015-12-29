#ifndef GFX_LIGHT_H
#define GFX_LIGHT_H

#include <gl/glew.h>

#include "../../../math/vector.h"

class GfxLight
{
public:
	GfxLight(Vec3f _pos = Vec3f(0.0f, 0.0f, 0.0f), 
		Vec4f _diffuse = Vec4f(1.0f, 1.0f, 1.0f, 1.0f), 
		Vec4f _specular = Vec4f(0.0f, 0.0f, 0.0f, 1.0f), 
		Vec4f _ambient = Vec4f(0.0f, 0.0f, 0.0f, 1.0f), 
		GLenum _light = GL_LIGHT0);

	void setPosition(Vec3f _pos);
	void setDiffuse(Vec4f _diffuse);
	void setSpecular(Vec4f _specular);
	void setAmbient(Vec4f _ambient);
	void setLight(GLenum _light);

	void enable();
	void disable();

	void update();

private:
	Vec3f m_position;
	Vec4f m_diffuse;
	Vec4f m_specular;
	Vec4f m_ambient;

	GLenum m_light;
};

#endif