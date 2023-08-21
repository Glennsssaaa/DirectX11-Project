#define _USE_MATH_DEFINES

#include "SphereMesh.h"
#include <cmath>


ReturnedData SphereMesh::CalculateShape()
{
	Vertex3D* vertices;
	std::vector<Vertex3D> verticesVector;
	std::vector<DWORD> indicesVector;
	resolution = 100;
	float radius = 5;
	
	vertexCount = ((6 * resolution) * resolution) * 6;
	indexCount = vertexCount;
	
	vertices = new Vertex3D[vertexCount];

	vertices[0].pos = XMFLOAT3(0, radius, 0);
	vertices[0].texCoord = XMFLOAT2(0, 0);
	vertices[0].normal = XMFLOAT3(0, 1, 0);
	verticesVector.push_back(vertices[0]);

	auto phiStep = M_PI / resolution;
	auto thetaStep = 2* M_PI / resolution;
	
	for (int i = 1; i <= resolution - 1; i++) {
		auto phi = i * phiStep;
		for (int j = 0; j <= resolution; j++) {
			
			auto theta = j * thetaStep;
			XMFLOAT3 p = XMFLOAT3(radius*sin(phi)*cos(theta),
				radius*cos(phi),
				radius*sin(phi)*sin(theta));
			
			XMFLOAT3 t = XMFLOAT3(-radius * sin(phi) * sin(theta), 0, radius * sin(phi) * cos(theta));
			XMFLOAT3 n = p;
			XMFLOAT2 uv = XMFLOAT2(theta / (M_PI * 2), phi / M_PI);
			
			vertices[j].pos = p;
			vertices[j].normal = n;
			vertices[j].texCoord = uv;
			verticesVector.push_back(vertices[j]);
		}
	}

	for (int i = 1; i <= resolution; i++) {
		indicesVector.push_back(0);
		indicesVector.push_back(i + 1);
		indicesVector.push_back(i);
	}

	int baseIndex = 1;
	int ringVertexCount = resolution + 1;
	for (int i = 0; i < resolution - 2; i++) {
		for (int j = 0; j < resolution; j++) {
			indicesVector.push_back(baseIndex + i * ringVertexCount + j);
			indicesVector.push_back(baseIndex + i * ringVertexCount + j+1);
			indicesVector.push_back(baseIndex + (i+1) * ringVertexCount + j);

			indicesVector.push_back(baseIndex + (i+1) * ringVertexCount + j);
			indicesVector.push_back(baseIndex + i * ringVertexCount + j+1);
			indicesVector.push_back(baseIndex + (i+1) * ringVertexCount + j+1);
		}
	}
	int southPoleIndex = vertexCount - 1;
	baseIndex = southPoleIndex - ringVertexCount;
	for (int i = 0; i < resolution; i++) {
		indicesVector.push_back(southPoleIndex);
		indicesVector.push_back(baseIndex + i);
		indicesVector.push_back(baseIndex + i + 1);
	}

	ReturnedData shapeData;
	shapeData.Indices = indicesVector;
	shapeData.Vertices = verticesVector;

	return shapeData;
}