/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_FUNCTIONAL_BIND_BIND_HPP
#define SPROUT_FUNCTIONAL_BIND_BIND_HPP

#include <utility>
#include <tuple>
#include <type_traits>
#include <functional>
#include <sprout/config.hpp>
#include <sprout/workaround/std/cstddef.hpp>
#include <sprout/index_tuple/metafunction.hpp>
#include <sprout/utility/forward.hpp>
#include <sprout/utility/lvalue_forward.hpp>
#include <sprout/tuple/tuple/tuple.hpp>
#include <sprout/tuple/tuple/get.hpp>
#include <sprout/tuple/tuple/make_tuple.hpp>
#include <sprout/type/algorithm/contains_if.hpp>
#include <sprout/type/algorithm/lower_bound_index.hpp>
#include <sprout/type/type_tuple.hpp>
#include <sprout/type/integral_array.hpp>
#include <sprout/functional/ref.hpp>
#include <sprout/functional/mem_fn.hpp>
#include <sprout/functional/type_traits/weak_result_type.hpp>
#include <sprout/functional/bind/placeholder.hpp>
#include <sprout/type_traits/integral_constant.hpp>
#include <sprout/type_traits/identity.hpp>

namespace sprout {
	// 20.8.9 bind

	//
	// is_bind_expression
	//
	template<typename T>
	struct is_bind_expression
		: public sprout::false_type
	{};
	template<typename T>
	struct is_bind_expression<T const>
		: public sprout::is_bind_expression<T>
	{};
	template<typename T>
	struct is_bind_expression<T volatile>
		: public sprout::is_bind_expression<T>
	{};
	template<typename T>
	struct is_bind_expression<T const volatile>
		: public sprout::is_bind_expression<T>
	{};

#if SPROUT_USE_VARIABLE_TEMPLATES
	template<typename T>
	SPROUT_STATIC_CONSTEXPR bool is_bind_expression_v = sprout::is_bind_expression<T>::value;
#endif	// #if SPROUT_USE_VARIABLE_TEMPLATES

	namespace detail {
		struct no_tuple_element;

		template<std::size_t I, typename Tuple, bool IsSafe>
		struct safe_tuple_element_impl
			: tuple_element<I, Tuple>
		{};
		template<std::size_t I, typename Tuple>
		struct safe_tuple_element_impl<I, Tuple, false> {
		public:
			typedef sprout::detail::no_tuple_element type;
		};
		template<std::size_t I, typename Tuple>
		struct safe_tuple_element
			: public sprout::detail::safe_tuple_element_impl<I, Tuple, (I < sprout::tuples::tuple_size<Tuple>::value)>
		{};

		template<
			typename Arg,
			bool IsBindExp = sprout::is_bind_expression<Arg>::value,
			bool IsPlaceholder = (sprout::is_placeholder<Arg>::value > 0)
		>
		class mu;
		template<typename T>
		class mu<sprout::reference_wrapper<T>, false, false> {
		public:
			typedef T& result_type;
		public:
			template<typename CVRef, typename Tuple>
			SPROUT_CONSTEXPR result_type operator()(CVRef& arg, Tuple&) const volatile {
				return const_cast<typename std::remove_volatile<typename std::remove_reference<CVRef>::type>::type&>(arg).get();
			}
		};
		template<typename T>
		class mu<std::reference_wrapper<T>, false, false> {
		public:
			typedef T& result_type;
		public:
			template<typename CVRef, typename Tuple>
			SPROUT_CONSTEXPR result_type operator()(CVRef& arg, Tuple&) const volatile {
				return const_cast<typename std::remove_volatile<typename std::remove_reference<CVRef>::type>::type&>(arg).get();
			}
		};
		template<typename Arg>
		class mu<Arg, true, false> {
		private:
			template<typename CVArg, typename... Args, sprout::index_t... Indexes>
			SPROUT_CONSTEXPR auto call(
				CVArg& arg,
				sprout::tuples::tuple<Args...>& tuple,
				sprout::index_tuple<Indexes...>
				) const volatile
			-> decltype(arg(std::declval<Args>()...))
			{
				return arg(SPROUT_FORWARD(Args, sprout::tuples::get<Indexes>(tuple))...);
			}
		public:
			template<typename CVArg, typename... Args>
			SPROUT_CONSTEXPR auto operator()(CVArg& arg, sprout::tuples::tuple<Args...>& tuple) const volatile
			-> decltype(arg(std::declval<Args>()...))
			{
				return call(arg, tuple, sprout::index_pack<Args...>::make());
			}
		};
		template<typename Arg>
		class mu<Arg, false, true> {
		public:
			template<typename Signature>
			class result;
			template<typename CVMu, typename CVArg, typename Tuple>
			class result<CVMu (CVArg, Tuple)> {
			private:
				typedef typename sprout::detail::safe_tuple_element<
					(sprout::is_placeholder<Arg>::value - 1),
					Tuple
				>::type base_type;
			public:
				typedef typename std::add_rvalue_reference<base_type>::type type;
			};
		public:
			template<typename Tuple>
			typename result<mu (Arg, Tuple)>::type
			SPROUT_CONSTEXPR operator()(Arg /*const volatile&*/, Tuple& tuple) const volatile {
			typedef typename result<mu (Arg, Tuple)>::type type;
				return SPROUT_FORWARD(type, sprout::tuples::get<(sprout::is_placeholder<Arg>::value - 1)>(tuple));
			}
		};
		template<typename Arg>
		class mu<Arg, false, false> {
		public:
			template<typename Signature>
			struct result;
			template<typename CVMu, typename CVArg, typename Tuple>
			struct result<CVMu (CVArg, Tuple)> {
			public:
				typedef typename std::add_lvalue_reference<CVArg>::type type;
			};
		public:
			template<typename CVArg, typename Tuple>
			SPROUT_CONSTEXPR CVArg&& operator()(CVArg&& arg, Tuple&) const volatile {
				return SPROUT_FORWARD(CVArg, arg);
			}
		};

