#pragma once

#include <d3d11.h>
#include "Macros.h"
#include <fstream>

class ColorShaderRenderer
{
public:
	ColorShaderRenderer();
	bool Initialize(ID3D11Device* device);
	void Shutdown();

	void OnFrameRender(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	ID3D11InputLayout* _inputLayout;
};

using namespace std;
static char* ReadShaderFile(const char* filename, size_t &size)
{
	ifstream inFile;
	inFile.open(filename, ios::in | ios::binary | ios::ate);

	if (inFile.is_open())
	{
		inFile.seekg(0, ios::end);
		size = (size_t)inFile.tellg();
		inFile.seekg(0, ios::beg);

		char *byteArray = new char[size];
		inFile.read(byteArray, size);
		return byteArray;
	}

	return nullptr;
}