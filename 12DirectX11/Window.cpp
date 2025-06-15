#include "Window.h"
#include "FixedCamera.h"
#include "MovingCamera.h"

gm::Window::Window(const std::wstring& title, uint32_t width, uint32_t height)
	: mTitle{ title }
	, mWidth{ width }
	, mHeight{ height }
	, mhWnd{}
	//, mpD2DFactory{ nullptr }
	//, mpRenderTarget{ nullptr }
	//, mpSolidColorBrush{ nullptr }
	, mpD3DDevice{ nullptr }
	, mpD3DImmediateContext{ nullptr }
	, mpSwapChain{ nullptr }
	, mpRenderTargetView{ nullptr }
	, mpCamera{ new FixedCamera{ width, height } }
	, mpDepthBuffer{ new float[width * height] }
	, mLightDirection{ -0.57735f, -0.57735f, 0.57735f, 0.f }
{
}

gm::Window::~Window()
{
	//SafeRelease(&mpSolidColorBrush);
	//SafeRelease(&mpRenderTarget);
	if (mpCamera != nullptr)
		delete mpCamera;
	delete[] mpDepthBuffer;
}

BOOL gm::Window::Initialize(const gm::Application* pApplication, const std::wstring& className, const int nCmdShow)
{
	HMODULE hModule{ GetModuleHandle(nullptr) };
	mhWnd = CreateWindowW(
		className.c_str(),
		mTitle.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		mWidth,
		mHeight,
		NULL,
		NULL,
		hModule,
		(LPVOID)pApplication
	);
	if (!mhWnd)
		return FALSE;

	BOOL darkMode{ TRUE };
	DwmSetWindowAttribute(mhWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &darkMode, sizeof(darkMode));
	//COLORREF colorBorder{ GetSysColor(COLOR_HIGHLIGHT) };
	COLORREF colorCaption{ GetSysColor(COLOR_WINDOWTEXT) };
	COLORREF colorText{ GetSysColor(COLOR_WINDOW) };
	//DwmSetWindowAttribute(mhWnd, DWMWA_BORDER_COLOR, &colorBorder, sizeof(colorBorder));
	DwmSetWindowAttribute(mhWnd, DWMWA_CAPTION_COLOR, &colorCaption, sizeof(colorCaption));
	DwmSetWindowAttribute(mhWnd, DWMWA_TEXT_COLOR, &colorText, sizeof(colorText));

	DXGI_SWAP_CHAIN_DESC scd{};
	scd.BufferCount = 1;
	scd.BufferDesc.Width = mWidth;
	scd.BufferDesc.Height = mHeight;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = mhWnd;
	scd.SampleDesc.Count = 1;
	scd.Windowed = TRUE;
	D3D11CreateDeviceAndSwapChain(
		NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0,
		D3D11_SDK_VERSION, &scd, &mpSwapChain, &mpD3DDevice,
		NULL, &mpD3DImmediateContext);

	ShowWindow(mhWnd, nCmdShow);
	UpdateWindow(mhWnd);

	ID3D11Texture2D* backBuffer{};
	mpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	mpD3DDevice->CreateRenderTargetView(backBuffer, NULL, &mpRenderTargetView);
	backBuffer->Release();
	mpD3DImmediateContext->OMSetRenderTargets(1, &mpRenderTargetView, NULL);
	D3D11_VIEWPORT viewport{ CD3D11_VIEWPORT(0.0f, 0.0f, float(mWidth), float(mHeight)) };
	mpD3DImmediateContext->RSSetViewports(1, &viewport);

	return TRUE;
}

//void gm::Window::SetFactory(ID2D1Factory* pD2DFactory)
//{
//	mpD2DFactory = pD2DFactory;
//}

//HRESULT gm::Window::CreateRenderTarget()
//{
//	if (mpD2DFactory == nullptr)
//		return E_POINTER;
//	if (mpRenderTarget != nullptr)
//		return NOERROR;
//	RECT rect{};
//	GetClientRect(mhWnd, &rect);
//	return mpD2DFactory->CreateHwndRenderTarget(
//		D2D1::RenderTargetProperties(),
//		D2D1::HwndRenderTargetProperties(
//			mhWnd,
//			D2D1::SizeU(
//				rect.right - rect.left,
//				rect.bottom - rect.top)
//		),
//		&mpRenderTarget);
//}

void gm::Window::InvalidateRectangle()
{
	if (mhWnd)
		InvalidateRect(mhWnd, NULL, FALSE);
}

//HRESULT gm::Window::CreateSolidColorBrush(D2D1::ColorF color)
//{
//	if (mpRenderTarget == nullptr)
//		return E_POINTER;
//	return mpRenderTarget->CreateSolidColorBrush(color, &mpSolidColorBrush);
//}

//ID2D1HwndRenderTarget* gm::Window::GetRenderTarget()
//{
//	return mpRenderTarget;
//}

//ID2D1SolidColorBrush* gm::Window::GetSolidColorBrush()
//{
//	return mpSolidColorBrush;
//}

gm::Camera* gm::Window::GetCamera()
{
	return mpCamera;
}

void gm::Window::SetFixedCamera(uint32_t width, uint32_t height)
{
	if (mpCamera != nullptr)
		delete mpCamera;
	mpCamera = new FixedCamera{ width, height };
}

void gm::Window::SetMovingCamera(uint32_t width, uint32_t height)
{
	if (mpCamera != nullptr)
		delete mpCamera;
	mpCamera = new MovingCamera{ width, height };
}

void gm::Window::Resize(uint32_t width, uint32_t height)
{
	mWidth = width;
	mHeight = height;
	//if (mpRenderTarget != nullptr)
	//	mpRenderTarget->Resize(D2D1::SizeU(mWidth, mHeight));
	if (mpRenderTargetView != nullptr)
	{
		mpRenderTargetView->Release();
		mpSwapChain->ResizeBuffers(0, mWidth, mHeight, DXGI_FORMAT_UNKNOWN, 0);
		ID3D11Texture2D* backBuffer{};
		mpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
		mpD3DDevice->CreateRenderTargetView(backBuffer, NULL, &mpRenderTargetView);
		backBuffer->Release();
		mpD3DImmediateContext->OMSetRenderTargets(1, &mpRenderTargetView, NULL);
		D3D11_VIEWPORT viewport{ CD3D11_VIEWPORT(0.0f, 0.0f, float(mWidth), float(mHeight)) };
		mpD3DImmediateContext->RSSetViewports(1, &viewport);
	}
	if (mpCamera != nullptr)
		mpCamera->SetAspectRatio(width, height);
	delete[] mpDepthBuffer;
	mpDepthBuffer = new float[mWidth * mHeight];
}

float* gm::Window::GetDepthBuffer()
{
	return mpDepthBuffer;
}

void gm::Window::ClearDepthBuffer()
{
	for (uint32_t i{ 0 }; i < mWidth * mHeight; ++i)
		mpDepthBuffer[i] = 1000000.0f;
}

const std::array<float, 4>& gm::Window::GetLightDirection()
{
	return mLightDirection;
}

void gm::Window::Present()
{
	mpSwapChain->Present(1, 0);
}
