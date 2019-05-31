#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "BoltEngine/GL.h"

namespace Bolt
{
	struct BoltChunkMeshVertex
	{
		float vertexPosition[3] = { 0 };
		float vertexNormal[3] = { 0 };
		int texCoord[3] = { 0 };

		BoltChunkMeshVertex() {}

		BoltChunkMeshVertex(glm::vec3 pos, glm::vec3 norm, glm::vec3 tex)
		{
			vertexPosition[0] = pos.x;
			vertexPosition[1] = pos.y;
			vertexPosition[2] = pos.z;

			vertexNormal[0] = norm.x;
			vertexNormal[1] = norm.y;
			vertexNormal[2] = norm.z;

			texCoord[0] = tex.x;
			texCoord[1] = tex.y;
			texCoord[2] = tex.z;
		}
	};

	struct BoltChunkMeshTriangle
	{
		unsigned int vertexIndices[3];
	};

	class BoltChunkMesh
	{
	private:
		std::vector<BoltChunkMeshTriangle> _triangles;
		std::vector<BoltChunkMeshVertex> _vertices;
	public:		
		bool building = false;
		bool needsRebuild = true;
		bool initialized = false;
		bool empty = true;

		GLuint _vaoID;
		GLuint _vertexBufferID;
		GLuint _indexBufferID;

		unsigned int numVertices = 0;
		unsigned int numIndices = 0;

        unsigned int addVertexToMesh(glm::vec3 positions, glm::vec3 normals, glm::vec3 texCoords);
        unsigned int addTriangleToMesh(unsigned int v1, unsigned int v2, unsigned int v3);

		void uploadMesh();
	};
}