		template<typename T>
		struct maybe_wrap_member_pointer {
		public:
			typedef T type;
		public:
			static SPROUT_CONSTEXPR T const& do_wrap(T const& x) {
				return x;
			}
			static SPROUT_CONSTEXPR T&& do_wrap(T&& x) {
				return static_cast<T&&>(x);
			}
		};
		template<typename T, typename Class>
		struct maybe_wrap_member_pointer<T Class::*> {
		public:
			typedef sprout::mem_fn_adaptor<T Class::*> type;
		public:
			static SPROUT_CONSTEXPR type do_wrap(T Class::* pm) {
				return type(pm);
			}
		};
		template<>
		struct maybe_wrap_member_pointer<void> {
		public:
			typedef void type;
		};

		template<std::size_t I, typename... Types>
		inline SPROUT_CONSTEXPR auto volget(sprout::tuples::tuple<Types...> volatile& tuple)
		-> typename sprout::tuples::tuple_element<I, sprout::tuples::tuple<Types...> >::type volatile&
		{
			return sprout::tuples::get<I>(const_cast<sprout::tuples::tuple<Types...>&>(tuple));
		}
		template<std::size_t I, typename... Types>
		inline SPROUT_CONSTEXPR auto volget(sprout::tuples::tuple<Types...> const volatile& tuple)
		-> typename sprout::tuples::tuple_element<I, sprout::tuples::tuple<Types...> >::type const volatile&
		{
			return sprout::tuples::get<I>(const_cast<sprout::tuples::tuple<Types...> const&>(tuple));
		}

		struct is_variadic_placeholder_pred {
		public:
			template<typename T>
			struct apply
				: public sprout::bool_constant<
					(sprout::is_variadic_placeholder<T>::value > 0)
				>
			{};
		};
		template<typename Bounds, typename = void>
		struct is_variadic_bounds
			: public sprout::bool_constant<
				sprout::types::contains_if<Bounds, sprout::detail::is_variadic_placeholder_pred>::value
			>
		{};

		template<typename T>
		struct tail_place
			: public sprout::integral_constant<int, sprout::is_variadic_placeholder<T>::value - 1>
		{};

		template<typename T, std::size_t ArgSize, typename = void>
		struct bound_size
			: public sprout::integral_constant<std::size_t, 1>
		{};
		template<typename T, std::size_t ArgSize>
		struct bound_size<
			T, ArgSize,
			typename std::enable_if<(sprout::is_variadic_placeholder<T>::value > 0)>::type
		>
			: public sprout::integral_constant<
				std::size_t,
				(ArgSize - sprout::detail::tail_place<T>::value)
			>
		{};

		template<std::size_t I, typename Bounds, std::size_t ArgSize, typename = void>
		struct bounds_size_impl
			: public sprout::integral_constant<std::size_t, 0>
		{};
		template<std::size_t I, typename Bounds, std::size_t ArgSize>
		struct bounds_size_impl<
			I, Bounds, ArgSize,
			typename std::enable_if<(I < sprout::tuples::tuple_size<Bounds>::value)>::type
		>
			: public sprout::integral_constant<
				std::size_t,
				(sprout::detail::bound_size<typename sprout::tuples::tuple_element<I, Bounds>::type, ArgSize>::value
					+ sprout::detail::bounds_size_impl<I + 1, Bounds, ArgSize>::value
					)
			>
		{};
		template<typename Bounds, std::size_t ArgSize>
		struct bounds_size
			: public sprout::detail::bounds_size_impl<0, Bounds, ArgSize>
		{};

		template<std::size_t I, typename Bounds, std::size_t ArgSize, std::size_t Current, typename Seq, typename = void>
		struct bounds_partial_size_impl {
		public:
			typedef Seq type;
		};
		template<std::size_t I, typename Bounds, std::size_t ArgSize, std::size_t Current, std::size_t... Elems>
		struct bounds_partial_size_impl<
			I, Bounds, ArgSize, Current, sprout::types::integral_array<std::size_t, Elems...>,
			typename std::enable_if<(I < sprout::tuples::tuple_size<Bounds>::value)>::type
		>
			: public sprout::detail::bounds_partial_size_impl<
				I + 1, Bounds, ArgSize,
				Current + sprout::detail::bound_size<typename sprout::tuples::tuple_element<I, Bounds>::type, ArgSize>::value,
				sprout::types::integral_array<std::size_t, Elems..., Current>
			>
		{};
		template<typename Bounds, std::size_t ArgSize>
		struct bounds_partial_size
			: public sprout::detail::bounds_partial_size_impl<0, Bounds, ArgSize, 0, sprout::types::integral_array<std::size_t> >
		{};

		template<typename Bounds, std::size_t ArgSize>
		struct bound_indexes
			: public sprout::make_index_tuple<sprout::detail::bounds_size<Bounds, ArgSize>::value>
		{};

		template<sprout::index_t I, typename Bounds, std::size_t ArgSize>
		struct bound_position
			: public sprout::integral_constant<
				sprout::index_t,
				(sprout::types::lower_bound_index<
					typename sprout::detail::bounds_partial_size<Bounds, ArgSize>::type,
					sprout::integral_constant<std::size_t, I + 1>
					>::type::value - 1
					)
			>
		{};

