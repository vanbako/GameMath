#pragma once
#include "Concepts.h"

namespace gm
{
	template<FloatOrDouble T>
	class Mesh
	{
	public:
		explicit Mesh()
			: mVertices{}
			//, mProjectedVertices{}
			, mTriangles{}
		{}
		virtual ~Mesh() = default;

		virtual void Render(ID2D1HwndRenderTarget* pRenderTarget, ID2D1SolidColorBrush* pSolidColorBrush)
		{
			//mProjectedVertices.reserve(mVertices.size());
			//for (auto& vertex : mVertices)
				//mProjectedVertices.emplace_back(MatrixMuliply(mOrthoXY, vertex));
			for (auto& triangle : mTriangles)
				RenderTriangle(
					pRenderTarget,
					pSolidColorBrush,
					//mProjectedVertices[triangle[0]],
					//mProjectedVertices[triangle[1]],
					//mProjectedVertices[triangle[2]]
					mVertices[triangle[0]],
					mVertices[triangle[1]],
					mVertices[triangle[2]]
					);
			//mProjectedVertices.clear();
		}
	protected:
		std::vector<std::array<T, 4>> mVertices;
		//std::vector<std::array<T, 4>> mProjectedVertices;
		std::vector<std::array<size_t, 3>> mTriangles;
	private:
		//static const std::array<T, 16> mOrthoXY;

		void RenderTriangle(ID2D1HwndRenderTarget* pRenderTarget, ID2D1SolidColorBrush* pSolidColorBrush, const std::array<T, 4>& v1, const std::array<T, 4>& v2, const std::array<T, 4>& v3)
		{
			T
				x{ T(640.) },
				y{ T(480.) };
			pRenderTarget->DrawLine(D2D1_POINT_2F{ v1[0] + x, y - v1[1] }, D2D1_POINT_2F{ v2[0] + x, y - v2[1] }, pSolidColorBrush);
			pRenderTarget->DrawLine(D2D1_POINT_2F{ v2[0] + x, y - v2[1] }, D2D1_POINT_2F{ v3[0] + x, y - v3[1] }, pSolidColorBrush);
			pRenderTarget->DrawLine(D2D1_POINT_2F{ v3[0] + x, y - v3[1] }, D2D1_POINT_2F{ v1[0] + x, y - v1[1] }, pSolidColorBrush);
		}
		//std::array<T, 4> MatrixMuliply(const std::array<T, 16>& matrix, const std::array<T, 4> vertex)
		//{
		//	std::array<T, 4> result{};
		//	result[0] = matrix[0] * vertex[0] + matrix[1] * vertex[1] + matrix[2] * vertex[2] + matrix[3] * vertex[3];
		//	result[1] = matrix[4] * vertex[0] + matrix[5] * vertex[1] + matrix[6] * vertex[2] + matrix[7] * vertex[3];
		//	result[2] = matrix[8] * vertex[0] + matrix[9] * vertex[1] + matrix[10] * vertex[2] + matrix[11] * vertex[3];
		//	result[3] = matrix[12] * vertex[0] + matrix[13] * vertex[1] + matrix[14] * vertex[2] + matrix[15] * vertex[3];
		//	return result;
		//}
	};

	//template<FloatOrDouble T>
	//const std::array<T, 16> Mesh<T>::mOrthoXY{
	//	1., 0., 0., 0.,
	//	0., 1., 0., 0.,
	//	0., 0., 0., 0.,
	//	0., 0., 0., 1. };
}
