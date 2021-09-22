/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_MATH_FLOAT2_SIG_EXP_HPP
#define SPROUT_MATH_FLOAT2_SIG_EXP_HPP

#include <type_traits>
#include <sprout/config.hpp>
#include <sprout/limits.hpp>
#include <sprout/detail/pow.hpp>
#include <sprout/math/detail/config.hpp>
#include <sprout/math/isnan.hpp>
#include <sprout/math/ilogb2.hpp>
#include <sprout/type_traits/enabler_if.hpp>
#include <sprout/utility/pair/pair.hpp>

namespace sprout {
	namespace math {
		namespace detail {
			template<typename T>
			inline SPROUT_CONSTEXPR sprout::pair<T, int>
			float2_sig_exp_impl(T x, int exp) {
				typedef sprout::pair<T, int> type;
				return type(x / sprout::detail::pow_n(T(2), exp), exp);
			}
		}	// namespace detail
		//
		// float2_sig_exp
		//
		template<
			typename FloatType,
			typename sprout::enabler_if<std::is_floating_point<FloatType>::value>::type = sprout::enabler
		>
		inline SPROUT_CONSTEXPR sprout::pair<FloatType, int>
		float2_sig_exp(FloatType x) {
			typedef sprout::pair<FloatType, int> type;
			return sprout::math::isnan(x) ? type(x, 0)
				: x == sprout::numeric_limits<FloatType>::infinity() ? type(sprout::numeric_limits<FloatType>::infinity(), 0)
				: x == -sprout::numeric_limits<FloatType>::infinity() ? type(-sprout::numeric_limits<FloatType>::infinity(), 0)
				: x == 0 ? type(x, 0)
				: sprout::math::detail::float2_sig_exp_impl(x, sprout::math::ilogb2(x) + 1)
				;
		}
		template<
			typename IntType,
			typename sprout::enabler_if<std::is_integral<IntType>::value>::type = sprout::enabler
		>
		inline SPROUT_CONSTEXPR sprout::pair<double, int>
		float2_sig_exp(IntType x) {
			return sprout::math::float2_sig_exp(static_cast<double>(x));
		}
	}	// namespace math

	using sprout::math::float2_sig_exp;
}	// namespace sprout

#endif	// #ifndef SPROUT_MATH_FLOAT2_SIG_EXP_HPP
