#pragma once
#include "Window.h"
#include "Concepts.h"
#include "CursorMode.h"

namespace gm
{
	template<FloatOrDouble T>
	class Mesh;

	class Application final
	{
	public:
		explicit Application(int nCmdShow);
		~Application();
		gm::WindowClass* GetWindowClass() const;
		gm::Window* GetWindow();
		int Run();
		void Input(double nanos);
		void Render();
		void OnResize(UINT width, UINT height);
	private:
		gm::WindowClass* mpWindowClass;
		gm::Window mWindow;
		ID2D1Factory* mpD2DFactory;
		std::vector<std::shared_ptr<gm::Mesh<float>>> mMeshesPtr;
		BYTE mKeyboard[256];
		bool mShowCursor;
		gm::CursorMode mCursorMode;
		POINT
			mCursorPos,
			mSaveCursorPos;

		POINT GetCursor();
	};
}
