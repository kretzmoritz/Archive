#include "sv_chunkmgr.h"
#include "../libs/simplex.h"

void SVChunkMgr::setup(Chunk* _chunk, Vec3f _pos)
{
	float world_dimension = BLOCK_RENDER_SIZE * CHUNK_SIZE * WORLD_SIZE;
	float world_half_dimension = world_dimension / 2.0f;
	float xf, yf, zf;

	_pos.x += world_half_dimension;
	_pos.y += world_half_dimension;
	_pos.z += world_half_dimension;

	for (uint i = 0; i < CHUNK_SIZE; ++i)
	{
		for (uint j = 0; j < CHUNK_SIZE; ++j)
		{
			for (uint k = 0; k < CHUNK_SIZE; ++k)
			{
				xf = (_pos.x + i * BLOCK_RENDER_SIZE) / world_dimension;
				yf = (_pos.y + j * BLOCK_RENDER_SIZE) / world_dimension;
				zf = (_pos.z - k * BLOCK_RENDER_SIZE) / world_dimension;

				if(getDensity(xf, yf, zf) > 3.1f)
				{
					_chunk->enableVoxel(i, j, k, BlockType::BT_Rock);
				}
			}
		}
	}

	for (uint i = 0; i < CHUNK_SIZE; ++i)
	{
		for (uint j = 0; j < CHUNK_SIZE; ++j)
		{
			for (uint k = 0; k < CHUNK_SIZE; ++k)
			{
				if (_chunk->isVoxelActive(i, j, k))
				{
					bool topIsAir = false;

					if (j < CHUNK_SIZE - 1)
					{
						if (!_chunk->isVoxelActive(i, j + 1, k))
						{
							topIsAir = true;
						}
					}
					else
					{
						xf = (_pos.x + i * BLOCK_RENDER_SIZE) / world_dimension;
						yf = (_pos.y + (j + 1) * BLOCK_RENDER_SIZE) / world_dimension;
						zf = (_pos.z - k * BLOCK_RENDER_SIZE) / world_dimension;

						if (getDensity(xf, yf, zf) <= 3.1f)
						{
							topIsAir = true;
						}
					}

					if (topIsAir)
					{
						_chunk->enableVoxel(i, j, k, BlockType::BT_Grass);

						bool voxelBelow = false;

						if (j > 0)
						{
							if (_chunk->isVoxelActive(i, j - 1, k))
							{
								voxelBelow = true;
							}
						}
						else
						{
							xf = (_pos.x + i * BLOCK_RENDER_SIZE) / world_dimension;
							yf = (_pos.y + (j - 1) * BLOCK_RENDER_SIZE) / world_dimension;
							zf = (_pos.z - k * BLOCK_RENDER_SIZE) / world_dimension;

							if (getDensity(xf, yf, zf) > 3.1f)
							{
								voxelBelow = true;
							}
						}

						if (voxelBelow)
						{
							_chunk->enableVoxel(i, j - 1, k, BlockType::BT_Dirt);
						}
					}
				}
			}
		}
	}
}

float SVChunkMgr::getDensity(float _xf, float _yf, float _zf)
{
	//http://codeflow.org/entries/2010/dec/09/minecraft-like-rendering-experiments-in-opengl-4/

	float caves, center_falloff, plateau_falloff, density;

	if(_yf <= 0.8){
		plateau_falloff = 1.0;
	}
	else if(0.8 < _yf && _yf < 0.9){
		plateau_falloff = 1.0f-(_yf-0.8f)*10.0f;
	}
	else{
		plateau_falloff = 0.0;
	}

	center_falloff = 0.1f/(
		pow((_xf-0.5f)*1.5f, 2) +
		pow((_yf-1.0f)*0.8f, 2) +
		pow((_zf-0.5f)*1.5f, 2)
	);
	caves = pow(simplex_noise(1, _xf*5, _yf*5, _zf*5), 3);
	density = (
		simplex_noise(5, _xf, _yf*0.5f, _zf) *
		center_falloff *
		plateau_falloff
	);
	density *= pow(
		noise((_xf+1)*3.0f, (_yf+1)*3.0f, (_zf+1)*3.0f)+0.4f, 1.8f
	);
	if(caves<0.5){
		density = 0;
	}

	return density;
}