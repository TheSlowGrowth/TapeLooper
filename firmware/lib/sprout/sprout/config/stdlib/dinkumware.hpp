/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_CONFIG_STDLIB_DINKUMWARE_HPP
#define SPROUT_CONFIG_STDLIB_DINKUMWARE_HPP

#if !defined(_CPPLIB_VER) || _CPPLIB_VER < 520
#	define SPROUT_NO_CXX11_HDR_ARRAY
#	define SPROUT_NO_CXX11_HDR_CODECVT
#	define SPROUT_NO_CXX11_HDR_FORWARD_LIST
#	define SPROUT_NO_CXX11_HDR_INITIALIZER_LIST
#	define SPROUT_NO_CXX11_HDR_RANDOM
#	define SPROUT_NO_CXX11_HDR_REGEX
#	define SPROUT_NO_CXX11_HDR_SYSTEM_ERROR
#	define SPROUT_NO_CXX11_HDR_UNORDERED_MAP
#	define SPROUT_NO_CXX11_HDR_UNORDERED_SET
#	define SPROUT_NO_CXX11_HDR_TUPLE
#	define SPROUT_NO_CXX11_HDR_TYPEINDEX
#	define SPROUT_NO_CXX11_HDR_FUNCTIONAL
#	define SPROUT_NO_CXX11_CHAR_TRAITS
#	define SPROUT_NO_CXX11_NUMERIC_LIMITS
#	define SPROUT_NO_CXX11_SMART_PTR
#endif

#if (!defined(_HAS_TR1_IMPORTS) || (_HAS_TR1_IMPORTS+0 == 0)) && !defined(SPROUT_NO_CXX11_HDR_TUPLE) && (!defined(_CPPLIB_VER) || _CPPLIB_VER < 610)
#	define SPROUT_NO_CXX11_HDR_TUPLE
#endif

#if !defined(_CPPLIB_VER) || _CPPLIB_VER < 540
#	define SPROUT_NO_CXX11_HDR_TYPE_TRAITS
#	define SPROUT_NO_CXX11_HDR_CHRONO
#	define SPROUT_NO_CXX11_HDR_CONDITION_VARIABLE
#	define SPROUT_NO_CXX11_HDR_FUTURE
#	define SPROUT_NO_CXX11_HDR_MUTEX
#	define SPROUT_NO_CXX11_HDR_RATIO
#	define SPROUT_NO_CXX11_HDR_THREAD
#	define SPROUT_NO_CXX11_ALLOCATOR
#	define SPROUT_NO_CXX11_ATOMIC_SMART_PTR
#endif

#if !defined(_CPPLIB_VER) || _CPPLIB_VER < 610
#	define SPROUT_NO_CXX11_HDR_INITIALIZER_LIST
#	define SPROUT_NO_CXX11_HDR_ATOMIC
#	define SPROUT_NO_CXX11_ALLOCATOR
#	define SPROUT_NO_CXX11_STD_ALIGN
#endif

#if !defined(_CPPLIB_VER) || _CPPLIB_VER < 650
#	define SPROUT_NO_CXX11_POINTER_TRAITS
#elif defined(_MSC_VER) && _MSC_VER < 1910
#	define SPROUT_NO_CXX11_POINTER_TRAITS
#endif

#if defined(__has_include)
#if !__has_include(<shared_mutex>)
#	define SPROUT_NO_CXX14_HDR_SHARED_MUTEX
#elif (__cplusplus < 201402) && !defined(_MSC_VER)
#	define SPROUT_NO_CXX14_HDR_SHARED_MUTEX
#endif
#elif !defined(_CPPLIB_VER) || (_CPPLIB_VER < 650)
#	define SPROUT_NO_CXX14_HDR_SHARED_MUTEX
#endif

#if !defined(_CPPLIB_VER) || (_CPPLIB_VER < 650)
#	define SPROUT_NO_CXX14_STD_EXCHANGE
#endif

#if !defined(_CPPLIB_VER) || (_CPPLIB_VER < 650) || !defined(_MSC_VER) || (_MSC_VER < 1910) || !defined(_HAS_CXX17) || (_HAS_CXX17 == 0)
#	define SPROUT_NO_CXX17_STD_APPLY
#endif
#if !defined(_CPPLIB_VER) || (_CPPLIB_VER < 650)
#	define SPROUT_NO_CXX17_STD_INVOKE
#endif

#if !defined(_CPPLIB_VER) || _CPPLIB_VER < 650
#	define SPROUT_NO_CXX11_ADDRESSOF
#endif

#if defined(_CPPLIB_VER) && (_CPPLIB_VER == 650)
#	define SPROUT_NO_CXX11_HDR_CODECVT
#endif

#if defined(_CPPLIB_VER) && (_CPPLIB_VER >= 650)
#	if defined(_HAS_AUTO_PTR_ETC) && (_HAS_AUTO_PTR_ETC == 0)
#		define SPROUT_NO_AUTO_PTR
#		define SPROUT_NO_CXX98_RANDOM_SHUFFLE
#		define SPROUT_NO_CXX98_FUNCTION_BASE
#		define SPROUT_NO_CXX98_BINDERS
#	endif
#endif

#ifdef _M_CEE
#	ifndef SPROUT_NO_CXX11_HDR_MUTEX
#		define SPROUT_NO_CXX11_HDR_MUTEX
#	endif
#	ifndef SPROUT_NO_CXX11_HDR_ATOMIC
#		define SPROUT_NO_CXX11_HDR_ATOMIC
#	endif
#	ifndef SPROUT_NO_CXX11_HDR_FUTURE
#		define SPROUT_NO_CXX11_HDR_FUTURE
#	endif
#	ifndef SPROUT_NO_CXX11_HDR_CONDITION_VARIABLE
#		define SPROUT_NO_CXX11_HDR_CONDITION_VARIABLE
#	endif
#	ifndef SPROUT_NO_CXX11_HDR_THREAD
#		define SPROUT_NO_CXX11_HDR_THREAD
#	endif
#	ifndef SPROUT_NO_CXX14_HDR_SHARED_MUTEX
#		define SPROUT_NO_CXX14_HDR_SHARED_MUTEX
#	endif
#	ifndef SPROUT_NO_CXX14_STD_EXCHANGE
#		define SPROUT_NO_CXX14_STD_EXCHANGE
#	endif
#	ifndef SPROUT_NO_FENV_H
#		define SPROUT_NO_FENV_H
#	endif
#endif

#define SPROUT_NO_CXX14_INITIALIZER_LIST

#endif	// #ifndef SPROUT_CONFIG_STDLIB_DINKUMWARE_HPP
