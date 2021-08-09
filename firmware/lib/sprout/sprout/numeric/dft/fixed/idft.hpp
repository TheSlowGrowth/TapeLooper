/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_NUMERIC_DFT_FIXED_IDFT_HPP
#define SPROUT_NUMERIC_DFT_FIXED_IDFT_HPP

#include <iterator>
#include <type_traits>
#include <sprout/config.hpp>
#include <sprout/index_tuple/metafunction.hpp>
#include <sprout/container/traits.hpp>
#include <sprout/container/functions.hpp>
#include <sprout/container/indexes.hpp>
#include <sprout/iterator/operation.hpp>
#include <sprout/iterator/idft_iterator.hpp>
#include <sprout/algorithm/fixed/results.hpp>
#include <sprout/pit/pit.hpp>
#include <sprout/math/less.hpp>
#include <sprout/numeric/dft/idft_element.hpp>
#include <sprout/detail/container_complate.hpp>

namespace sprout {
	namespace fixed {
		namespace detail {
			template<typename RandomAccessIterator, typename Result, sprout::index_t... Indexes>
			inline SPROUT_CONSTEXPR typename sprout::fixed::results::algorithm<Result>::type
			idft_impl_ra(
				RandomAccessIterator const& first, RandomAccessIterator const& last, Result const& result,
				sprout::index_tuple<Indexes...>,
				typename sprout::container_traits<Result>::difference_type offset,
				typename sprout::container_traits<Result>::size_type size,
				typename sprout::container_traits<Result>::size_type input_size
				)
			{
				return sprout::remake<Result>(
					result, size,
					(Indexes >= offset && sprout::math::less(Indexes, offset + size) && sprout::math::less(Indexes, offset + input_size)
						? sprout::detail::idft_element_impl(first, last, Indexes - offset, input_size)
						: *sprout::next(sprout::internal_begin(result), Indexes)
						)...
					);
			}
			template<typename RandomAccessIterator, typename Result>
			inline SPROUT_CONSTEXPR typename sprout::fixed::results::algorithm<Result>::type
			idft(
				RandomAccessIterator const& first, RandomAccessIterator const& last, Result const& result,
				std::random_access_iterator_tag*
				)
			{
				return sprout::fixed::detail::idft_impl_ra(
					first, last, result,
					sprout::container_indexes<Result>::make(),
					sprout::internal_begin_offset(result),
					sprout::size(result),
					sprout::distance(first, last)
					);
			}

			template<typename ForwardIterator, typename Result, typename... Args>
			inline SPROUT_CONSTEXPR typename std::enable_if<
				sprout::container_traits<Result>::static_size == sizeof...(Args),
				typename sprout::fixed::results::algorithm<Result>::type
			>::type
			idft_impl(
				ForwardIterator, ForwardIterator, Result const& result,
				typename sprout::container_traits<Result>::size_type,
				ForwardIterator, typename sprout::container_traits<Result>::difference_type,
				Args const&... args
				)
			{
				return sprout::remake<Result>(result, sprout::size(result), args...);
			}
			template<typename ForwardIterator, typename Result, typename... Args>
			inline SPROUT_CONSTEXPR typename std::enable_if<
				sprout::container_traits<Result>::static_size != sizeof...(Args),
				typename sprout::fixed::results::algorithm<Result>::type
			>::type
			idft_impl(
				ForwardIterator const& first, ForwardIterator const& last, Result const& result,
				typename sprout::container_traits<Result>::size_type size,
				ForwardIterator const& first_, typename sprout::container_traits<Result>::difference_type i,
				Args const&... args
				)
			{
				return first != last && sizeof...(Args) < size
					? sprout::fixed::detail::idft_impl(
						sprout::next(first), last, result, size, first_, i + 1,
						args..., sprout::detail::idft_element_impl(first_, last, i, size)
						)
					: sprout::detail::container_complate(result, args...)
					;
			}
			template<typename ForwardIterator, typename Result>
			inline SPROUT_CONSTEXPR typename sprout::fixed::results::algorithm<Result>::type
			idft(
				ForwardIterator const& first, ForwardIterator const& last, Result const& result,
				std::forward_iterator_tag*
				)
			{
				return sprout::fixed::detail::idft_impl(first, last, result, sprout::size(result), first, 0);
			}

			template<typename ForwardIterator, typename Result>
			inline SPROUT_CONSTEXPR typename std::enable_if<
				sprout::is_fixed_container<Result>::value,
				typename sprout::fixed::results::algorithm<Result>::type
			>::type
			idft(ForwardIterator const& first, ForwardIterator const& last, Result const& result) {
				typedef typename std::iterator_traits<ForwardIterator>::iterator_category* category;
				return sprout::fixed::detail::idft(first, last, result, category());
			}

			template<typename ForwardIterator, typename Result>
			inline SPROUT_CONSTEXPR typename std::enable_if<
				!sprout::is_fixed_container<Result>::value,
				typename sprout::fixed::results::algorithm<Result>::type
			>::type
			idft(ForwardIterator const& first, ForwardIterator const& last, Result const& result) {
				return sprout::remake<Result>(
					result, sprout::size(result),
					sprout::make_idft_iterator(first, last),
					sprout::make_idft_iterator(first, last, sprout::distance(first, last))
					);
			}
		}	// namespace detail
		//
		// idft
		//
		template<typename ForwardIterator, typename Result>
		inline SPROUT_CONSTEXPR typename sprout::fixed::results::algorithm<Result>::type
		idft(ForwardIterator first, ForwardIterator last, Result const& result) {
			return sprout::fixed::detail::idft(first, last, result);
		}

		template<typename Result, typename ForwardIterator>
		inline SPROUT_CONSTEXPR typename sprout::fixed::results::algorithm<Result>::type
		idft(ForwardIterator first, ForwardIterator last) {
			return sprout::fixed::idft(first, last, sprout::pit<Result>());
		}
	}	// namespace fixed

	using sprout::fixed::idft;
}	// namespace sprout

#endif	// #ifndef SPROUT_NUMERIC_DFT_FIXED_IDFT_HPP
