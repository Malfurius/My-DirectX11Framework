#pragma once

#include <d3d11.h>
#include "Macros.h"

#pragma comment (lib, "d3d11.lib")

static bool Error(const LPSTR msg)
{
	MessageBoxA(nullptr, msg, "Error", MB_ICONERROR);
	return false;
}

class DXManager
{
public:
	DXManager();
	void Shutdown();
	bool Initialize(HWND hwnd, UINT screenWidth, UINT screenHeight);

	void EndFrameRender();
	void StartFrameRender();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

private:
	float _clearColor[4];
	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;

	IDXGISwapChain* _swapChain;
	ID3D11RenderTargetView* _renderTargetView;

	ID3D11Texture2D* _depthStencilBuffer;
	ID3D11DepthStencilView* _depthStencilView;
	D3D11_VIEWPORT _viewport;
};