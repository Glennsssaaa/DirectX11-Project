
#include "CubeMesh.h"

bool CubeMesh::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader, int lresolution)
{
	resolution = lresolution;
	initBuffers(device);
	RenderableGameObject::Initialize(device, deviceContext, cb_vs_vertexshader);
	return true;
}


void CubeMesh::initBuffers(ID3D11Device* device)
{
	Vertex3D* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	vertexCount = ((6 * resolution) * resolution) * 6;
	
	indexCount = vertexCount;

	vertices = new Vertex3D[vertexCount];
	indices = new unsigned long[indexCount];

	float yincrement = 2.0f / resolution;
	float xincrement = 2.0f / resolution;
	float ystart = 1.0f;
	float xstart = -1.0f;
	float txu = 0.0f;
	float txv = 0.0f;
	float txuinc = 1.0f / resolution;	
	float txvinc = 1.0f / resolution;
	int v = 0;
	int i = 0;

	for (int y = 0; y < resolution; y++)	
	{
		for (int x = 0; x < resolution; x++)	
		{
			vertices[v].pos = XMFLOAT3(xstart, ystart - yincrement, -1.0f);  
			vertices[v].texCoord = XMFLOAT2(txu, txv + txvinc);
			vertices[v].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

			indices[i] = i;
			v++;
			i++;

			vertices[v].pos = XMFLOAT3(xstart + xincrement, ystart, -1.0f);
			vertices[v].texCoord = XMFLOAT2(txu + txuinc, txv);
			vertices[v].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

			indices[i] = i;
			v++;
			i++;

			vertices[v].pos = XMFLOAT3(xstart, ystart, -1.0f); 
			vertices[v].texCoord = XMFLOAT2(txu, txv);
			vertices[v].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

			indices[i] = i;
			v++;
			i++;

			vertices[v].pos = XMFLOAT3(xstart, ystart - yincrement, -1.0f);  
			vertices[v].texCoord = XMFLOAT2(txu, txv + txvinc);
			vertices[v].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

			indices[i] = i;
			v++;
			i++;

			vertices[v].pos = XMFLOAT3(xstart + xincrement, ystart - yincrement, -1.0f); 
			vertices[v].texCoord = XMFLOAT2(txu + txuinc, txv + txvinc);
			vertices[v].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

			indices[i] = i;
			v++;
			i++;

			vertices[v].pos = XMFLOAT3(xstart + xincrement, ystart, -1.0f);  
			vertices[v].texCoord = XMFLOAT2(txu + txuinc, txv);
			vertices[v].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

			indices[i] = i;
			v++;
			i++;

			xstart += xincrement;
			txu += txuinc;

		}

		ystart -= yincrement;
		xstart = -1;

		txu = 0;
		txv += txvinc;

	}

	txv = 0;

	ystart = 1;
	xstart = 1;
	for (int y = 0; y < resolution; y++)
	{
		for (int x = 0; x < resolution; x++)
		{
			vertices[v].pos = XMFLOAT3(xstart, ystart - yincrement, 1.0f);
			vertices[v].texCoord = XMFLOAT2(txu, txv + txvinc);
			vertices[v].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);

			indices[i] = i;
			v++;
			i++;

			vertices[v].pos = XMFLOAT3(xstart - xincrement, ystart, 1.0f);
			vertices[v].texCoord = XMFLOAT2(txu + txuinc, txv);
			vertices[v].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);

			indices[i] = i;
			v++;
			i++;

			vertices[v].pos = XMFLOAT3(xstart, ystart, 1.0f);
			vertices[v].texCoord = XMFLOAT2(txu, txv);
			vertices[v].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);

			indices[i] = i;
			v++;
			i++;

			vertices[v].pos = XMFLOAT3(xstart, ystart - yincrement, 1.0f); 
			vertices[v].texCoord = XMFLOAT2(txu, txv + txvinc);
			vertices[v].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);

			indices[i] = i;
			v++;
			i++;

			vertices[v].pos = XMFLOAT3(xstart - xincrement, ystart - yincrement, 1.0f);
			vertices[v].texCoord = XMFLOAT2(txu + txuinc, txv + txvinc);
			vertices[v].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);

			indices[i] = i;
			v++;
			i++;

			vertices[v].pos = XMFLOAT3(xstart - xincrement, ystart, 1.0f);
			vertices[v].texCoord = XMFLOAT2(txu + txuinc, txv);
			vertices[v].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);

			indices[i] = i;
			v++;
			i++;

			xstart -= xincrement;
			txu += txuinc;

		}

		ystart -= yincrement;
		xstart = 1;

		txu = 0;
		txv += txvinc;

	}

	txv = 0;

	ystart = 1;
	xstart = -1;
	for (int y = 0; y < resolution; y++)
	{
		for (int x = 0; x < resolution; x++)
		{
			vertices[v].pos = XMFLOAT3(1.0f, ystart - yincrement, xstart); 
			vertices[v].texCoord = XMFLOAT2(txu, txv + txvinc);
			vertices[v].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			vertices[v].pos = XMFLOAT3(1.0f, ystart, xstart + xincrement);
			vertices[v].texCoord = XMFLOAT2(txu + txuinc, txv);
			vertices[v].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			vertices[v].pos = XMFLOAT3(1.0f, ystart, xstart);
			vertices[v].texCoord = XMFLOAT2(txu, txv);
			vertices[v].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			vertices[v].pos = XMFLOAT3(1.0f, ystart - yincrement, xstart);
			vertices[v].texCoord = XMFLOAT2(txu, txv + txvinc);
			vertices[v].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			vertices[v].pos = XMFLOAT3(1.0f, ystart - yincrement, xstart + xincrement);
			vertices[v].texCoord = XMFLOAT2(txu + txuinc, txv + txvinc);
			vertices[v].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			vertices[v].pos = XMFLOAT3(1.0f, ystart, xstart + xincrement);
			vertices[v].texCoord = XMFLOAT2(txu + txuinc, txv);
			vertices[v].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			xstart += xincrement;
			txu += txuinc;

		}

		ystart -= yincrement;
		xstart = -1;
		txu = 0;
		txv += txvinc;
	}

	txv = 0;

	ystart = 1;
	xstart = 1;
	for (int y = 0; y < resolution; y++)
	{
		for (int x = 0; x < resolution; x++)
		{
			vertices[v].pos = XMFLOAT3(-1.0f, ystart - yincrement, xstart);
			vertices[v].texCoord = XMFLOAT2(txu, txv + txvinc);
			vertices[v].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			vertices[v].pos = XMFLOAT3(-1.0f, ystart, xstart - xincrement);
			vertices[v].texCoord = XMFLOAT2(txu + txuinc, txv);
			vertices[v].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			vertices[v].pos = XMFLOAT3(-1.0f, ystart, xstart);
			vertices[v].texCoord = XMFLOAT2(txu, txv);
			vertices[v].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			vertices[v].pos = XMFLOAT3(-1.0f, ystart - yincrement, xstart);
			vertices[v].texCoord = XMFLOAT2(txu, txv + txvinc);
			vertices[v].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			vertices[v].pos = XMFLOAT3(-1.0f, ystart - yincrement, xstart - xincrement);
			vertices[v].texCoord = XMFLOAT2(txu + txuinc, txv + txvinc);
			vertices[v].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			vertices[v].pos = XMFLOAT3(-1.0f, ystart, xstart - xincrement);
			vertices[v].texCoord = XMFLOAT2(txu + txuinc, txv);
			vertices[v].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			xstart -= xincrement;
			txu += txuinc;
		}

		ystart -= yincrement;
		xstart = 1;
		txu = 0;
		txv += txvinc;
	}

	txv = 0;

	ystart = 1;
	xstart = -1;

	for (int y = 0; y < resolution; y++)
	{
		for (int x = 0; x < resolution; x++)
		{
			vertices[v].pos = XMFLOAT3(xstart, 1.0f, ystart - yincrement);
			vertices[v].texCoord = XMFLOAT2(txu, txv + txvinc);
			vertices[v].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			vertices[v].pos = XMFLOAT3(xstart + xincrement, 1.0f, ystart);
			vertices[v].texCoord = XMFLOAT2(txu + txuinc, txv);
			vertices[v].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			vertices[v].pos = XMFLOAT3(xstart, 1.0f, ystart);  // Top left.	-1.0, 1.0
			vertices[v].texCoord = XMFLOAT2(txu, txv);
			vertices[v].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			//0
			vertices[v].pos = XMFLOAT3(xstart, 1.0f, ystart - yincrement);  // Bottom left. -1. -1. 0
			vertices[v].texCoord = XMFLOAT2(txu, txv + txvinc);
			vertices[v].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			//3
			vertices[v].pos = XMFLOAT3(xstart + xincrement, 1.0f, ystart - yincrement);  // Bottom right.	1.0, -1.0, 0.0
			vertices[v].texCoord = XMFLOAT2(txu + txuinc, txv + txvinc);
			vertices[v].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			//2
			vertices[v].pos = XMFLOAT3(xstart + xincrement, 1.0f, ystart);  // Top right.	1.0, 1.0 0.0
			vertices[v].texCoord = XMFLOAT2(txu + txuinc, txv);
			vertices[v].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			// increment
			xstart += xincrement;
			//ystart -= yincrement;
			txu += txuinc;
		}

		ystart -= yincrement;
		xstart = -1;
		txu = 0;
		txv += txvinc;
	}

	txv = 0;

	//bottom face
	ystart = -1;
	xstart = -1;

	for (int y = 0; y < resolution; y++)	// for each quad in the y direction
	{
		for (int x = 0; x < resolution; x++)	// for each quad in the x direction
		{
			// Load the vertex array with data.
			//0
			vertices[v].pos = XMFLOAT3(xstart, -1.0f, ystart + yincrement);  // Bottom left. -1. -1. 0
			vertices[v].texCoord = XMFLOAT2(txu, txv + txvinc);
			vertices[v].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			//2
			vertices[v].pos = XMFLOAT3(xstart + xincrement, -1.0f, ystart);  // Top right.	1.0, 1.0 0.0
			vertices[v].texCoord = XMFLOAT2(txu + txuinc, txv);
			vertices[v].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			//1
			vertices[v].pos = XMFLOAT3(xstart, -1.0f, ystart);  // Top left.	-1.0, 1.0
			vertices[v].texCoord = XMFLOAT2(txu, txv);
			vertices[v].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			//0
			vertices[v].pos = XMFLOAT3(xstart, -1.0f, ystart + yincrement);  // Bottom left. -1. -1. 0
			vertices[v].texCoord = XMFLOAT2(txu, txv + txvinc);
			vertices[v].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			//3
			vertices[v].pos = XMFLOAT3(xstart + xincrement, -1.0f, ystart + yincrement);  // Bottom right.	1.0, -1.0, 0.0
			vertices[v].texCoord = XMFLOAT2(txu + txuinc, txv + txvinc);
			vertices[v].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			//2
			vertices[v].pos = XMFLOAT3(xstart + xincrement, -1.0f, ystart);  // Top right.	1.0, 1.0 0.0
			vertices[v].texCoord = XMFLOAT2(txu + txuinc, txv);
			vertices[v].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

			indices[i] = i;
			v++;
			i++;

			// increment
			xstart += xincrement;
			//ystart -= yincrement;
			txu += txuinc;
		}

		ystart += yincrement;
		xstart = -1;
		txu = 0;
		txv += txvinc;
	}

	
	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex3D) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	// Now create the vertex buffer.
	device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;
	// Create the index buffer.
	device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);

	// Release the arrays now that the vertex and index buffers have been created and loaded.

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;
}

int CubeMesh::GetIndexCount()
{
	return indexCount;
}