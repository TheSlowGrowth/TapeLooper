/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_ALGORITHM_FIT_COPY_WHILE_HPP
#define SPROUT_ALGORITHM_FIT_COPY_WHILE_HPP

#include <sprout/config.hpp>
#include <sprout/iterator/distance.hpp>
#include <sprout/container/traits.hpp>
#include <sprout/container/functions.hpp>
#include <sprout/algorithm/find_if_not.hpp>
#include <sprout/algorithm/fixed/copy_while.hpp>
#include <sprout/algorithm/fit/results.hpp>
#include <sprout/sub_array/sub_array.hpp>
#include <sprout/sub_array/sub.hpp>
#include <sprout/iterator/type_traits/category.hpp>

namespace sprout {
	namespace fit {
		namespace detail {
			template<typename InputIterator, typename Result, typename Predicate>
			inline SPROUT_CONSTEXPR typename sprout::fit::results::algorithm<Result>::type
			copy_while_impl(
				InputIterator const& first, InputIterator const& last, Result const& result, Predicate pred,
				typename sprout::container_traits<Result>::difference_type offset
				)
			{
				return sprout::sub_copy(
					sprout::get_internal(sprout::fixed::copy_while(first, last, result, pred)),
					offset,
					offset + sprout::fit_size(result, sprout::distance(first, sprout::find_if_not(first, last, pred)))
					);
			}
		}	// namespace detail
		//
		// copy_while
		//
		template<typename InputIterator, typename Result, typename Predicate>
		inline SPROUT_CONSTEXPR typename sprout::fit::results::algorithm<Result>::type
		copy_while(InputIterator first, InputIterator last, Result const& result, Predicate pred) {
			static_assert(sprout::is_forward_iterator<InputIterator>::value, "Sorry, not implemented.");
			return sprout::fit::detail::copy_while_impl(first, last, result, pred, sprout::internal_begin_offset(result));
		}
	}	// namespace fit
}	// namespace sprout

#endif	// #ifndef SPROUT_ALGORITHM_FIT_COPY_WHILE_HPP
