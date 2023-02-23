#include "pch.h"
#include "Application.h"

int APIENTRY wWinMain(_In_ [[maybe_unused]] HINSTANCE hInstance, _In_opt_ [[maybe_unused]] HINSTANCE hPrevInstance, _In_ [[maybe_unused]] LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	MSG msg{};
	bool doesEnd{ false };
	gm::Application application{ nCmdShow };

	timeBeginPeriod(1);
	std::chrono::steady_clock::time_point start{};
	std::chrono::duration<double, std::nano> endMinusStart{}, frameDuration{ 10000000. };
	while (!doesEnd)
	{
		start = std::chrono::steady_clock::now();
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				doesEnd = true;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		application.Input(frameDuration.count());
		application.OnRender();
		endMinusStart = std::chrono::steady_clock::now() - start;
		if (frameDuration.count() - endMinusStart.count() > 0)
			std::this_thread::sleep_for(frameDuration - endMinusStart);
	}
	timeEndPeriod(1);
	return (int)msg.wParam;
}
