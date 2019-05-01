#include "WorldRenderer.h"

#include <functional>
#include <glm/glm.hpp>
#include <chrono>

#include "BoltEngine/voxel/World.h"
#include "BoltEngine/util/TaskScheduler.h"
#include "BlockManager.h"

namespace Bolt
{
	void WorldRenderer::initRenderer()
	{
		_worldRenderShader.initFromFile("shader.vert", "shader.frag");
	}

	void WorldRenderer::initialBuild(World* world)
	{
		for (auto iter = world->getChunkMap().begin(); iter != world->getChunkMap().end(); iter++)
		{
			auto column = iter->second;
			for (int i = 0; i < BOLT_WORLD_HEIGHT; i++)
			{
				ChunkPtr chunk = column->getChunkAt(i);
				chunk->generateMesh();
				chunk->getMesh().uploadMesh();
			}
		}
	}

	void WorldRenderer::checkForRebuildChunks(World* world)
	{
		ChunkList& chunksToRebuild = world->getChunksToRebuild();
		if (!chunksToRebuild.empty())
		{
			for (ChunkPtr chunk : chunksToRebuild)
			{
				auto meshGenFunc = std::bind([](ChunkPtr l_chunk, WorldRenderer* l_worldRenderer)
				{
					l_chunk->generateMesh();
					l_worldRenderer->queueChunkMeshForUpload(l_chunk);
				}, chunk, this);
				if (chunk->getMesh().needsRebuild && !chunk->getMesh().building)
					TaskScheduler::getInstance().createTask(meshGenFunc);
			}
			chunksToRebuild.clear();
		}

		if (_uploadChunksMtx.try_lock())
		{
			if (!_uploadChunks.empty())
			{
				for (ChunkPtr chunk : _uploadChunks)
				{
					chunk->getMesh().uploadMesh();
				}
				_uploadChunks.clear();
			}
			_uploadChunksMtx.unlock();
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