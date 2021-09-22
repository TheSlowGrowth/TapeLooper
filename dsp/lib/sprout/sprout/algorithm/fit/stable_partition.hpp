/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_ALGORITHM_FIT_STABLE_PARTITION_HPP
#define SPROUT_ALGORITHM_FIT_STABLE_PARTITION_HPP

#include <sprout/config.hpp>
#include <sprout/container/traits.hpp>
#include <sprout/container/functions.hpp>
#include <sprout/algorithm/count_if.hpp>
#include <sprout/algorithm/fixed/stable_partition.hpp>
#include <sprout/algorithm/fit/results.hpp>
#include <sprout/sub_array/sub_array.hpp>
#include <sprout/sub_array/sub.hpp>

namespace sprout {
	namespace fit {
		namespace detail {
			template<typename Container, typename Predicate>
			inline SPROUT_CONSTEXPR typename sprout::fit::results::algorithm<Container>::type
			stable_partition_impl(
				Container const& cont, Predicate pred,
				typename sprout::container_traits<Container>::difference_type offset
				)
			{
				return sprout::sub_copy(
					sprout::get_internal(sprout::fixed::stable_partition(cont, pred)),
					offset,
					offset + sprout::size(cont) - sprout::count_if(sprout::begin(cont), sprout::end(cont), pred)
					);
			}
		}	// namespace detail
		//
		// stable_partition
		//
		template<typename Container, typename Predicate>
		inline SPROUT_CONSTEXPR typename sprout::fit::results::algorithm<Container>::type
		stable_partition(Container const& cont, Predicate pred) {
			return sprout::fit::detail::stable_partition_impl(cont, pred, sprout::internal_begin_offset(cont));
		}
	}	// namespace fit
}	// namespace sprout

#endif	// #ifndef SPROUT_ALGORITHM_FIT_STABLE_PARTITION_HPP
