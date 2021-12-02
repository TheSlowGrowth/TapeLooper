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

#include <gtest/gtest.h>

#include "dsp/AudioBuffer.h"

TEST(AudioBuffer, constructor_)
{
    constexpr size_t numSamplesPerChannel = 5;
    std::array<float, 2 * numSamplesPerChannel> array;
    array[2] = 4.0f; // set a single value before constructing the AudioBufferPtr
    float* channelArray[2] = { &array[0], &array[numSamplesPerChannel] };
    const auto bufferPtr = AudioBufferPtr<2>(channelArray, numSamplesPerChannel);

    EXPECT_EQ(bufferPtr.getChannelPointers()[0], channelArray[0]);
    EXPECT_EQ(bufferPtr.getChannelPointers()[1], channelArray[1]);
    EXPECT_EQ(bufferPtr.size_, numSamplesPerChannel);
    // the buffer is unchanged
    EXPECT_EQ(bufferPtr[0][2], 4.0f);
}

TEST(AudioBuffer, copyConstructor)
{
    constexpr size_t numSamplesPerChannel = 5;
    std::array<float, 2 * numSamplesPerChannel> array;
    float* channelArray[2] = { &array[0], &array[numSamplesPerChannel] };
    const auto bufferPtr1 = AudioBufferPtr<2>(channelArray, numSamplesPerChannel);

    const auto bufferPtr2 = AudioBufferPtr(bufferPtr1);

    EXPECT_EQ(bufferPtr2.getChannelPointers()[0], channelArray[0]);
    EXPECT_EQ(bufferPtr2.getChannelPointers()[1], channelArray[1]);
    EXPECT_EQ(bufferPtr2.size_, numSamplesPerChannel);
}

TEST(AudioBuffer, conversionToConst)
{
    constexpr size_t numSamplesPerChannel = 5;
    std::array<float, 2 * numSamplesPerChannel> array;
    float* channelArray[2] = { &array[0], &array[numSamplesPerChannel] };
    const auto bufferPtr = AudioBufferPtr<2>(channelArray, numSamplesPerChannel);
    const auto constBufferPtr = AudioBufferPtr<2, const float>(bufferPtr);

    EXPECT_EQ(constBufferPtr.getChannelPointers()[0], channelArray[0]);
    EXPECT_EQ(constBufferPtr.getChannelPointers()[1], channelArray[1]);
    EXPECT_EQ(constBufferPtr.size_, numSamplesPerChannel);
}

TEST(AudioBuffer, indexOperator)
{
    constexpr size_t numSamplesPerChannel = 5;
    std::array<float, 2 * numSamplesPerChannel> array;
    float* channelArray[2] = { &array[0], &array[numSamplesPerChannel] };
    const auto bufferPtr = AudioBufferPtr<2>(channelArray, numSamplesPerChannel);
    const auto constBufferPtr = AudioBufferPtr<2, const float>(bufferPtr);

    EXPECT_EQ(bufferPtr[0], channelArray[0]);
    EXPECT_EQ(bufferPtr[1], channelArray[1]);
    EXPECT_EQ(constBufferPtr[0], channelArray[0]);
    EXPECT_EQ(constBufferPtr[1], channelArray[1]);
}

TEST(AudioBuffer, fill)
{
    constexpr size_t numSamplesPerChannel = 5;
    std::array<float, 4 * numSamplesPerChannel> array;
    float* channelArray[4] = { &array[0],
                               &array[numSamplesPerChannel],
                               &array[2 * numSamplesPerChannel],
                               &array[3 * numSamplesPerChannel] };
    auto bufferPtr = AudioBufferPtr<4>(channelArray, numSamplesPerChannel);

    bufferPtr.fill(4.0f);
    EXPECT_TRUE(std::all_of(array.begin(),
                            array.end(),
                            [](float val)
                            { return val == 4.0f; }));
    bufferPtr.fill(1.0f);
    EXPECT_TRUE(std::all_of(array.begin(),
                            array.end(),
                            [](float val)
                            { return val == 1.0f; }));
}

