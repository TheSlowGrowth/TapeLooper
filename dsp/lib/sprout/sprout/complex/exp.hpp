/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_COMPLEX_EXP_HPP
#define SPROUT_COMPLEX_EXP_HPP

#include <sprout/config.hpp>
#include <sprout/limits.hpp>
#include <sprout/math/isnan.hpp>
#include <sprout/math/isinf.hpp>
#include <sprout/math/isinf.hpp>
#include <sprout/math/copysign.hpp>
#include <sprout/complex/complex.hpp>
#include <sprout/complex/polar.hpp>
#include <sprout/complex/euler.hpp>
#include <sprout/complex/detail/copysign_mul.hpp>

namespace sprout {
	//
	// exp
	//
	// G.6.3.1 The cexp functions
	// cexp(conj(z)) = conj(cexp(z)).
	// cexp(�}0 + i0) returns 1 + i0.
	// cexp(x + i��) returns NaN + iNaN and raises the �e�einvalid�f�f floating-point exception, for finite x.
	// cexp(x + iNaN) returns NaN + iNaN and optionally raises the �e�einvalid�f�f floating-point exception, for finite x.
	// cexp(+��+ i0) returns +��+ i0.
	// cexp(-��+ iy) returns +0 cis(y), for finite y.
	// cexp(+��+ iy) returns +�� cis(y), for finite nonzero y.
	// cexp(-��+ i��) returns �}0 �} i0 (where the signs of the real and imaginary parts of the result are unspecified).
	// cexp(+��+ i��) returns �}��+ iNaN and raises the �e�einvalid�f�f floating-point exception (where the sign of the real part of the result is unspecified).
	// cexp(-��+ iNaN) returns �}0 �} i0 (where the signs of the real and imaginary parts of the result are unspecified).
	// cexp(+��+ iNaN) returns �}��+ iNaN (where the sign of the real part of the result is unspecified).
	// cexp(NaN + i0) returns NaN + i0.
	// cexp(NaN + iy) returns NaN + iNaN and optionally raises the �e�einvalid�f�f floating-point exception, for all non-zero numbers y.
	// cexp(NaN + iNaN) returns NaN + iNaN.
	//
	template<typename T>
	inline SPROUT_CONSTEXPR sprout::complex<T>
	exp(sprout::complex<T> const& x) {
		typedef sprout::complex<T> type;
		return sprout::math::isnan(x.real())
				? sprout::math::isnan(x.imag()) ? type(x.real(), x.real())
				: x.imag() == 0 ? type(x.real(), x.imag())
				: type(x.real(), x.real())
			: sprout::math::isnan(x.imag())
				? x.real() == sprout::numeric_limits<T>::infinity() ? type(x.real(), x.imag())
				: x.real() == -sprout::numeric_limits<T>::infinity() ? type(T(0), T(0))
				: sprout::math::isinf(x.real()) ? type(sprout::numeric_limits<T>::infinity(), sprout::numeric_limits<T>::quiet_NaN())
				: type(x.imag(), x.imag())
			: x.real() == sprout::numeric_limits<T>::infinity()
				? sprout::math::isinf(x.imag()) ? type(sprout::numeric_limits<T>::infinity(), sprout::numeric_limits<T>::quiet_NaN())
				: x.imag() == 0 ? type(sprout::numeric_limits<T>::infinity(), x.imag())
				: sprout::detail::copysign_mul(sprout::numeric_limits<T>::infinity(), sprout::euler(x.imag()))
			: x.real() == -sprout::numeric_limits<T>::infinity()
				? sprout::math::isinf(x.imag()) ? type(T(0), T(0))
				: T(0) * sprout::euler(x.imag())
			: sprout::math::isinf(x.imag())
				? type(-sprout::numeric_limits<T>::quiet_NaN(), -sprout::numeric_limits<T>::quiet_NaN())
			: x.real() == 0 && x.imag() == 0 ? type(T(1), x.imag())
			: sprout::polar(sprout::math::exp(x.real()), x.imag())
			;
	}
}	// namespace sprout

#endif	// #ifndef SPROUT_COMPLEX_EXP_HPP