		template<sprout::index_t I, typename Bounds, std::size_t ArgSize>
		struct is_variadic_part
			: public sprout::bool_constant<
				(sprout::is_variadic_placeholder<
						typename sprout::tuples::tuple_element<
							sprout::detail::bound_position<I, Bounds, ArgSize>::value,
							Bounds
						>::type
					>::value > 0
					)
			>
		{};

		template<sprout::index_t I, typename Bounds, std::size_t ArgSize, sprout::index_t BoundPos, typename SizeSeq, typename = void>
		struct bound_element_impl
			: public sprout::tuples::tuple_element<BoundPos, Bounds>
		{};
		template<sprout::index_t I, typename Bounds, std::size_t ArgSize, sprout::index_t BoundPos, typename SizeSeq>
		struct bound_element_impl<
			I, Bounds, ArgSize, BoundPos, SizeSeq,
			typename std::enable_if<sprout::detail::is_variadic_part<I, Bounds, ArgSize>::value>::type
		> {
		public:
			typedef sprout::placeholder<
				(I + 1
					- sprout::tuples::tuple_element<BoundPos, SizeSeq>::type::value
					+ sprout::detail::tail_place<typename sprout::tuples::tuple_element<BoundPos, Bounds>::type>::value
					)
			> type;
		};
		template<sprout::index_t I, typename Bounds, std::size_t ArgSize>
		struct bound_element
			: public sprout::detail::bound_element_impl<
				I, Bounds, ArgSize, sprout::detail::bound_position<I, Bounds, ArgSize>::value,
				typename sprout::detail::bounds_partial_size<Bounds, ArgSize>::type
			>
		{};

		struct get_bound_helper {
		public:
			template<
				sprout::index_t I, typename Bounds,
				typename sprout::enabler_if<!std::is_const<Bounds>::value && !std::is_volatile<Bounds>::value>::type = sprout::enabler
			>
			static SPROUT_CONSTEXPR auto
			get_bound(Bounds& bound_args)
			-> decltype(sprout::tuples::get<I>(bound_args))
			{
				return sprout::tuples::get<I>(bound_args);
			}
			template<
				sprout::index_t I, typename Bounds,
				typename sprout::enabler_if<std::is_const<Bounds const>::value && !std::is_volatile<Bounds const>::value>::type = sprout::enabler
			>
			static SPROUT_CONSTEXPR auto
			get_bound(Bounds const& bound_args)
			-> decltype(sprout::tuples::get<I>(bound_args))
			{
				return sprout::tuples::get<I>(bound_args);
			}
			template<
				sprout::index_t I, typename Bounds,
				typename sprout::enabler_if<!std::is_const<Bounds volatile>::value && std::is_volatile<Bounds volatile>::value>::type = sprout::enabler
			>
			static SPROUT_CONSTEXPR auto
			get_bound(Bounds volatile& bound_args)
			-> decltype(sprout::detail::volget<I>(bound_args))
			{
				return sprout::detail::volget<I>(bound_args);
			}
			template<
				sprout::index_t I, typename Bounds,
				typename sprout::enabler_if<std::is_const<Bounds const volatile>::value && std::is_volatile<Bounds const volatile>::value>::type = sprout::enabler
			>
			static SPROUT_CONSTEXPR auto
			get_bound(Bounds const volatile& bound_args)
			-> decltype(sprout::detail::volget<I>(bound_args))
			{
				return sprout::detail::volget<I>(bound_args);
			}
		};
		template<
			sprout::index_t I, std::size_t ArgSize, typename Bounds,
			typename sprout::enabler_if<sprout::detail::is_variadic_part<I, typename std::remove_reference<Bounds>::type, ArgSize>::value>::type = sprout::enabler
		>
		inline SPROUT_CONSTEXPR typename sprout::detail::bound_element<I, typename std::remove_reference<Bounds>::type, ArgSize>::type
		get_bound(Bounds&&) {
			typedef typename sprout::detail::bound_element<I, typename std::remove_reference<Bounds>::type, ArgSize>::type type;
			return type();
		}
		template<
			sprout::index_t I, std::size_t ArgSize, typename Bounds,
			typename sprout::enabler_if<!sprout::detail::is_variadic_part<I, typename std::remove_reference<Bounds>::type, ArgSize>::value>::type = sprout::enabler
		>
		inline SPROUT_CONSTEXPR auto
		get_bound(Bounds&& bound_args)
		-> decltype(
			sprout::detail::get_bound_helper::get_bound<
				sprout::detail::bound_position<I, typename std::remove_reference<Bounds>::type, ArgSize>::value
				>(SPROUT_FORWARD(Bounds, bound_args))
			)
		{
			return sprout::detail::get_bound_helper::get_bound<
				sprout::detail::bound_position<I, typename std::remove_reference<Bounds>::type, ArgSize>::value
				>(SPROUT_FORWARD(Bounds, bound_args));
		}
	}	// namespace detail

