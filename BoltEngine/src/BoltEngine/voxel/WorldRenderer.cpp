#include "WorldRenderer.h"

#include <glm/glm.hpp>

namespace Bolt
{
    void WorldRenderer::createChunkMesh(ChunkPtr chunk)
    {
        BoltChunkMesh& mesh = chunk->getMesh();

        for (int x = 0; x < BOLT_CHUNK_WIDTH; x++)
        {
            for (int y = 0; y < BOLT_CHUNK_WIDTH; y++)
            {
                for (int z = 0; z < BOLT_CHUNK_WIDTH; z++)
                {
                    Block& currentBlock = chunk->getBlockAt(ChunkBlockPos(x, y, z));
                    if (currentBlock.shouldRender())
                    {
                        glm::vec3 p1(x - BOLT_BLOCK_RENDER_SIZE, y - BOLT_BLOCK_RENDER_SIZE, z + BOLT_BLOCK_RENDER_SIZE);
                        glm::vec3 p2(x + BOLT_BLOCK_RENDER_SIZE, y - BOLT_BLOCK_RENDER_SIZE, z + BOLT_BLOCK_RENDER_SIZE);
                        glm::vec3 p3(x + BOLT_BLOCK_RENDER_SIZE, y + BOLT_BLOCK_RENDER_SIZE, z + BOLT_BLOCK_RENDER_SIZE);
                        glm::vec3 p4(x - BOLT_BLOCK_RENDER_SIZE, y + BOLT_BLOCK_RENDER_SIZE, z + BOLT_BLOCK_RENDER_SIZE);
						glm::vec3 p5(x + BOLT_BLOCK_RENDER_SIZE, y - BOLT_BLOCK_RENDER_SIZE, z - BOLT_BLOCK_RENDER_SIZE);
						glm::vec3 p6(x - BOLT_BLOCK_RENDER_SIZE, y - BOLT_BLOCK_RENDER_SIZE, z - BOLT_BLOCK_RENDER_SIZE);
						glm::vec3 p7(x - BOLT_BLOCK_RENDER_SIZE, y + BOLT_BLOCK_RENDER_SIZE, z - BOLT_BLOCK_RENDER_SIZE);
						glm::vec3 p8(x + BOLT_BLOCK_RENDER_SIZE, y + BOLT_BLOCK_RENDER_SIZE, z - BOLT_BLOCK_RENDER_SIZE);

                        unsigned int v1, v2, v3, v4;
                        unsigned int t1, t2, t3, t4;

						glm::vec3 normal;

						bool renderXPositive = !chunk->getContainingWorld()->getBlockAt(chunk->getPos().toBlockPos(x + 1, y, z)).shouldRender();
						bool renderXNegative = !chunk->getContainingWorld()->getBlockAt(chunk->getPos().toBlockPos(x - 1, y, z)).shouldRender();
						bool renderYPositive = !chunk->getContainingWorld()->getBlockAt(chunk->getPos().toBlockPos(x, y + 1, z)).shouldRender();
						bool renderYNegative = !chunk->getContainingWorld()->getBlockAt(chunk->getPos().toBlockPos(x, y - 1, z)).shouldRender();
						bool renderZPositive = !chunk->getContainingWorld()->getBlockAt(chunk->getPos().toBlockPos(x, y, z + 1)).shouldRender();
						bool renderZNegative = !chunk->getContainingWorld()->getBlockAt(chunk->getPos().toBlockPos(x, y, z - 1)).shouldRender();

						Texture::TextureArray::Key textureID;

						// Front
						if (renderZPositive)
						{
							textureID = currentBlock.getTextureKey(BLOCK_FACE_X_POSITIVE);
							normal = glm::vec3(0.0f, 0.0f, 1.0f);
							v1 = mesh.addVertexToMesh(p1, normal, glm::vec3(0.0f, 0.0f, textureID));
							v2 = mesh.addVertexToMesh(p2, normal, glm::vec3(1.0f, 0.0f, textureID));
							v3 = mesh.addVertexToMesh(p3, normal, glm::vec3(1.0f, 1.0f, textureID));
							v4 = mesh.addVertexToMesh(p4, normal, glm::vec3(0.0f, 1.0f, textureID));

							mesh.addTriangleToMesh(v1, v2, v3);
							mesh.addTriangleToMesh(v1, v3, v4);
						}

						// Back
						if (renderZNegative)
						{
							textureID = currentBlock.getTextureKey(BLOCK_FACE_X_POSITIVE);
							normal = glm::vec3(0.0f, 0.0f, -1.0f);
							v1 = mesh.addVertexToMesh(p5, normal, glm::vec3(0.0f, 0.0f, textureID));
							v2 = mesh.addVertexToMesh(p6, normal, glm::vec3(1.0f, 0.0f, textureID));
							v3 = mesh.addVertexToMesh(p7, normal, glm::vec3(1.0f, 1.0f, textureID));
							v4 = mesh.addVertexToMesh(p8, normal, glm::vec3(0.0f, 1.0f, textureID));

							mesh.addTriangleToMesh(v1, v2, v3);
							mesh.addTriangleToMesh(v1, v3, v4);
						}

						// Right
						if (renderXPositive)
						{
							textureID = currentBlock.getTextureKey(BLOCK_FACE_X_POSITIVE);
							normal = glm::vec3(1.0f, 0.0f, 0.0f);
							v1 = mesh.addVertexToMesh(p2, normal, glm::vec3(0.0f, 0.0f, textureID));
							v2 = mesh.addVertexToMesh(p5, normal, glm::vec3(1.0f, 0.0f, textureID));
							v3 = mesh.addVertexToMesh(p8, normal, glm::vec3(1.0f, 1.0f, textureID));
							v4 = mesh.addVertexToMesh(p3, normal, glm::vec3(0.0f, 1.0f, textureID));

							mesh.addTriangleToMesh(v1, v2, v3);
							mesh.addTriangleToMesh(v1, v3, v4);
						}

						// Left
						if (renderXNegative)
						{
							textureID = currentBlock.getTextureKey(BLOCK_FACE_X_POSITIVE);
							normal = glm::vec3(-1.0f, 0.0f, 0.0f);
							v1 = mesh.addVertexToMesh(p6, normal, glm::vec3(0.0f, 0.0f, textureID));
							v2 = mesh.addVertexToMesh(p1, normal, glm::vec3(1.0f, 0.0f, textureID));
							v3 = mesh.addVertexToMesh(p4, normal, glm::vec3(1.0f, 1.0f, textureID));
							v4 = mesh.addVertexToMesh(p7, normal, glm::vec3(0.0f, 1.0f, textureID));

							mesh.addTriangleToMesh(v1, v2, v3);
							mesh.addTriangleToMesh(v1, v3, v4);
						}

						// Top
						if (renderYPositive)
						{
							textureID = currentBlock.getTextureKey(BLOCK_FACE_X_POSITIVE);
							normal = glm::vec3(0.0f, 1.0f, 0.0f);
							v1 = mesh.addVertexToMesh(p4, normal, glm::vec3(0.0f, 0.0f, textureID));
							v2 = mesh.addVertexToMesh(p3, normal, glm::vec3(1.0f, 0.0f, textureID));
							v3 = mesh.addVertexToMesh(p8, normal, glm::vec3(1.0f, 1.0f, textureID));
							v4 = mesh.addVertexToMesh(p7, normal, glm::vec3(0.0f, 1.0f, textureID));

							mesh.addTriangleToMesh(v1, v2, v3);
							mesh.addTriangleToMesh(v1, v3, v4);
						}

						// Back
						if (renderXNegative)
						{
							textureID = currentBlock.getTextureKey(BLOCK_FACE_X_POSITIVE);
							normal = glm::vec3(0.0f, -1.0f, 0.0f);
							v1 = mesh.addVertexToMesh(p6, normal, glm::vec3(0.0f, 0.0f, textureID));
							v2 = mesh.addVertexToMesh(p5, normal, glm::vec3(1.0f, 0.0f, textureID));
							v3 = mesh.addVertexToMesh(p2, normal, glm::vec3(1.0f, 1.0f, textureID));
							v4 = mesh.addVertexToMesh(p1, normal, glm::vec3(0.0f, 1.0f, textureID));

							mesh.addTriangleToMesh(v1, v2, v3);
							mesh.addTriangleToMesh(v1, v3, v4);
						}
                    }
                }
            }
        }
    }
}