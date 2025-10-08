
cmake_minimum_required(VERSION 3.30)

if(WIN32)
    set(TOOLCHAIN_EXT ".exe")
else()
    set(TOOLCHAIN_EXT "")
endif()

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}/bin/arm-none-eabi-gcc${TOOLCHAIN_EXT})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}/bin/arm-none-eabi-g++${TOOLCHAIN_EXT})

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    include(${CMAKE_CURRENT_LIST_DIR}/../lib/libDaisy/cmake/toolchains/ArmGNUToolchain.cmake)
    #include(${CMAKE_CURRENT_LIST_DIR}/../lib/libDaisy/cmake/toolchains/autodetect.cmake)
endif()

project(TapeLoope)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

set(FIRMWARE_NAME TapeLooper)
set(FIRMWARE_SOURCES
    src/main.cpp
)

# include dsp library
add_subdirectory(../dsp libTapeLooperDsp)

# include libDaisy
add_subdirectory(../lib/libDaisy daisy)

cmake_path(SET LOCAL_CMAKE_PATH "../lib/libDaisy/cmake")
cmake_path(ABSOLUTE_PATH LOCAL_CMAKE_PATH)

list(APPEND CMAKE_MODULE_PATH ${LOCAL_CMAKE_PATH})
include(DaisyProject)

target_link_libraries(${FIRMWARE_NAME}
  PRIVATE
  daisy
  tapeLooperDsp
)

set_target_properties(${FIRMWARE_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
)