	namespace detail {
		template<typename Signature>
		class binder_impl;
		template<typename Functor, typename... BoundArgs>
		class binder_impl<Functor(BoundArgs...)>
			: public sprout::weak_result_type<Functor>
		{
		protected:
			typedef sprout::tuples::tuple<BoundArgs...> bounds_type;
		private:
			Functor f_;
			bounds_type bound_args_;
		protected:
			template<typename Result, typename... Args, sprout::index_t... Indexes>
			Result call(sprout::tuples::tuple<Args...>&& args, sprout::index_tuple<Indexes...>) {
				return f_(
					sprout::detail::mu<typename sprout::detail::bound_element<Indexes, bounds_type, sizeof...(Args)>::type>()
						(sprout::detail::get_bound<Indexes, sizeof...(Args)>(bound_args_), args)...
					);
			}
			template<typename Result, typename... Args, sprout::index_t... Indexes>
			SPROUT_CONSTEXPR Result call_c(sprout::tuples::tuple<Args...>&& args, sprout::index_tuple<Indexes...>) const {
				return f_(
					sprout::detail::mu<typename sprout::detail::bound_element<Indexes, bounds_type, sizeof...(Args)>::type const>()
						(sprout::detail::get_bound<Indexes, sizeof...(Args)>(bound_args_), args)...
					);
			}
			template<typename Result, typename... Args, sprout::index_t... Indexes>
			Result call_v(sprout::tuples::tuple<Args...>&& args, sprout::index_tuple<Indexes...>) volatile {
				return f_(
					sprout::detail::mu<typename sprout::detail::bound_element<Indexes, bounds_type, sizeof...(Args)>::type volatile>()
						(sprout::detail::get_bound<Indexes, sizeof...(Args)>(bound_args_), args)...
					);
			}
			template<typename Result, typename... Args, sprout::index_t... Indexes>
			SPROUT_CONSTEXPR Result call_cv(sprout::tuples::tuple<Args...>&& args, sprout::index_tuple<Indexes...>) const volatile {
				return f_(
					sprout::detail::mu<typename sprout::detail::bound_element<Indexes, bounds_type, sizeof...(Args)>::type const volatile>()
						(sprout::detail::get_bound<Indexes, sizeof...(Args)>(bound_args_), args)...
					);
			}
		public:
			template<
				typename... Args, sprout::index_t... Indexes
			>
			static decltype(
				std::declval<Functor&>()(
					sprout::detail::mu<typename sprout::detail::bound_element<Indexes, bounds_type, sizeof...(Args)>::type>()
						(sprout::detail::get_bound<Indexes, sizeof...(Args)>(std::declval<bounds_type&>()), std::declval<sprout::tuples::tuple<Args...>&>())...
					)
				)
			call_(sprout::tuples::tuple<Args...>&& args, sprout::index_tuple<Indexes...>);
			template<
				typename... Args, sprout::index_t... Indexes
			>
			static decltype(
				std::declval<typename std::enable_if<(sizeof...(Args) >= 0), typename std::add_const<Functor>::type>::type&>()(
					sprout::detail::mu<typename sprout::detail::bound_element<Indexes, bounds_type, sizeof...(Args)>::type const>()
						(sprout::detail::get_bound<Indexes, sizeof...(Args)>(std::declval<bounds_type const&>()), std::declval<sprout::tuples::tuple<Args...>&>())...
					)
				)
			call_c_(sprout::tuples::tuple<Args...>&& args, sprout::index_tuple<Indexes...>);
			template<
				typename... Args, sprout::index_t... Indexes
			>
			static decltype(
				std::declval<typename std::enable_if<(sizeof...(Args) >= 0), typename std::add_volatile<Functor>::type>::type&>()(
					sprout::detail::mu<typename sprout::detail::bound_element<Indexes, bounds_type, sizeof...(Args)>::type volatile>()
						(sprout::detail::get_bound<Indexes, sizeof...(Args)>(std::declval<bounds_type volatile&>()), std::declval<sprout::tuples::tuple<Args...>&>())...
					)
				)
			call_v_(sprout::tuples::tuple<Args...>&& args, sprout::index_tuple<Indexes...>);
			template<
				typename... Args, sprout::index_t... Indexes
			>
			static decltype(
				std::declval<typename std::enable_if<(sizeof...(Args) >= 0), typename std::add_cv<Functor>::type>::type&>()(
					sprout::detail::mu<typename sprout::detail::bound_element<Indexes, bounds_type, sizeof...(Args)>::type const volatile>()
						(sprout::detail::get_bound<Indexes, sizeof...(Args)>(std::declval<bounds_type const volatile&>()), std::declval<sprout::tuples::tuple<Args...>&>())...
					)
				)
			call_cv_(sprout::tuples::tuple<Args...>&& args, sprout::index_tuple<Indexes...>);
		public:
			template<typename... Args>
			explicit SPROUT_CONSTEXPR binder_impl(Functor const& f, Args&&... args)
				: f_(f)
				, bound_args_(SPROUT_FORWARD(Args, args)...)
			{}
			binder_impl(binder_impl const&) = default;
		};
	}	// namespace detail
	//
	// binder
	//
	template<typename Signature>
	class binder;
	template<typename Functor, typename... BoundArgs>
	class binder<Functor(BoundArgs...)>
		: private sprout::detail::binder_impl<Functor(BoundArgs...)>
	{
	private:
		typedef sprout::detail::binder_impl<Functor(BoundArgs...)> impl_type;
		typedef typename impl_type::bounds_type bounds_type;
	public:
		template<typename... Args>
		explicit SPROUT_CONSTEXPR binder(Functor const& f, Args&&... args)
			: impl_type(f, SPROUT_FORWARD(Args, args)...)
		{}
		binder(binder const&) = default;
		template<
			typename... Args,
			typename Result = typename sprout::identity<decltype(
				impl_type::template call_(std::declval<sprout::tuples::tuple<Args...> >(), sprout::detail::bound_indexes<bounds_type, sizeof...(Args)>::make())
				)>::type
		>
		Result operator()(Args&&... args) {
			return impl_type::template call<Result>(
				sprout::tuples::forward_as_tuple(SPROUT_FORWARD(Args, args)...),
				sprout::detail::bound_indexes<bounds_type, sizeof...(Args)>::make()
				);
		}
		template<
			typename... Args,
			typename Result = typename sprout::identity<decltype(
				impl_type::template call_c_(std::declval<sprout::tuples::tuple<Args...> >(), sprout::detail::bound_indexes<bounds_type, sizeof...(Args)>::make())
				)>::type
		>
		SPROUT_CONSTEXPR Result operator()(Args&&... args) const {
			return impl_type::template call_c<Result>(
				sprout::tuples::forward_as_tuple(SPROUT_FORWARD(Args, args)...),
				sprout::detail::bound_indexes<bounds_type, sizeof...(Args)>::make()
				);
		}
		template<
			typename... Args,
			typename Result = typename sprout::identity<decltype(
				impl_type::template call_v_(std::declval<sprout::tuples::tuple<Args...> >(), sprout::detail::bound_indexes<bounds_type, sizeof...(Args)>::make())
				)>::type
		>
		Result operator()(Args&&... args) volatile {
			return impl_type::template call_v<Result>(
				sprout::tuples::forward_as_tuple(SPROUT_FORWARD(Args, args)...),
				sprout::detail::bound_indexes<bounds_type, sizeof...(Args)>::make()
				);
		}
		template<
			typename... Args,
			typename Result = typename sprout::identity<decltype(
				impl_type::template call_cv_(std::declval<sprout::tuples::tuple<Args...> >(), sprout::detail::bound_indexes<bounds_type, sizeof...(Args)>::make())
				)>::type
		>
		SPROUT_CONSTEXPR Result operator()(Args&&... args) const volatile {
			return impl_type::template call_cv<Result>(
				sprout::tuples::forward_as_tuple(SPROUT_FORWARD(Args, args)...),
				sprout::detail::bound_indexes<bounds_type, sizeof...(Args)>::make()
				);
		}
	};
	//
	// cbinder
	//
	template<typename Signature>
	class cbinder;
	template<typename Functor, typename... BoundArgs>
	class cbinder<Functor(BoundArgs...)>
		: private sprout::detail::binder_impl<Functor(BoundArgs...)>
	{
	private:
		typedef sprout::detail::binder_impl<Functor(BoundArgs...)> impl_type;
		typedef typename impl_type::bounds_type bounds_type;
	public:
		template<typename... Args>
		explicit SPROUT_CONSTEXPR cbinder(Functor const& f, Args&&... args)
			: impl_type(f, SPROUT_FORWARD(Args, args)...)
		{}
		cbinder(cbinder const&) = default;
		template<
			typename... Args,
			typename Result = typename sprout::identity<decltype(
				impl_type::template call_c_(std::declval<sprout::tuples::tuple<Args...> >(), sprout::detail::bound_indexes<bounds_type, sizeof...(Args)>::make())
				)>::type
		>
		SPROUT_CONSTEXPR Result operator()(Args&&... args) const {
			return impl_type::template call_c<Result>(
				sprout::tuples::forward_as_tuple(SPROUT_FORWARD(Args, args)...),
				sprout::detail::bound_indexes<bounds_type, sizeof...(Args)>::make()
				);
		}
		template<
			typename... Args,
			typename Result = typename sprout::identity<decltype(
				impl_type::template call_cv_(std::declval<sprout::tuples::tuple<Args...> >(), sprout::detail::bound_indexes<bounds_type, sizeof...(Args)>::make())
				)>::type
		>
		SPROUT_CONSTEXPR Result operator()(Args&&... args) const volatile {
			return impl_type::template call_cv<Result>(
				sprout::tuples::forward_as_tuple(SPROUT_FORWARD(Args, args)...),
				sprout::detail::bound_indexes<bounds_type, sizeof...(Args)>::make()
				);
		}
	};

