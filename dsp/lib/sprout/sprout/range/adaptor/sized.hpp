/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_RANGE_ADAPTOR_SIZED_HPP
#define SPROUT_RANGE_ADAPTOR_SIZED_HPP

#include <type_traits>
#include <sprout/config.hpp>
#include <sprout/workaround/std/cstddef.hpp>
#include <sprout/container/traits.hpp>
#include <sprout/container/functions.hpp>
#include <sprout/range/adaptor/detail/sized_range_default.hpp>
#include <sprout/type_traits/integral_constant.hpp>
#include <sprout/type_traits/lvalue_reference.hpp>
#include <sprout/utility/forward.hpp>
#include <sprout/utility/lvalue_forward.hpp>

namespace sprout {
	namespace adaptors {
		//
		// sized_range
		//
		template<typename Range, typename sprout::container_traits<Range>::size_type Size>
		class sized_range
			: public sprout::adaptors::detail::sized_range_default<Range, Size>
		{
		public:
			typedef sprout::adaptors::detail::sized_range_default<Range, Size> base_type;
			typedef typename base_type::range_type range_type;
			typedef typename base_type::size_type size_type;
		public:
			SPROUT_CONSTEXPR sized_range() SPROUT_DEFAULTED_DEFAULT_CONSTRUCTOR_DECL
			sized_range(sized_range const&) = default;
			explicit SPROUT_CONSTEXPR sized_range(range_type& range)
				: base_type(
					sprout::begin(range),
					base_type::static_size < static_cast<size_type>(sprout::size(range))
						? sprout::next(sprout::begin(range), base_type::static_size)
						: sprout::end(range)
					)
			{}
		};

		//
		// size_holder
		//
		template<std::size_t Size>
		class size_holder
			: public sprout::integral_constant<std::size_t, Size>
		{};

		//
		// sized
		//
		template<std::size_t Size>
		inline SPROUT_CONSTEXPR sprout::adaptors::size_holder<Size> sized() {
			return sprout::adaptors::size_holder<Size>();
		}

		//
		// operator|
		//
		template<typename Range, std::size_t Size>
		inline SPROUT_CONSTEXPR sprout::adaptors::sized_range<
			typename std::remove_reference<typename sprout::lvalue_reference<Range>::type>::type,
			Size
		>
		operator|(Range&& lhs, sprout::adaptors::size_holder<Size> const&) {
			return sprout::adaptors::sized_range<
				typename std::remove_reference<typename sprout::lvalue_reference<Range>::type>::type,
				Size
			>(
				sprout::lvalue_forward<Range>(lhs)
				);
		}
	}	// namespace adaptors

	//
	// container_construct_traits
	//
	template<typename Range, typename sprout::container_traits<Range>::size_type Size>
	struct container_construct_traits<sprout::adaptors::sized_range<Range, Size> >
		: public sprout::container_construct_traits<typename sprout::adaptors::sized_range<Range, Size>::base_type>
	{};
}	// namespace sprout

#endif	// #ifndef SPROUT_RANGE_ADAPTOR_SIZED_HPP
