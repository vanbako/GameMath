#ifdef _WIN32
#include "Application.h"

int APIENTRY wWinMain(_In_ [[maybe_unused]] HINSTANCE hInstance,
                      _In_opt_ [[maybe_unused]] HINSTANCE hPrevInstance,
                      _In_ [[maybe_unused]] LPWSTR lpCmdLine, _In_ int nCmdShow)
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
    return static_cast<int>(msg.wParam);
}
#else
#include <SDL.h>
#include <array>
#include <vector>
#include <cmath>
#include <numbers>
#include "Math.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        return 1;

    SDL_Window* window = SDL_CreateWindow(
        "GameMath 03: Object To World",
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

    // Generate cylinder geometry
    float radius = 100.f;
    float height = 300.f;
    uint32_t segments = 16;
    std::vector<std::array<float, 4>> vertices;
    std::vector<std::array<uint32_t, 3>> triangles;

    uint32_t cnt = segments;
    vertices.reserve(cnt * 2 + 2);
    triangles.reserve(cnt * 4);
    vertices.push_back({ 0.f, 0.f, 0.f, 1.f });
    vertices.push_back({ 0.f, height, 0.f, 1.f });
    float x{}, z{};
    float totalAngle = 0.f;
    float segmentAngle = 2.f * std::numbers::pi_v<float> / static_cast<float>(cnt);
    vertices.push_back({ radius, 0.f, 0.f, 1.f });
    vertices.push_back({ radius, height, 0.f, 1.f });
    uint32_t oldBottom = 2, oldTop = 3, newBottom = 4, newTop = 5;
    for (uint32_t i = 1; i < cnt; ++i)
    {
        totalAngle += segmentAngle;
        x = std::cos(totalAngle) * radius;
        z = std::sin(totalAngle) * radius;
        vertices.push_back({ x, 0.f, z, 1.f });
        vertices.push_back({ x, height, z, 1.f });
        triangles.push_back({ 0, newBottom, oldBottom });
        triangles.push_back({ 1, oldTop, newTop });
        triangles.push_back({ oldBottom, newBottom, oldTop });
        triangles.push_back({ oldTop, newBottom, newTop });
        oldBottom += 2;
        oldTop += 2;
        newBottom += 2;
        newTop += 2;
    }
    triangles.push_back({ 0, 2, oldBottom });
    triangles.push_back({ 1, oldTop, 3 });
    triangles.push_back({ oldBottom, 2, oldTop });
    triangles.push_back({ oldTop, 2, 3 });

    // Object to world transform
    float scale = 0.5f;
    std::array<float, 3> translation{ 0.f, 0.f, 0.f };
    std::array<float, 3> rotation{
        std::numbers::pi_v<float> / 6.f,
        0.f,
        std::numbers::pi_v<float> / 6.f };

    std::array<float, 16> objectToWorld{
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f };

    objectToWorld[0] = scale;
    objectToWorld[5] = scale;
    objectToWorld[10] = scale;

    float c = std::cos(rotation[0]);
    float s = std::sin(rotation[0]);
    std::array<float, 16> r{
        1.f, 0.f, 0.f, 0.f,
        0.f,   c,   s, 0.f,
        0.f,  -s,   c, 0.f,
        0.f, 0.f, 0.f, 1.f };
    objectToWorld = gm::MatrixMultiply(objectToWorld, r);

    c = std::cos(rotation[1]);
    s = std::sin(rotation[1]);
    r = {
          c, 0.f,  -s, 0.f,
        0.f, 1.f, 0.f, 0.f,
          s, 0.f,   c, 0.f,
        0.f, 0.f, 0.f, 1.f };
    objectToWorld = gm::MatrixMultiply(objectToWorld, r);

    c = std::cos(rotation[2]);
    s = std::sin(rotation[2]);
    r = {
          c,   s, 0.f, 0.f,
         -s,   c, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f };
    objectToWorld = gm::MatrixMultiply(objectToWorld, r);

    r = {
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        translation[0], translation[1], translation[2], 1.f };
    objectToWorld = gm::MatrixMultiply(objectToWorld, r);

    std::vector<std::array<float, 4>> transformed;
    transformed.reserve(vertices.size());
    for (const auto& v : vertices)
        transformed.emplace_back(gm::MatrixMultiply(v, objectToWorld));

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    int winW = 0, winH = 0;
    SDL_GetRendererOutputSize(renderer, &winW, &winH);
    float cx = winW / 2.f;
    float cy = winH / 2.f;

    auto draw_line = [&](const std::array<float,4>& a, const std::array<float,4>& b)
    {
        SDL_RenderDrawLine(renderer,
            static_cast<int>(a[0] + cx), static_cast<int>(cy - a[1]),
            static_cast<int>(b[0] + cx), static_cast<int>(cy - b[1]));
    };

    for (const auto& tri : triangles)
    {
        draw_line(transformed[tri[0]], transformed[tri[1]]);
        draw_line(transformed[tri[1]], transformed[tri[2]]);
        draw_line(transformed[tri[2]], transformed[tri[0]]);
    }

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
