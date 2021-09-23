/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_ALGORITHM_FIXED_UNFOLD_HPP
#define SPROUT_ALGORITHM_FIXED_UNFOLD_HPP

#include <type_traits>
#include <sprout/config.hpp>
#include <sprout/container/traits.hpp>
#include <sprout/container/functions.hpp>
#include <sprout/algorithm/fixed/results.hpp>
#include <sprout/generator/functions.hpp>
#include <sprout/pit/pit.hpp>
#include <sprout/detail/container_complate.hpp>

namespace sprout {
	namespace fixed {
		namespace detail {
			template<typename Container, typename Generator, typename Next, typename... Args>
			inline SPROUT_CONSTEXPR typename std::enable_if<
				sprout::container_traits<Container>::static_size == sizeof...(Args) + 1,
				typename sprout::fixed::results::algorithm<Container>::type
			>::type
			unfold_impl_1(
				Container const& cont, Generator const&, Next const& next,
				typename sprout::container_traits<Container>::size_type size,
				Args const&... args
				)
			{
				return sprout::remake<Container>(cont, size, args..., sprout::generators::generated_value(next));
			}
			template<typename Container, typename Generator, typename Next, typename... Args>
			inline SPROUT_CONSTEXPR typename std::enable_if<
				sprout::container_traits<Container>::static_size != sizeof...(Args) + 1,
				typename sprout::fixed::results::algorithm<Container>::type
			>::type
			unfold_impl_1(
				Container const& cont, Generator const& gen, Next const& next,
				typename sprout::container_traits<Container>::size_type size,
				Args const&... args
				)
			{
				return sizeof...(Args) + 1 < size
					? sprout::fixed::detail::unfold_impl_1(
						cont, gen, gen(sprout::generators::next_generator(next)), size,
						args..., sprout::generators::generated_value(next)
						)
					: sprout::detail::container_complate(cont, args..., sprout::generators::generated_value(next))
					;
			}
			template<typename Container, typename Generator, typename Init>
			inline SPROUT_CONSTEXPR typename std::enable_if<
				sprout::container_traits<Container>::static_size == 0,
				typename sprout::fixed::results::algorithm<Container>::type
			>::type unfold_impl(
				Container const& cont, Generator const&, Init const&,
				typename sprout::container_traits<Container>::size_type size
				)
			{
				return sprout::remake<Container>(cont, size);
			}
			template<typename Container, typename Generator, typename Init>
			inline SPROUT_CONSTEXPR typename std::enable_if<
				sprout::container_traits<Container>::static_size != 0,
				typename sprout::fixed::results::algorithm<Container>::type
			>::type
			unfold_impl(
				Container const& cont, Generator const& gen, Init const& init,
				typename sprout::container_traits<Container>::size_type size
				)
			{
				return size > 0
					? sprout::fixed::detail::unfold_impl_1(cont, gen, gen(init), size)
					: sprout::detail::container_complate(cont)
					;
			}
		}	// namespace detail
		//
		// unfold
		//
		template<typename Container, typename Generator, typename Init>
		inline SPROUT_CONSTEXPR typename sprout::fixed::results::algorithm<Container>::type
		unfold(Container const& cont, Generator const& gen, Init const& init) {
			return sprout::fixed::detail::unfold_impl(cont, gen, init, sprout::size(cont));
		}

		template<typename Container, typename Generator, typename Init>
		inline SPROUT_CONSTEXPR typename sprout::fixed::results::algorithm<Container>::type
		unfold(Generator const& gen, Init const& init) {
			return sprout::fixed::unfold(sprout::pit<Container>(), gen, init);
		}
	}	// namespace fixed

	using sprout::fixed::unfold;
}	// namespace sprout

#endif	// #ifndef SPROUT_ALGORITHM_FIXED_UNFOLD_HPP
