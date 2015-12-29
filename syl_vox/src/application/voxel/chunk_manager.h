#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include <queue>

#include "data/chunk.h"
#include "../../utility/octree.h"
#include "../../math/vector.h"
#include "../../utility/common_types.h"

class GfxRenderer;

class ChunkManager
{
public:
	ChunkManager();
	virtual ~ChunkManager();

	void drawOctree(float _unused);
	void update(GfxRenderer* _renderer);

protected:
	virtual void setup(Chunk* _chunk, Vec3f _pos) = 0;

	float BLOCK_RENDER_SIZE;
	uint CHUNK_SIZE;
	uint WORLD_SIZE;

private:
	uint MAX_CHUNK_ACTIONS_PER_FRAME;

	bool m_drawOctree;

	std::queue<OctNode<Chunk*>*> m_setup;
	std::queue<OctNode<Chunk*>*> m_rebuild;

	Octree<Chunk*>* m_octree;
};

#endif