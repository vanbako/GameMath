#ifdef _WIN32
#include "Application.h"

int APIENTRY wWinMain(_In_ [[maybe_unused]] HINSTANCE hInstance,
                      _In_opt_ [[maybe_unused]] HINSTANCE hPrevInstance,
                      _In_ [[maybe_unused]] LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    gm::Application application{ nCmdShow };
    return application.Run();
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
        "GameMath 05: Perspective",
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
    uint32_t segments = 12;
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

    float scale = 0.5f;
    std::array<float, 3> translation{ 0.f, 0.f, 0.f };
    std::array<float, 3> rotation{ 0.f, 0.f, 0.f };

    // Camera setup
    std::array<float, 4> camPosition{ 0.f, 0.f, -1000.f, 0.f };
    std::array<float, 4> camLook{ 0.f, 0.f, 1.f, 0.f };
    std::array<float, 4> camUp{ 0.f, 1.f, 0.f, 0.f };
    std::array<float, 4> camRight{ 1.f, 0.f, 0.f, 0.f };

    bool running = true;
    bool relative = false;
    uint32_t buttons = 0;

    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                buttons |= SDL_BUTTON(event.button.button);
                SDL_SetRelativeMouseMode(SDL_TRUE);
                relative = true;
            }
            else if (event.type == SDL_MOUSEBUTTONUP)
            {
                buttons &= ~SDL_BUTTON(event.button.button);
                if (buttons == 0)
                {
                    SDL_SetRelativeMouseMode(SDL_FALSE);
                    relative = false;
                }
            }
            else if (event.type == SDL_MOUSEMOTION && relative)
            {
                int dx = event.motion.xrel;
                int dy = event.motion.yrel;
                if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT))
                {
                    translation[0] += static_cast<float>(dx);
                    translation[1] -= static_cast<float>(dy);
                }
                else if (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT))
                {
                    rotation[0] -= static_cast<float>(dy) / 180.f;
                    rotation[1] -= static_cast<float>(dx) / 180.f;
                }
                else if (buttons & SDL_BUTTON(SDL_BUTTON_MIDDLE))
                {
                    rotation[2] -= static_cast<float>(dx) / 180.f;
                }
            }
        }

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

        int winW = 0, winH = 0;
        SDL_GetRendererOutputSize(renderer, &winW, &winH);
        float aspectRatio = static_cast<float>(winW) / static_cast<float>(winH);
        float fov = std::numbers::pi_v<float> / 3.f;
        float near = 10.f;
        float far = 5000.f;
        float fovTan = std::tan(fov / 2.f);
        std::array<float, 16> viewInv{
            camRight[0], camUp[0], camLook[0], 0.f,
            camRight[1], camUp[1], camLook[1], 0.f,
            camRight[2], camUp[2], camLook[2], 0.f,
            -gm::Dot(camRight, camPosition),
            -gm::Dot(camUp, camPosition),
            -gm::Dot(camLook, camPosition),
            1.f };
        std::array<float, 16> proj{
            1.f / (aspectRatio * fovTan), 0.f, 0.f, 0.f,
            0.f, 1.f / fovTan, 0.f, 0.f,
            0.f, 0.f, far / (far - near), 1.f,
            0.f, 0.f, -(near * far) / (far - near), 0.f };
        std::array<float, 16> wvp = gm::MatrixMultiply(objectToWorld, viewInv);
        wvp = gm::MatrixMultiply(wvp, proj);

        std::vector<std::array<float, 4>> transformed;
        transformed.reserve(vertices.size());
        for (const auto& v : vertices)
        {
            std::array<float,4> tv = gm::MatrixMultiply(v, wvp);
            tv[0] /= tv[3];
            tv[1] /= tv[3];
            tv[2] /= tv[3];
            tv[0] *= (winW / 2.f);
            tv[1] *= (winH / 2.f);
            transformed.emplace_back(tv);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

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
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
#endif
