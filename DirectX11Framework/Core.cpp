#include "Core.h"
#include <iostream>

Core::Core() :
	_hwnd(nullptr),
	_hinst(nullptr),
	_isRunning(true),
	_DXManager(nullptr),
	_triangle(nullptr),
	_colorShaderRenderer(nullptr)
{
}

int Core::Run(void)
{
	MSG msg;
	while (_isRunning)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT) _isRunning = false;


		}
		else
		{
			OnFrameRender();
		}
	}
	return (int)msg.wParam;
}

bool Core::Initialize(void)
{
	_DXManager = new DXManager();

	kCoreHandle = this;

	if (!CreateMainWindow()) return false;
	ShowWindow(_hwnd, SW_SHOW);

	if (!_DXManager->Initialize(_hwnd, kWidth, kHeight)) return false;

	_triangle = new TriangleObject();
	if (!_triangle->Initialize(_DXManager->GetDevice())) return false;

	_colorShaderRenderer = new ColorShaderRenderer();
	if (!_colorShaderRenderer->Initialize(_DXManager->GetDevice())) return false;

	return true;
}

void Core::Shutdown(void)
{
	DestroyMainWindow();
	kCoreHandle = nullptr;
	SAFE_SHUTDOWN(_DXManager);
}

LRESULT Core::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

bool Core::CreateMainWindow(void)
{
	_hinst = GetModuleHandle(NULL);

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = _hinst;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = kWindowClass;
	wcex.hIconSm = nullptr;

	if (!RegisterClassEx(&wcex)) return false;

	_hwnd = CreateWindow(kWindowClass, kTitle, WS_OVERLAPPEDWINDOW, 0, 0, kWidth, kHeight, nullptr, nullptr, _hinst, nullptr);
	
	if (!_hwnd) return false;

	return true;

}

void Core::DestroyMainWindow(void)
{
	DestroyWindow(_hwnd);
	UnregisterClass(kTitle, _hinst);
}

void Core::OnFrameRender()
{
	_DXManager->StartFrameRender();
	_triangle->OnFrameRender(_DXManager->GetDeviceContext());
	_colorShaderRenderer->OnFrameRender(_DXManager->GetDeviceContext(), _triangle->GetIndexCount());
	_DXManager->EndFrameRender();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if ((message == WM_KEYDOWN) && (wParam == VK_ESCAPE)) PostQuitMessage(EXIT_SUCCESS);

	switch (message)
	{
	case WM_QUIT:
		break;
	case WM_DESTROY: PostQuitMessage(EXIT_SUCCESS);
		break;
	default: return kCoreHandle->MessageHandler(hWnd, message, wParam, lParam);
	}

	return 0;
}
