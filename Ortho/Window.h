#pragma once

namespace gm
{
	class WindowClass;
	class Application;

	class Window final
	{
	public:
		explicit Window(const std::wstring& title);
		~Window();
		BOOL Initialize([[maybe_unused]] const gm::Application* pApplication, int nCmdShow, const int width = 1280, const int height = 960);
		void SetFactory(ID2D1Factory* pD2DFactory);
		HRESULT CreateRenderTarget();
		HRESULT CreateSolidColorBrush(D2D1::ColorF color = D2D1::ColorF::White);
		void InvalidateRectangle();
		ID2D1HwndRenderTarget* GetRenderTarget();
		ID2D1SolidColorBrush* GetSolidColorBrush();
	private:
		const std::wstring mTitle;
		HWND mhWnd;
		ID2D1Factory* mpD2DFactory;
		ID2D1HwndRenderTarget* mpRenderTarget;
		ID2D1SolidColorBrush* mpSolidColorBrush;
	};
}
