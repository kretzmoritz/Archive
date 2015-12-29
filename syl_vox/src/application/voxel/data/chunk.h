#ifndef CHUNK_H
#define CHUNK_H

#include "block.h"
#include "../../gfx/data/gfx_mesh.h"
#include "../../gfx/data/gfx_vbo.h"
#include "../../../math/vector.h"
#include "../../../utility/common_types.h"

class GfxRenderer;

class Chunk
{
public:
	Chunk(uint _chunkSize);
	~Chunk();

	void enableVoxel(uint _x, uint _y, uint _z, BlockType _type);
	void disableVoxel(uint _x, uint _y, uint _z);

	bool isVoxelActive(uint _x, uint _y, uint _z) const;
	bool shouldRender() const;
	bool IsRebuilding() const;

	void rebuild(Vec3f _pos, float _blockRenderSize);
	void render(GfxRenderer* _renderer);

private:
	uint m_chunkSize;
	Block*** m_blocks;

	GfxMesh m_mesh;
	BufferObjects m_bo;

	bool m_shouldRender;
	bool m_isRebuilding;
};

#endif