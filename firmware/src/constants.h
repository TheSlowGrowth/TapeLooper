#pragma once

#ifndef UNIT_TEST
#    include "daisy_seed.h"
constexpr daisy::SaiHandle::Config::SampleRate sampleRate = daisy::SaiHandle::Config::SampleRate::SAI_48KHZ;
#endif

constexpr uint8_t blockSize = 48;
constexpr uint16_t sampleRateHz = 48000;
