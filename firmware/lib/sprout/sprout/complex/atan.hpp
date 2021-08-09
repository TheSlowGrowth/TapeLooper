/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_COMPLEX_ATAN_HPP
#define SPROUT_COMPLEX_ATAN_HPP

#include <sprout/config.hpp>
#include <sprout/complex/complex.hpp>
#include <sprout/complex/arithmetic_operators.hpp>
#include <sprout/complex/perp.hpp>
#include <sprout/complex/atanh.hpp>

namespace sprout {
	//
	// atan
	//
	// catan(z) = -i catanh(iz)
	//
	template<typename T>
	inline SPROUT_CONSTEXPR sprout::complex<T>
	atan(sprout::complex<T> const& x) {
		return -sprout::perp(sprout::atanh(sprout::perp(x)));
	}
}	// namespace sprout

#endif	// #ifndef SPROUT_COMPLEX_ATAN_HPP
