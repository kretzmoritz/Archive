#ifndef GFX_MODELVIEW_H
#define GFX_MODELVIEW_H

#include "../../../math/matrix.h"
#include "../../../math/vector.h"

class GfxModelview
{
public:
	GfxModelview();

	void setTranslation(Vec3f _position);
	void modTranslation(Vec3f _direction);
	Vec3f getTranslation() const;

	void setRotation(Vec3f _angle);
	void setRotation(Vec3f _left, Vec3f _up, Vec3f _fwd);
	void modRotation(Vec3f _angle);
	Vec3f getLeft() const;
	Vec3f getUp() const;
	Vec3f getFwd() const;

	void setScale(Vec3f _scale);
	void modScale(Vec3f _scale);
	Vec3f getScale() const;

	const float* operator*();
	const float* inverse();

private:
	Matf<4, 4>::CM m_translate;
	Matf<4, 4>::CM m_rotate;
	Matf<4, 4>::CM m_scale;

	Matf<4, 4>::CM m_modelview;

	void anglesToAxes(Vec3f _angle, Matf<4, 4>::CM& _m);
};

#endif