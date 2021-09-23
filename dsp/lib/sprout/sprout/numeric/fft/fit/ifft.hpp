/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_NUMERIC_FFT_FIT_IFFT_HPP
#define SPROUT_NUMERIC_FFT_FIT_IFFT_HPP

#include <sprout/config.hpp>
#include <sprout/iterator/distance.hpp>
#include <sprout/container/traits.hpp>
#include <sprout/container/functions.hpp>
#include <sprout/numeric/fft/fixed/ifft.hpp>
#include <sprout/algorithm/fit/results.hpp>
#include <sprout/sub_array/sub_array.hpp>
#include <sprout/sub_array/sub.hpp>
#include <sprout/pit/pit.hpp>

namespace sprout {
	namespace fit {
		namespace detail {
			template<typename ForwardIterator, typename Result>
			inline SPROUT_CONSTEXPR typename sprout::fit::results::algorithm<Result>::type
			ifft_impl(
				ForwardIterator const& first, ForwardIterator const& last, Result const& result,
				typename sprout::container_traits<Result>::difference_type offset
				)
			{
				return sprout::sub_copy(
					sprout::get_internal(sprout::fixed::ifft(first, last, result)),
					offset,
					offset + sprout::fit_size(result, sprout::distance(first, last))
					);
			}
		}	// namespace detail
		//
		// ifft
		//
		template<typename ForwardIterator, typename Result>
		inline SPROUT_CONSTEXPR typename sprout::fit::results::algorithm<Result>::type
		ifft(ForwardIterator first, ForwardIterator last, Result const& result) {
			return sprout::fit::detail::ifft_impl(first, last, result, sprout::internal_begin_offset(result));
		}

		template<typename Result, typename ForwardIterator>
		inline SPROUT_CONSTEXPR typename sprout::fixed::results::algorithm<Result>::type
		ifft(ForwardIterator first, ForwardIterator last) {
			return sprout::fit::ifft(first, last, sprout::pit<Result>());
		}
	}	// namespace fit
}	// namespace sprout

#endif	// #ifndef SPROUT_NUMERIC_FFT_FIT_IFFT_HPP
