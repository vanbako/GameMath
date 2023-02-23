#pragma once
#include "Window.h"
#include "Concepts.h"

namespace gm
{
	class Application final
	{
	public:
		explicit Application(int nCmdShow);
		~Application();
		gm::WindowClass* GetWindowClass() const;
		gm::Window* GetWindow();
		void OnRender();
		void OnResize(UINT width, UINT height);
	private:
		gm::WindowClass* mpWindowClass;
		gm::Window mWindow;
		ID2D1Factory* mpD2DFactory;
	};
}
