/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_ITERATOR_INDEXED_ITERATOR_HPP
#define SPROUT_ITERATOR_INDEXED_ITERATOR_HPP

#include <utility>
#include <sprout/config.hpp>
#include <sprout/iterator/next.hpp>
#include <sprout/iterator/prev.hpp>
#include <sprout/iterator/distance.hpp>
#include <sprout/iterator/detail/iterator_base.hpp>
#include <sprout/utility/swap.hpp>

namespace sprout {
	//
	// indexed_iterator
	//
	template<typename Iterator>
	class indexed_iterator
		: public sprout::detail::iterator_base<Iterator>::type
	{
	private:
		typedef typename sprout::detail::iterator_base<Iterator>::type base_type;
	public:
		typedef Iterator iterator_type;
		typedef typename base_type::iterator_category iterator_category;
		typedef typename base_type::value_type value_type;
		typedef typename base_type::difference_type difference_type;
		typedef typename base_type::pointer pointer;
		typedef typename base_type::reference reference;
		typedef difference_type index_type;
	protected:
		iterator_type current;
		index_type idx;
	public:
		SPROUT_CONSTEXPR indexed_iterator()
			: current(), idx()
		{}
		SPROUT_CONSTEXPR indexed_iterator(indexed_iterator const& other)
			: current(other.current)
			, idx(other.idx)
		{}
		explicit SPROUT_CONSTEXPR indexed_iterator(iterator_type it, index_type idx = 0)
			: current(it)
			, idx(idx)
		{}
		template<typename U>
		SPROUT_CONSTEXPR indexed_iterator(indexed_iterator<U> const& it)
			: current(it.base())
			, idx(it.index())
		{}
		template<typename U>
		SPROUT_CXX14_CONSTEXPR indexed_iterator& operator=(indexed_iterator<U> const& it) {
			indexed_iterator temp(it);
			temp.swap(*this);
			return *this;
		}
		SPROUT_CONSTEXPR iterator_type base() const {
			return current;
		}
		SPROUT_CONSTEXPR index_type index() const {
			return idx;
		}
		SPROUT_CONSTEXPR reference operator*() const {
			return *current;
		}
		SPROUT_CONSTEXPR pointer operator->() const {
			return &*current;
		}
		SPROUT_CXX14_CONSTEXPR indexed_iterator& operator++() {
			++current;
			++idx;
			return *this;
		}
		SPROUT_CXX14_CONSTEXPR indexed_iterator operator++(int) {
			indexed_iterator result(*this);
			++current;
			++idx;
			return result;
		}
		SPROUT_CXX14_CONSTEXPR indexed_iterator& operator--() {
			--current;
			--idx;
			return *this;
		}
		SPROUT_CXX14_CONSTEXPR indexed_iterator operator--(int) {
			indexed_iterator temp(*this);
			--current;
			--idx;
			return temp;
		}
		SPROUT_CONSTEXPR indexed_iterator operator+(difference_type n) const {
			return indexed_iterator(current + n, idx + n);
		}
		SPROUT_CONSTEXPR indexed_iterator operator-(difference_type n) const {
			return indexed_iterator(current - n, idx - n);
		}
		SPROUT_CXX14_CONSTEXPR indexed_iterator& operator+=(difference_type n) {
			indexed_iterator temp(current + n, idx + n);
			temp.swap(*this);
			return *this;
		}
		SPROUT_CXX14_CONSTEXPR indexed_iterator& operator-=(difference_type n) {
			indexed_iterator temp(current - n, idx - n);
			temp.swap(*this);
			return *this;
		}
		SPROUT_CONSTEXPR reference operator[](difference_type n) const {
			return *(current + n);
		}
		SPROUT_CONSTEXPR indexed_iterator next() const {
			return indexed_iterator(sprout::next(current), idx + 1);
		}
		SPROUT_CONSTEXPR indexed_iterator prev() const {
			return indexed_iterator(sprout::prev(current), idx - 1);
		}
		SPROUT_CXX14_CONSTEXPR void swap(indexed_iterator& other)
		SPROUT_NOEXCEPT_IF(
			SPROUT_NOEXCEPT_EXPR(swap(current, other.current))
			&& SPROUT_NOEXCEPT_EXPR(swap(idx, other.idx))
			)
		{
			swap(current, other.current);
			swap(idx, other.idx);
		}
	};

