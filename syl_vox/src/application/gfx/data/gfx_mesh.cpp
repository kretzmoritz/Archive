#include "gfx_mesh.h"

GfxMesh::GfxMesh()
	: m_vertexArraySize(0), m_indexCount(0)
{

}

void GfxMesh::clear()
{
	m_vertex.clear();
	m_normal.clear();
	m_color.clear();
	m_index.clear();
}

void GfxMesh::addCube(float _size, Vec3f _pos, Vec4f _color, bool _front, bool _back, bool _right, bool _left, bool _top, bool _bottom)
{
	_size /= 2.0f;

	Vec3f p0(_pos.x - _size, _pos.y - _size, _pos.z + _size);
	Vec3f p1(_pos.x + _size, _pos.y - _size, _pos.z + _size);
	Vec3f p2(_pos.x + _size, _pos.y + _size, _pos.z + _size);
	Vec3f p3(_pos.x - _size, _pos.y + _size, _pos.z + _size);
	Vec3f p4(_pos.x + _size, _pos.y - _size, _pos.z - _size);
	Vec3f p5(_pos.x - _size, _pos.y - _size, _pos.z - _size);
	Vec3f p6(_pos.x - _size, _pos.y + _size, _pos.z - _size);
	Vec3f p7(_pos.x + _size, _pos.y + _size, _pos.z - _size);

	Vec3f n;

	unsigned int v0, v1, v2, v3, v4, v5, v6, v7;

	// Front
	if (_front)
	{
		n = Vec3f(0.0f, 0.0f, 1.0f);

		v0 = addVertex(p0, n, _color);
		v1 = addVertex(p1, n, _color);
		v2 = addVertex(p2, n, _color);
		v3 = addVertex(p3, n, _color);

		addTriangle(v0, v1, v2);
		addTriangle(v0, v2, v3);
	}

	// Back
	if (_back)
	{
		n = Vec3f(0.0f, 0.0f, -1.0f);

		v4 = addVertex(p4, n, _color);
		v5 = addVertex(p5, n, _color);
		v6 = addVertex(p6, n, _color);
		v7 = addVertex(p7, n, _color);

		addTriangle(v4, v5, v6);
		addTriangle(v4, v6, v7);
	}

	// Right
	if (_right)
	{
		n = Vec3f(1.0f, 0.0f, 0.0f);

		v1 = addVertex(p1, n, _color);
		v4 = addVertex(p4, n, _color);
		v7 = addVertex(p7, n, _color);
		v2 = addVertex(p2, n, _color);

		addTriangle(v1, v4, v7);
		addTriangle(v1, v7, v2);
	}

	// Left
	if (_left)
	{
		n = Vec3f(-1.0f, 0.0f, 0.0f);

		v5 = addVertex(p5, n, _color);
		v0 = addVertex(p0, n, _color);
		v3 = addVertex(p3, n, _color);
		v6 = addVertex(p6, n, _color);

		addTriangle(v5, v0, v3);
		addTriangle(v5, v3, v6);
	}

	// Top
	if (_top)
	{
		n = Vec3f(0.0f, 1.0f, 0.0f);

		v3 = addVertex(p3, n, _color);
		v2 = addVertex(p2, n, _color);
		v7 = addVertex(p7, n, _color);
		v6 = addVertex(p6, n, _color);

		addTriangle(v3, v2, v7);
		addTriangle(v3, v7, v6);
	}

	// Bottom
	if (_bottom)
	{
		n = Vec3f(0.0f, -1.0f, 0.0f);

		v5 = addVertex(p5, n, _color);
		v4 = addVertex(p4, n, _color);
		v1 = addVertex(p1, n, _color);
		v0 = addVertex(p0, n, _color);

		addTriangle(v5, v4, v1);
		addTriangle(v5, v1, v0);
	}
}

