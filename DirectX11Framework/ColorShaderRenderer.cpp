#include "ColorShaderRenderer.h"

ColorShaderRenderer::ColorShaderRenderer() :
	_vertexShader(nullptr),
	_pixelShader(nullptr),
	_inputLayout(nullptr)

{
}

bool ColorShaderRenderer::Initialize(ID3D11Device* device)
{
	size_t VSSize, PSSize;
	char *VS = nullptr, *PS = nullptr;

	const D3D11_INPUT_ELEMENT_DESC colorShaderVertexInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	    if ((VS = ReadShaderFile("ColorShader_VS.cso", VSSize)) == nullptr)
		return false;

		if ((PS = ReadShaderFile("ColorShader_PS.cso", PSSize)) == nullptr)
		return false;

	device->CreateVertexShader(VS, VSSize, nullptr, &_vertexShader);
	device->CreatePixelShader(PS, PSSize, nullptr, &_pixelShader);
	device->CreateInputLayout(colorShaderVertexInputLayout, 2, VS, VSSize, &_inputLayout);

	SAFE_DELETE_A(VS);
	SAFE_DELETE_A(PS);

	return true;
}

void ColorShaderRenderer::Shutdown()
{
	SAFE_RELEASE(_vertexShader);
	SAFE_RELEASE(_pixelShader);
	SAFE_RELEASE(_inputLayout);
}

void ColorShaderRenderer::OnFrameRender(ID3D11DeviceContext * deviceContext, int indexCount)
{
	deviceContext->IASetInputLayout(_inputLayout);

	deviceContext->VSSetShader(_vertexShader, NULL, 0);
	deviceContext->PSSetShader(_pixelShader, NULL, 0);

	deviceContext->DrawIndexed(indexCount, 0, 0);
}