TEST(AudioBuffer, subBlock)
{
    constexpr size_t numSamplesPerChannel = 5;
    std::array<float, 2 * numSamplesPerChannel> array;
    float* channelArray[2] = { &array[0], &array[numSamplesPerChannel] };
    const auto bufferPtr = AudioBufferPtr<2>(channelArray, numSamplesPerChannel);

    auto subBuffer1 = bufferPtr.subBlock(4); // no end provided => extend up to last sample
    EXPECT_EQ(subBuffer1[0], &array[4]);
    EXPECT_EQ(subBuffer1[1], &array[4 + numSamplesPerChannel]);
    EXPECT_EQ(subBuffer1.size_, numSamplesPerChannel - 4);

    auto subBuffer2 = bufferPtr.subBlock(3, size_t(-1)); // extend up to last sample
    EXPECT_EQ(subBuffer2[0], &array[3]);
    EXPECT_EQ(subBuffer2[1], &array[3 + numSamplesPerChannel]);
    EXPECT_EQ(subBuffer2.size_, numSamplesPerChannel - 3);

    auto subBuffer3 = bufferPtr.subBlock(2, 2); // specify end
    EXPECT_EQ(subBuffer3[0], &array[2]);
    EXPECT_EQ(subBuffer3[1], &array[2 + numSamplesPerChannel]);
    EXPECT_EQ(subBuffer3.size_, 2ul);
}

TEST(AudioBuffer, copyToChannel)
{
    constexpr size_t numSamplesPerChannel = 5;
    std::array<float, 2 * numSamplesPerChannel> array;
    float* channelArray[2] = { &array[0], &array[numSamplesPerChannel] };
    auto bufferPtr = AudioBufferPtr<2>(channelArray, numSamplesPerChannel);

    std::array<float, 7> srcSamples = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f };

    // copy 2 samples to channel 1
    bufferPtr.copyToChannel(1, srcSamples.data(), 2);
    EXPECT_EQ(std::vector(channelArray[1], channelArray[1] + 2), std::vector({ 1.0f, 2.0f }));

    bufferPtr.fill(0.0f);

    // copy more samples than fit one channel
    bufferPtr.copyToChannel(0, srcSamples.data(), 7);
    EXPECT_EQ(std::vector(channelArray[0], channelArray[0] + numSamplesPerChannel),
              std::vector(srcSamples.begin(), srcSamples.begin() + numSamplesPerChannel));
    // the seconds channel should be untouched
    EXPECT_TRUE(std::all_of(channelArray[1],
                            channelArray[1] + numSamplesPerChannel,
                            [](float val)
                            { return val == 0.0f; }));
}

TEST(AudioBuffer, addToChannel)
{
    constexpr size_t numSamplesPerChannel = 5;
    std::array<float, 2 * numSamplesPerChannel> array;
    float* channelArray[2] = { &array[0], &array[numSamplesPerChannel] };
    auto bufferPtr = AudioBufferPtr<2>(channelArray, numSamplesPerChannel);
    bufferPtr.fill(10.0f);

    std::array<float, 7> srcSamples = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f };

    // add 2 samples to channel 1
    bufferPtr.addToChannel(1, srcSamples.data(), 2);
    EXPECT_EQ(std::vector(channelArray[1], channelArray[1] + 2), std::vector({ 11.0f, 12.0f }));

    bufferPtr.fill(10.0f);

    // add more samples than fit one channel
    bufferPtr.addToChannel(0, srcSamples.data(), 7);
    EXPECT_EQ(std::vector(channelArray[0], channelArray[0] + numSamplesPerChannel),
              std::vector({ 11.0f, 12.0f, 13.0f, 14.0f, 15.0f }));
    // the seconds channel should be untouched
    EXPECT_TRUE(std::all_of(channelArray[1],
                            channelArray[1] + numSamplesPerChannel,
                            [](float val)
                            { return val == 10.0f; }));
}
TEST(AudioBuffer, applyGain)
{
    constexpr size_t numSamplesPerChannel = 2;
    std::array<float, 2 * numSamplesPerChannel> array;
    float* channelArray[2] = { &array[0], &array[numSamplesPerChannel] };
    auto bufferPtr = AudioBufferPtr<2>(channelArray, numSamplesPerChannel);
    bufferPtr.fill(1.0f);

    bufferPtr.applyGain(4.0f);
    EXPECT_TRUE(std::all_of(array.begin(),
                            array.end(),
                            [](float val)
                            { return val == 4.0f; }));
    bufferPtr.applyGain(0.5f);
    EXPECT_TRUE(std::all_of(array.begin(),
                            array.end(),
                            [](float val)
                            { return val == 2.0f; }));
}