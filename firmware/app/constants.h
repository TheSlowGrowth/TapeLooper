#pragma once

#include "daisy_seed.h"

constexpr uint8_t blockSize = 48;
constexpr uint16_t sampleRateHz = 48000;
constexpr daisy::SaiHandle::Config::SampleRate sampleRate = daisy::SaiHandle::Config::SampleRate::SAI_48KHZ;