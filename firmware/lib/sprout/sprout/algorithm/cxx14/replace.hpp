/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_ALGORITHM_CXX14_REPLACE_HPP
#define SPROUT_ALGORITHM_CXX14_REPLACE_HPP

#include <sprout/config.hpp>

namespace sprout {
	//
	// 25.3.5 Replace
	//
	template<typename ForwardIterator, typename T>
	inline SPROUT_CXX14_CONSTEXPR void
	replace(ForwardIterator first, ForwardIterator last, T const& old_value, T const& new_value) {
		for (; first != last; ++first) {
			if (*first == old_value) {
				*first = new_value;
			}
		}
	}
}	// namespace sprout

#endif	// #ifndef SPROUT_ALGORITHM_CXX14_REPLACE_HPP
