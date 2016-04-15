#pragma once

#include "WinSDKVer.h"

#define _WIN32_WINNT_WIN32_WINNT_WIN76

#include "DXManager.h"

#include "TriangleObject.h"
#include "ColorShaderRenderer.h"

class Core
{
public:
	Core();
	int Run(void);
	bool Initialize(void);
	void Shutdown(void);

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	HWND _hwnd;
	HINSTANCE _hinst;
	bool _isRunning;
	TriangleObject* _triangle;
	ColorShaderRenderer* _colorShaderRenderer;

	DXManager* _DXManager;

	bool CreateMainWindow(void);
	void DestroyMainWindow(void);
	void OnFrameRender();

};

static Core* kCoreHandle = nullptr;
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static const short kWidth = 800;
static const short kHeight = 600;
static const TCHAR *kTitle = L"MainFrame";
static const TCHAR *kWindowClass = L"win32app";