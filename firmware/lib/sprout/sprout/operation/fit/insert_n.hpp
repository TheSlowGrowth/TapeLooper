/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_OPERATION_FIT_INSERT_N_HPP
#define SPROUT_OPERATION_FIT_INSERT_N_HPP

#include <sprout/config.hpp>
#include <sprout/workaround/std/cstddef.hpp>
#include <sprout/container/traits.hpp>
#include <sprout/container/functions.hpp>
#include <sprout/operation/fixed/insert_n.hpp>
#include <sprout/sub_array/sub_array.hpp>
#include <sprout/sub_array/sub.hpp>

namespace sprout {
	namespace fit {
		namespace results {
			//
			// insert_n
			//
			template<std::size_t N, typename Container, typename T, typename... Values>
			struct insert_n {
			public:
				typedef sprout::sub_array<
					typename sprout::container_traits<
						typename sprout::fixed::results::insert_n<N, Container, T, Values...>::type
					>::internal_type
				> type;
			};
		}	// namespace results

		//
		// insert_n
		//
		template<std::size_t N, typename Container, typename T, typename... Values>
		inline SPROUT_CONSTEXPR typename sprout::fit::results::insert_n<N, Container, T, Values...>::type
		insert_n(
			Container const& cont, typename sprout::container_traits<Container>::const_iterator pos,
			T const& v, Values const&... values
			)
		{
			return sprout::sub_copy(
				sprout::get_internal(sprout::fixed::insert_n<N>(cont, pos, v, values...)),
				sprout::internal_begin_offset(cont),
				sprout::internal_end_offset(cont) + (1 + sizeof...(Values)) * N
				);
		}
		template<std::size_t N, typename Container, typename T, typename... Values>
		inline SPROUT_CONSTEXPR typename sprout::fit::results::insert_n<N, Container, T, Values...>::type
		insert_n(
			Container const& cont, typename sprout::container_traits<Container>::difference_type pos,
			T const& v, Values const&... values
			)
		{
			return sprout::sub_copy(
				sprout::get_internal(sprout::fixed::insert_n<N>(cont, pos, v, values...)),
				sprout::internal_begin_offset(cont),
				sprout::internal_end_offset(cont) + (1 + sizeof...(Values)) * N
				);
		}
	}	// namespace fit
}	// namespace sprout

#endif	// #ifndef SPROUT_OPERATION_FIT_INSERT_N_HPP
