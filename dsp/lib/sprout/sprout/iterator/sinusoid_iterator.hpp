/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_ITERATOR_SINUSOID_ITERATOR_HPP
#define SPROUT_ITERATOR_SINUSOID_ITERATOR_HPP

#include <iterator>
#include <utility>
#include <type_traits>
#include <sprout/config.hpp>
#include <sprout/workaround/std/cstddef.hpp>
#include <sprout/iterator/iterator.hpp>
#include <sprout/iterator/next.hpp>
#include <sprout/iterator/prev.hpp>
#include <sprout/iterator/distance.hpp>
#include <sprout/utility/swap.hpp>
#include <sprout/numeric/dft/detail/sinusoid.hpp>

namespace sprout {
	//
	// sinusoid_iterator
	//
	template<typename Value>
	class sinusoid_iterator
		: public sprout::iterator<
			std::random_access_iterator_tag,
			Value,
			std::ptrdiff_t,
			Value*,
			Value
		>
	{
	public:
		typedef std::random_access_iterator_tag iterator_category;
		typedef Value value_type;
		typedef std::ptrdiff_t difference_type;
		typedef value_type* pointer;
		typedef value_type reference;
	private:
		difference_type index_;
		value_type frequency_;
		value_type amplitude_;
		value_type phase_;
		value_type d_;
	private:
		explicit SPROUT_CONSTEXPR sinusoid_iterator(sinusoid_iterator const& other, difference_type index)
			: index_(index)
			, frequency_(other.frequency_)
			, amplitude_(other.amplitude_)
			, phase_(other.phase_)
			, d_(other.d_)
		{}
	public:
		SPROUT_CONSTEXPR sinusoid_iterator()
			: index_()
			, frequency_(1)
			, amplitude_(1)
			, phase_(0)
			, d_(sprout::detail::sinusoid_value_d(frequency_))
		{}
		sinusoid_iterator(sinusoid_iterator const&) = default;
		explicit SPROUT_CONSTEXPR sinusoid_iterator(
			difference_type index,
			value_type const& frequency = 1,
			value_type const& amplitude = 1,
			value_type const& phase = 0
			)
			: index_(index)
			, frequency_(frequency)
			, amplitude_(amplitude)
			, phase_(phase)
			, d_(sprout::detail::sinusoid_value_d(frequency_))
		{}
		template<typename U>
		SPROUT_CONSTEXPR sinusoid_iterator(sinusoid_iterator<U> const& it)
			: index_(it.index_)
			, frequency_(it.frequency_)
			, amplitude_(it.amplitude_)
			, phase_(it.phase_)
			, d_(it.d_)
		{}
		template<typename U>
		SPROUT_CXX14_CONSTEXPR sinusoid_iterator& operator=(sinusoid_iterator<U> const& it) {
			sinusoid_iterator temp(it);
			temp.swap(*this);
			return *this;
		}
		SPROUT_CONSTEXPR difference_type index() const {
			return index_;
		}
		SPROUT_CONSTEXPR value_type const& frequency() const {
			return frequency_;
		}
		SPROUT_CONSTEXPR value_type const& amplitude() const {
			return amplitude_;
		}
		SPROUT_CONSTEXPR value_type const& phase() const {
			return phase_;
		}
		SPROUT_CONSTEXPR reference operator*() const {
			return amplitude_ == 0 ? 0
				: sprout::detail::sinusoid_value(amplitude_, phase_, d_, index_)
				;
		}
		SPROUT_CONSTEXPR pointer operator->() const {
			return &operator*()();
		}
		SPROUT_CXX14_CONSTEXPR sinusoid_iterator& operator++() {
			++index_;
			return *this;
		}
		SPROUT_CXX14_CONSTEXPR sinusoid_iterator operator++(int) {
			sinusoid_iterator result(*this);
			++index_;
			return result;
		}
		SPROUT_CXX14_CONSTEXPR sinusoid_iterator& operator--() {
			--index_;
			return *this;
		}
		SPROUT_CXX14_CONSTEXPR sinusoid_iterator operator--(int) {
			sinusoid_iterator temp(*this);
			--index_;
			return temp;
		}
		SPROUT_CONSTEXPR sinusoid_iterator operator+(difference_type n) const {
			return sinusoid_iterator(*this, index_ + n);
		}
		SPROUT_CONSTEXPR sinusoid_iterator operator-(difference_type n) const {
			return sinusoid_iterator(*this, index_ - n);
		}
		SPROUT_CXX14_CONSTEXPR sinusoid_iterator& operator+=(difference_type n) {
			sinusoid_iterator temp(*this, index_ + n);
			temp.swap(*this);
			return *this;
		}
		SPROUT_CXX14_CONSTEXPR sinusoid_iterator& operator-=(difference_type n) {
			sinusoid_iterator temp(*this, index_ - n);
			temp.swap(*this);
			return *this;
		}
		SPROUT_CONSTEXPR reference operator[](difference_type n) const {
			return amplitude_ == 0 ? 0
				: sprout::detail::sinusoid_value(amplitude_, phase_, d_, index_ + n)
				;
		}
		SPROUT_CONSTEXPR sinusoid_iterator next() const {
			return sinusoid_iterator(*this, index_ + 1);
		}
		SPROUT_CONSTEXPR sinusoid_iterator prev() const {
			return sinusoid_iterator(*this, index_ - 1);
		}
		SPROUT_CXX14_CONSTEXPR void swap(sinusoid_iterator& other) SPROUT_NOEXCEPT {
			sprout::swap(index_, other.index_);
			sprout::swap(frequency_, other.frequency_);
			sprout::swap(amplitude_, other.amplitude_);
			sprout::swap(phase_, other.phase_);
			sprout::swap(d_, other.d_);
		}
	};

