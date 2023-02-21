#include "pch.h"
#include "Application.h"
#include "WindowClass.h"

gm::Application::Application(int nCmdShow)
	: mpWindowClass{ nullptr }
	, mWindow{ L"GameMath 01: Line" }
	, mpD2DFactory{ nullptr }
{
	mpWindowClass = new gm::WindowClass{ L"GameMath" };
	mWindow.Initialize(this, nCmdShow);
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &mpD2DFactory);
	mWindow.SetFactory(mpD2DFactory);
	mWindow.CreateRenderTarget();
	mWindow.CreateSolidColorBrush();
}

gm::Application::~Application()
{
	SafeRelease(&mpD2DFactory);
	delete mpWindowClass;
}

gm::WindowClass* gm::Application::GetWindowClass() const
{
	return mpWindowClass;
}

gm::Window* gm::Application::GetWindow()
{
	return &mWindow;
}

void gm::Application::OnRender()
{
	ID2D1HwndRenderTarget* pRenderTarget{ mWindow.GetRenderTarget() };
	if (pRenderTarget == nullptr)
		return;
	ID2D1SolidColorBrush* pSolidColorBrush{ mWindow.GetSolidColorBrush() };
	if (pSolidColorBrush == nullptr)
		return;

	pRenderTarget->BeginDraw();
	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

	pRenderTarget->DrawLine({ 320.f, 240.f }, { 960.f, 720.f }, pSolidColorBrush, 1.0f);

	pRenderTarget->EndDraw();
}

void gm::Application::OnResize(UINT width, UINT height)
{
	ID2D1HwndRenderTarget* pRenderTarget{ mWindow.GetRenderTarget() };
	if (pRenderTarget != nullptr)
		pRenderTarget->Resize(D2D1::SizeU(width, height));
}
