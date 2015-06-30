#pragma once

#include <type_traits>

namespace TIS
{
	namespace Util
	{
		template <typename X, typename Y, typename Z>
		typename std::common_type<X, Y, Z>::type
		clamp(X in, Y low, Z high)
		{
			if (in < low)
				return low;

			if (in > high)
				return high;

			return in;
		}
	}
}