	template<typename Value1, typename Value2>
	inline SPROUT_CONSTEXPR bool
	operator==(sprout::sinusoid_iterator<Value1> const& lhs, sprout::sinusoid_iterator<Value2> const& rhs) {
		return lhs.index() == rhs.index();
	}
	template<typename Value1, typename Value2>
	inline SPROUT_CONSTEXPR bool
	operator!=(sprout::sinusoid_iterator<Value1> const& lhs, sprout::sinusoid_iterator<Value2> const& rhs) {
		return !(lhs == rhs);
	}
	template<typename Value1, typename Value2>
	inline SPROUT_CONSTEXPR bool
	operator<(sprout::sinusoid_iterator<Value1> const& lhs, sprout::sinusoid_iterator<Value2> const& rhs) {
		return lhs.index() < rhs.index();
	}
	template<typename Value1, typename Value2>
	inline SPROUT_CONSTEXPR bool
	operator>(sprout::sinusoid_iterator<Value1> const& lhs, sprout::sinusoid_iterator<Value2> const& rhs) {
		return rhs < lhs;
	}
	template<typename Value1, typename Value2>
	inline SPROUT_CONSTEXPR bool
	operator<=(sprout::sinusoid_iterator<Value1> const& lhs, sprout::sinusoid_iterator<Value2> const& rhs) {
		return !(rhs < lhs);
	}
	template<typename Value1, typename Value2>
	inline SPROUT_CONSTEXPR bool
	operator>=(sprout::sinusoid_iterator<Value1> const& lhs, sprout::sinusoid_iterator<Value2> const& rhs) {
		return !(lhs < rhs);
	}
	template<typename Value1, typename Value2>
	inline SPROUT_CONSTEXPR typename sprout::sinusoid_iterator<Value1>::difference_type
	operator-(sprout::sinusoid_iterator<Value1> const& lhs, sprout::sinusoid_iterator<Value2> const& rhs) {
		return lhs.index() - rhs.index();
	}
	template<typename Value>
	inline SPROUT_CONSTEXPR sprout::sinusoid_iterator<Value>
	operator+(typename sprout::sinusoid_iterator<Value>::difference_type n, sprout::sinusoid_iterator<Value> const& it) {
		return it + n;
	}

	//
	// swap
	//
	template<typename Value>
	inline SPROUT_CXX14_CONSTEXPR void
	swap(sprout::sinusoid_iterator<Value>& lhs, sprout::sinusoid_iterator<Value>& rhs)
	SPROUT_NOEXCEPT_IF_EXPR(lhs.swap(rhs))
	{
		lhs.swap(rhs);
	}

	//
	// iterator_next
	//
	template<typename Value>
	inline SPROUT_CONSTEXPR sprout::sinusoid_iterator<Value>
	iterator_next(sprout::sinusoid_iterator<Value> const& it) {
		return it.next();
	}

	//
	// iterator_prev
	//
	template<typename Value>
	inline SPROUT_CONSTEXPR sprout::sinusoid_iterator<Value>
	iterator_prev(sprout::sinusoid_iterator<Value> const& it) {
		return it.prev();
	}
}	// namespace sprout

#endif	// #ifndef SPROUT_ITERATOR_SINUSOID_ITERATOR_HPP
