#include "gfx_light.h"

GfxLight::GfxLight(Vec3f _pos, Vec4f _diffuse, Vec4f _specular, Vec4f _ambient, GLenum _light)
	: m_position(_pos), m_diffuse(_diffuse), m_specular(_specular), m_ambient(_ambient), m_light(_light)
{

}

void GfxLight::setPosition(Vec3f _pos)
{
	m_position = _pos;
}

void GfxLight::setDiffuse(Vec4f _diffuse)
{
	m_diffuse = _diffuse;
}

void GfxLight::setSpecular(Vec4f _specular)
{
	m_specular = _specular;
}

void GfxLight::setAmbient(Vec4f _ambient)
{
	m_ambient = _ambient;
}

void GfxLight::setLight(GLenum _light)
{
	m_light = _light;
}

void GfxLight::enable()
{
	glEnable(m_light);
}

void GfxLight::disable()
{
	glDisable(m_light);
}

void GfxLight::update()
{
	float pos[4] = { m_position.x, m_position.y, m_position.z, 1.0f };
	
	glLightfv(m_light, GL_POSITION, pos);
	glLightfv(m_light, GL_DIFFUSE, &m_diffuse.x);
	glLightfv(m_light, GL_SPECULAR, &m_specular.x);
	glLightfv(m_light, GL_AMBIENT, &m_ambient.x);
}