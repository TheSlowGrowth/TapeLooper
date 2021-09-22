/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_ALGORITHM_TRISTATE_LEXICOGRAPHICAL_COMPARE_HPP
#define SPROUT_ALGORITHM_TRISTATE_LEXICOGRAPHICAL_COMPARE_HPP

#include <iterator>
#include <type_traits>
#include <sprout/config.hpp>
#include <sprout/detail/algorithm/lexicographical_compare.hpp>
#include <sprout/iterator/operation.hpp>
#include <sprout/iterator/type_traits/category.hpp>
#include <sprout/iterator/type_traits/common.hpp>
#include <sprout/utility/pair/pair.hpp>
#include <sprout/functional/less.hpp>
#include HDR_ALGORITHM_MIN_MAX_SSCRISK_CEL_OR_SPROUT

namespace sprout {
	namespace detail {
		template<typename Difference1, typename Difference2>
		inline SPROUT_CONSTEXPR int
		tristate_lexicographical_compare_impl_ra_check(
			Difference1 size1, Difference2 size2, int found
			)
		{
			return found != 2 ? found
				: size1 < size2 ? -1
				: size1 > size2 ? 1 : 0
				;
		}

		template<typename RandomAccessIterator1, typename RandomAccessIterator2, typename Compare>
		inline SPROUT_CONSTEXPR typename std::enable_if<
			sprout::is_constant_distance_iterator<RandomAccessIterator1>::value,
			int
		>::type
		tristate_lexicographical_compare(
			RandomAccessIterator1 first1, RandomAccessIterator1 last1, RandomAccessIterator2 first2, RandomAccessIterator2 last2, Compare comp,
			std::random_access_iterator_tag*
			)
		{
			return first2 == last2 ? (first1 == last1 ? 0 : 1)
				: first1 == last1 ? -1
				: sprout::detail::tristate_lexicographical_compare_impl_ra_check(
					sprout::distance(first1, last1), sprout::distance(first2, last2),
					sprout::detail::lexicographical_compare_impl_ra(
						first1, first2, comp,
						NS_SSCRISK_CEL_OR_SPROUT::min(sprout::distance(first1, last1), sprout::distance(first2, last2)),
						2
						)
					)
				;
		}

		template<typename InputIterator1, typename InputIterator2, typename Compare>
		inline SPROUT_CONSTEXPR int
		tristate_lexicographical_compare_impl_check(
				InputIterator1 last1, InputIterator2 last2, Compare comp,
				sprout::pair<InputIterator1, InputIterator2> const& found
				)
		{
				return found.second == last2 ? (found.first == last1 ? 0 : 1)
						: found.first == last1 ? -1
						: comp(*found.first, *found.second) ? -1
						: comp(*found.second, *found.first) ? 1
						: 0
						;
		}

		template<typename InputIterator1, typename InputIterator2, typename Compare>
		inline SPROUT_CONSTEXPR int
		tristate_lexicographical_compare(
			InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp,
			std::input_iterator_tag*
			)
		{
			typedef sprout::pair<InputIterator1, InputIterator2> type;
			return sprout::detail::tristate_lexicographical_compare_impl_check(
				last1, last2, comp,
				sprout::detail::lexicographical_compare_impl(type(first1, first2), last1, last2, comp, 1)
				);
		}
	}	// namespace detail

	//
	// tristate_lexicographical_compare
	//
	//	recursion depth:
	//		O(log(N1+N2))
	//
	template<typename InputIterator1, typename InputIterator2, typename Compare>
	inline SPROUT_CONSTEXPR int
	tristate_lexicographical_compare(
		InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		Compare comp
		)
	{
		typedef typename sprout::common_iterator_category<InputIterator1, InputIterator2>::type* category;
		return sprout::detail::tristate_lexicographical_compare(first1, last1, first2, last2, comp, category());
	}

	template<typename InputIterator1, typename InputIterator2>
	inline SPROUT_CONSTEXPR int
	tristate_lexicographical_compare(
		InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2
		)
	{
		return sprout::tristate_lexicographical_compare(
			first1, last1, first2, last2,
			sprout::less<>()
			);
	}

	namespace detail {
		template<typename RandomAccessIterator1, typename T1, typename RandomAccessIterator2, typename T2, typename Compare>
		inline SPROUT_CONSTEXPR int
		tristate_lexicographical_compare_2_impl_ra(
			RandomAccessIterator1 first1, T1 const& delim1,
			RandomAccessIterator2 first2, T2 const& delim2,
			Compare comp,
			typename std::iterator_traits<RandomAccessIterator1>::difference_type size, int found
			)
		{
			return found != 2 ? found
				: size == 1
					? !comp(*first2, delim2) && !comp(delim2, *first2)
						? !comp(*first1, delim1) && !comp(delim1, *first1) ? 0 : 1
					: (!comp(*first1, delim1) && !comp(delim1, *first1)) || comp(*first1, *first2)
						? -1
					: comp(*first2, *first1) ? 1 : 2
				: sprout::detail::tristate_lexicographical_compare_2_impl_ra(
					sprout::next(first1, size / 2), delim1, sprout::next(first2, size / 2), delim2, comp,
					size - size / 2,
					sprout::detail::tristate_lexicographical_compare_2_impl_ra(
						first1, delim1, first2, delim2, comp,
						size / 2,
						2
						)
					)
				;
		}
		template<typename RandomAccessIterator1, typename T1, typename RandomAccessIterator2, typename T2, typename Compare>
		inline SPROUT_CONSTEXPR int
		tristate_lexicographical_compare_2(
			RandomAccessIterator1 first1, RandomAccessIterator1 last1, T1 const& delim1,
			RandomAccessIterator2 first2, RandomAccessIterator2 last2, T2 const& delim2,
			Compare comp,
			std::random_access_iterator_tag*
			)
		{
			return sprout::detail::tristate_lexicographical_compare_impl_ra_check(
				sprout::distance(first1, last1), sprout::distance(first2, last2),
				first2 == last2 || first1 == last1
				? 2
				: sprout::detail::tristate_lexicographical_compare_2_impl_ra(
					first1, delim1, first2, delim2, comp,
					NS_SSCRISK_CEL_OR_SPROUT::min(sprout::distance(first1, last1), sprout::distance(first2, last2)),
					2
					)
				)
				;
		}

