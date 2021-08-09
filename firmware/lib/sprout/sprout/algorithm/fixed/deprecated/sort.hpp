/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_ALGORITHM_FIXED_DEPRECATED_SORT_HPP
#define SPROUT_ALGORITHM_FIXED_DEPRECATED_SORT_HPP

#include <type_traits>
#include <sprout/config.hpp>
#include <sprout/container/traits.hpp>
#include <sprout/container/functions.hpp>
#include <sprout/iterator/operation.hpp>
#include <sprout/algorithm/fixed/results.hpp>
#include <sprout/algorithm/fixed/swap_element.hpp>
#include HDR_FUNCTIONAL_SSCRISK_CEL_OR_SPROUT

namespace sprout {
	namespace fixed {
		namespace detail {
			template<typename Container, typename Compare>
			inline SPROUT_CONSTEXPR typename sprout::fixed::results::algorithm<Container>::type
			sort_lr(
				Container const& cont,
				typename sprout::container_traits<Container>::difference_type start,
				typename sprout::container_traits<Container>::difference_type end,
				Compare comp,
				typename sprout::container_traits<Container>::difference_type l,
				typename sprout::container_traits<Container>::difference_type r,
				typename sprout::container_traits<Container>::value_type const& p
				);
			template<typename Container, typename Compare>
			inline SPROUT_CONSTEXPR typename sprout::fixed::results::algorithm<Container>::type
			sort_start(
				Container const& cont,
				typename sprout::container_traits<Container>::difference_type start,
				typename sprout::container_traits<Container>::difference_type end,
				Compare comp
				);

