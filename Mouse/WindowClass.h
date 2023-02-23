#pragma once

namespace gm
{
	class WindowClass final
	{
	public:
		explicit WindowClass(const std::wstring& className);

		const std::wstring& GetClassName() const;
	private:
		std::wstring mClassName;

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};
}
