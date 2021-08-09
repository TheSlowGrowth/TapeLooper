/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_NUMERIC_DFT_CXX14_SQUARE_HPP
#define SPROUT_NUMERIC_DFT_CXX14_SQUARE_HPP

#include <iterator>
#include <sprout/config.hpp>
#include <sprout/iterator/distance.hpp>
#include <sprout/numeric/dft/detail/square.hpp>

namespace sprout {
	//
	// square
	//
	template<typename ForwardIterator>
	inline SPROUT_CXX14_CONSTEXPR void
	square(
		ForwardIterator first, ForwardIterator last,
		typename std::iterator_traits<ForwardIterator>::value_type const& frequency = 1,
		typename std::iterator_traits<ForwardIterator>::value_type const& amplitude = 1,
		typename std::iterator_traits<ForwardIterator>::value_type const& phase = 0
		)
	{
		typedef typename std::iterator_traits<ForwardIterator>::difference_type difference_type;
		difference_type i = 0;
		while (first != last) {
			*first++ = sprout::detail::square_value(frequency, amplitude, phase, i);
			++i;
		}
	}
}	// namespace sprout

#endif	// #ifndef SPROUT_NUMERIC_DFT_CXX14_SQUARE_HPP