			template<typename Container, typename RandomAccessIterator>
			inline SPROUT_CONSTEXPR typename sprout::container_traits<Container>::value_type const&
			sort_select_pivot(
				RandomAccessIterator const& origin,
				typename sprout::container_traits<Container>::difference_type start,
				typename sprout::container_traits<Container>::difference_type end
				)
			{	// select pivot (center)
				return *sprout::next(origin, (end + start) / 2);
			}
			template<typename Container, typename RandomAccessIterator, typename Compare>
			inline SPROUT_CONSTEXPR typename sprout::container_traits<Container>::difference_type
			sort_find_l(
				RandomAccessIterator const& origin,
				Compare comp,
				typename sprout::container_traits<Container>::difference_type l,
				typename sprout::container_traits<Container>::value_type const& p
				)
			{	// find left
				return comp(*sprout::next(origin, l), p)
					? sprout::fixed::detail::sort_find_l<Container>(origin, comp, l + 1, p)
					: l
					;
			}
			template<typename Container, typename RandomAccessIterator, typename Compare>
			inline SPROUT_CONSTEXPR typename sprout::container_traits<Container>::difference_type
			sort_find_r(
				RandomAccessIterator const& origin,
				Compare comp,
				typename sprout::container_traits<Container>::difference_type r,
				typename sprout::container_traits<Container>::value_type const& p
				)
			{	// find right
				return comp(p, *sprout::next(origin, r))
					? sprout::fixed::detail::sort_find_r<Container>(origin, comp, r - 1, p)
					: r
					;
			}
			template<typename Container, typename Compare>
			inline SPROUT_CONSTEXPR typename sprout::fixed::results::algorithm<Container>::type
			sort_part_l(
				Container const& cont,
				typename sprout::container_traits<Container>::difference_type start,
				Compare comp,
				typename sprout::container_traits<Container>::difference_type l
				)
			{	// sort left side
				return start < l - 1
					? sprout::fixed::detail::sort_start(cont, start, l - 1, comp)
					: sprout::deep_copy(cont)
					;
			}
			template<typename Container, typename Compare>
			inline SPROUT_CONSTEXPR typename sprout::fixed::results::algorithm<Container>::type
			sort_part_r(
				Container const& cont,
				typename sprout::container_traits<Container>::difference_type end,
				Compare comp,
				typename sprout::container_traits<Container>::difference_type r
				)
			{	// sort right side
				return r + 1 < end
					? sprout::fixed::detail::sort_start(cont, r + 1, end, comp)
					: sprout::deep_copy(cont)
					;
			}
			template<typename Container, typename Compare>
			inline SPROUT_CONSTEXPR typename sprout::fixed::results::algorithm<Container>::type
			sort_part_lr(
				Container const& cont,
				typename sprout::container_traits<Container>::difference_type start,
				typename sprout::container_traits<Container>::difference_type end,
				Compare comp,
				typename sprout::container_traits<Container>::difference_type l,
				typename sprout::container_traits<Container>::difference_type r
				)
			{	// sort both side
				return sprout::fixed::detail::sort_part_r(
					sprout::fixed::detail::sort_part_l(cont, start, comp, l),
					end,
					comp,
					r
					);
			}
			template<typename Container, typename Compare>
			inline SPROUT_CONSTEXPR typename sprout::fixed::results::algorithm<Container>::type
			sort_next(
				Container const& cont,
				typename sprout::container_traits<Container>::difference_type start,
				typename sprout::container_traits<Container>::difference_type end,
				Compare comp,
				typename sprout::container_traits<Container>::difference_type l,
				typename sprout::container_traits<Container>::difference_type r,
				typename sprout::container_traits<Container>::value_type const& p
				)
			{	// comparing right and left, sort both side, or swap and continue
				return l >= r
					? sprout::fixed::detail::sort_part_lr(cont, start, end, comp, l, r)
					: sprout::fixed::detail::sort_lr(
						sprout::fixed::swap_element(
							cont,
							sprout::next(sprout::internal_begin(cont), l),
							sprout::next(sprout::internal_begin(cont), r)
							),
						start,
						end,
						comp,
						l + 1,
						r - 1,
						p
						)
					;
			}
			template<typename Container, typename Compare>
			inline SPROUT_CONSTEXPR typename sprout::fixed::results::algorithm<Container>::type
			sort_lr(
				Container const& cont,
				typename sprout::container_traits<Container>::difference_type start,
				typename sprout::container_traits<Container>::difference_type end,
				Compare comp,
				typename sprout::container_traits<Container>::difference_type l,
				typename sprout::container_traits<Container>::difference_type r,
				typename sprout::container_traits<Container>::value_type const& p
				)
			{	// find left and right
				return sprout::fixed::detail::sort_next(
					cont,
					start,
					end,
					comp,
					sprout::fixed::detail::sort_find_l<Container>(sprout::internal_begin(cont), comp, l, p),
					sprout::fixed::detail::sort_find_r<Container>(sprout::internal_begin(cont), comp, r, p),
					p
					);
			}
			template<typename Container, typename Compare>
			inline SPROUT_CONSTEXPR typename sprout::fixed::results::algorithm<Container>::type
			sort_start(
				Container const& cont,
				typename sprout::container_traits<Container>::difference_type start,
				typename sprout::container_traits<Container>::difference_type end,
				Compare comp
				)
			{	// start sort
				return sprout::fixed::detail::sort_lr(
					cont,
					start,
					end,
					comp,
					start,
					end,
					sprout::fixed::detail::sort_select_pivot<Container>(sprout::internal_begin(cont), start, end)
					);
			}
			template<typename Container, typename Compare>
			inline SPROUT_CONSTEXPR typename std::enable_if<
				(sprout::container_traits<Container>::static_size <= 1),
				typename sprout::fixed::results::algorithm<Container>::type
			>::type
			sort(Container const& cont, Compare comp) {
				return sprout::deep_copy(cont);
			}
			template<typename Container, typename Compare>
			inline SPROUT_CONSTEXPR typename std::enable_if<
				(sprout::container_traits<Container>::static_size > 1),
				typename sprout::fixed::results::algorithm<Container>::type
			>::type
			sort(Container const& cont, Compare comp) {
				return sprout::fixed::detail::sort_start(
					cont,
					sprout::internal_begin_offset(cont),
					sprout::distance(sprout::internal_begin(cont), sprout::end(cont) - 1),
					comp
					);
			}
			template<typename Container>
			inline SPROUT_CONSTEXPR typename std::enable_if<
				(sprout::container_traits<Container>::static_size <= 1),
				typename sprout::fixed::results::algorithm<Container>::type
			>::type
			sort(Container const& cont) {
				return sprout::deep_copy(cont);
			}
			template<typename Container>
			inline SPROUT_CONSTEXPR typename std::enable_if<
				(sprout::container_traits<Container>::static_size > 1),
				typename sprout::fixed::results::algorithm<Container>::type
			>::type
			sort(Container const& cont) {
				return sprout::fixed::detail::sort_start(
					cont,
					sprout::internal_begin_offset(cont),
					sprout::distance(sprout::internal_begin(cont), sprout::end(cont) - 1),
					NS_SSCRISK_CEL_OR_SPROUT::less<typename sprout::container_traits<Container>::value_type>()
					);
			}
		}	// namespace detail
		namespace deprecated {
			//
			// sort
			//
			template<typename Container, typename Compare>
			inline SPROUT_CONSTEXPR typename sprout::fixed::results::algorithm<Container>::type
			sort(Container const& cont, Compare comp) {
				return sprout::fixed::detail::sort(cont, comp);
			}
			template<typename Container>
			inline SPROUT_CONSTEXPR typename sprout::fixed::results::algorithm<Container>::type
			sort(Container const& cont) {
				return sprout::fixed::detail::sort(cont);
			}
		}	// namespace deprecated
	}	// namespace fixed

	namespace deprecated {
		using sprout::fixed::deprecated::sort;
	}	// namespace deprecated
}	// namespace sprout

#endif	// #ifndef SPROUT_ALGORITHM_FIXED_DEPRECATED_SORT_HPP
