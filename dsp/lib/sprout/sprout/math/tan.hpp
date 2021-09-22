/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_MATH_TAN_HPP
#define SPROUT_MATH_TAN_HPP

#include <type_traits>
#include <sprout/config.hpp>
#include <sprout/limits.hpp>
#include <sprout/math/detail/config.hpp>
#include <sprout/math/detail/float_compute.hpp>
#include <sprout/math/isnan.hpp>
#include <sprout/math/cos.hpp>
#include <sprout/math/sin.hpp>
#include <sprout/type_traits/enabler_if.hpp>

namespace sprout {
	namespace math {
		namespace detail {
#if SPROUT_USE_BUILTIN_CMATH_FUNCTION
			inline SPROUT_CONSTEXPR float
			builtin_tan(float x) {
				return __builtin_tanf(x);
			}
			inline SPROUT_CONSTEXPR double
			builtin_tan(double x) {
				return __builtin_tan(x);
			}
			inline SPROUT_CONSTEXPR long double
			builtin_tan(long double x) {
				return __builtin_tanl(x);
			}
#endif

			template<typename T>
			inline SPROUT_CONSTEXPR T
			tan_impl(T x) {
				return sprout::math::sin(x) / sprout::math::cos(x);
			}
		}	// namespace detail
		//
		// tan
		//
		template<
			typename FloatType,
			typename sprout::enabler_if<std::is_floating_point<FloatType>::value>::type = sprout::enabler
		>
		inline SPROUT_CONSTEXPR FloatType
		tan(FloatType x) {
			return sprout::math::isnan(x) ? x
				: x == sprout::numeric_limits<FloatType>::infinity() || x == -sprout::numeric_limits<FloatType>::infinity()
					? -sprout::numeric_limits<FloatType>::quiet_NaN()
#if SPROUT_USE_BUILTIN_CMATH_FUNCTION
				: sprout::math::detail::builtin_tan(x)
#else
				: x == 0 ? x
				: static_cast<FloatType>(sprout::math::detail::tan_impl(static_cast<typename sprout::math::detail::float_compute<FloatType>::type>(x)))
#endif
				;
		}
		template<
			typename IntType,
			typename sprout::enabler_if<std::is_integral<IntType>::value>::type = sprout::enabler
		>
		inline SPROUT_CONSTEXPR double
		tan(IntType x) {
			return sprout::math::tan(static_cast<double>(x));
		}
	}	// namespace math

	using sprout::math::tan;
}	// namespace sprout

#endif	// #ifndef SPROUT_MATH_TAN_HPP
