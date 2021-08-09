/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_ALGORITHM_FIXED_BOGO_SORT_HPP
#define SPROUT_ALGORITHM_FIXED_BOGO_SORT_HPP

#include <sprout/config.hpp>
#include <sprout/utility/forward.hpp>
#include <sprout/utility/pair/pair.hpp>
#include <sprout/utility/pair/access.hpp>
#include <sprout/algorithm/fixed/results.hpp>
#include <sprout/algorithm/fixed/bogo_sort_result.hpp>

namespace sprout {
	namespace fixed {
		//
		// bogo_sort
		//
		template<typename Container, typename UniformRandomNumberGenerator, typename Compare>
		inline SPROUT_CONSTEXPR typename sprout::fixed::results::algorithm<Container>::type
		bogo_sort(Container const& cont, UniformRandomNumberGenerator&& g, Compare comp) {
			return sprout::first(
				sprout::fixed::bogo_sort_result(cont, SPROUT_FORWARD(UniformRandomNumberGenerator, g), comp)
				);
		}
		template<typename Container, typename UniformRandomNumberGenerator>
		inline SPROUT_CONSTEXPR typename sprout::fixed::results::algorithm<Container>::type
		bogo_sort(Container const& cont, UniformRandomNumberGenerator&& g) {
			return sprout::first(
				sprout::fixed::bogo_sort_result(cont, SPROUT_FORWARD(UniformRandomNumberGenerator, g))
				);
		}
	}	// namespace fixed

	using sprout::fixed::bogo_sort;
}	// namespace sprout

#endif	// #ifndef SPROUT_ALGORITHM_FIXED_BOGO_SORT_HPP
