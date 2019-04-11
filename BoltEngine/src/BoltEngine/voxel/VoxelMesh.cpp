#include "VoxelMesh.h"

namespace Bolt
{
    unsigned int BoltChunkMesh::addVertexToMesh(glm::vec3 position, glm::vec3 normal, glm::vec3 texCoord)
    {
		BoltChunkMeshVertex newVertex;

        newVertex.vertexPosition[0] = position.x;
        newVertex.vertexPosition[1] = position.y;
        newVertex.vertexPosition[2] = position.z;

        newVertex.vertexNormal[0] = normal.x;
        newVertex.vertexNormal[1] = normal.y;
        newVertex.vertexNormal[2] = normal.z;

		newVertex.texCoord[0] = texCoord.x;
		newVertex.texCoord[1] = texCoord.y;
		newVertex.texCoord[2] = texCoord.z;

        _vertices.push_back(newVertex);

        unsigned int vertexID = (int)_vertices.size() - 1;
        return vertexID;
    }

    unsigned int BoltChunkMesh::addTriangleToMesh(unsigned int v1, unsigned int v2, unsigned int v3)
    {
		BoltChunkMeshTriangle newTriangle;
        newTriangle.vertexIndices[0] = v1;
        newTriangle.vertexIndices[1] = v2;
        newTriangle.vertexIndices[2] = v3;

        _triangles.push_back(std::move(newTriangle));

        unsigned int triangleID = (int)_triangles.size() - 1;
        return triangleID;
    }

	void BoltChunkMesh::uploadMesh()
	{
		if (_vertices.size() <= 0)
			return;

		if (!initialized)
			glGenVertexArrays(1, &_vaoID); // TODO (Brendan): This should be calling my own function, and grabbing from a pool of VAO's that are no longer being used
		glBindVertexArray(_vaoID);

		if (!initialized)
		{
			glGenBuffers(1, &_vertexBufferID);
			glGenBuffers(1, &_indexBufferID);
		}

		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(BoltChunkMeshVertex), &_vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _triangles.size() * sizeof(BoltChunkMeshTriangle), &_triangles[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BoltChunkMeshVertex), (GLvoid*)offsetof(BoltChunkMeshVertex, vertexPosition));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_INT, GL_FALSE, sizeof(BoltChunkMeshVertex), (GLvoid*)offsetof(BoltChunkMeshVertex, texCoord));

		numIndices = _triangles.size() * 3;
		numVertices = _vertices.size();

		_triangles.clear();
		_vertices.clear();

		if (!initialized)
			initialized = true;
		needsRebuild = false;
	}
}