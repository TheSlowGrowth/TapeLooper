/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_ALGORITHM_ANY_OF_HPP
#define SPROUT_ALGORITHM_ANY_OF_HPP

#include <iterator>
#include <type_traits>
#include <sprout/config.hpp>
#include <sprout/iterator/operation.hpp>
#include <sprout/iterator/type_traits/category.hpp>
#include <sprout/utility/pair/pair.hpp>

namespace sprout {
	namespace detail {
		template<typename RandomAccessIterator, typename Predicate>
		inline SPROUT_CONSTEXPR bool
		any_of_impl_ra(
			RandomAccessIterator const& first, RandomAccessIterator const& last, Predicate pred,
			typename std::iterator_traits<RandomAccessIterator>::difference_type pivot
			)
		{
			return pivot == 0 ? pred(*first)
				: sprout::detail::any_of_impl_ra(
					first, sprout::next(first, pivot), pred,
					pivot / 2
					)
					|| sprout::detail::any_of_impl_ra(
						sprout::next(first, pivot), last, pred,
						(sprout::distance(first, last) - pivot) / 2
						)
				;
		}
		template<typename RandomAccessIterator, typename Predicate>
		inline SPROUT_CONSTEXPR typename std::enable_if<
			sprout::is_constant_distance_iterator<RandomAccessIterator>::value,
			bool
		>::type
		any_of(
			RandomAccessIterator const& first, RandomAccessIterator const& last, Predicate pred,
			std::random_access_iterator_tag*
			)
		{
			return first == last ? false
				: sprout::detail::any_of_impl_ra(first, last, pred, sprout::distance(first, last) / 2)
				;
		}

		template<typename InputIterator, typename Predicate>
		inline SPROUT_CONSTEXPR sprout::pair<InputIterator, bool>
		any_of_impl_1(
			sprout::pair<InputIterator, bool> const& current,
			InputIterator const& last, Predicate pred, typename std::iterator_traits<InputIterator>::difference_type n
			)
		{
			typedef sprout::pair<InputIterator, bool> type;
			return current.second || current.first == last ? current
				: n == 1 ? pred(*current.first) ? type(current.first, true) : type(sprout::next(current.first), false)
				: sprout::detail::any_of_impl_1(
					sprout::detail::any_of_impl_1(
						current,
						last, pred, n / 2
						),
					last, pred, n - n / 2
					)
				;
		}
		template<typename InputIterator, typename Predicate>
		inline SPROUT_CONSTEXPR sprout::pair<InputIterator, bool>
		any_of_impl(
			sprout::pair<InputIterator, bool> const& current,
			InputIterator const& last, Predicate pred, typename std::iterator_traits<InputIterator>::difference_type n
			)
		{
			return current.second || current.first == last ? current
				: sprout::detail::any_of_impl(
					sprout::detail::any_of_impl_1(
						current,
						last, pred, n
						),
					last, pred, n * 2
					)
				;
		}
		template<typename InputIterator, typename Predicate>
		inline SPROUT_CONSTEXPR bool
		any_of(
			InputIterator const& first, InputIterator const& last, Predicate pred,
			std::input_iterator_tag*
			)
		{
			typedef sprout::pair<InputIterator, bool> type;
			return sprout::detail::any_of_impl(type(first, false), last, pred, 1).second;
		}
	}	// namespace detail

	// 25.2.2 Any of
	//
	//	recursion depth:
	//		O(log N)
	//
	template<typename InputIterator, typename Predicate>
	inline SPROUT_CONSTEXPR bool
	any_of(InputIterator first, InputIterator last, Predicate pred) {
		typedef typename std::iterator_traits<InputIterator>::iterator_category* category;
		return sprout::detail::any_of(first, last, pred, category());
	}
}	// namespace sprout

#endif	// #ifndef SPROUT_ALGORITHM_ANY_OF_HPP
