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

#include <array>

template <size_t numChannels, typename FloatType = float>
class AudioBufferPtr
{
public:
    AudioBufferPtr(FloatType* const* buffer, size_t numSamples) :
        size_(numSamples)
    {
        for (size_t ch = 0; ch < numChannels; ch++)
            buffer_[ch] = buffer[ch];
    }

    AudioBufferPtr(const AudioBufferPtr& other) = default;

    FloatType* operator[](size_t channelIdx)
    {
        return buffer_[channelIdx];
    }

    FloatType* operator[](size_t channelIdx) const
    {
        return buffer_[channelIdx];
    }

    operator AudioBufferPtr<numChannels, const FloatType>() const
    {
        return AudioBufferPtr<numChannels, const FloatType>(buffer_, size_);
    }

    void fill(FloatType value)
    {
        for (size_t ch = 0; ch < numChannels; ch++)
            for (size_t i = 0; i < size_; i++)
                buffer_[ch][i] = value;
    }

    AudioBufferPtr<numChannels, FloatType> subBlock(size_t startSample, size_t length = size_t(-1)) const
    {
        const auto resultMaxLength = std::max(size_ - startSample, size_t(0));
        const auto resultLength = std::min(resultMaxLength, length);
        AudioBufferPtr<numChannels, FloatType> result(resultLength);
        for (size_t ch = 0; ch < numChannels; ch++)
            result.buffer_[ch] = buffer_[ch] + startSample;
        return result;
    }

    FloatType* buffer_[numChannels];
    const size_t size_;

protected:
    AudioBufferPtr(size_t numSamples) :
        size_(numSamples) {}
};

template <size_t numSamples, size_t numChannels, typename FloatType = float>
class AudioBuffer : public AudioBufferPtr<numChannels, FloatType>
{
public:
    AudioBuffer() :
        AudioBufferPtr<numChannels, FloatType>(numSamples)
    {
        for (size_t ch = 0; ch < numChannels; ch++)
            this->buffer_[ch] = channelBuffers_[ch];
    }

private:
    std::array<FloatType[numSamples], numChannels> channelBuffers_;
};