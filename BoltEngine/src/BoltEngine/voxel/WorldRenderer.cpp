#include "WorldRenderer.h"

#include <glm/glm.hpp>

namespace Bolt
{
	void WorldRenderer::initRenderer()
	{
		_worldRenderShader.initFromFile("shader.vert", "shader.frag");
	}

    void WorldRenderer::createChunkMesh(ChunkPtr chunk, int chunkY)
    {
		if (chunkY >= BOLT_WORLD_HEIGHT)
			return;

		int worldHeight = chunkY == -1 ? BOLT_WORLD_HEIGHT : chunkY + 1;

		while(chunkY != worldHeight)
		{
			if (chunkY == -1)
				chunkY = 0;

			ChunkPos chunkPos = chunk->getPos();
			chunkPos.y = chunkY;

			BoltChunkMesh& mesh = chunk->getMesh(chunkY);

			for (int x = 0; x < BOLT_CHUNK_WIDTH; x++)
			{
				for (int worldY = chunkY * BOLT_CHUNK_WIDTH; worldY < (chunkY + 1) * BOLT_CHUNK_WIDTH; worldY++)
				{
					for (int z = 0; z < BOLT_CHUNK_WIDTH; z++)
					{
						int y = worldY & (BOLT_CHUNK_WIDTH - 1);
						Block& currentBlock = chunk->getBlockAt(ChunkBlockPos(x, y, z, chunkPos));
						if (currentBlock.shouldRender())
						{
							glm::vec3 p1(x,								y,							z);
							glm::vec3 p2(x + BOLT_BLOCK_RENDER_SIZE,	y,							z);
							glm::vec3 p3(x + BOLT_BLOCK_RENDER_SIZE,	y + BOLT_BLOCK_RENDER_SIZE,	z);
							glm::vec3 p4(x,								y + BOLT_BLOCK_RENDER_SIZE, z);
							glm::vec3 p5(x + BOLT_BLOCK_RENDER_SIZE,	y,							z + BOLT_BLOCK_RENDER_SIZE);
							glm::vec3 p6(x,								y,							z + BOLT_BLOCK_RENDER_SIZE);
							glm::vec3 p7(x,								y + BOLT_BLOCK_RENDER_SIZE, z + BOLT_BLOCK_RENDER_SIZE);
							glm::vec3 p8(x + BOLT_BLOCK_RENDER_SIZE,	y + BOLT_BLOCK_RENDER_SIZE, z + BOLT_BLOCK_RENDER_SIZE);

							unsigned int v1, v2, v3, v4;

							glm::vec3 normal;

							bool renderXPositive = !chunk->getContainingWorld()->getBlockAt(chunk->getPos().toBlockPos(x + 1,	worldY,		z)).shouldRender();
							bool renderXNegative = !chunk->getContainingWorld()->getBlockAt(chunk->getPos().toBlockPos(x - 1,	worldY,		z)).shouldRender();
							bool renderYPositive = !chunk->getContainingWorld()->getBlockAt(chunk->getPos().toBlockPos(x,		worldY + 1,	z)).shouldRender();
							bool renderYNegative = !chunk->getContainingWorld()->getBlockAt(chunk->getPos().toBlockPos(x,		worldY - 1,	z)).shouldRender();
							bool renderZPositive = !chunk->getContainingWorld()->getBlockAt(chunk->getPos().toBlockPos(x,		worldY,		z + 1)).shouldRender();
							bool renderZNegative = !chunk->getContainingWorld()->getBlockAt(chunk->getPos().toBlockPos(x,		worldY,		z - 1)).shouldRender();

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
								BlockPos pos = chunk->getPos().toBlockPos(x, y + 1, z);
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

			mesh.uploadMesh();
			chunkY++;
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
		glUniform3f(_worldRenderShader.uniform("lightPos"), 0.0f, 64.f, 0.0f);

		glm::mat4 translation(1.0f);

		//glBindTexture(GL_TEXTURE_2D_ARRAY, _textureAtlas.textureID);
		//glActiveTexture(GL_TEXTURE0);

		for (auto iter = world->getChunkMap().begin(); iter != world->getChunkMap().end(); iter++)
		{
			ChunkPtr chunk = iter->second;
			for (int y = 0; y < BOLT_WORLD_HEIGHT; y++)
			{
				if (chunk->getMesh(y).initialized)
				{
					translation = glm::mat4(1.0f);
					translation = glm::translate(translation, glm::vec3(chunk->getPos().x * BOLT_CHUNK_WIDTH, y * BOLT_CHUNK_WIDTH, chunk->getPos().z * BOLT_CHUNK_WIDTH));
					glUniformMatrix4fv(_worldRenderShader.uniform("translationMatrix"), 1, GL_FALSE, &translation[0][0]);
					glBindVertexArray(chunk->getMesh(y)._vaoID);
					glDrawElements(GL_TRIANGLES, chunk->getMesh(y).numIndices, GL_UNSIGNED_INT, nullptr);
				}
			}
		}
	}
}