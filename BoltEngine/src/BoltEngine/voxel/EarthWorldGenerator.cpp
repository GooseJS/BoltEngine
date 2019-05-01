#include "EarthWorldGenerator.h"

#include "BlockManager.h"

namespace Bolt
{
	void EarthWorldGenerator::setup()
	{
		
	}

	void EarthWorldGenerator::generateChunk(ChunkColumn* chunkColumn)
	{
		noise::utils::NoiseMapBuilderPlane _heightMapBuilder;
		noise::utils::NoiseMap heightMap;
		_heightMapBuilder.SetSourceModule(_perlin);
		_heightMapBuilder.SetDestNoiseMap(heightMap);
		_heightMapBuilder.SetDestSize(BOLT_CHUNK_WIDTH, BOLT_CHUNK_WIDTH);
		ChunkPtr chunkPtr = chunkColumn->getChunkAt(0);
		_heightMapBuilder.SetBounds(chunkPtr->getPos().x / 2.f, (chunkPtr->getPos().x + 1) / 2.f, chunkPtr->getPos().z / 2.f, (chunkPtr->getPos().z + 1) / 2.f);
		_heightMapBuilder.Build();

		ChunkPos pos = chunkPtr->getPos();
		for (int z = pos.z; z < pos.z + BOLT_CHUNK_WIDTH; z++)
		{
			for (int x = pos.x; x < pos.x + BOLT_CHUNK_WIDTH; x++)
			{
				for (int y = getTopHeight(pos.toBlockPos(x, 0, z), &heightMap).y; y >= 0; y--)
				{
					chunkColumn->setBlockAt(BlockPos(x, y, z), BlockManager::getInstance().getBlock(2));
				}
			}
		}
	}

	BlockPos EarthWorldGenerator::getTopHeight(const BlockPos& pos, noise::utils::NoiseMap* _heightMap)
	{
		ChunkBlockPos localPos = pos;
		return BlockPos(pos.x, _lowestAboveGround + (((_heightMap->GetValue(localPos.x, localPos.z) + 1.00f) / 2.00f) * 20), pos.z);
	}
}