#pragma once
#include "Mesh.h"

namespace gm
{
	template<FloatOrDouble T, size_t C> requires (C >= 3)
	class Cylinder final
		: public Mesh<T>
	{
	public:
		Cylinder(T radius = .5, T height = 1.)
			: Mesh<T>()
			, mRadius{ radius }
			, mHeight{ height }
		{
			size_t cnt{ C };
			Mesh<T>::mVertices.reserve(cnt * 2 + 2);
			Mesh<T>::mTriangles.reserve(cnt * 4);
			Mesh<T>::mVertices.emplace_back(std::array<T, 4>{ 0., 0., 0., 1.});
			Mesh<T>::mVertices.emplace_back(std::array<T, 4>{ 0., mHeight, 0., 1.});
			T
				x{}, z{},
				totalAngle{ 0. },
				segmentAngle{ T(2.) * std::numbers::pi_v<T> / T(cnt) };
			Mesh<T>::mVertices.emplace_back(std::array<T, 4>{ mRadius, 0., 0, 1. });
			Mesh<T>::mVertices.emplace_back(std::array<T, 4>{ mRadius, mHeight, 0, 1. });
			size_t
				oldBottom{ 2 }, oldTop{ 3 },
				newBottom{ 4 }, newTop{ 5 };
			for (size_t i{ 1 }; i < cnt; ++i)
			{
				totalAngle += segmentAngle;
				x = std::cos(totalAngle) * mRadius;
				z = std::sin(totalAngle) * mRadius;
				Mesh<T>::mVertices.emplace_back(std::array<T, 4>{ x, 0., z, 1. });
				Mesh<T>::mVertices.emplace_back(std::array<T, 4>{ x, mHeight, z, 1. });
				Mesh<T>::mTriangles.emplace_back(std::array<size_t, 3>{ 0, oldBottom, newBottom });
				Mesh<T>::mTriangles.emplace_back(std::array<size_t, 3>{ 0, oldTop, newTop });
				Mesh<T>::mTriangles.emplace_back(std::array<size_t, 3>{ oldBottom, newBottom, oldTop });
				Mesh<T>::mTriangles.emplace_back(std::array<size_t, 3>{ oldTop, newBottom, newTop });
				oldBottom += 2;
				oldTop += 2;
				newBottom += 2;
				newTop += 2;
			}
			Mesh<T>::mTriangles.emplace_back(std::array<size_t, 3>{ 0, oldBottom, 2 });
			Mesh<T>::mTriangles.emplace_back(std::array<size_t, 3>{ 0, oldTop, 3 });
			Mesh<T>::mTriangles.emplace_back(std::array<size_t, 3>{ oldBottom, 2, oldTop });
			Mesh<T>::mTriangles.emplace_back(std::array<size_t, 3>{ oldTop, 2, 3 });
		}
	private:
		T mRadius;
		T mHeight;
		uint32_t mSegmentCount;
	};
}
