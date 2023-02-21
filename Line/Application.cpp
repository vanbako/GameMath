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

	pRenderTarget->DrawLine({ 100.f, 100.f }, { 300.f, 200.f }, pSolidColorBrush, 1.0f);

	//D2D1_SIZE_F rtSize{ pRenderTarget->GetSize() };
	//int
	//	width{ static_cast<int>(rtSize.width) },
	//	height{ static_cast<int>(rtSize.height) };
	//
	//for (int x = 0; x < width; x += 10)
	//{
	//	pRenderTarget->DrawLine(
	//		D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
	//		D2D1::Point2F(static_cast<FLOAT>(x), rtSize.height),
	//		pSolidColorBrush,
	//		0.5f
	//	);
	//}
	//for (int y = 0; y < height; y += 10)
	//{
	//	pRenderTarget->DrawLine(
	//		D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
	//		D2D1::Point2F(rtSize.width, static_cast<FLOAT>(y)),
	//		pSolidColorBrush,
	//		0.5f
	//	);
	//}

	pRenderTarget->EndDraw();
}

void gm::Application::OnResize(UINT width, UINT height)
{
	ID2D1HwndRenderTarget* pRenderTarget{ mWindow.GetRenderTarget() };
	if (pRenderTarget != nullptr)
		pRenderTarget->Resize(D2D1::SizeU(width, height));
}
