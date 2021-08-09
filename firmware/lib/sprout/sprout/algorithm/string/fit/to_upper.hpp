/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_ALGORITHM_STRING_FIT_TO_UPPER_HPP
#define SPROUT_ALGORITHM_STRING_FIT_TO_UPPER_HPP

#include <sprout/config.hpp>
#include <sprout/container/traits.hpp>
#include <sprout/container/functions.hpp>
#include <sprout/algorithm/string/fixed/to_upper.hpp>
#include <sprout/algorithm/fit/results.hpp>
#include <sprout/sub_array/sub_array.hpp>
#include <sprout/sub_array/sub.hpp>

namespace sprout {
	namespace algorithm {
		namespace fit {
			namespace detail {
				template<typename Container>
				inline SPROUT_CONSTEXPR typename sprout::fit::results::algorithm<Container>::type
				to_upper_impl(
					Container const& cont,
					typename sprout::container_traits<Container>::difference_type offset
					)
				{
					return sprout::sub_copy(
						sprout::get_internal(sprout::algorithm::fixed::to_upper(cont)),
						offset,
						offset + sprout::size(cont)
						);
				}
			}	// namespace detail
			//
			// to_upper
			//
			template<typename Container>
			inline SPROUT_CONSTEXPR typename sprout::fit::results::algorithm<Container>::type
			to_upper(Container const& cont) {
				return sprout::algorithm::fit::detail::to_upper_impl(cont, sprout::internal_begin_offset(cont));
			}
		}	// namespace fit
	}	// namespace algorithm

	namespace fit {
		using sprout::algorithm::fit::to_upper;
	}	// namespace fit
}	// namespace sprout

#endif	// #ifndef SPROUT_ALGORITHM_STRING_FIT_TO_UPPER_HPP
