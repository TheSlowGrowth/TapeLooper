/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_RANGE_NUMERIC_DFT_CXX14_SINUSOID_HPP
#define SPROUT_RANGE_NUMERIC_DFT_CXX14_SINUSOID_HPP

#include <sprout/config.hpp>
#include <sprout/container/traits.hpp>
#include <sprout/container/functions.hpp>
#include <sprout/utility/forward.hpp>
#include <sprout/numeric/dft/cxx14/sinusoid.hpp>

namespace sprout {
	namespace range {
		//
		// sinusoid
		//
		template<typename ForwardRange>
		inline SPROUT_CXX14_CONSTEXPR void
		sinusoid(
			ForwardRange&& rng,
			typename sprout::container_traits<ForwardRange>::value_type const& frequency = 1,
			typename sprout::container_traits<ForwardRange>::value_type const& amplitude = 1,
			typename sprout::container_traits<ForwardRange>::value_type const& phase = 0
			)
		{
			sprout::sinusoid(
				sprout::begin(SPROUT_FORWARD(ForwardRange, rng)), sprout::end(SPROUT_FORWARD(ForwardRange, rng)),
				frequency, amplitude, phase
				);
		}
	}	// namespace range
}	// namespace sprout

#endif	// #ifndef SPROUT_RANGE_NUMERIC_DFT_CXX14_SINUSOID_HPP