	namespace detail {
		template<typename Result, typename Signature>
		class res_binder_impl;
		template<typename Result, typename Functor, typename... BoundArgs>
		class res_binder_impl<Result, Functor(BoundArgs...)> {
		public:
			typedef Result result_type;
		protected:
			typedef sprout::tuples::tuple<BoundArgs...> bounds_type;
		private:
			template<typename Res>
			struct enable_if_void
				: public std::enable_if<std::is_void<Res>::value, int>
			{};
			template<typename Res>
			struct disable_if_void
				: public std::enable_if<!std::is_void<Res>::value, int>
			{};
		private:
			Functor f_;
			sprout::tuples::tuple<BoundArgs...> bound_args_;
		protected:
			template<typename Res, typename... Args, sprout::index_t... Indexes>
			Result call(
				sprout::tuples::tuple<Args...>&& args,
				sprout::index_tuple<Indexes...>,
				typename disable_if_void<Res>::type = 0
				)
			{
				return f_(sprout::detail::mu<BoundArgs>()(sprout::detail::get_bound<Indexes, sizeof...(Args)>(bound_args_), args)...);
			}
			template<typename Res, typename... Args, sprout::index_t... Indexes>
			Result call(
				sprout::tuples::tuple<Args...>&& args,
				sprout::index_tuple<Indexes...>,
				typename enable_if_void<Res>::type = 0
				)
			{
				f_(sprout::detail::mu<BoundArgs>()(sprout::detail::get_bound<Indexes, sizeof...(Args)>(bound_args_), args)...);
			}
			template<typename Res, typename... Args, sprout::index_t... Indexes>
			SPROUT_CONSTEXPR Result call(
				sprout::tuples::tuple<Args...>&& args,
				sprout::index_tuple<Indexes...>,
				typename disable_if_void<Res>::type = 0
				) const
			{
				return f_(sprout::detail::mu<BoundArgs>()(sprout::detail::get_bound<Indexes, sizeof...(Args)>(bound_args_), args)...);
			}
			template<typename Res, typename... Args, sprout::index_t... Indexes>
			Result call(
				sprout::tuples::tuple<Args...>&& args,
				sprout::index_tuple<Indexes...>,
				typename enable_if_void<Res>::type = 0
				) const
			{
				f_(sprout::detail::mu<BoundArgs>()(sprout::detail::get_bound<Indexes, sizeof...(Args)>(bound_args_), args)...);
			}
			template<typename Res, typename... Args, sprout::index_t... Indexes>
			Result call(
				sprout::tuples::tuple<Args...>&& args,
				sprout::index_tuple<Indexes...>,
				typename disable_if_void<Res>::type = 0
				) volatile
			{
				return f_(sprout::detail::mu<BoundArgs>()(sprout::detail::get_bound<Indexes, sizeof...(Args)>(bound_args_), args)...);
			}
			template<typename Res, typename... Args, sprout::index_t... Indexes>
			Result call(
				sprout::tuples::tuple<Args...>&& args,
				sprout::index_tuple<Indexes...>,
				typename enable_if_void<Res>::type = 0
				) volatile
			{
				f_(sprout::detail::mu<BoundArgs, Indexes>()(sprout::detail::get_bound<Indexes, sizeof...(Args)>(bound_args_), args)...);
			}
			template<typename Res, typename... Args, sprout::index_t... Indexes>
			SPROUT_CONSTEXPR Result call(
				sprout::tuples::tuple<Args...>&& args,
				sprout::index_tuple<Indexes...>,
				typename disable_if_void<Res>::type = 0
				) const volatile
			{
				return f_(sprout::detail::mu<BoundArgs>()(sprout::detail::get_bound<Indexes, sizeof...(Args)>(bound_args_), args)...);
			}
			template<typename Res, typename... Args, sprout::index_t... Indexes>
			Result call(
				sprout::tuples::tuple<Args...>&& args,
				sprout::index_tuple<Indexes...>,
				typename enable_if_void<Res>::type = 0
				) const volatile
			{
				f_(sprout::detail::mu<BoundArgs>()(sprout::detail::get_bound<Indexes, sizeof...(Args)>(bound_args_), args)...);
			}
		public:
			template<typename... Args>
			explicit SPROUT_CONSTEXPR res_binder_impl(Functor const& f, Args&&... args)
				: f_(f)
				, bound_args_(SPROUT_FORWARD(Args, args)...)
			{}
			res_binder_impl(res_binder_impl const&) = default;
		};
	}	// namespace detail
	//
	// res_binder
	//
	template<typename Result, typename Signature>
	class res_binder;
	template<typename Result, typename Functor, typename... BoundArgs>
	class res_binder<Result, Functor(BoundArgs...)>
		: private sprout::detail::res_binder_impl<Result, Functor(BoundArgs...)>
	{
	public:
		typedef Result result_type;
	private:
		typedef sprout::detail::res_binder_impl<Result, Functor(BoundArgs...)> impl_type;
		typedef typename impl_type::bounds_type bounds_type;
	public:
		template<typename... Args>
		explicit res_binder(Functor const& f, Args&&... args)
			: impl_type(f, SPROUT_FORWARD(Args, args)...)
		{}
		res_binder(res_binder const&) = default;
		template<typename... Args>
		result_type operator()(Args&&... args) {
			return impl_type::template call<Result>(
				sprout::tuples::forward_as_tuple(SPROUT_FORWARD(Args, args)...),
				sprout::detail::bound_indexes<bounds_type, sizeof...(Args)>::make()
				);
		}
		template<typename... Args>
		SPROUT_CONSTEXPR result_type operator()(Args&&... args) const {
			return impl_type::template call<Result>(
				sprout::tuples::forward_as_tuple(SPROUT_FORWARD(Args, args)...),
				sprout::detail::bound_indexes<bounds_type, sizeof...(Args)>::make()
				);
		}
		template<typename... Args>
		result_type operator()(Args&&... args) volatile {
			return impl_type::template call<Result>(
				sprout::tuples::forward_as_tuple(SPROUT_FORWARD(Args, args)...),
				sprout::detail::bound_indexes<bounds_type, sizeof...(Args)>::make()
				);
		}
		template<typename... Args>
		SPROUT_CONSTEXPR result_type operator()(Args&&... args) const volatile {
			return impl_type::template call<Result>(
				sprout::tuples::forward_as_tuple(SPROUT_FORWARD(Args, args)...),
				sprout::detail::bound_indexes<bounds_type, sizeof...(Args)>::make()
				);
		}
	};
	//
	// res_cbinder
	//
	template<typename Result, typename Signature>
	class res_cbinder;
	template<typename Result, typename Functor, typename... BoundArgs>
	class res_cbinder<Result, Functor(BoundArgs...)>
		: private sprout::detail::res_binder_impl<Result, Functor(BoundArgs...)>
	{
	public:
		typedef Result result_type;
	private:
		typedef sprout::detail::res_binder_impl<Result, Functor(BoundArgs...)> impl_type;
		typedef typename impl_type::bounds_type bounds_type;
	public:
		template<typename... Args>
		explicit SPROUT_CONSTEXPR res_cbinder(Functor const& f, Args&&... args)
			: impl_type(f, SPROUT_FORWARD(Args, args)...)
		{}
		res_cbinder(res_cbinder const&) = default;
		template<typename... Args>
		SPROUT_CONSTEXPR result_type operator()(Args&&... args) const {
			return impl_type::template call<Result>(
				sprout::tuples::forward_as_tuple(SPROUT_FORWARD(Args, args)...),
				sprout::detail::bound_indexes<bounds_type, sizeof...(Args)>::make()
				);
		}
		template<typename... Args>
		SPROUT_CONSTEXPR result_type operator()(Args&&... args) const volatile {
			return impl_type::template call<Result>(
				sprout::tuples::forward_as_tuple(SPROUT_FORWARD(Args, args)...),
				sprout::detail::bound_indexes<bounds_type, sizeof...(Args)>::make()
				);
		}
	};

