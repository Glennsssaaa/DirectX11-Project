
#include "PlaneMesh.h"

ReturnedData PlaneMesh::CalculateShape()
{
	Vertex3D* vertices;
	std::vector<Vertex3D> verticesVector;
	unsigned long* indices;
	std::vector<DWORD> indicesVector;
	D3D11_BUFFER_DESC Vertex3DBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA Vertex3DData, indexData;
	resolution = 5;
	vertexCount = ((6 * resolution) * resolution) * 6;

	indexCount = vertexCount;

	vertices = new Vertex3D[vertexCount];
	indices = new unsigned long[indexCount];

	float w2 = 0.5f * resolution;

	// bottom
	vertices[0] = Vertex3D(-w2, +w2, -w2, 0, 1, 0, 1, 0);
	vertices[1] = Vertex3D(-w2, +w2, +w2, 0, 0, 0, 1, 0);
	vertices[2] = Vertex3D(+w2, +w2, +w2, 1, 0, 0, 1, 0);
	vertices[3] = Vertex3D(+w2, +w2, -w2, 1, 1, 0, 1, 0);

	for (int i = 0; i < vertexCount; i++)
	{
		verticesVector.push_back(vertices[i]);
	}

	int indicesArray[] = {
		0,1,2,
		0,2,3,
	};

	for (int i = 0; i < sizeof(indicesArray); i++)
	{
		indicesVector.push_back(indicesArray[i]);
	}

	ReturnedData shapeData;
	shapeData.Indices = indicesVector;
	shapeData.Vertices = verticesVector;

	delete vertices;
	delete indices;

	return shapeData;
}