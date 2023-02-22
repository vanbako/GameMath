#pragma once

namespace gm
{
	template<class T>
	concept FloatOrDouble = std::same_as<T, float> || std::same_as<T, double>;
}