	template<typename Iterator1, typename Iterator2>
	inline SPROUT_CONSTEXPR bool
	operator==(sprout::indexed_iterator<Iterator1> const& lhs, sprout::indexed_iterator<Iterator2> const& rhs) {
		return lhs.base() == rhs.base();
	}
	template<typename Iterator1, typename Iterator2>
	inline SPROUT_CONSTEXPR bool
	operator!=(sprout::indexed_iterator<Iterator1> const& lhs, sprout::indexed_iterator<Iterator2> const& rhs) {
		return !(lhs == rhs);
	}
	template<typename Iterator1, typename Iterator2>
	inline SPROUT_CONSTEXPR bool
	operator<(sprout::indexed_iterator<Iterator1> const& lhs, sprout::indexed_iterator<Iterator2> const& rhs) {
		return lhs.base() < rhs.base();
	}
	template<typename Iterator1, typename Iterator2>
	inline SPROUT_CONSTEXPR bool
	operator>(sprout::indexed_iterator<Iterator1> const& lhs, sprout::indexed_iterator<Iterator2> const& rhs) {
		return rhs < lhs;
	}
	template<typename Iterator1, typename Iterator2>
	inline SPROUT_CONSTEXPR bool
	operator<=(sprout::indexed_iterator<Iterator1> const& lhs, sprout::indexed_iterator<Iterator2> const& rhs) {
		return !(rhs < lhs);
	}
	template<typename Iterator1, typename Iterator2>
	inline SPROUT_CONSTEXPR bool
	operator>=(sprout::indexed_iterator<Iterator1> const& lhs, sprout::indexed_iterator<Iterator2> const& rhs) {
		return !(lhs < rhs);
	}
	template<typename Iterator1, typename Iterator2>
	inline SPROUT_CONSTEXPR decltype(std::declval<Iterator1>() - std::declval<Iterator2>())
	operator-(sprout::indexed_iterator<Iterator1> const& lhs, sprout::indexed_iterator<Iterator2> const& rhs) {
		return lhs.base() - rhs.base();
	}
	template<typename Iterator>
	inline SPROUT_CONSTEXPR sprout::indexed_iterator<Iterator>
	operator+(
		typename sprout::indexed_iterator<Iterator>::difference_type n,
		sprout::indexed_iterator<Iterator> const& it
		)
	{
		return it + n;
	}

	//
	// make_indexed_iterator
	//
	template<typename Iterator>
	inline SPROUT_CONSTEXPR sprout::indexed_iterator<Iterator>
	make_indexed_iterator(Iterator it, typename sprout::indexed_iterator<Iterator>::index_type idx = 0) {
		return sprout::indexed_iterator<Iterator>(it, idx);
	}

	//
	// swap
	//
	template<typename Iterator>
	inline SPROUT_CXX14_CONSTEXPR void
	swap(sprout::indexed_iterator<Iterator>& lhs, sprout::indexed_iterator<Iterator>& rhs)
	SPROUT_NOEXCEPT_IF_EXPR(lhs.swap(rhs))
	{
		lhs.swap(rhs);
	}

	//
	// iterator_next
	//
	template<typename Iterator>
	inline SPROUT_CONSTEXPR sprout::indexed_iterator<Iterator>
	iterator_next(sprout::indexed_iterator<Iterator> const& it) {
		return it.next();
	}

	//
	// iterator_prev
	//
	template<typename Iterator>
	inline SPROUT_CONSTEXPR sprout::indexed_iterator<Iterator>
	iterator_prev(sprout::indexed_iterator<Iterator> const& it) {
		return it.prev();
	}
}	// namespace sprout

#endif	// #ifndef SPROUT_ITERATOR_INDEXED_ITERATOR_HPP
