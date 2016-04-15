#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Macros.h"

using namespace DirectX;

class TriangleObject
{
public:
	TriangleObject();
	bool Initialize(ID3D11Device* device);
	void Shutdown();

	int GetIndexCount();
	void OnFrameRender(ID3D11DeviceContext* deviceContext);

private:
	ID3D11Buffer* _vertexBuffer;
	ID3D11Buffer* _indexBuffer;

	int _vertexCount;
	int _indexCount;

	struct ColorShaderVertex
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};
};