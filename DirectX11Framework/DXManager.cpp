#include "DXManager.h"

DXManager::DXManager() :
	_device(nullptr),
	_deviceContext(nullptr),
	_swapChain(nullptr),
	_renderTargetView(nullptr),
	_depthStencilBuffer(nullptr),
	_depthStencilView(nullptr)
{
	_clearColor[0] = 0.0f;
	_clearColor[1] = 0.0f;
	_clearColor[2] = 0.0f;
	_clearColor[3] = 1.f;
}

void DXManager::Shutdown()
{
	SAFE_RELEASE(_renderTargetView);
	SAFE_RELEASE(_swapChain);
	SAFE_RELEASE(_deviceContext);
	SAFE_RELEASE(_device);
	SAFE_RELEASE(_depthStencilBuffer);
	SAFE_RELEASE(_depthStencilView);
}

bool DXManager::Initialize(HWND hwnd, UINT screenWidth, UINT screenHeight)
{

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = true;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	
	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		&featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&_swapChain,
		&_device,
		nullptr,
		&_deviceContext)))
	{
		return Error("failed to create device and swap chain");
	}

	ID3D11Texture2D* backBufferPtr;
	if (FAILED(_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr)))
	{
		return Error("Failed to recieve back buffer pointer");
	}

	if (FAILED(_device->CreateRenderTargetView(backBufferPtr, nullptr, &_renderTargetView)))
	{
		return Error("failed to create render target view");
	}

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	if (FAILED(_device->CreateTexture2D(&depthBufferDesc, NULL, &_depthStencilBuffer)))
	{
		return Error("failed to create depth stencil buffer");
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	if (FAILED(_device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_depthStencilView)))
	{
		return Error("failed to create depth stencil view");
	}

	_deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

	ZeroMemory(&_viewport, sizeof(_viewport));
	_viewport.Width = static_cast<float>(screenWidth);
	_viewport.Height = static_cast<float>(screenHeight);
	_viewport.MinDepth = 0.0f;
	_viewport.MaxDepth = 1.0f;
	_viewport.TopLeftX = 0.0f;
	_viewport.TopLeftY = 0.0f;

	_deviceContext->RSSetViewports(1, &_viewport);

	backBufferPtr->Release();

	return true;

}

void DXManager::EndFrameRender()
{
	_swapChain->Present(0, 0);
}

void DXManager::StartFrameRender()
{
	_deviceContext->ClearRenderTargetView(_renderTargetView, _clearColor);
}

ID3D11Device * DXManager::GetDevice()
{
	return _device;
}

ID3D11DeviceContext* DXManager::GetDeviceContext()
{
	return _deviceContext;
}