void GfxMesh::addCube(float _size, Vec3f _pos, CubeColor _color, bool _front, bool _back, bool _right, bool _left, bool _top, bool _bottom)
{
	_size /= 2.0f;

	Vec3f p0(_pos.x - _size, _pos.y - _size, _pos.z + _size);
	Vec3f p1(_pos.x + _size, _pos.y - _size, _pos.z + _size);
	Vec3f p2(_pos.x + _size, _pos.y + _size, _pos.z + _size);
	Vec3f p3(_pos.x - _size, _pos.y + _size, _pos.z + _size);
	Vec3f p4(_pos.x + _size, _pos.y - _size, _pos.z - _size);
	Vec3f p5(_pos.x - _size, _pos.y - _size, _pos.z - _size);
	Vec3f p6(_pos.x - _size, _pos.y + _size, _pos.z - _size);
	Vec3f p7(_pos.x + _size, _pos.y + _size, _pos.z - _size);

	Vec3f n;

	unsigned int v0, v1, v2, v3, v4, v5, v6, v7;

	// Front
	if (_front)
	{
		n = Vec3f(0.0f, 0.0f, 1.0f);

		v0 = addVertex(p0, n, _color.front_bottom_left);
		v1 = addVertex(p1, n, _color.front_bottom_right);
		v2 = addVertex(p2, n, _color.front_top_right);
		v3 = addVertex(p3, n, _color.front_top_left);

		addTriangle(v0, v1, v2);
		addTriangle(v0, v2, v3);
	}

	// Back
	if (_back)
	{
		n = Vec3f(0.0f, 0.0f, -1.0f);

		v4 = addVertex(p4, n, _color.back_bottom_right);
		v5 = addVertex(p5, n, _color.back_bottom_left);
		v6 = addVertex(p6, n, _color.back_top_left);
		v7 = addVertex(p7, n, _color.back_top_right);

		addTriangle(v4, v5, v6);
		addTriangle(v4, v6, v7);
	}

	// Right
	if (_right)
	{
		n = Vec3f(1.0f, 0.0f, 0.0f);

		v1 = addVertex(p1, n, _color.right_bottom_front);
		v4 = addVertex(p4, n, _color.right_bottom_back);
		v7 = addVertex(p7, n, _color.right_top_back);
		v2 = addVertex(p2, n, _color.right_top_front);

		addTriangle(v1, v4, v7);
		addTriangle(v1, v7, v2);
	}

	// Left
	if (_left)
	{
		n = Vec3f(-1.0f, 0.0f, 0.0f);

		v5 = addVertex(p5, n, _color.left_bottom_back);
		v0 = addVertex(p0, n, _color.left_bottom_front);
		v3 = addVertex(p3, n, _color.left_top_front);
		v6 = addVertex(p6, n, _color.left_top_back);

		addTriangle(v5, v0, v3);
		addTriangle(v5, v3, v6);
	}

	// Top
	if (_top)
	{
		n = Vec3f(0.0f, 1.0f, 0.0f);

		v3 = addVertex(p3, n, _color.top_front_left);
		v2 = addVertex(p2, n, _color.top_front_right);
		v7 = addVertex(p7, n, _color.top_back_right);
		v6 = addVertex(p6, n, _color.top_back_left);

		addTriangle(v3, v2, v7);
		addTriangle(v3, v7, v6);
	}

	// Bottom
	if (_bottom)
	{
		n = Vec3f(0.0f, -1.0f, 0.0f);

		v5 = addVertex(p5, n, _color.bottom_back_left);
		v4 = addVertex(p4, n, _color.bottom_back_right);
		v1 = addVertex(p1, n, _color.bottom_front_right);
		v0 = addVertex(p0, n, _color.bottom_front_left);

		addTriangle(v5, v4, v1);
		addTriangle(v5, v1, v0);
	}
}

void GfxMesh::updateVBO(GLuint _vbo, GLuint _ibo, GLenum _usage)
{
	if (m_vertex.empty())
	{
		return;
	}

	m_vertexArraySize = m_vertex.size() * sizeof(float);
	m_indexCount = m_index.size();

	unsigned int vertexArraySize2x = m_vertexArraySize * 2;
	unsigned int indexArraySize = m_indexCount * sizeof(unsigned int);
	unsigned int colorArraySize = m_color.size() * sizeof(float);
	
	GLint newBufferSize = vertexArraySize2x + colorArraySize;
	GLint oldBufferSize;

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &oldBufferSize);

	if (newBufferSize > oldBufferSize) //Buffers can only grow (never shrink) - memory requests are expensive
	{
		glBufferData(GL_ARRAY_BUFFER, newBufferSize, 0, _usage);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArraySize, 0, _usage);
	}

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertexArraySize, &m_vertex[0]);
	glBufferSubData(GL_ARRAY_BUFFER, m_vertexArraySize, m_vertexArraySize, &m_normal[0]);
	glBufferSubData(GL_ARRAY_BUFFER, vertexArraySize2x, colorArraySize, &m_color[0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indexArraySize, &m_index[0]);
}

unsigned int GfxMesh::getVertexArraySize() const
{
	return m_vertexArraySize;
}

unsigned int GfxMesh::getIndexCount() const
{
	return m_indexCount;
}

unsigned int GfxMesh::addVertex(Vec3f _v, Vec3f _n, Vec4f _c)
{
	unsigned int index = m_vertex.size() / 3;

	m_vertex.push_back(_v.x);
	m_vertex.push_back(_v.y);
	m_vertex.push_back(_v.z);

	m_normal.push_back(_n.x);
	m_normal.push_back(_n.y);
	m_normal.push_back(_n.z);

	m_color.push_back(_c.x);
	m_color.push_back(_c.y);
	m_color.push_back(_c.z);
	m_color.push_back(_c.w);
	
	return index;
}

void GfxMesh::addTriangle(unsigned int _v0, unsigned int _v1, unsigned int _v2)
{
	m_index.push_back(_v0);
	m_index.push_back(_v1);
	m_index.push_back(_v2);
}