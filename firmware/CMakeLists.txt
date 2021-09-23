cmake_policy(SET CMP0048 NEW)
project(TapeLooper)
cmake_minimum_required(VERSION 3.19)

set(FIRMWARE_NAME TapeLooper)

set(FIRMWARE_SOURCES
    src/main.cpp
)

# include dsp library
add_subdirectory(../dsp libTapeLooperDsp)

# include libDaisy
set(LIBDAISY_DIR ../lib/libDaisy)
add_subdirectory(${LIBDAISY_DIR} libDaisy)

# linker script
get_filename_component(LIBDAISY_DIR_ABS "${LIBDAISY_DIR}"
                       REALPATH BASE_DIR "${CMAKE_SOURCE_DIR}")
set(LINKER_SCRIPT ${LIBDAISY_DIR_ABS}/core/STM32H750IB_flash.lds)

# firmware binary image
add_executable(${FIRMWARE_NAME} ${FIRMWARE_SOURCES})

target_link_libraries(${FIRMWARE_NAME}
  PRIVATE
  daisy
  tapeLooperDsp
)

set_target_properties(${FIRMWARE_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    SUFFIX ".elf"
    LINK_DEPENDS ${LINKER_SCRIPT}
)

target_link_options(${FIRMWARE_NAME} PUBLIC
    -T ${LINKER_SCRIPT}
    -Wl,-Map=${FIRMWARE_NAME}.map,--cref
    -Wl,--check-sections
    -Wl,--unresolved-symbols=report-all
    -Wl,--warn-common
    -Wl,--warn-section-align
    -Wl,--print-memory-usage
)

add_custom_command(TARGET ${FIRMWARE_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY}
    ARGS -O ihex
    -S ${FIRMWARE_NAME}.elf
    ${FIRMWARE_NAME}.hex
    BYPRODUCTS
    ${FIRMWARE_NAME}.hex
    COMMENT "Generating HEX image"
VERBATIM)

add_custom_command(TARGET ${FIRMWARE_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY}
    ARGS -O binary
    -S ${FIRMWARE_NAME}.elf
    ${FIRMWARE_NAME}.bin
    BYPRODUCTS
    ${FIRMWARE_NAME}.bin
    COMMENT "Generating binary image"
VERBATIM)
