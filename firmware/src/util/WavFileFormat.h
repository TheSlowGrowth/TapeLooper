/**
 * Copyright (C) Johannes Elliesen, 2024
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

#include <cstdint>

enum class WavFileFormatCode
{
    pcm = 0x0001,
    ieeeFloat = 0x0003,
    aLaw = 0x0006,
    uLaw = 0x0007,
    extensible = 0xFFFE,
};

#ifdef _MSC_VER
#    pragma pack(push, 1)
#    define PACKED_ATTRIBUTE
#else
#    define PACKED_ATTRIBUTE __attribute__((__packed__))
#endif
struct WavFileHeader
{
    uint32_t fileTypeBlocId = 0x46464952; // "RIFF"
    uint32_t fileSizeMinus8Bytes;
    uint32_t fileFormatId = 0x45564157; // "WAVE"
    uint32_t formatBlocId = 0x20746d66; // "fmt "
    uint32_t formatBlocSize = 0x10;
    WavFileFormatCode audioFormat : 16;
    uint16_t numAudioChannels;
    uint32_t sampleRate;
    uint32_t numBytesPerSec;
    uint16_t numBytesPerBloc;
    uint16_t numBitsPerSample;
    uint32_t dataBlocId = 0x61746164; // "data"
    uint32_t dataBlocSize;
} PACKED_ATTRIBUTE;
#ifdef _MSC_VER
#    pragma pack(pop)
#endif

void updateWavFileHeaderSizes(WavFileHeader& header, int numSamples);

WavFileHeader makeWavFileHeader32bitInt(int sampleRate, int numChannels, int numSamples = 0);

WavFileHeader makeWavFileHeader32bitFloat(int sampleRate, int numChannels, int numSamples = 0);

uint32_t getNumSamples(const WavFileHeader& header);
