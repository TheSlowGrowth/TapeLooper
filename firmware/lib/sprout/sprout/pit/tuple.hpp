/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_PIT_TUPLE_HPP
#define SPROUT_PIT_TUPLE_HPP

#include <tuple>
#include <type_traits>
#include <sprout/config.hpp>
#include <sprout/workaround/std/cstddef.hpp>
#include <sprout/pit/pit.hpp>
#include <sprout/utility/move.hpp>
#include <sprout/tuple/tuple/tuple_element.hpp>
#include <sprout/tuple/tuple/get.hpp>

namespace sprout {
	//
	// tuple_get
	//
	template<std::size_t I, typename Container>
	inline SPROUT_CONSTEXPR typename sprout::container_traits<sprout::pit<Container> >::value_type&
	tuple_get(sprout::pit<Container>& t) SPROUT_NOEXCEPT {
		static_assert(I < sprout::container_traits<sprout::pit<Container> >::static_size, "tuple_get: index out of range");
		return t[I];
	}
	template<std::size_t I, typename Container>
	inline SPROUT_CONSTEXPR typename sprout::container_traits<sprout::pit<Container> >::value_type const&
	tuple_get(sprout::pit<Container> const& t) SPROUT_NOEXCEPT {
		static_assert(I < sprout::container_traits<sprout::pit<Container> >::static_size, "tuple_get: index out of range");
		return t[I];
	}
	template<std::size_t I, typename Container>
	inline SPROUT_CONSTEXPR typename sprout::container_traits<sprout::pit<Container> >::value_type&&
	tuple_get(sprout::pit<Container>&& t)
	SPROUT_NOEXCEPT_IF_EXPR(sprout::move(sprout::tuples::get<I>(t)))
	{
		return sprout::move(sprout::tuples::get<I>(t));
	}
}	// namespace sprout

namespace std {
#if defined(__clang__)
#	pragma clang diagnostic push
#	pragma clang diagnostic ignored "-Wmismatched-tags"
#endif
	//
	// tuple_size
	//
	template<typename Container>
	struct tuple_size<sprout::pit<Container> >
		: public std::tuple_size<Container>
	{};

	//
	// tuple_element
	//
	template<std::size_t I, typename Container>
	struct tuple_element<I, sprout::pit<Container> >
		: public sprout::tuples::tuple_element<I, Container>
	{};
#if defined(__clang__)
#	pragma clang diagnostic pop
#endif
}	// namespace std

#endif	// #ifndef SPROUT_PIT_TUPLE_HPP
