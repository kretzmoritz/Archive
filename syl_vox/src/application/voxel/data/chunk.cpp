#include "chunk.h"
#include "../../gfx/render/gfx_renderer.h"

Chunk::Chunk(uint _chunkSize)
	: m_chunkSize(_chunkSize), m_shouldRender(false), m_isRebuilding(false)
{
	GfxVBO vbo;
	m_bo = vbo.request();

	m_blocks = new Block**[m_chunkSize];

	for (uint i = 0; i < m_chunkSize; ++i)
	{
		m_blocks[i] = new Block*[m_chunkSize];

		for (uint j = 0; j < m_chunkSize; ++j)
		{
			m_blocks[i][j] = new Block[m_chunkSize];
		}
	}
}

Chunk::~Chunk()
{
	GfxVBO vbo;
	vbo.release(m_bo);

	for (uint i = 0; i < m_chunkSize; ++i)
	{
		for (uint j = 0; j < m_chunkSize; ++j)
		{
			delete [] m_blocks[i][j];
		}

		delete [] m_blocks[i];
	}

	delete [] m_blocks;
}

void Chunk::enableVoxel(uint _x, uint _y, uint _z, BlockType _type)
{
	m_blocks[_x][_y][_z].m_active = true;
	m_blocks[_x][_y][_z].m_type = _type;

	m_isRebuilding = true;
}

void Chunk::disableVoxel(uint _x, uint _y, uint _z)
{
	m_blocks[_x][_y][_z].m_active = false;

	m_isRebuilding = true;
}

bool Chunk::isVoxelActive(uint _x, uint _y, uint _z) const
{
	if (_x < m_chunkSize && _y < m_chunkSize && _z < m_chunkSize)
	{
		return m_blocks[_x][_y][_z].m_active;
	}

	return false;
}

bool Chunk::shouldRender() const
{
	return m_shouldRender;
}

bool Chunk::IsRebuilding() const
{
	return m_isRebuilding;
}

void Chunk::rebuild(Vec3f _pos, float _blockRenderSize)
{
	Vec3f position_in_chunk;
	Vec4f color;
	bool front, back, right, left, top, bottom;

	for (uint i = 0; i < m_chunkSize; ++i)
	{
		position_in_chunk.x = i * _blockRenderSize;

		for (uint j = 0; j < m_chunkSize; ++j)
		{
			position_in_chunk.y = j * _blockRenderSize;

			for (uint k = 0; k < m_chunkSize; ++k)
			{
				if (!m_blocks[i][j][k].m_active)
				{
					continue;
				}

				position_in_chunk.z = k * -_blockRenderSize;

				switch (m_blocks[i][j][k].m_type)
				{
				case BT_Default:
					color(1.0f, 1.0f, 1.0f, 1.0f);
					break;
				case BT_Dirt:
					color(0.54f, 0.27f, 0.07f, 1.0f);
					break;
				case BT_Grass:
					color(0.42f, 0.56f, 0.14f, 1.0f);
					break;
				case BT_Rock:
					color(0.65f, 0.65f, 0.65f, 1.0f);
					break;
				}

				front = isVoxelActive(i, j, k - 1);
				back = isVoxelActive(i, j, k + 1);
				right = isVoxelActive(i + 1, j, k);
				left = isVoxelActive(i - 1, j, k);
				top = isVoxelActive(i, j + 1, k);
				bottom = isVoxelActive(i, j - 1, k);

				m_mesh.addCube(_blockRenderSize, _pos + position_in_chunk, color, !front, !back, !right, !left, !top, !bottom);
			}
		}
	}

	m_mesh.updateVBO(m_bo.vbo, m_bo.ibo, GL_DYNAMIC_DRAW);
	m_mesh.clear();

	m_shouldRender = m_mesh.getVertexArraySize() > 0;
	m_isRebuilding = false;
}

void Chunk::render(GfxRenderer* _renderer)
{
	_renderer->vboToScreen(m_bo.vbo, m_bo.ibo, m_mesh.getVertexArraySize(), m_mesh.getIndexCount());
}