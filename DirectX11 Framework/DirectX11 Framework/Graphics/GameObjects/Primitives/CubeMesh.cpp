
#include "CubeMesh.h"

ReturnedData CubeMesh::CalculateShape()
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

	
	vertices[0] = Vertex3D(-w2, -w2, -w2, 0, 1, 0, 0, -1);
	vertices[1] = Vertex3D(-w2, +w2, -w2, 0, 0, 0, 0, -1);
	vertices[2] = Vertex3D(+w2, +w2, -w2, 1, 0, 0, 0, -1);
	vertices[3] = Vertex3D(+w2, -w2, -w2, 1, 1, 0, 0, -1);
	// back
	vertices[4] = Vertex3D(-w2, -w2, +w2, 1, 1, 0, 0, 1);
	vertices[5] = Vertex3D(+w2, -w2, +w2, 0, 1, 0, 0, 1);
	vertices[6] = Vertex3D(+w2, +w2, +w2, 0, 0, 0, 0, 1);
	vertices[7] = Vertex3D(-w2, +w2, +w2, 1, 0, 0, 0, 1);
	// top
	
	vertices[8] = Vertex3D(-w2, +w2, -w2, 0, 1, 0, 1, 0);
	vertices[9] = Vertex3D(-w2, +w2, +w2, 0, 0, 0, 1, 0);
	vertices[10] = Vertex3D(+w2, +w2, +w2, 1, 0, 0, 1, 0);
	vertices[11] = Vertex3D(+w2, +w2, -w2, 1, 1, 0, 1, 0);

	// bottom
	vertices[12] = Vertex3D(-w2, -w2, -w2, 1, 1, 0, -1, 0);
	vertices[13] = Vertex3D(+w2, -w2, -w2, 0, 1, 0, -1, 0);
	vertices[14] = Vertex3D(+w2, -w2, +w2, 0, 0, 0, -1, 0);
	vertices[15] = Vertex3D(-w2, -w2, +w2, 1, 0, 0, -1, 0);

	// left
	vertices[16] = Vertex3D(-w2, -w2, +w2, 0, 1, -1, 0, 0);
	vertices[17] = Vertex3D(-w2, +w2, +w2, 0, 0, -1, 0, 0);
	vertices[18] = Vertex3D(-w2, +w2, -w2, 1, 0, -1, 0, 0);
	vertices[19] = Vertex3D(-w2, -w2, -w2, 1, 1, -1, 0, 0);

	// right
	vertices[20] = Vertex3D(+w2, -w2, -w2, 0, 1, 1, 0, 0);
	vertices[21] = Vertex3D(+w2, +w2, -w2, 0, 0, 1, 0, 0);
	vertices[22] = Vertex3D(+w2, +w2, +w2, 1, 0, 1, 0, 0);
	vertices[23] = Vertex3D(+w2, -w2, +w2, 1, 1, 1, 0, 0);

	for (int i = 0; i < vertexCount; i++)
	{
		verticesVector.push_back(vertices[i]);
	}

	int indicesArray[] = {
		0,1,2,0,2,3,
		4,5,6,4,6,7,
		8,9,10,8,10,11,
		12,13,14,12,14,15,
		16,17,18,16,18,19,
		20,21,22,20,22,23
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