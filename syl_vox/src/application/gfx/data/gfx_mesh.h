#ifndef GFX_MESH_H
#define GFX_MESH_H

#include <gl/glew.h>
#include <vector>

#include "../../../math/vector.h"
#include "../../../utility/cube_color.h"

class GfxMesh
{
public:
	GfxMesh();

	void clear();
	void addCube(float _size, Vec3f _pos, Vec4f _color, 
		bool _front = true, bool _back = true, bool _right = true, bool _left = true, bool _top = true, bool _bottom = true);
	void addCube(float _size, Vec3f _pos, CubeColor _color, 
		bool _front = true, bool _back = true, bool _right = true, bool _left = true, bool _top = true, bool _bottom = true);
	void updateVBO(GLuint _vbo, GLuint _ibo, GLenum _usage);

	unsigned int getVertexArraySize() const;
	unsigned int getIndexCount() const;

private:
	unsigned int m_vertexArraySize;
	unsigned int m_indexCount;

	std::vector<float> m_vertex;
	std::vector<float> m_normal;
	std::vector<float> m_color;
	std::vector<unsigned int> m_index;

	unsigned int addVertex(Vec3f _v, Vec3f _n, Vec4f _c);
	void addTriangle(unsigned int _v0, unsigned int _v1, unsigned int _v2);
};

#endif