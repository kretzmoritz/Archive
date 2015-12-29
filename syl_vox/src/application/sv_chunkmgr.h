#ifndef SV_CHUNKMGR_H
#define SV_CHUNKMGR_H

#include "voxel/chunk_manager.h"

class SVChunkMgr : public ChunkManager
{
protected:
	void setup(Chunk* _chunk, Vec3f _pos);

private:
	float getDensity(float _xf, float _yf, float _zf);
};

#endif