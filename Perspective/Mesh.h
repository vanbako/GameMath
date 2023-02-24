#pragma once
#include "Concepts.h"
#include "MouseButton.h"

namespace gm
{
	template<FloatOrDouble T>
	class Mesh
	{
	public:
		explicit Mesh()
			: mVertices{}
			, mTriangles{}
			, mScale{ .5f }
			, mTranslation{ 0., 0., 0. }
			, mRotation{ 0., 0., 0. }
			, mHasMoved{ true }
			, mTransformedVertices{}
			, mObjectToWorldMatrix{}
		{}
		virtual ~Mesh() = default;

		virtual void Move(const T duration, const gm::MouseButton mouseButton, const POINT& pos)
		{
			switch (mouseButton)
			{
			case gm::MouseButton::Left:
				mTranslation[0] -= T(pos.x);
				mTranslation[1] += T(pos.y);
				mHasMoved = true;
				break;
			case gm::MouseButton::Right:
				mRotation[0] -= T(pos.y) / T(180.);
				mRotation[1] += T(pos.x) / T(180.);
				mHasMoved = true;
				break;
			case gm::MouseButton::Middle:
				mRotation[2] += T(pos.x) / T(180.);
				mHasMoved = true;
				break;
			}
		}
		virtual void Render(ID2D1HwndRenderTarget* pRenderTarget, ID2D1SolidColorBrush* pSolidColorBrush)
		{
			if (mHasMoved)
			{
				mHasMoved = false;
				T c{}, s{};
				std::array<T, 16> r{};
				std::memcpy((void*)mObjectToWorldMatrix.data(), (void*)mIdentityMatrix.data(), 16 * sizeof(T));
				mObjectToWorldMatrix[0] = mScale;
				mObjectToWorldMatrix[5] = mScale;
				mObjectToWorldMatrix[10] = mScale;
				c = std::cos(mRotation[0]);
				s = std::sin(mRotation[0]);
				r = std::array<T, 16>{
					1., 0., 0., 0.,
						0., c, -s, 0.,
						0., s, c, 0.,
						0., 0., 0., 1. };
				mObjectToWorldMatrix = MatrixMultiply(r, mObjectToWorldMatrix);
				c = std::cos(mRotation[1]);
				s = std::sin(mRotation[1]);
				r = std::array<T, 16>{
					c, 0., s, 0.,
						0., 1., 0., 0.,
						-s, 0., c, 0.,
						0., 0., 0., 1. };
				mObjectToWorldMatrix = MatrixMultiply(r, mObjectToWorldMatrix);
				c = std::cos(mRotation[2]);
				s = std::sin(mRotation[2]);
				r = std::array<T, 16>{
					c, -s, 0., 0.,
						s, c, 0., 0.,
						0., 0., 1., 0.,
						0., 0., 0., 1. };
				mObjectToWorldMatrix = MatrixMultiply(r, mObjectToWorldMatrix);
				std::memcpy((void*)r.data(), (void*)mIdentityMatrix.data(), 16 * sizeof(T));
				r[3] = mTranslation[0];
				r[7] = mTranslation[1];
				r[11] = mTranslation[2];
				mObjectToWorldMatrix = MatrixMultiply(r, mObjectToWorldMatrix);
				mTransformedVertices.clear();
				mTransformedVertices.reserve(mVertices.size());
				for (auto& vertex : mVertices)
					mTransformedVertices.emplace_back(MatrixMuliply(mObjectToWorldMatrix, vertex));
			}
			for (auto& triangle : mTriangles)
				RenderTriangle(
					pRenderTarget,
					pSolidColorBrush,
					mTransformedVertices[triangle[0]],
					mTransformedVertices[triangle[1]],
					mTransformedVertices[triangle[2]]
				);
		}
	protected:
		std::vector<std::array<T, 4>> mVertices;
		std::vector<std::array<size_t, 3>> mTriangles;
		T mScale;
		std::array<T, 3> mTranslation;
		std::array<T, 3> mRotation;
	private:
		bool mHasMoved;
		std::vector<std::array<T, 4>> mTransformedVertices;
		std::array<T, 16> mObjectToWorldMatrix;

		static const std::array<T, 16> mIdentityMatrix;

		void RenderTriangle(ID2D1HwndRenderTarget* pRenderTarget, ID2D1SolidColorBrush* pSolidColorBrush, const std::array<T, 4>& v1, const std::array<T, 4>& v2, const std::array<T, 4>& v3)
		{
			D2D1_SIZE_F size{ pRenderTarget->GetSize() };
			T
				x{ T(size.width) / T(2.) },
				y{ T(size.height) / T(2.) };
			pRenderTarget->DrawLine(D2D1_POINT_2F{ v1[0] + x, y - v1[1] }, D2D1_POINT_2F{ v2[0] + x, y - v2[1] }, pSolidColorBrush);
			pRenderTarget->DrawLine(D2D1_POINT_2F{ v2[0] + x, y - v2[1] }, D2D1_POINT_2F{ v3[0] + x, y - v3[1] }, pSolidColorBrush);
			pRenderTarget->DrawLine(D2D1_POINT_2F{ v3[0] + x, y - v3[1] }, D2D1_POINT_2F{ v1[0] + x, y - v1[1] }, pSolidColorBrush);
		}
		//std::array<T, 16> MatrixMultiply(const std::array<T, 16>& matrix, const T scalar)
		//{
		//	for (int i{ 0 }; i < 15; ++i)
		//		matrix[i] *= scalar;
		//}
		std::array<T, 4> MatrixMuliply(const std::array<T, 16>& matrix, const std::array<T, 4>& vertex)
		{
			std::array<T, 4> result{};
			result[0] = matrix[0] * vertex[0] + matrix[1] * vertex[1] + matrix[2] * vertex[2] + matrix[3] * vertex[3];
			result[1] = matrix[4] * vertex[0] + matrix[5] * vertex[1] + matrix[6] * vertex[2] + matrix[7] * vertex[3];
			result[2] = matrix[8] * vertex[0] + matrix[9] * vertex[1] + matrix[10] * vertex[2] + matrix[11] * vertex[3];
			result[3] = matrix[12] * vertex[0] + matrix[13] * vertex[1] + matrix[14] * vertex[2] + matrix[15] * vertex[3];
			return result;
		}
		std::array<T, 16> MatrixMultiply(const std::array<T, 16>& matrix1, const std::array<T, 16>& matrix2)
		{
			std::array<T, 16> result{};
			for (size_t row{ 0 }; row < 4; ++row)
				for (size_t col{ 0 }; col < 4; ++col)
				{
					T sum{ 0 };
					for (size_t i{ 0 }; i < 4; ++i)
						sum += matrix1[row * 4 + i] * matrix2[i * 4 + col];
					result[row * 4 + col] = sum;
				}
			return result;
		}
	};

	template<FloatOrDouble T>
	const std::array<T, 16> Mesh<T>::mIdentityMatrix{
		1., 0., 0., 0.,
		0., 1., 0., 0.,
		0., 0., 1., 0.,
		0., 0., 0., 1.
	};
}