	//
	// is_bind_expression
	//
	template<typename Signature>
	struct is_bind_expression<sprout::binder<Signature> >
		: public sprout::true_type
	{};
	template<typename Result, typename Signature>
	struct is_bind_expression<sprout::res_binder<Result, Signature> >
		: public sprout::true_type
	{};
	template<typename Signature>
	struct is_bind_expression<sprout::cbinder<Signature> >
		: public sprout::true_type
	{};
	template<typename Result, typename Signature>
	struct is_bind_expression<sprout::res_cbinder<Result, Signature> >
		: public sprout::true_type
	{};

	namespace detail {
		template<std::size_t N, typename BoundArg, typename = void>
		struct complete_placeholder {
		public:
			typedef BoundArg type;
		};
		template<std::size_t N, typename BoundArg>
		struct complete_placeholder<
			N, BoundArg,
			typename std::enable_if<sprout::is_positional_placeholder<BoundArg>::value>::type
		> {
		public:
			typedef sprout::placeholder<N + 1> type;
		};

		template<std::size_t I, std::size_t N, typename Bounds, typename Binder, typename = void>
		struct binder_complete_placeholders_impl {
		public:
			typedef Binder type;
		};
		template<std::size_t I, std::size_t N, typename Bounds, typename Func, typename... As>
		struct binder_complete_placeholders_impl<
			I, N, Bounds, sprout::binder<Func (As...)>,
			typename std::enable_if<(I < sprout::tuples::tuple_size<Bounds>::value)>::type
		>
			: public std::conditional<
				sprout::is_positional_placeholder<typename sprout::tuples::tuple_element<I, Bounds>::type>::value,
				sprout::detail::binder_complete_placeholders_impl<
					I + 1, N + 1, Bounds,
					sprout::binder<Func (As..., sprout::placeholder<N + 1>)>
				>,
				sprout::detail::binder_complete_placeholders_impl<
					I + 1, N, Bounds,
					sprout::binder<Func (As..., typename sprout::tuples::tuple_element<I, Bounds>::type)>
				>
			>::type
		{};
		template<std::size_t I, std::size_t N, typename Bounds, typename Func, typename... As>
		struct binder_complete_placeholders_impl<
			I, N, Bounds, sprout::cbinder<Func (As...)>,
			typename std::enable_if<(I < sprout::tuples::tuple_size<Bounds>::value)>::type
		>
			: public std::conditional<
				sprout::is_positional_placeholder<typename sprout::tuples::tuple_element<I, Bounds>::type>::value,
				sprout::detail::binder_complete_placeholders_impl<
					I + 1, N + 1, Bounds,
					sprout::cbinder<Func (As..., sprout::placeholder<N + 1>)>
				>,
				sprout::detail::binder_complete_placeholders_impl<
					I + 1, N, Bounds,
					sprout::cbinder<Func (As..., typename sprout::tuples::tuple_element<I, Bounds>::type)>
				>
			>::type
		{};
		template<bool Const, typename Func, typename... BoundArgs>
		struct binder_complete_placeholders;
		template<typename Func, typename... BoundArgs>
		struct binder_complete_placeholders<
			false, Func, BoundArgs...
		>
			: public sprout::detail::binder_complete_placeholders_impl<
				0, 0, sprout::types::type_tuple<BoundArgs...>, sprout::binder<Func ()>
			>
		{};
		template<typename Func, typename... BoundArgs>
		struct binder_complete_placeholders<
			true, Func, BoundArgs...
		>
			: public sprout::detail::binder_complete_placeholders_impl<
				0, 0, sprout::types::type_tuple<BoundArgs...>, sprout::cbinder<Func ()>
			>
		{};

