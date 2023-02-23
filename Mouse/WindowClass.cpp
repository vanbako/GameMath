#include "pch.h"
#include "WindowClass.h"
#include "Application.h"

LRESULT CALLBACK gm::WindowClass::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs{ (LPCREATESTRUCT)lParam };
		gm::Application* pApplication{ (gm::Application*)pcs->lpCreateParams };
		SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pApplication));
	}
	else
	{
		gm::Application* pApplication{ reinterpret_cast<gm::Application*>(static_cast<LONG_PTR>(GetWindowLongPtrW(hWnd, GWLP_USERDATA))) };
		if (pApplication != nullptr)
			switch (message)
			{
			case WM_SIZE:
				pApplication->OnResize(LOWORD(lParam), HIWORD(lParam));
				break;
			//case WM_MOUSEMOVE:
			//	InvalidateRect(hWnd, NULL, FALSE);
			//	break;
			//case WM_PAINT:
			//	pApplication->OnRender();
			//	break;
			case WM_DESTROY:
				PostQuitMessage(0);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

gm::WindowClass::WindowClass(const std::wstring& className)
	: mClassName{ className }
{
	HMODULE hModule{ GetModuleHandle(nullptr) };
	WNDCLASSEXW wcex{};
	wcex.cbSize = sizeof(WNDCLASSEXW);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = gm::WindowClass::WndProc;
	wcex.cbClsExtra = sizeof(LPVOID);
	wcex.cbWndExtra = 0;
	wcex.hInstance = hModule;
	wcex.hIcon = LoadIcon(0, IDI_APPLICATION);
	wcex.hCursor = (HCURSOR)LoadImageW(nullptr, MAKEINTRESOURCE(OCR_NORMAL), IMAGE_CURSOR, 0, 0, LR_SHARED);
	wcex.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);
	wcex.lpszClassName = mClassName.c_str();
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"GameMath";
	[[maybe_unused]] ATOM wc{ RegisterClassExW(&wcex) };
}

const std::wstring& gm::WindowClass::GetClassName() const
{
	return mClassName;
}
