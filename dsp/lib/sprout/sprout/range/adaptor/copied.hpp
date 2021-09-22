/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_RANGE_ADAPTOR_COPIED_HPP
#define SPROUT_RANGE_ADAPTOR_COPIED_HPP

#include <type_traits>
#include <sprout/config.hpp>
#include <sprout/container/traits.hpp>
#include <sprout/container/functions.hpp>
#include <sprout/algorithm/fixed/results.hpp>
#include <sprout/range/adaptor/detail/adapted_range_default.hpp>
#include <sprout/range/algorithm/fixed/copy.hpp>
#include <sprout/type_traits/lvalue_reference.hpp>
#include <sprout/utility/forward.hpp>
#include <sprout/utility/lvalue_forward.hpp>
#include <sprout/utility/value_holder/value_holder.hpp>

namespace sprout {
	namespace adaptors {
		//
		// copied_range
		//
		template<typename Range>
		class copied_range
			: public sprout::adaptors::detail::adapted_range_default<Range>
		{
		public:
			typedef sprout::adaptors::detail::adapted_range_default<Range> base_type;
			typedef typename base_type::range_type range_type;
		public:
			SPROUT_CONSTEXPR copied_range() SPROUT_DEFAULTED_DEFAULT_CONSTRUCTOR_DECL
			copied_range(copied_range const&) = default;
			explicit SPROUT_CONSTEXPR copied_range(range_type& range)
				: base_type(
					sprout::begin(range),
					sprout::end(range)
					)
			{}
			template<typename Result>
			SPROUT_CONSTEXPR operator Result() const {
				return sprout::range::fixed::copy<Result>(*this);
			}
		};

		//
		// copy_holder
		//
		template<typename Result>
		class copy_holder {
		public:
			typedef Result result_type;
		private:
			sprout::value_holder<result_type&> result_;
		public:
			SPROUT_CONSTEXPR copy_holder() SPROUT_DEFAULTED_DEFAULT_CONSTRUCTOR_DECL
			copy_holder(copy_holder const&) = default;
			explicit SPROUT_CONSTEXPR copy_holder(result_type& result)
				: result_(result)
			{}
			SPROUT_CONSTEXPR result_type const& result() const {
				return result_;
			}
		};

		//
		// copied_forwarder
		//
		class copied_forwarder {
		public:
			template<typename Result>
			SPROUT_CONSTEXPR typename sprout::adaptors::copy_holder<Result const>
			operator()(Result const& result) const {
				return typename sprout::adaptors::copy_holder<Result const>(result);
			}
		};

		//
		// copied
		//
		namespace {
			SPROUT_STATIC_CONSTEXPR sprout::adaptors::copied_forwarder copied = {};
		}	// anonymous-namespace

		//
		// operator|
		//
		template<typename Range, typename Result>
		inline SPROUT_CONSTEXPR typename sprout::fixed::results::algorithm<Result>::type
		operator|(Range&& lhs, sprout::adaptors::copy_holder<Result> const& rhs) {
			return sprout::range::fixed::copy(sprout::lvalue_forward<Range>(lhs), rhs.result());
		}
		template<typename Range>
		inline SPROUT_CONSTEXPR sprout::adaptors::copied_range<
			typename std::remove_reference<typename sprout::lvalue_reference<Range>::type>::type
		>
		operator|(Range&& lhs, sprout::adaptors::copied_forwarder const&) {
			return sprout::adaptors::copied_range<
				typename std::remove_reference<typename sprout::lvalue_reference<Range>::type>::type
			>(
				sprout::lvalue_forward<Range>(lhs)
				);
		}
	}	// namespace adaptors

	//
	// container_construct_traits
	//
	template<typename Range>
	struct container_construct_traits<sprout::adaptors::copied_range<Range> >
		: public sprout::container_construct_traits<typename sprout::adaptors::copied_range<Range>::base_type>
	{};
}	// namespace sprout

#endif	// #ifndef SPROUT_RANGE_ADAPTOR_COPIED_HPP
