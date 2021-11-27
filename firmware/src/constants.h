#pragma once

#ifndef UNIT_TEST
#    include "daisy_seed.h"
constexpr daisy::SaiHandle::Config::SampleRate sampleRate = daisy::SaiHandle::Config::SampleRate::SAI_48KHZ;
#endif

constexpr uint8_t blockSize = 48;
constexpr uint16_t sampleRateHz = 48000;
constexpr uint8_t numChannelsPerLooper = 2;
constexpr uint8_t numLoopers = 4;

constexpr size_t sdramSize = 64 * 1024 * 1024;
constexpr size_t totalLooperMemorySize = sdramSize - 256;
constexpr size_t looperSamplesPerChannel = totalLooperMemorySize / numChannelsPerLooper / numLoopers / sizeof(float);
