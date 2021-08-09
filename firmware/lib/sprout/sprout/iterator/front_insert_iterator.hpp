/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_ITERATOR_FRONT_INSERT_ITERATOR_HPP
#define SPROUT_ITERATOR_FRONT_INSERT_ITERATOR_HPP

#include <iterator>
#include <sprout/config.hpp>
#include <sprout/iterator/iterator.hpp>
#include <sprout/iterator/next.hpp>
#include <sprout/iterator/prev.hpp>
#include <sprout/iterator/distance.hpp>
#include <sprout/iterator/insert_range_iterator.hpp>
#include <sprout/container/traits.hpp>
#include <sprout/container/functions.hpp>
#include <sprout/container/container_holder.hpp>
#include <sprout/utility/forward.hpp>
#include <sprout/utility/move.hpp>
#include <sprout/utility/limited.hpp>
#include <sprout/type_traits/arithmetic_promote.hpp>

namespace sprout {
	//
	// front_insert_iterator
	//
	template<typename Container>
	class front_insert_iterator
		: public sprout::iterator<std::output_iterator_tag, void, void, void, void>
		, public sprout::container_holder<Container>
	{
	private:
		typedef sprout::container_holder<Container> base_type;
	public:
		typedef typename base_type::container_type container_type;
		typedef typename base_type::internal_type internal_type;
		typedef typename base_type::iterator iterator;
		typedef typename base_type::const_iterator const_iterator;
		typedef typename base_type::reference reference;
		typedef typename base_type::const_reference const_reference;
		typedef typename base_type::size_type size_type;
		typedef typename base_type::difference_type difference_type;
		typedef typename base_type::pointer pointer;
		typedef typename base_type::const_pointer const_pointer;
		typedef typename base_type::param_type param_type;
		typedef typename base_type::const_param_type const_param_type;
		typedef typename base_type::holder_type holder_type;
	protected:
		using base_type::container;
	public:
		SPROUT_CONSTEXPR front_insert_iterator() SPROUT_DEFAULTED_DEFAULT_CONSTRUCTOR_DECL
		explicit SPROUT_CONSTEXPR front_insert_iterator(param_type x)
			: base_type(x)
		{}
		front_insert_iterator(front_insert_iterator const&) = default;
		SPROUT_CXX14_CONSTEXPR front_insert_iterator& operator=(typename container_type::value_type const& value) {
			container->push_front(value);
			return *this;
		}
		SPROUT_CXX14_CONSTEXPR front_insert_iterator& operator=(typename container_type::value_type&& value) {
			container->push_front(sprout::move(value));
			return *this;
		}
		SPROUT_CONSTEXPR front_insert_iterator& operator*() const {
			return *this;
		}
		SPROUT_CXX14_CONSTEXPR front_insert_iterator& operator*() {
			return *this;
		}
		SPROUT_CXX14_CONSTEXPR front_insert_iterator& operator++() {
			return *this;
		}
		SPROUT_CXX14_CONSTEXPR front_insert_iterator operator++(int) {
			return *this;
		}

		SPROUT_CXX14_CONSTEXPR void swap(front_insert_iterator& other)
		SPROUT_NOEXCEPT_IF_EXPR(base_type::swap(other))
		{
			base_type::swap(other);
		}
	};

	//
	// swap
	//
	template<typename Container>
	inline SPROUT_CXX14_CONSTEXPR void
	swap(sprout::front_insert_iterator<Container>& lhs, sprout::front_insert_iterator<Container>& rhs)
	SPROUT_NOEXCEPT_IF_EXPR(lhs.swap(rhs))
	{
		lhs.swap(rhs);
	}

	//
	// front_inserter
	//
	template<typename Container>
	inline SPROUT_CONSTEXPR sprout::front_insert_iterator<Container>
	front_inserter(Container& x) {
		return sprout::front_insert_iterator<Container>(x);
	}
	template<typename Container>
	inline SPROUT_CONSTEXPR sprout::front_insert_iterator<Container const>
	front_inserter(Container const& x) {
		return sprout::front_insert_iterator<Container const>(x);
	}

	//
	// container_construct_traits
	//
	template<typename Container>
	struct container_construct_traits<sprout::front_insert_iterator<Container> > {
	public:
		typedef typename sprout::container_construct_traits<Container>::copied_type copied_type;
	private:
		template<typename Cont, typename InputIterator>
		static SPROUT_CONSTEXPR copied_type
		remake_impl(Cont&& cont, InputIterator first, InputIterator last) {
			return sprout::make<copied_type>(
				sprout::make_insert_range_iterator(
					sprout::internal_begin(cont), sprout::begin(cont),
					first, first, last
					),
				sprout::make_insert_range_iterator(
					sprout::internal_end(cont), sprout::begin(cont),
					last, first, last
					)
				);
		}
	public:
		template<typename Cont>
		static SPROUT_CONSTEXPR copied_type
		deep_copy(Cont&& cont) {
			return sprout::deep_copy(sprout::get_internal(SPROUT_FORWARD(Cont, cont)));
		}
		template<typename... Args>
		static SPROUT_CONSTEXPR copied_type
		make(Args&&... args) {
			return sprout::make<copied_type>(SPROUT_FORWARD(Args, args)...);
		}
		template<typename Cont, typename... Args>
		static SPROUT_CONSTEXPR copied_type
		remake(Cont&& cont, typename sprout::container_traits<Container>::difference_type, Args&&... args) {
			return remake_impl(sprout::get_internal(SPROUT_FORWARD(Cont, cont)), SPROUT_FORWARD(Args, args)...);
		}
	};

	//
	// container_fitness_traits
	//
	template<typename Container>
	struct container_fitness_traits<sprout::front_insert_iterator<Container> > {
	public:
		template<typename Cont>
		static SPROUT_CONSTEXPR typename sprout::container_traits<Container>::difference_type
		fit_size(Cont&& cont, typename sprout::container_traits<Container>::difference_type size) {
			return size + sprout::size(SPROUT_FORWARD(Cont, cont));
		}
	};

	//
	// sub_container_traits
	//
	template<typename Container>
	struct sub_container_traits<sprout::front_insert_iterator<Container> >
		: public sprout::sub_container_traits<sprout::container_holder<Container> >
	{};
}	// namespace sprout

#endif	// #ifndef SPROUT_ITERATOR_FRONT_INSERT_ITERATOR_HPP
