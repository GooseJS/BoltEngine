#include "Chunk.h"

#include "World.h"
#include "BlockManager.h"

namespace Bolt
{
	Block& ChunkStorage::getBlockAt(int index)
	{
		return BlockManager::getInstance().getBlock(_storage[index]);
	}

	Block& ChunkStorage::setBlockAt(int index, const Block& block)
	{
		Block& prevBlock = BlockManager::getInstance().getBlock(_storage[index]);
		_storage[index] = block.getID();
		return prevBlock;
	}

	Chunk::Chunk(ChunkPos pos) : _pos(pos)
	{

	}

	Block& Chunk::getBlockAt(ChunkBlockPos pos)
	{
		return _storage.getBlockAt(pos.index());
	}
	
	Block& Chunk::setBlockAt(ChunkBlockPos pos, const Block& block)
	{
		Block& prevBlock = _storage.setBlockAt(pos.index(), block);
		if (block.getID() != prevBlock.getID())
		{
			if (getMesh().initialized && !getMesh().needsRebuild)
			{
				getMesh().needsRebuild = true;
				_containingWorld->getChunksToRebuild().push_back(this);
			}
			_slices[Axis::X][pos.x].addBlock(block);
			_slices[Axis::Y][pos.y].addBlock(block);
			_slices[Axis::Z][pos.z].addBlock(block);
		}
		return prevBlock;
	}

