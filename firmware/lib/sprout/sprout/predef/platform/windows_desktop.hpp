/*=============================================================================
  Copyright (c) 2011-2019 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/Sprout

  Distributed under the sprout Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.sprout.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SPROUT_PREDEF_PLATFORM_WINDOWS_DESKTOP_HPP
#define SPROUT_PREDEF_PLATFORM_WINDOWS_DESKTOP_HPP

#include <sprout/config.hpp>
#include <sprout/predef/os/windows.hpp>

#define SPROUT_PLAT_WINDOWS_DESKTOP 0

#if SPROUT_OS_WINDOWS && (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
#	undef SPROUT_PLAT_WINDOWS_DESKTOP
#	define SPROUT_PLAT_WINDOWS_DESKTOP 1
#endif

#if SPROUT_PLAT_WINDOWS_DESKTOP
#	define SPROUT_PLAT_WINDOWS_DESKTOP_AVALIABLE
#	include <sprout/predef/detail/platform_detected.hpp>
#endif

#define SPROUT_PLAT_WINDOWS_DESKTOP_NAME "Windows Desktop"

#endif	// #ifndef SPROUT_PREDEF_PLATFORM_WINDOWS_DESKTOP_HPP