		template<std::size_t I, std::size_t N, typename Bounds, typename Binder, typename = void>
		struct res_binder_complete_placeholders_impl {
		public:
			typedef Binder type;
		};
		template<std::size_t I, std::size_t N, typename Bounds, typename Result, typename Func, typename... As>
		struct res_binder_complete_placeholders_impl<
			I, N, Bounds, sprout::res_binder<Result, Func (As...)>,
			typename std::enable_if<(I < sprout::tuples::tuple_size<Bounds>::value)>::type
		>
			: public std::conditional<
				sprout::is_positional_placeholder<typename sprout::tuples::tuple_element<I, Bounds>::type>::value,
				sprout::detail::res_binder_complete_placeholders_impl<
					I + 1, N + 1, Bounds,
					sprout::res_binder<Result, Func (As..., sprout::placeholder<N + 1>)>
				>,
				sprout::detail::res_binder_complete_placeholders_impl<
					I + 1, N, Bounds,
					sprout::res_binder<Result, Func (As..., typename sprout::tuples::tuple_element<I, Bounds>::type)>
				>
			>::type
		{};
		template<std::size_t I, std::size_t N, typename Bounds, typename Result, typename Func, typename... As>
		struct res_binder_complete_placeholders_impl<
			I, N, Bounds, sprout::res_cbinder<Result, Func (As...)>,
			typename std::enable_if<(I < sprout::tuples::tuple_size<Bounds>::value)>::type
		>
			: public std::conditional<
				sprout::is_positional_placeholder<typename sprout::tuples::tuple_element<I, Bounds>::type>::value,
				sprout::detail::res_binder_complete_placeholders_impl<
					I + 1, N + 1, Bounds,
					sprout::res_cbinder<Result, Func (As..., sprout::placeholder<N + 1>)>
				>,
				sprout::detail::res_binder_complete_placeholders_impl<
					I + 1, N, Bounds,
					sprout::res_cbinder<Result, Func (As..., typename sprout::tuples::tuple_element<I, Bounds>::type)>
				>
			>::type
		{};
		template<bool Const, typename Result, typename Func, typename... BoundArgs>
		struct res_binder_complete_placeholders;
		template<typename Result, typename Func, typename... BoundArgs>
		struct res_binder_complete_placeholders<
			false, Result, Func, BoundArgs...
		>
			: public sprout::detail::res_binder_complete_placeholders_impl<
				0, 0, sprout::types::type_tuple<BoundArgs...>, sprout::res_binder<Result, Func ()>
			>
		{};
		template<typename Result, typename Func, typename... BoundArgs>
		struct res_binder_complete_placeholders<
			true, Result, Func, BoundArgs...
		>
			: public sprout::detail::res_binder_complete_placeholders_impl<
				0, 0, sprout::types::type_tuple<BoundArgs...>, sprout::res_cbinder<Result, Func ()>
			>
		{};

