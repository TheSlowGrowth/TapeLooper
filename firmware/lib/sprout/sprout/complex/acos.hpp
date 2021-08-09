/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_COMPLEX_ACOS_HPP
#define SPROUT_COMPLEX_ACOS_HPP

#include <sprout/config.hpp>
#include <sprout/limits.hpp>
#include <sprout/math/constants.hpp>
#include <sprout/math/isnan.hpp>
#include <sprout/math/isinf.hpp>
#include <sprout/math/copysign.hpp>
#include <sprout/complex/complex.hpp>
#include <sprout/complex/asin.hpp>

namespace sprout {
	//
	// acos
	//
	// G.6.1.1 The cacos functions
	// cacos(conj(z)) = conj(cacos(z)).
	// cacos(�}0 + i0) returns p /2 - i0.
	// cacos(�}0 + iNaN) returns p /2 + iNaN.
	// cacos(x + i��) returns p /2 - i��, for finite x.
	// cacos(x + iNaN) returns NaN + iNaN and optionally raises the �e�einvalid�f�f floating-point exception, for nonzero finite x.
	// cacos(-��+ iy) returns p - i��, for positive-signed finite y.
	// cacos(+��+ iy) returns +0 - i��, for positive-signed finite y.
	// cacos(-��+ i��) returns 3p /4 - i��.
	// cacos(+��+ i��) returns p /4 - i��.
	// cacos(�}��+ iNaN) returns NaN �} i�� (where the sign of the imaginary part of the result is unspecified).
	// cacos(NaN + iy) returns NaN + iNaN and optionally raises the �e�einvalid�f�f floating-point exception, for finite y.
	// cacos(NaN + i��) returns NaN - i��.
	// cacos(NaN + iNaN) returns NaN + iNaN.
	//
	namespace detail {
		template<typename T>
		inline SPROUT_CONSTEXPR sprout::complex<T>
		acos_impl(sprout::complex<T> const& t) {
			return sprout::complex<T>(sprout::math::half_pi<T>() - t.real(), -t.imag());
		}
	}	// namespace detail
	template<typename T>
	inline SPROUT_CONSTEXPR sprout::complex<T>
	acos(sprout::complex<T> const& x) {
		typedef sprout::complex<T> type;
		return sprout::math::isnan(x.real())
				? sprout::math::isnan(x.imag()) ? x
				: sprout::math::isinf(x.imag()) ? type(x.real(), -x.imag())
				: type(x.real(), sprout::numeric_limits<T>::quiet_NaN())
			: sprout::math::isnan(x.imag())
				? sprout::math::isinf(x.real()) ? type(sprout::numeric_limits<T>::quiet_NaN(), x.real())
				: x.real() == 0 ? type(sprout::math::half_pi<T>(), x.imag())
				: type(sprout::numeric_limits<T>::quiet_NaN(), sprout::numeric_limits<T>::quiet_NaN())
			: x.real() == sprout::numeric_limits<T>::infinity()
				? sprout::math::isinf(x.imag()) ? type(sprout::math::quarter_pi<T>(), -x.imag())
				: type(T(0), sprout::math::copysign(sprout::numeric_limits<T>::infinity(), -x.imag()))
			: x.real() == -sprout::numeric_limits<T>::infinity()
				? sprout::math::isinf(x.imag()) ? type(sprout::math::three_quarters_pi<T>(), -x.imag())
				: type(sprout::math::pi<T>(), sprout::math::copysign(sprout::numeric_limits<T>::infinity(), -x.imag()))
			: sprout::math::isinf(x.imag()) ? type(sprout::math::half_pi<T>(), sprout::math::copysign(sprout::numeric_limits<T>::infinity(), -x.imag()))
			: x.real() == 0 && x.imag() == 0 ? type(sprout::math::half_pi<T>(), -x.imag())
			: sprout::detail::acos_impl(sprout::asin(x))
			;
	}
}	// namespace sprout

#endif	// #ifndef SPROUT_COMPLEX_ACOS_HPP
