/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_OPERATION_FIT_REALIGN_HPP
#define SPROUT_OPERATION_FIT_REALIGN_HPP

#include <sprout/config.hpp>
#include <sprout/container/traits.hpp>
#include <sprout/container/functions.hpp>
#include <sprout/operation/fixed/realign.hpp>
#include <sprout/sub_array/sub_array.hpp>
#include <sprout/sub_array/sub.hpp>

namespace sprout {
	namespace fit {
		namespace results {
			//
			// realign
			//
			template<typename Container>
			struct realign {
			public:
				typedef sprout::sub_array<
					typename sprout::container_traits<
						typename sprout::fixed::results::realign<Container>::type
					>::internal_type
				> type;
			};
		}	// namespace results

		//
		// realign
		//
		template<typename Container, typename T>
		inline SPROUT_CONSTEXPR typename sprout::fit::results::realign<Container>::type
		realign(Container const& cont, T const& v) {
			return sprout::sub_copy(
				sprout::get_internal(sprout::fixed::realign(cont, v)),
				0,
				sprout::size(cont)
				);
		}
		template<typename Container>
		inline SPROUT_CONSTEXPR typename sprout::fit::results::realign<Container>::type
		realign(Container const& cont) {
			return sprout::sub_copy(
				sprout::get_internal(sprout::fixed::realign(cont)),
				0,
				sprout::size(cont)
				);
		}
	}	// namespace fit
}	// namespace sprout

#endif	// #ifndef SPROUT_OPERATION_FIT_REALIGN_HPP
