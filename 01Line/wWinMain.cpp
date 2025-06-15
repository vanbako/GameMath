#ifdef _WIN32
#include "Application.h"

int APIENTRY wWinMain(_In_ [[maybe_unused]] HINSTANCE hInstance, _In_opt_ [[maybe_unused]] HINSTANCE hPrevInstance, _In_ [[maybe_unused]] LPWSTR lpCmdLine, _In_ int nCmdShow)
{
        MSG msg{};
        {
                gm::Application application{ nCmdShow };

                while (GetMessage(&msg, nullptr, 0, 0))
                {
                        if (msg.message == WM_DWMCOMPOSITIONCHANGED)
                                application.GetWindow()->InvalidateRectangle();
                        TranslateMessage(&msg);
                        DispatchMessage(&msg);
                }
        }
        return (int)msg.wParam;
}
#else
#include <SDL.h>

int main(int argc, char** argv)
{
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
                return 1;

        SDL_Window* window = SDL_CreateWindow(
                "GameMath 01: Line",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                1280,
                960,
                0);

        if (!window)
        {
                SDL_Quit();
                return 1;
        }

        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, 320, 240, 960, 720);
        SDL_RenderPresent(renderer);

        SDL_Event event;
        bool running = true;
        while (running)
        {
                while (SDL_PollEvent(&event))
                        if (event.type == SDL_QUIT)
                                running = false;
                SDL_Delay(16);
        }

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
}
#endif
