# Project Name
TARGET = TapeLooper

# gcc path
ifeq ($(OS),Windows_NT)
	GCC_PATH ?= "C:\\Program Files (x86)\\GNU Arm Embedded Toolchain\\10 2020-q4-major\\bin"
else
	GCC_PATH ?= ~/dev/gcc-arm-none-eabi-9-2020-q2-update/bin
endif

# programming 
PGM_DEVICE = interface/ftdi/olimex-arm-usb-tiny-h.cfg

CPP_STANDARD ?= -std=gnu++17

# Sources
CPP_SOURCES = app/main.cpp

C_INCLUDES ?= 
C_INCLUDES += \
-Ilib/sprout \
-Iapp/

ifeq ($(DEBUG), 1)
OPT = -O0
else 
OPT = -O3
endif

# Library Locations
LIBDAISY_DIR = lib/libDaisy
#DAISYSP_DIR = ../../DaisySP

# Core location, and generic makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