		template<bool Const, typename Func, typename... BoundArgs>
		struct bind_helper {
		public:
			typedef sprout::detail::maybe_wrap_member_pointer<typename std::decay<Func>::type> maybe_type;
			typedef typename maybe_type::type func_type;
			typedef typename sprout::detail::binder_complete_placeholders<Const, func_type, typename std::decay<BoundArgs>::type...>::type type;
		};
		template<bool Const, typename Result, typename Func, typename... BoundArgs>
		struct res_bind_helper {
		public:
			typedef sprout::detail::maybe_wrap_member_pointer<typename std::decay<Func>::type> maybe_type;
			typedef typename maybe_type::type functor_type;
			typedef typename sprout::detail::res_binder_complete_placeholders<Const, Result, functor_type, typename std::decay<BoundArgs>::type...>::type type;
		};
	}	// namespace detail

	//
	// bind_result
	// cbind_result
	//
	template<typename F, typename... BoundArgs>
	struct bind_result {
	public:
		typedef typename sprout::detail::bind_helper<false, F, BoundArgs...>::type type;
	};
	template<typename F, typename... BoundArgs>
	struct cbind_result {
	public:
		typedef typename sprout::detail::bind_helper<true, F, BoundArgs...>::type type;
	};
	//
	// res_bind_result
	// res_cbind_result
	//
	template<typename R, typename F, typename... BoundArgs>
	struct res_bind_result {
	public:
		typedef typename sprout::detail::res_bind_helper<false, R, F, BoundArgs...>::type type;
	};
	template<typename R, typename F, typename... BoundArgs>
	struct res_cbind_result {
	public:
		typedef typename sprout::detail::res_bind_helper<true, R, F, BoundArgs...>::type type;
	};

	//
	// bind
	//
	template<typename F, typename... BoundArgs>
	inline SPROUT_CONSTEXPR typename sprout::bind_result<F, BoundArgs...>::type
	bind(F&& f, BoundArgs&&... args) {
		typedef sprout::detail::bind_helper<false, F, BoundArgs...> helper_type;
		typedef typename helper_type::maybe_type maybe_type;
		typedef typename helper_type::type result_type;
		return result_type(maybe_type::do_wrap(SPROUT_FORWARD(F, f)), SPROUT_FORWARD(BoundArgs, args)...);
	}
	template<typename R, typename F, typename... BoundArgs>
	inline SPROUT_CONSTEXPR typename sprout::res_bind_result<R, F, BoundArgs...>::type
	bind(F&& f, BoundArgs&&... args) {
		typedef sprout::detail::res_bind_helper<false, R, F, BoundArgs...> helper_type;
		typedef typename helper_type::maybe_type maybe_type;
		typedef typename helper_type::type result_type;
		return result_type(maybe_type::do_wrap(SPROUT_FORWARD(F, f)), SPROUT_FORWARD(BoundArgs, args)...);
	}

	//
	// cbind
	//
	template<typename F, typename... BoundArgs>
	inline SPROUT_CONSTEXPR typename sprout::cbind_result<F, BoundArgs...>::type
	cbind(F&& f, BoundArgs&&... args) {
		typedef sprout::detail::bind_helper<true, F, BoundArgs...> helper_type;
		typedef typename helper_type::maybe_type maybe_type;
		typedef typename helper_type::type result_type;
		return result_type(maybe_type::do_wrap(SPROUT_FORWARD(F, f)), SPROUT_FORWARD(BoundArgs, args)...);
	}
	template<typename R, typename F, typename... BoundArgs>
	inline SPROUT_CONSTEXPR typename sprout::res_cbind_result<R, F, BoundArgs...>::type
	cbind(F&& f, BoundArgs&&... args) {
		typedef sprout::detail::res_bind_helper<true, R, F, BoundArgs...> helper_type;
		typedef typename helper_type::maybe_type maybe_type;
		typedef typename helper_type::type result_type;
		return result_type(maybe_type::do_wrap(SPROUT_FORWARD(F, f)), SPROUT_FORWARD(BoundArgs, args)...);
	}
}	// namespace sprout

#endif	// #ifndef SPROUT_FUNCTIONAL_BIND_BIND_HPP
