#include "WorldRenderer.h"

#include <functional>
#include <glm/glm.hpp>
#include <chrono>

#include "BlockManager.h"

namespace Bolt
{
	ChunkPtr generateChunkMesh(ChunkPtr chunk)
	{
		ChunkPos chunkPos = chunk->getPos();

		BoltChunkMesh& mesh = chunk->getMesh();
		mesh.building = true;

		for (int x = 0; x < BOLT_CHUNK_WIDTH; x++)
		{
			const ChunkSlice& xSlice = chunk->getSliceAt(Axis::X, x);
			if (xSlice.isEmpty() || xSlice.isFull())
				continue;

			for (int y = 0; y < BOLT_CHUNK_WIDTH; y++)
			{
				const ChunkSlice& ySlice = chunk->getSliceAt(Axis::Y, y);
				if (ySlice.isEmpty() || ySlice.isFull())
					continue;

				for (int z = 0; z < BOLT_CHUNK_WIDTH; z++)
				{
					const ChunkSlice& zSlice = chunk->getSliceAt(Axis::Z, z);
					if (zSlice.isEmpty() || zSlice.isFull())
						continue;

					Block& currentBlock = chunk->getBlockAt(ChunkBlockPos(x, y, z, chunkPos));
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

						bool renderXPositive = !chunk->getBlockAt(chunk->getPos().toBlockPos(x + 1, y, z)).shouldRender();
						bool renderXNegative = !chunk->getBlockAt(chunk->getPos().toBlockPos(x - 1, y, z)).shouldRender();
						bool renderYPositive = !chunk->getBlockAt(chunk->getPos().toBlockPos(x, y + 1, z)).shouldRender();
						bool renderYNegative = !chunk->getBlockAt(chunk->getPos().toBlockPos(x, y - 1, z)).shouldRender();
						bool renderZPositive = !chunk->getBlockAt(chunk->getPos().toBlockPos(x, y, z + 1)).shouldRender();
						bool renderZNegative = !chunk->getBlockAt(chunk->getPos().toBlockPos(x, y, z - 1)).shouldRender();

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

		return chunk;
	}

	//ChunkList rebuildChunks(ChunkList chunksToRebuild)
	//{
	//	for (auto chunkIter = chunksToRebuild.begin(); chunkIter != chunksToRebuild.end(); chunkIter++)
	//	{
	//		if ((*chunkIter)->getMesh().needsRebuild)
	//			generateChunkMesh(*chunkIter); // TODO: Chunk mesh creation needs to be queued before being sent off to another thread
	//	}
	//
	//	return chunksToRebuild;
	//}

	void WorldRenderer::initRenderer()
	{
		_worldRenderShader.initFromFile("shader.vert", "shader.frag");
	}

	void WorldRenderer::createChunkMesh(ChunkPtr chunk)
	{
		generateChunkMesh(chunk);
	}

	void WorldRenderer::checkForRebuildChunks(World* world)
	{
		static std::chrono::high_resolution_clock::time_point last;

		if (!_chunkGenerationThreads.empty())
		{
			// Process the rebuild chunk threads
			for (auto iter = _chunkGenerationThreads.begin(); iter != _chunkGenerationThreads.end();)
			{
				std::future<ChunkPtr>& future = *iter;
				auto status = future.wait_for(std::chrono::microseconds(0));
				if (status == std::future_status::ready)
				{
					future.get()->getMesh().uploadMesh();
					iter = _chunkGenerationThreads.erase(iter);
					BOLT_INFO("Time taken: {}ms", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - last).count());
				}
				else
				{
					iter++;
				}
			}
		}

		if (!world->getChunksToRebuild().empty())
		{
			// Queue chunks for rebuild
			if (_chunkGenerationThreads.size() < maxChunkGenerationThreads)
			{
				auto iter = world->getChunksToRebuild().begin();
				ChunkPtr chunk = *iter;
				if (chunk->getMesh().needsRebuild)
				{
					_chunkGenerationThreads.push_back(std::async(std::launch::async, generateChunkMesh, chunk));
					last = std::chrono::high_resolution_clock::now();
				}
				world->getChunksToRebuild().erase(iter);
			}
		}
	}

	void WorldRenderer::renderWorld(World* world)
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		_worldRenderCam->generateMatrices();
		glm::mat4 transformedMatrix = _worldRenderCam->getProjectionMatrix() * _worldRenderCam->getViewMatrix();

		_worldRenderShader.use();

		glUniformMatrix4fv(_worldRenderShader.uniform("projectionViewMatrix"), 1, GL_FALSE, &transformedMatrix[0][0]);
		//glUniform3f(_worldRenderShader.uniform("lightPos"), 0.0f, 64.f, 0.0f);

		glm::mat4 translation(1.0f);

		glBindTexture(GL_TEXTURE_2D_ARRAY, BlockManager::getInstance().getTexture());
		glActiveTexture(GL_TEXTURE0);

		for (auto iter = world->getChunkMap().begin(); iter != world->getChunkMap().end(); iter++)
		{
			ChunkColumn* chunkColumn = iter->second;
			for (int y = 0; y < BOLT_WORLD_HEIGHT; y++)
			{
				ChunkPtr chunk = chunkColumn->getChunkAt(y);
				if (chunk->getMesh().initialized)
				{
					translation = glm::mat4(1.0f);
					translation = glm::translate(translation, glm::vec3(chunk->getPos().x * BOLT_CHUNK_WIDTH, chunk->getPos().y * BOLT_CHUNK_WIDTH, chunk->getPos().z * BOLT_CHUNK_WIDTH));
					glUniformMatrix4fv(_worldRenderShader.uniform("translationMatrix"), 1, GL_FALSE, &translation[0][0]);
					glBindVertexArray(chunk->getMesh()._vaoID);
					glDrawElements(GL_TRIANGLES, chunk->getMesh().numIndices, GL_UNSIGNED_INT, nullptr);
				}
			}
		}
	}
}