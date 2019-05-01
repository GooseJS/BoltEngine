#include "World.h"

#include "BlockManager.h"
#include "BoltEngine/util/TaskScheduler.h"

namespace Bolt
{
	ChunkPtr World::getChunkAt(ChunkPos pos)
	{
		if (_lastAccessedChunk != nullptr && _lastAccessedChunk->inColumn(pos)) // Check if this chunk is in the last accessed chunk column as we already have it cached
			return _lastAccessedChunk->getChunkAt(pos.y);

		ChunkPtr foundChunk;
		auto findChunk = _chunkMap.find(pos.value);
		if (findChunk != _chunkMap.end()) // We have the chunk loaded already, just grab it
		{
			foundChunk = findChunk->second->getChunkAt(pos.y);
		}
		else // No chunk found, need to create / load it
		{
			ChunkColumn* chunkColumn = DBG_NEW ChunkColumn(this, pos.x, pos.z);
			for (int y = 0; y < BOLT_WORLD_HEIGHT; y++) // Filling up all the chunks in the chunk column and doing initial setup
			{
				chunkColumn->getChunkAt(pos.y)->setContainingWorld(this);
				chunkColumn->getChunkAt(pos.y)->setContainingColumn(chunkColumn);
			}
			foundChunk = chunkColumn->getChunkAt(pos.y);				// Get the chunk they asked for
			_lastAccessedChunk = chunkColumn;							// Cache the chunk column
			_chunkMap.insert(std::make_pair(pos.value, chunkColumn));	// Push the chunkColumn into the map
		}
		return foundChunk;
	}

	ChunkColumn* World::getChunkColumnAt(ChunkPos pos)
	{
		if (_lastAccessedChunk != nullptr && _lastAccessedChunk->inColumn(pos)) // Check if this chunkcolumn is the last accessed chunk column as we already have it cached
			return _lastAccessedChunk;

		ChunkPtr foundChunk;
		auto findChunk = _chunkMap.find(pos.value);
		if (findChunk != _chunkMap.end()) // We have the chunkcolumn loaded already, just grab it
		{
			foundChunk = findChunk->second->getChunkAt(pos.y);
		}
		else // No chunkcolumn found, need to create / load it
		{
			ChunkColumn* chunkColumn = DBG_NEW ChunkColumn(this, pos.x, pos.z);
			for (int y = 0; y < BOLT_WORLD_HEIGHT; y++) // Initial setup for the chunk column
			{
				chunkColumn->getChunkAt(pos.y)->setContainingWorld(this);
				chunkColumn->getChunkAt(pos.y)->setContainingColumn(chunkColumn);
			}
			_lastAccessedChunk = chunkColumn; // Caching the column
			_chunkMap.insert(std::make_pair(pos.value, chunkColumn));
		}
		return _lastAccessedChunk;
	}

	Block& World::getBlockAt(BlockPos pos)
	{
		if (pos.y < 0) return BlockManager::getInstance().getBlock(0); // If we are under 0, just return air / default block
		else if (pos.y >= BOLT_CHUNK_WIDTH * BOLT_WORLD_HEIGHT) return BlockManager::getInstance().getBlock(0); // If we are over max Y, just return air / default block 
		return getChunkAt(pos)->getBlockAt(pos);
	}

	Block& World::setBlockAt(BlockPos pos, Block& block)
	{
		if (pos.y < 0) return BlockManager::getInstance().getBlock(0); // If we are under 0, just return air / default block
		else if (pos.y >= BOLT_CHUNK_WIDTH * BOLT_WORLD_HEIGHT) return BlockManager::getInstance().getBlock(0); // If we are over max Y, just return air / default block 
		return getChunkAt(pos)->setBlockAt(pos, block);
	}

	void World::updateRenderChunksFromPos(BlockPos pos, WorldRenderer* worldRenderer)
	{
		ChunkPos initialPos = pos;

		for (int x = -_renderDistance; x < _renderDistance; x++) // For x = -renderDistance to positive renderDistance will essentially iterate over a rectangle with a diamater of the render distance around the player
		{
			for (int z = -_renderDistance; z < _renderDistance; z++)
			{
				ChunkPos chunkPos = initialPos;
				chunkPos.x += x; // Add the offset to our initial position
				chunkPos.z += z;
				ChunkColumn* column = getChunkColumnAt(chunkPos); // Get the chunk column
				if (!column->isGenerated()) // If it's not generated yet
				{
					auto generateNewMeshesFunc = std::bind([](ChunkColumn* l_column, WorldRenderer* l_worldRenderer)
					{
						for (int y = 0; y < BOLT_WORLD_HEIGHT; y++)
						{
							ChunkPtr chunk = l_column->getChunkAt(y);
							auto meshGenFunc = std::bind([](ChunkPtr l_chunk, WorldRenderer* l_worldRenderer)
							{
								l_chunk->generateMesh();
								l_worldRenderer->queueChunkMeshForUpload(l_chunk);
							}, chunk, l_worldRenderer);
							if (chunk->getMesh().needsRebuild && !chunk->getMesh().building)
							{
								TaskScheduler::getInstance().createTask(meshGenFunc);
							}
						}
					}, column, worldRenderer);
					TaskScheduler::getInstance().createTask(std::bind(&WorldGenerator::generateChunk, _generator, column), generateNewMeshesFunc); // Generate it!
					column->setGenerated(true);
				}
			}
		}

		auto checkForUnloadChunksFunc = std::bind([this, pos]()
		{
			for (auto chunkPair : _chunkMap)
			{
				ChunkColumn* chunkColumn = chunkPair.second;
				ChunkPos chunkPos = chunkColumn->getChunkAt(0)->getPos();
				if (chunkPos.x > pos.x + _renderDistance || chunkPos.x < pos.x - _renderDistance)
				{
					if (chunkPos.z > pos.z + _renderDistance || chunkPos.z < pos.z - _renderDistance)
					{
						std::lock_guard<std::mutex> lock(_unloadMtx);
						_unloadChunks.push_back(chunkColumn);
					}
				}
			}
		});
	}
}