#include "TriangleObject.h"

TriangleObject::TriangleObject():
	_vertexBuffer(nullptr),
	_indexBuffer(nullptr)
{
	_vertexCount = 3;
	_indexCount = 3;
}

bool TriangleObject::Initialize(ID3D11Device* device)
{
	ColorShaderVertex* vertices = new ColorShaderVertex[_vertexCount];
	unsigned long* indices = new unsigned long[_indexCount];

	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);

	vertices[0].color = XMFLOAT4(0.0f, 0.15f, 0.55f, 1.0f);
	vertices[1].color = XMFLOAT4(0.40f, 0.75f, 0.90f, 1.0f);
	vertices[2].color = XMFLOAT4(0.0f, 0.15f, 0.55f, 1.0f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(ColorShaderVertex) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer)))
		return false;

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory(&indexData, sizeof(indexData));
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer)))
		return false;

	SAFE_DELETE_A(vertices);
	SAFE_DELETE_A(indices);

	return true;

}

void TriangleObject::Shutdown()
{
	SAFE_RELEASE(_vertexBuffer);
	SAFE_RELEASE(_indexBuffer);
}

int TriangleObject::GetIndexCount()
{
	return _indexCount;
}

void TriangleObject::OnFrameRender(ID3D11DeviceContext * deviceContext)
{
	unsigned int stride = sizeof(ColorShaderVertex);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
