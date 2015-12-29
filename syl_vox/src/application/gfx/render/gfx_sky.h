#ifndef GFX_SKY_H
#define GFX_SKY_H

#include "../base/gfx_transform.h"
#include "../data/gfx_mesh.h"
#include "../data/gfx_vbo.h"
#include "../../../math/vector.h"

class Camera;
class GfxRenderer;

class GfxSky
{
public:
	GfxSky(Vec3f _tColor = Vec3f(0.098f, 0.098f, 0.439f), Vec3f _bColor = Vec3f(0.792f, 1.0f, 1.0f));
	~GfxSky();

	void setGradient(Vec3f _tColor, Vec3f _bColor);
	void render(Camera* _camera, GfxRenderer* _renderer);

private:
	GfxTransform m_transform;
	GfxMesh m_mesh;
	BufferObjects m_bo;
};

#endif