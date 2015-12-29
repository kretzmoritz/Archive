#include "gfx_sky.h"
#include "../../camera/camera.h"
#include "gfx_renderer.h"
#include "../../../utility/cube_color.h"

GfxSky::GfxSky(Vec3f _tColor, Vec3f _bColor)
{
	GfxVBO vbo;
	m_bo = vbo.request();

	setGradient(_tColor, _bColor);
}

GfxSky::~GfxSky()
{
	GfxVBO vbo;
	vbo.release(m_bo);
}

void GfxSky::setGradient(Vec3f _tColor, Vec3f _bColor)
{
	CubeColor color;
	color.setGradientTopBottom(Vec4f(_tColor.x, _tColor.y, _tColor.z, 1.0f), Vec4f(_bColor.x, _bColor.y, _bColor.z, 1.0f));

	m_mesh.addCube(1.0f, Vec3f(0.0f, 0.0f, 0.0f), color);
	m_mesh.updateVBO(m_bo.vbo, m_bo.ibo, GL_STATIC_DRAW);
	m_mesh.clear();
}

void GfxSky::render(Camera* _camera, GfxRenderer* _renderer)
{
	glPushMatrix();
		glPushAttrib(GL_LIGHTING_BIT | GL_POLYGON_BIT | GL_VIEWPORT_BIT);
			
			glDisable(GL_LIGHTING);
			
			glCullFace(GL_FRONT);
			glDepthRange(0.9999f, 1.0f);

			Vec3f position = _camera->getPosition();
			m_transform.translate(position.x, position.y, position.z);

			_renderer->vboToScreen(m_bo.vbo, m_bo.ibo, m_mesh.getVertexArraySize(), m_mesh.getIndexCount());

		glPopAttrib();
	glPopMatrix();
}