#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "BoltEngine/GL.h"

namespace Bolt
{
	struct BoltChunkMeshVertex
	{
		float vertexPosition[3];
		float vertexNormal[3];
		int texCoord[3];
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
		bool needsRebuild = true;
		bool initialized = false;

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