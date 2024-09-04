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

#include "WavFileFormat.h"

void updateWavFileHeaderSizes(WavFileHeader& header, const int numSamples)
{
    header.dataBlocSize = numSamples * header.numBytesPerBloc;
    header.fileSizeMinus8Bytes = header.dataBlocSize + sizeof(WavFileHeader) - 8;
}

WavFileHeader makeWavFileHeader32bitInt(const int sampleRate,
                                        const int numChannels,
                                        const int numSamples)
{
    WavFileHeader header;
    header.audioFormat = WavFileFormatCode::pcm;
    header.numAudioChannels = numChannels;
    header.sampleRate = sampleRate;
    header.numBitsPerSample = 32;
    header.numBytesPerBloc = header.numAudioChannels
                             * header.numBitsPerSample / 8;
    header.numBytesPerSec = sampleRate * header.numBytesPerBloc;
    updateWavFileHeaderSizes(header, numSamples);
    return header;
}

WavFileHeader makeWavFileHeader32bitFloat(const int sampleRate,
                                          const int numChannels,
                                          const int numSamples)
{
    WavFileHeader header;
    header.audioFormat = WavFileFormatCode::ieeeFloat;
    header.numAudioChannels = numChannels;
    header.sampleRate = sampleRate;
    header.numBitsPerSample = 32;
    header.numBytesPerBloc = header.numAudioChannels
                             * header.numBitsPerSample / 8;
    header.numBytesPerSec = sampleRate * header.numBytesPerBloc;
    updateWavFileHeaderSizes(header, numSamples);
    return header;
}

uint32_t getNumSamples(const WavFileHeader& header)
{
    return uint32_t(header.dataBlocSize / header.numBytesPerBloc);
}