	void Chunk::generateMesh()
	{
		ChunkPos chunkPos = getPos();

		BoltChunkMesh& mesh = getMesh();
		
		if (mesh.building)
			return; // Return if this mesh is already being built as we don't want to build it again

		mesh.building = true;

		for (int x = 0; x < BOLT_CHUNK_WIDTH; x++)
		{
			const ChunkSlice& xSlice = getSliceAt(Axis::X, x);
			if (xSlice.isEmpty() || xSlice.isFull())
				continue;

			for (int y = 0; y < BOLT_CHUNK_WIDTH; y++)
			{
				const ChunkSlice& ySlice = getSliceAt(Axis::Y, y);
				if (ySlice.isEmpty() || ySlice.isFull())
					continue;

				for (int z = 0; z < BOLT_CHUNK_WIDTH; z++)
				{
					const ChunkSlice& zSlice = getSliceAt(Axis::Z, z);
					if (zSlice.isEmpty() || zSlice.isFull())
						continue;

					Block& currentBlock = getBlockAt(ChunkBlockPos(x, y, z, chunkPos));
					if (currentBlock.shouldRender())
					{
						glm::vec3 p1(x, y, z);
						glm::vec3 p2(x + BOLT_BLOCK_RENDER_SIZE, y, z);
						glm::vec3 p3(x + BOLT_BLOCK_RENDER_SIZE, y + BOLT_BLOCK_RENDER_SIZE, z);
						glm::vec3 p4(x, y + BOLT_BLOCK_RENDER_SIZE, z);
						glm::vec3 p5(x + BOLT_BLOCK_RENDER_SIZE, y, z + BOLT_BLOCK_RENDER_SIZE);
						glm::vec3 p6(x, y, z + BOLT_BLOCK_RENDER_SIZE);
						glm::vec3 p7(x, y + BOLT_BLOCK_RENDER_SIZE, z + BOLT_BLOCK_RENDER_SIZE);
						glm::vec3 p8(x + BOLT_BLOCK_RENDER_SIZE, y + BOLT_BLOCK_RENDER_SIZE, z + BOLT_BLOCK_RENDER_SIZE);

						unsigned int v1, v2, v3, v4;

						glm::vec3 normal;

						bool renderXPositive = !getBlockAt(getPos().toBlockPos(x + 1, y, z)).shouldRender();
						bool renderXNegative = !getBlockAt(getPos().toBlockPos(x - 1, y, z)).shouldRender();
						bool renderYPositive = !getBlockAt(getPos().toBlockPos(x, y + 1, z)).shouldRender();
						bool renderYNegative = !getBlockAt(getPos().toBlockPos(x, y - 1, z)).shouldRender();
						bool renderZPositive = !getBlockAt(getPos().toBlockPos(x, y, z + 1)).shouldRender();
						bool renderZNegative = !getBlockAt(getPos().toBlockPos(x, y, z - 1)).shouldRender();

						if (x == BOLT_CHUNK_WIDTH - 1) renderXPositive = true;
						else if (x == 0) renderXNegative = true;
						if (y == BOLT_CHUNK_WIDTH - 1) renderYPositive = true;
						else if (y == 0) renderYNegative = true;
						if (z == BOLT_CHUNK_WIDTH - 1) renderZPositive = true;
						else if (z == 0) renderZNegative = true;

						if (y == 0)
							renderYNegative = true;

						Texture::TextureArray::Key textureID;

						// Front
						if (renderZNegative)
						{
							textureID = currentBlock.getTextureKey(BLOCK_FACE_Z_NEGATIVE);
							normal = glm::vec3(0.0f, 0.0f, -1.0f);
							v1 = mesh.addVertexToMesh(p1, normal, glm::vec3(0.0f, 1.0f, textureID));
							v2 = mesh.addVertexToMesh(p2, normal, glm::vec3(1.0f, 1.0f, textureID));
							v3 = mesh.addVertexToMesh(p3, normal, glm::vec3(1.0f, 0.0f, textureID));
							v4 = mesh.addVertexToMesh(p4, normal, glm::vec3(0.0f, 0.0f, textureID));

							mesh.addTriangleToMesh(v2, v1, v3);
							mesh.addTriangleToMesh(v3, v1, v4);
						}

						// Back
						if (renderZPositive)
						{
							textureID = currentBlock.getTextureKey(BLOCK_FACE_Z_POSITIVE);
							normal = glm::vec3(0.0f, 0.0f, 1.0f);
							v1 = mesh.addVertexToMesh(p5, normal, glm::vec3(0.0f, 1.0f, textureID));
							v2 = mesh.addVertexToMesh(p6, normal, glm::vec3(1.0f, 1.0f, textureID));
							v3 = mesh.addVertexToMesh(p7, normal, glm::vec3(1.0f, 0.0f, textureID));
							v4 = mesh.addVertexToMesh(p8, normal, glm::vec3(0.0f, 0.0f, textureID));

							mesh.addTriangleToMesh(v2, v1, v3);
							mesh.addTriangleToMesh(v3, v1, v4);
						}

						// Right
						if (renderXPositive)
						{
							textureID = currentBlock.getTextureKey(BLOCK_FACE_X_POSITIVE);
							normal = glm::vec3(1.0f, 0.0f, 0.0f);
							v1 = mesh.addVertexToMesh(p2, normal, glm::vec3(0.0f, 1.0f, textureID));
							v2 = mesh.addVertexToMesh(p5, normal, glm::vec3(1.0f, 1.0f, textureID));
							v3 = mesh.addVertexToMesh(p8, normal, glm::vec3(1.0f, 0.0f, textureID));
							v4 = mesh.addVertexToMesh(p3, normal, glm::vec3(0.0f, 0.0f, textureID));

							mesh.addTriangleToMesh(v2, v1, v3);
							mesh.addTriangleToMesh(v3, v1, v4);
						}

						// Left
						if (renderXNegative)
						{
							textureID = currentBlock.getTextureKey(BLOCK_FACE_X_NEGATIVE);
							normal = glm::vec3(-1.0f, 0.0f, 0.0f);
							v1 = mesh.addVertexToMesh(p6, normal, glm::vec3(0.0f, 1.0f, textureID));
							v2 = mesh.addVertexToMesh(p1, normal, glm::vec3(1.0f, 1.0f, textureID));
							v3 = mesh.addVertexToMesh(p4, normal, glm::vec3(1.0f, 0.0f, textureID));
							v4 = mesh.addVertexToMesh(p7, normal, glm::vec3(0.0f, 0.0f, textureID));

							mesh.addTriangleToMesh(v2, v1, v3);
							mesh.addTriangleToMesh(v3, v1, v4);
						}

						// Top
						if (renderYPositive)
						{
							textureID = currentBlock.getTextureKey(BLOCK_FACE_Y_POSITIVE);
							normal = glm::vec3(0.0f, 1.0f, 0.0f);
							v1 = mesh.addVertexToMesh(p4, normal, glm::vec3(0.0f, 1.0f, textureID));
							v2 = mesh.addVertexToMesh(p3, normal, glm::vec3(1.0f, 1.0f, textureID));
							v3 = mesh.addVertexToMesh(p8, normal, glm::vec3(1.0f, 0.0f, textureID));
							v4 = mesh.addVertexToMesh(p7, normal, glm::vec3(0.0f, 0.0f, textureID));

							mesh.addTriangleToMesh(v2, v1, v3);
							mesh.addTriangleToMesh(v3, v1, v4);
						}
						else
						{
							//BlockPos pos = chunk->getPos().toBlockPos(x, y + 1, z);
							//BOLT_INFO("Block at {}, {}, {} is {} - Block + 1 Y: {}", x, y, z, currentBlock.getName(), chunk->getContainingWorld()->getBlockAt(pos).getName());
							//BOLT_INFO("Block {}, {}, {}", pos.x, pos.y, pos.z);
						}

						// Bottom
						if (renderYNegative)
						{
							textureID = currentBlock.getTextureKey(BLOCK_FACE_Y_NEGATIVE);
							normal = glm::vec3(0.0f, -1.0f, 0.0f);
							v1 = mesh.addVertexToMesh(p6, normal, glm::vec3(0.0f, 0.0f, textureID));
							v2 = mesh.addVertexToMesh(p5, normal, glm::vec3(1.0f, 0.0f, textureID));
							v3 = mesh.addVertexToMesh(p2, normal, glm::vec3(1.0f, 1.0f, textureID));
							v4 = mesh.addVertexToMesh(p1, normal, glm::vec3(0.0f, 1.0f, textureID));

							mesh.addTriangleToMesh(v2, v1, v3);
							mesh.addTriangleToMesh(v3, v1, v4);
						}
						else
						{

						}
					}
				}
			}
		}
		mesh.building = false;
	}
}