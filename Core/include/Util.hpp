#pragma once

#include <type_traits>

namespace TIS
{
	namespace Util
	{
		template <typename T, typename U>
		typename std::common_type<T, U>::type 
		clamp(T in, U low, U high)
		{
			if (in < low)
				return low;

			if (in > high)
				return high;

			return in;
		}
	}
}