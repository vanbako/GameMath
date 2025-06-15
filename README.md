# GameMath

GameMath is a collection of small C++ projects that introduce mathematics and graphics concepts used in game development. Each numbered directory contains a Visual Studio project that builds on the previous example, starting from drawing a single line and gradually moving towards a basic software rasterizer and a small DirectX 11 demo.

## Directory overview

- `01Line` – draws a line using Direct2D
- `02Ortho` – adds simple meshes
- `03ObjectToWorld` – applies object to world transforms
- `04Mouse` – handles mouse input
- `05Perspective` – introduces perspective projection
- `06BackfaceCulling` – culls back faces
- `07Camera` – basic camera movement
- `08Rasterizer` – simple software rasterizer
- `09Barycentric` – barycentric coordinate interpolation
- `10NormalInterpolation` – per‑vertex normal interpolation
- `11Texture` – texture mapping
- `12DirectX11` – DirectX 11 renderer
- `Shared` – common utility classes used by the projects
- `Course` – LaTeX source for the accompanying course notes

## Building

The projects are intended for Windows and require Visual Studio 2022 or later. Open `GameMath.sln` and build the desired example. Some early projects also include `CMakeLists.txt` for alternative builds.

## License

This repository is distributed under the terms of the GNU General Public License v3.0. See [`LICENSE`](LICENSE) for details.
