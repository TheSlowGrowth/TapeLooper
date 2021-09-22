/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_COMPLEX_COSH_HPP
#define SPROUT_COMPLEX_COSH_HPP

#include <sprout/config.hpp>
#include <sprout/limits.hpp>
#include <sprout/math/isnan.hpp>
#include <sprout/math/isinf.hpp>
#include <sprout/math/abs.hpp>
#include <sprout/math/cos.hpp>
#include <sprout/math/sin.hpp>
#include <sprout/math/cosh.hpp>
#include <sprout/math/sinh.hpp>
#include <sprout/complex/complex.hpp>
#include <sprout/complex/arithmetic_operators.hpp>
#include <sprout/complex/euler.hpp>
#include <sprout/complex/detail/copysign_mul.hpp>

namespace sprout {
	//
	// cosh
	//
	// G.6.2.4 The ccosh functions
	// ccosh(conj(z)) = conj(ccosh(z)) and ccosh is even.
	// ccosh(+0 + i0) returns 1 + i0.
	// ccosh(+0 + i��) returns NaN �} i0 (where the sign of the imaginary part of the result is unspecified) and raises the �e�einvalid�f�f floating-point exception.
	// ccosh(+0 + iNaN) returns NaN �} i0 (where the sign of the imaginary part of the result is unspecified).
	// ccosh(x + i��) returns NaN + iNaN and raises the �e�einvalid�f�f floating-point exception, for finite nonzero x.
	// ccosh(x + iNaN) returns NaN + iNaN and optionally raises the �e�einvalid�f�f floating-point exception, for finite nonzero x.
	// ccosh(+��+ i0) returns +��+ i0.
	// ccosh(+��+ iy) returns +�� cis(y), for finite nonzero y.
	// ccosh(+��+ i��) returns �}��+ iNaN (where the sign of the real part of the result is unspecified) and raises the �e�einvalid�f�f floating-point exception.
	// ccosh(+��+ iNaN) returns +��+ iNaN.
	// ccosh(NaN + i0) returns NaN �} i0 (where the sign of the imaginary part of the result is unspecified).
	// ccosh(NaN + iy) returns NaN + iNaN and optionally raises the �e�einvalid�f�f floating-point exception, for all nonzero numbers y.
	// ccosh(NaN + iNaN) returns NaN + iNaN.
	//
	template<typename T>
	inline SPROUT_CONSTEXPR sprout::complex<T>
	cosh(sprout::complex<T> const& x) {
		typedef sprout::complex<T> type;
		return sprout::math::isnan(x.real())
				? sprout::math::isnan(x.imag()) ? type(x.real(), x.real())
				: x.imag() == 0 ? type(sprout::numeric_limits<T>::quiet_NaN(), x.imag())
				: type(x.real(), x.real())
			: sprout::math::isnan(x.imag())
				? sprout::math::isinf(x.real()) ? type(sprout::numeric_limits<T>::infinity(), sprout::numeric_limits<T>::quiet_NaN())
				: x.real() == 0 ? type(sprout::numeric_limits<T>::quiet_NaN(), x.real())
				: type(x.imag(), x.imag())
			: x.real() == sprout::numeric_limits<T>::infinity()
				? sprout::math::isinf(x.imag()) ? type(sprout::numeric_limits<T>::infinity(), sprout::numeric_limits<T>::quiet_NaN())
				: x.imag() == 0 ? type(sprout::numeric_limits<T>::infinity(), x.imag())
				: sprout::detail::copysign_mul(sprout::numeric_limits<T>::infinity(), sprout::euler(x.imag()))
			: x.real() == -sprout::numeric_limits<T>::infinity()
				? sprout::math::isinf(x.imag()) ? type(sprout::numeric_limits<T>::infinity(), sprout::numeric_limits<T>::quiet_NaN())
				: x.imag() == 0 ? type(sprout::numeric_limits<T>::infinity(), x.imag())	// ???? GCC or Clang
				: sprout::detail::copysign_mul(sprout::numeric_limits<T>::infinity(), sprout::euler(-x.imag()))
			: sprout::math::isinf(x.imag())
				? x.real() == 0 ? type(sprout::numeric_limits<T>::quiet_NaN(), x.real())
				: type(-sprout::numeric_limits<T>::quiet_NaN(), -sprout::numeric_limits<T>::quiet_NaN())
			: x.real() == 0 && x.imag() == 0 ? type(T(1), x.imag())
			: type(sprout::math::cosh(x.real()) * sprout::math::cos(x.imag()), sprout::math::sinh(x.real()) * sprout::math::sin(x.imag()))
			;
	}
}	// namespace sprout

#endif	// #ifndef SPROUT_COMPLEX_COSH_HPP
