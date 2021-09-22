/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_COMPLEX_LOG_HPP
#define SPROUT_COMPLEX_LOG_HPP

#include <sprout/config.hpp>
#include <sprout/limits.hpp>
#include <sprout/math/constants.hpp>
#include <sprout/math/isnan.hpp>
#include <sprout/math/isinf.hpp>
#include <sprout/math/copysign.hpp>
#include <sprout/math/signbit.hpp>
#include <sprout/math/log.hpp>
#include <sprout/complex/complex.hpp>
#include <sprout/complex/abs.hpp>
#include <sprout/complex/arg.hpp>

namespace sprout {
	//
	// log
	//
	// G.6.3.2 The clog functions
	// clog(conj(z)) = conj(clog(z)).
	// clog(-0 + i0) returns -��+ ip and raises the �e�edivide-by-zero�f�f floating-point exception.
	// clog(+0 + i0) returns -��+ i0 and raises the �e�edivide-by-zero�f�f floating-point exception.
	// clog(x + i��) returns +��+ ip /2, for finite x.
	// clog(x + iNaN) returns NaN + iNaN and optionally raises the �e�einvalid�f�f floating-point exception, for finite x.
	// clog(-��+ iy) returns +��+ ip , for finite positive-signed y.
	// clog(+��+ iy) returns +��+ i0, for finite positive-signed y.
	// clog(-��+ i��) returns +��+ i3p /4.
	// clog(+��+ i��) returns +��+ ip /4.
	// clog(�}��+ iNaN) returns +��+ iNaN.
	// clog(NaN + iy) returns NaN + iNaN and optionally raises the �e�einvalid�f�f floating-point exception, for finite y.
	// clog(NaN + i��) returns +��+ iNaN.
	// clog(NaN + iNaN) returns NaN + iNaN.
	//
	template<typename T>
	inline SPROUT_CONSTEXPR sprout::complex<T>
	log(sprout::complex<T> const& x) {
		typedef sprout::complex<T> type;
		return sprout::math::isnan(x.real())
				? sprout::math::isnan(x.imag()) ? type(sprout::numeric_limits<T>::quiet_NaN(), x.real())
				: sprout::math::isinf(x.imag()) ? type(sprout::numeric_limits<T>::infinity(), x.real())
				: type(sprout::numeric_limits<T>::quiet_NaN(), x.real())
			: sprout::math::isnan(x.imag())
				? sprout::math::isinf(x.real()) ? type(sprout::numeric_limits<T>::infinity(), x.imag())
				: type(sprout::numeric_limits<T>::quiet_NaN(), x.imag())
			: x.real() == sprout::numeric_limits<T>::infinity()
				? x.imag() == sprout::numeric_limits<T>::infinity() ? type(sprout::numeric_limits<T>::infinity(), sprout::math::quarter_pi<T>())
				: x.imag() == -sprout::numeric_limits<T>::infinity() ? type(sprout::numeric_limits<T>::infinity(), -sprout::math::quarter_pi<T>())
				: type(sprout::numeric_limits<T>::infinity(), (x.imag() == 0 ? x.imag() : sprout::math::copysign(T(0), x.imag())))
			: x.real() == -sprout::numeric_limits<T>::infinity()
				? x.imag() == sprout::numeric_limits<T>::infinity() ? type(sprout::numeric_limits<T>::infinity(), sprout::math::three_quarters_pi<T>())
				: x.imag() == -sprout::numeric_limits<T>::infinity() ? type(sprout::numeric_limits<T>::infinity(), -sprout::math::three_quarters_pi<T>())
				: type(sprout::numeric_limits<T>::infinity(), sprout::math::copysign(sprout::math::pi<T>(), x.imag()))
			: sprout::math::isinf(x.imag()) ? type(sprout::numeric_limits<T>::infinity(), sprout::math::copysign(sprout::math::half_pi<T>(), x.imag()))
			: x.real() == 0 && x.imag() == 0
				? sprout::math::signbit(x.real()) ? type(-sprout::numeric_limits<T>::infinity(), sprout::math::copysign(sprout::math::pi<T>(), x.imag()))
				: type(-sprout::numeric_limits<T>::infinity(), x.imag())
			: type(sprout::math::log(sprout::abs(x)), sprout::arg(x))
			;
	}
}	// namespace sprout

#endif	// #ifndef SPROUT_COMPLEX_LOG_HPP
