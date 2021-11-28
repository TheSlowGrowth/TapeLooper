/**	
 * Copyright (C) Johannes Elliesen, 2021
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *  
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

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