		template<typename InputIterator1, typename T1, typename InputIterator2, typename T2, typename Compare>
		inline SPROUT_CONSTEXPR sprout::pair<InputIterator1, InputIterator2>
		tristate_lexicographical_compare_impl_1(
			sprout::pair<InputIterator1, InputIterator2> const& current,
			InputIterator1 last1, T1 const& delim1, InputIterator2 last2, T2 const& delim2, Compare comp,
			typename std::iterator_traits<InputIterator1>::difference_type n
			)
		{
			typedef sprout::pair<InputIterator1, InputIterator2> type;
			return current.second == last2 || current.first == last1 ? current
				: n == 1
					? !comp(*current.second, delim2) && !comp(delim2, *current.second)
						? type(!comp(*current.first, delim1) && !comp(delim1, *current.first) ? last1 : current.first, last2)
					: (!comp(*current.first, delim1) && !comp(delim1, *current.first)) || comp(*current.first, *current.second)
						? type(last1, current.second)
					: comp(*current.second, *current.first) ? type(current.first, last2)
					: type(sprout::next(current.first), sprout::next(current.second))
				: sprout::detail::tristate_lexicographical_compare_impl_1(
					sprout::detail::tristate_lexicographical_compare_impl_1(
						current,
						last1, delim1, last2, delim2, comp, n / 2
						),
					last1, delim1, last2, delim2, comp, n - n / 2
					)
				;
		}
		template<typename InputIterator1, typename T1, typename InputIterator2, typename T2, typename Compare>
		inline SPROUT_CONSTEXPR sprout::pair<InputIterator1, InputIterator2>
		tristate_lexicographical_compare_impl(
			sprout::pair<InputIterator1, InputIterator2> const& current,
			InputIterator1 last1, T1 const& delim1, InputIterator2 last2, T2 const& delim2, Compare comp,
			typename std::iterator_traits<InputIterator1>::difference_type n
			)
		{
			return current.second == last2 || current.first == last1 ? current
				: sprout::detail::tristate_lexicographical_compare_impl(
					sprout::detail::tristate_lexicographical_compare_impl_1(
						current,
						last1, delim1, last2, delim2, comp, n
						),
					last1, delim1, last2, delim2, comp, n * 2
					)
				;
		}
		template<typename InputIterator1, typename T1, typename InputIterator2, typename T2, typename Compare>
		inline SPROUT_CONSTEXPR int
		tristate_lexicographical_compare_2(
			InputIterator1 first1, InputIterator1 last1, T1 const& delim1,
			InputIterator2 first2, InputIterator2 last2, T2 const& delim2,
			Compare comp,
			std::input_iterator_tag*
			)
		{
			typedef sprout::pair<InputIterator1, InputIterator2> type;
			return sprout::detail::tristate_lexicographical_compare_impl_check(
				last1, last2, comp,
				sprout::detail::tristate_lexicographical_compare_impl(type(first1, first2), last1, delim1, last2, delim2, comp, 1)
				);
		}
	}	// namespace detail

	//
	// tristate_lexicographical_compare
	//
	//	recursion depth:
	//		O(log(N1+N2))
	//
	template<typename InputIterator1, typename T1, typename InputIterator2, typename T2, typename Compare>
	inline SPROUT_CONSTEXPR int
	tristate_lexicographical_compare(
		InputIterator1 first1, InputIterator1 last1, T1 const& delim1,
		InputIterator2 first2, InputIterator2 last2, T2 const& delim2,
		Compare comp
		)
	{
		typedef typename sprout::common_iterator_category<InputIterator1, InputIterator2>::type* category;
		return sprout::detail::tristate_lexicographical_compare_2(first1, last1, delim1, first2, last2, delim2, comp, category());
	}

	template<typename InputIterator1, typename T1, typename InputIterator2, typename T2>
	inline SPROUT_CONSTEXPR int
	tristate_lexicographical_compare(
		InputIterator1 first1, InputIterator1 last1, T1 const& delim1,
		InputIterator2 first2, InputIterator2 last2, T2 const& delim2
		)
	{
		return sprout::tristate_lexicographical_compare(
			first1, last1, delim1, first2, last2, delim2,
			sprout::less<>()
			);
	}
}	// namespace sprout

#endif	// #ifndef SPROUT_ALGORITHM_TRISTATE_LEXICOGRAPHICAL_COMPARE_HPP
