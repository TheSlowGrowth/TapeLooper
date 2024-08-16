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
#include <gmock/gmock.h>
#include <random>
#include "dsp/Player.h"

using ::testing::_;

/** A "float" that, when compared, allows for some tolerance */
struct FloatWithTolerance
{
    static constexpr float maxTolerance_ = 0.0001f;

    FloatWithTolerance(float value = 0.0f) :
        value_(value) {}

    operator const float&() const { return value_; }
    operator float&() { return value_; };

    bool operator==(float rhs) const
    {
        return (std::abs((*this) - rhs) <= maxTolerance_);
    }
    bool operator!=(float rhs) const
    {
        return !(*this == rhs);
    }

    float value_ = 0;
};

template <size_t numChannels>
std::array<std::vector<FloatWithTolerance>, numChannels> toArrOfVecs(const AudioBufferPtr<numChannels, float>& buffer)
{
    std::array<std::vector<FloatWithTolerance>, numChannels> result;
    for (size_t ch = 0; ch < numChannels; ch++)
        result[ch] = std::vector<FloatWithTolerance>(buffer[ch], buffer[ch] + buffer.size_);
    return result;
}

template <size_t numSamples, size_t numChannels>
std::array<std::array<FloatWithTolerance, numSamples>, numChannels> toArrOfArrs(const AudioBufferPtr<numChannels, float>& buffer)
{
    std::array<std::array<FloatWithTolerance, numSamples>, numChannels> result;
    for (size_t ch = 0; ch < numChannels; ch++)
        result[ch] = std::array<FloatWithTolerance, numSamples>(buffer[ch], buffer[ch] + buffer.size_);
    return result;
}

class DSP_Player : public ::testing::Test
{
protected:
    class MockProcessor
    {
    public:
        struct Parameters
        {
            float value;
        };

        MOCK_METHOD(void, reset, (), ());
        MOCK_METHOD(void, process, (float input[2], const Parameters& params), ());
    };

    class MockSpeedModulator
    {
    public:
        MOCK_METHOD(void, reset, (), ());
        MOCK_METHOD(float, getAndAdvance, (), ());
    };

    DSP_Player() :
        player_(AudioBufferPtr<numChannels_, const float>(
            buffer_.getChannelPointers(),
            buffer_.size_))
    {
        fillInputSequence();
    }

    void fillInputSequence()
    {
        for (size_t ch = 0; ch < numChannels_; ch++)
            for (size_t i = 0; i < numSamples_; i++)
                buffer_[ch][i] = float(100 * ch + i + 1);
    }

    static constexpr size_t sampleRate_ = 48000u;
    static constexpr size_t numChannels_ = 2;
    static constexpr size_t numSamples_ = 5;
    AudioBuffer<numChannels_, numSamples_> buffer_;

    Player<MockProcessor,
           ::testing::NiceMock<MockSpeedModulator>,
           sampleRate_,
           numChannels_>
        player_;
};

TEST_F(DSP_Player, a_stateAfterInit)
{
    EXPECT_FALSE(player_.isPlaying());
}

TEST_F(DSP_Player, b_reset)
{
    EXPECT_CALL(player_.getProcessor(), reset())
        .Times(1);

    player_.reset();
}

TEST_F(DSP_Player, c_playForwardsNormalSpeed)
{
    const auto expectedPostGain = 0.89563f;
    const auto numSamplesToPlay = 2 * numSamples_;

    // the processor mock stores its argument values and
    // returns a modified value for examination later
    std::array<std::vector<FloatWithTolerance>, numChannels_> processCallArgs_input;
    std::vector<FloatWithTolerance> processCallArgs_param;
    ON_CALL(player_.getProcessor(), process(_, _))
        .WillByDefault([&](float inputsAndOutputs[numChannels_], const MockProcessor::Parameters& params) {
            for (size_t ch = 0; ch < numChannels_; ch++)
            {
                processCallArgs_input[ch].push_back(inputsAndOutputs[ch]);
                inputsAndOutputs[ch] += 100;
            }
            processCallArgs_param.push_back(params.value);
        });

    // expect processor to be called the correct number of times.
    // linear interpolation requires samples i and i+1, even if fractional is 0 (and i+1 isn't actually used)
    // that's why an additional process() call is made
    EXPECT_CALL(player_.getProcessor(), process(_, _)).Times(numSamplesToPlay + 1);

    // play the full buffer
    player_.startPlaying(numSamples_);

    AudioBuffer<numChannels_, numSamplesToPlay, float> outputBuffer;
    outputBuffer.fill(0.0f);
    // process in two blocks
    player_.process(1.0f, // speed
                    0.0f, // speed modulation amount
                    Direction::forwards,
                    expectedPostGain,
                    { 0.0f }, // processor parameters
                    outputBuffer.subBlock(0, numSamplesToPlay / 2 + 1), // first half of the buffer
                    ExponentialSmoother::TimeConstant::instantaneous(), // post gain time constant
                    ExponentialSmoother::TimeConstant::instantaneous()); // speed time constant
    player_.process(1.0f, // speed
                    0.0f, // speed modulation amount
                    Direction::forwards,
                    expectedPostGain,
                    { 1.0f }, // processor parameters (changed this time)
                    outputBuffer.subBlock(numSamplesToPlay / 2 + 1), // second half of the buffer
                    ExponentialSmoother::TimeConstant::instantaneous(), // post gain time constant
                    ExponentialSmoother::TimeConstant::instantaneous()); // speed time constant

    // expect correct raw samples to be passed to the processor
    std::array<std::vector<FloatWithTolerance>, numChannels_> expectedProcessorCallArgs_input = {
        // left channel
        std::vector<FloatWithTolerance>(
            { 1.0f,
              2.0f,
              3.0f,
              4.0f,
              5.0f,
              1.0f, // here the playback buffer must wrap around to the beginning (just 5 samples long!)
              2.0f,
              3.0f,
              4.0f,
              5.0f, // last sample of second buffer
              // this is the additonal sample from the linear interpolation
              1.0f }),
        // right channel
        std::vector<FloatWithTolerance>(
            { 101.0f,
              102.0f,
              103.0f,
              104.0f,
              105.0f,
              101.0f, // here the playback buffer must wrap around to the beginning (just 5 samples long!)
              102.0f,
              103.0f,
              104.0f,
              105.0f, // last sample of second buffer
              // this is the additonal sample from the linear interpolation
              101.0f })
    };
    EXPECT_EQ(processCallArgs_input, expectedProcessorCallArgs_input);
    // expect correct parameters to be passed to the processor
    std::vector<FloatWithTolerance> expectedProcessorCallArgs_param(
        { 0.0f,
          0.0f,
          0.0f,
          0.0f,
          0.0f,
          0.0f,
          0.0f, /* start of second block with changed parameters. This sample was already processed during
                   the first block (as required for linear interpolation) hence it has the "old value" */
          1.0f,
          1.0f,
          1.0f,
          // as before - this is the additonal sample from the linear interpolation
          1.0f });
    EXPECT_EQ(processCallArgs_param, expectedProcessorCallArgs_param);

    // expect return values to be what the processor returned, with the gain added
    std::array<std::vector<FloatWithTolerance>, 2> expectedOutputSamples = {
        // left channel
        std::vector<FloatWithTolerance>(
            { (100 + 1) * expectedPostGain,
              (100 + 2) * expectedPostGain,
              (100 + 3) * expectedPostGain,
              (100 + 4) * expectedPostGain,
              (100 + 5) * expectedPostGain,
              (100 + 1) * expectedPostGain, // this is where the buffer wrapped around
              (100 + 2) * expectedPostGain,
              (100 + 3) * expectedPostGain,
              (100 + 4) * expectedPostGain,
              (100 + 5) * expectedPostGain }),
        // right channel
        std::vector<FloatWithTolerance>(
            { (100 + 101) * expectedPostGain,
              (100 + 102) * expectedPostGain,
              (100 + 103) * expectedPostGain,
              (100 + 104) * expectedPostGain,
              (100 + 105) * expectedPostGain,
              (100 + 101) * expectedPostGain, // this is where the buffer wrapped around
              (100 + 102) * expectedPostGain,
              (100 + 103) * expectedPostGain,
              (100 + 104) * expectedPostGain,
              (100 + 105) * expectedPostGain })
    };
    EXPECT_EQ(toArrOfVecs(outputBuffer), expectedOutputSamples);
}

TEST_F(DSP_Player, d_playForwardsHalfSpeed)
{
    const auto expectedPostGain = 0.89563f;
    const auto numSamplesToPlay = 2 * numSamples_;

    // the processor mock stores its argument values and
    // returns a modified value for examination later
    std::array<std::vector<FloatWithTolerance>, numChannels_> processCallArgs_input;
    std::vector<FloatWithTolerance> processCallArgs_param;
    ON_CALL(player_.getProcessor(), process(_, _))
        .WillByDefault([&](float inputsAndOutputs[numChannels_], const MockProcessor::Parameters& params) {
            for (size_t ch = 0; ch < numChannels_; ch++)
            {
                processCallArgs_input[ch].push_back(inputsAndOutputs[ch]);
                inputsAndOutputs[ch] += 100;
            }
            processCallArgs_param.push_back(params.value);
        });

    // expect processor to be called once for two output samples.
    // linear interpolation requires samples i and i+1, even if fractional is 0 (and i+1 isn't actually used)
    // that's why an additional process() call is made at the end
    EXPECT_CALL(player_.getProcessor(), process(_, _)).Times(numSamplesToPlay / 2 + 1);

    // play the full buffer
    player_.startPlaying(numSamples_);

    AudioBuffer<numChannels_, numSamplesToPlay, float> outputBuffer;
    outputBuffer.fill(0.0f);
    // process in two blocks
    player_.process(0.5f, // speed
                    0.0f, // speed modulation amount
                    Direction::forwards,
                    expectedPostGain,
                    { 0.0f }, // processor params
                    outputBuffer.subBlock(0, numSamplesToPlay / 2 + 1), // first half of the buffer
                    ExponentialSmoother::TimeConstant::instantaneous(), // post gain time constant
                    ExponentialSmoother::TimeConstant::instantaneous()); // speed time constant
    player_.process(0.5f, // speed
                    0.0f, // speed modulation amount
                    Direction::forwards,
                    expectedPostGain,
                    { 1.0f }, // processor params (changed this time)
                    outputBuffer.subBlock(numSamplesToPlay / 2 + 1), // second half of the buffer
                    ExponentialSmoother::TimeConstant::instantaneous(), // post gain time constant
                    ExponentialSmoother::TimeConstant::instantaneous()); // speed time constant

    // expect correct raw samples to be passed to the processor. Since the processor is
    // processed before the resampling, we expect numSamples/2 + 1 calls.
    std::array<std::vector<FloatWithTolerance>, numChannels_> expectedProcessorCallArgs_input = {
        // left channel
        std::vector<FloatWithTolerance>(
            { 1.0f,
              2.0f,
              3.0f,
              4.0f,
              5.0f, // here the playback buffer must wrap around to the beginning (just 5 samples long!)
              // this is the additonal sample from the linear interpolation
              1.0f }),
        // right channel
        std::vector<FloatWithTolerance>(
            { 101.0f,
              102.0f,
              103.0f,
              104.0f,
              105.0f, // here the playback buffer must wrap around to the beginning (just 5 samples long!)
              // this is the additonal sample from the linear interpolation
              101.0f })
    };
    EXPECT_EQ(processCallArgs_input, expectedProcessorCallArgs_input);
    // expect correct parameters to be passed to the processor
    std::vector<FloatWithTolerance> expectedProcessorCallArgs_param(
        { 0.0f,
          0.0f,
          0.0f,
          0.0f, /* start of second block with changed parameters. This sample was already processed during
                   the first block (as required for linear interpolation) hence it has the "old value" */
          1.0f,
          1.0f }); // as before - this is the additonal sample from the linear interpolation
    EXPECT_EQ(processCallArgs_param, expectedProcessorCallArgs_param);

    // expect return values to be what the processor returned, with the gain added
    std::array<std::vector<FloatWithTolerance>, 2> expectedOutputSamples = {
        // left channel
        std::vector<FloatWithTolerance>(
            {
                (100 + 1.0f) * expectedPostGain,
                (100 + 1.5f) * expectedPostGain, // interpolated between 1.0f and 2.0f due to half speed
                (100 + 2.0f) * expectedPostGain,
                (100 + 2.5f) * expectedPostGain, // interpolated
                (100 + 3.0f) * expectedPostGain,
                (100 + 3.5f) * expectedPostGain, // interpolated
                (100 + 4.0f) * expectedPostGain,
                (100 + 4.5f) * expectedPostGain, // interpolated
                (100 + 5.0f) * expectedPostGain,
                (100 + 3.0f) * expectedPostGain // interpolated between 5.0f and 1.0f
            }),
        // right channel
        std::vector<FloatWithTolerance>(
            {
                (100 + 101.0f) * expectedPostGain,
                (100 + 101.5f) * expectedPostGain, // interpolated between 101.0f and 102.0f due to half speed
                (100 + 102.0f) * expectedPostGain,
                (100 + 102.5f) * expectedPostGain, // interpolated
                (100 + 103.0f) * expectedPostGain,
                (100 + 103.5f) * expectedPostGain, // interpolated
                (100 + 104.0f) * expectedPostGain,
                (100 + 104.5f) * expectedPostGain, // interpolated
                (100 + 105.0f) * expectedPostGain,
                (100 + 103.0f) * expectedPostGain // interpolated between 105.0f and 101.0f
            })
    };
    EXPECT_EQ(toArrOfVecs(outputBuffer), expectedOutputSamples);
}

TEST_F(DSP_Player, e_playForwardsDoubleSpeed)
{
    const auto expectedPostGain = 0.89563f;
    const auto numSamplesToPlay = 6;

    // the processor mock stores its argument values and
    // returns a modified value for examination later
    std::array<std::vector<FloatWithTolerance>, numChannels_> processCallArgs_input;
    std::vector<FloatWithTolerance> processCallArgs_param;
    ON_CALL(player_.getProcessor(), process(_, _))
        .WillByDefault([&](float inputsAndOutputs[numChannels_], const MockProcessor::Parameters& params) {
            for (size_t ch = 0; ch < numChannels_; ch++)
            {
                processCallArgs_input[ch].push_back(inputsAndOutputs[ch]);
                inputsAndOutputs[ch] += 100;
            }
            processCallArgs_param.push_back(params.value);
        });

    // expect processor to be called twice for each output sample.
    // linear interpolation requires samples i and i+1, even if fractional is 0 (and i+1 isn't actually used)
    // that's why we also expect two raw samples read for the last output sample
    EXPECT_CALL(player_.getProcessor(), process(_, _)).Times(numSamplesToPlay * 2);

    // play the full buffer
    player_.startPlaying(numSamples_);

    AudioBuffer<numChannels_, numSamplesToPlay, float> outputBuffer;
    outputBuffer.fill(0.0f);
    // process in two blocks
    player_.process(2.0f, // speed
                    0.0f, // speed modulation amount
                    Direction::forwards,
                    expectedPostGain,
                    { 0.0f }, // processor params
                    outputBuffer.subBlock(0, numSamplesToPlay / 2 + 1), // first half of the buffer
                    ExponentialSmoother::TimeConstant::instantaneous(), // post gain time constant
                    ExponentialSmoother::TimeConstant::instantaneous()); // speed time constant
    player_.process(2.0f, // speed
                    0.0f, // speed modulation amount
                    Direction::forwards,
                    expectedPostGain,
                    { 1.0f }, // processor params (changed this time)
                    outputBuffer.subBlock(numSamplesToPlay / 2 + 1), // second half of the buffer
                    ExponentialSmoother::TimeConstant::instantaneous(), // post gain time constant
                    ExponentialSmoother::TimeConstant::instantaneous()); // speed time constant

    // expect correct raw samples to be passed to the processor. Since the processor is
    // processed before the resampling, we expect numSamples*2 calls.
    std::array<std::vector<FloatWithTolerance>, numChannels_> expectedProcessorCallArgs_input = {
        // left channel
        std::vector<FloatWithTolerance>(
            { 1.0f,
              2.0f,
              3.0f,
              4.0f,
              5.0f, // here the playback buffer must wrap around to the beginning (just 5 samples long!)
              1.0f,
              2.0f,
              3.0f,
              4.0f,
              5.0f, // here the playback buffer must wrap around to the beginning (just 5 samples long!)
              1.0f,
              // this is the additonal sample from the linear interpolation
              2.0f }),
        // right channel
        std::vector<FloatWithTolerance>(
            { 101.0f,
              102.0f,
              103.0f,
              104.0f,
              105.0f, // here the playback buffer must wrap around to the beginning (just 5 samples long!)
              101.0f,
              102.0f,
              103.0f,
              104.0f,
              105.0f, // here the playback buffer must wrap around to the beginning (just 5 samples long!)
              101.0f,
              // this is the additonal sample from the linear interpolation
              102.0f })
    };
    EXPECT_EQ(processCallArgs_input, expectedProcessorCallArgs_input);
    // expect correct parameters to be passed to the processor
    std::vector<FloatWithTolerance> expectedProcessorCallArgs_param(
        { 0.0f,
          0.0f,
          0.0f,
          0.0f,
          0.0f,
          0.0f,
          0.0f,
          0.0f, /* start of second block with changed parameters. This sample was already processed during
                   the first block (as required for linear interpolation) hence it has the "old value" */
          1.0f,
          1.0f,
          1.0f,
          1.0f }); // as before - this is the additonal sample from the linear interpolation
    EXPECT_EQ(processCallArgs_param, expectedProcessorCallArgs_param);

    // expect return values to be what the processor returned, with the gain added
    std::array<std::vector<FloatWithTolerance>, numChannels_> expectedOutputSamples = {
        // left channel
        std::vector<FloatWithTolerance>(
            { (100 + 1.0f) * expectedPostGain,
              (100 + 3.0f) * expectedPostGain,
              (100 + 5.0f) * expectedPostGain,
              (100 + 2.0f) * expectedPostGain,
              (100 + 4.0f) * expectedPostGain,
              (100 + 1.0f) * expectedPostGain }),
        // right channel
        std::vector<FloatWithTolerance>(
            { (100 + 101.0f) * expectedPostGain,
              (100 + 103.0f) * expectedPostGain,
              (100 + 105.0f) * expectedPostGain,
              (100 + 102.0f) * expectedPostGain,
              (100 + 104.0f) * expectedPostGain,
              (100 + 101.0f) * expectedPostGain }),
    };
    EXPECT_EQ(toArrOfVecs(outputBuffer), expectedOutputSamples);
}

TEST_F(DSP_Player, f_playBackwardsNormalSpeed)
{
    const auto expectedPostGain = 0.89563f;
    const auto numSamplesToPlay = 2 * numSamples_;

    // the processor mock stores its argument values and
    // returns a modified value for examination later
    std::array<std::vector<FloatWithTolerance>, numChannels_> processCallArgs_input;
    std::vector<FloatWithTolerance> processCallArgs_param;
    ON_CALL(player_.getProcessor(), process(_, _))
        .WillByDefault([&](float inputsAndOutputs[numChannels_], const MockProcessor::Parameters& params) {
            for (size_t ch = 0; ch < numChannels_; ch++)
            {
                processCallArgs_input[ch].push_back(inputsAndOutputs[ch]);
                inputsAndOutputs[ch] += 100;
            }
            processCallArgs_param.push_back(params.value);
        });

    // expect processor to be called the correct number of times.
    // linear interpolation requires samples i and i+1, even if fractional is 0 (and i+1 isn't actually used)
    // that's why an additional process() call is made
    EXPECT_CALL(player_.getProcessor(), process(_, _)).Times(numSamplesToPlay + 1);

    // play the full buffer
    player_.startPlaying(numSamples_);

    AudioBuffer<numChannels_, numSamplesToPlay, float> outputBuffer;
    outputBuffer.fill(0.0f);
    // process in two blocks
    player_.process(1.0f, // speed
                    0.0f, // speed modulation amount
                    Direction::backwards,
                    expectedPostGain,
                    { 0.0f }, // processor params
                    outputBuffer.subBlock(0, numSamplesToPlay / 2 + 1), // first half of the buffer
                    ExponentialSmoother::TimeConstant::instantaneous(), // post gain time constant
                    ExponentialSmoother::TimeConstant::instantaneous()); // speed time constant
    player_.process(1.0f, // speed
                    0.0f, // speed modulation amount
                    Direction::backwards,
                    expectedPostGain,
                    { 1.0f }, // processor params (changed this time)
                    outputBuffer.subBlock(numSamplesToPlay / 2 + 1), // second half of the buffer
                    ExponentialSmoother::TimeConstant::instantaneous(), // post gain time constant
                    ExponentialSmoother::TimeConstant::instantaneous()); // speed time constant

    // expect correct raw samples to be passed to the processor
    std::array<std::vector<FloatWithTolerance>, numChannels_> expectedProcessorCallArgs_input = {
        // left channel
        std::vector<FloatWithTolerance>(
            { 5.0f,
              4.0f,
              3.0f,
              2.0f,
              1.0f,
              5.0f, // here the playback buffer must wrap around to the beginning (just 5 samples long!)
              4.0f,
              3.0f,
              2.0f,
              1.0f, // last sample of second buffer
              // this is the additonal sample from the linear interpolation
              5.0f }),
        // right channel
        std::vector<FloatWithTolerance>(
            { 105.0f,
              104.0f,
              103.0f,
              102.0f,
              101.0f,
              105.0f, // here the playback buffer must wrap around to the beginning (just 5 samples long!)
              104.0f,
              103.0f,
              102.0f,
              101.0f, // last sample of second buffer
              // this is the additonal sample from the linear interpolation
              105.0f }),
    };
    EXPECT_EQ(processCallArgs_input, expectedProcessorCallArgs_input);
    // expect correct parameters to be passed to the processor
    std::vector<FloatWithTolerance> expectedProcessorCallArgs_param(
        { 0.0f,
          0.0f,
          0.0f,
          0.0f,
          0.0f,
          0.0f,
          0.0f, /* start of second block with changed parameters. This sample was already processed during
                   the first block (as required for linear interpolation) hence it has the "old value" */
          1.0f,
          1.0f,
          1.0f,
          // as before - this is the additonal sample from the linear interpolation
          1.0f });
    EXPECT_EQ(processCallArgs_param, expectedProcessorCallArgs_param);

    // expect return values to be what the processor returned, with the gain added
    std::array<std::vector<FloatWithTolerance>, numChannels_> expectedOutputSamples = {
        // left channel
        std::vector<FloatWithTolerance>(
            { (100 + 5) * expectedPostGain,
              (100 + 4) * expectedPostGain,
              (100 + 3) * expectedPostGain,
              (100 + 2) * expectedPostGain,
              (100 + 1) * expectedPostGain,
              (100 + 5) * expectedPostGain, // this is where the buffer wrapped around
              (100 + 4) * expectedPostGain,
              (100 + 3) * expectedPostGain,
              (100 + 2) * expectedPostGain,
              (100 + 1) * expectedPostGain }),
        // right channel
        std::vector<FloatWithTolerance>(
            { (100 + 105) * expectedPostGain,
              (100 + 104) * expectedPostGain,
              (100 + 103) * expectedPostGain,
              (100 + 102) * expectedPostGain,
              (100 + 101) * expectedPostGain,
              (100 + 105) * expectedPostGain, // this is where the buffer wrapped around
              (100 + 104) * expectedPostGain,
              (100 + 103) * expectedPostGain,
              (100 + 102) * expectedPostGain,
              (100 + 101) * expectedPostGain })
    };
    EXPECT_EQ(toArrOfVecs(outputBuffer), expectedOutputSamples);
}

TEST_F(DSP_Player, g_playBackwardsHalfSpeed)
{
    const auto expectedPostGain = 0.89563f;
    const auto numSamplesToPlay = 2 * numSamples_;

    // the processor mock stores its argument values and
    // returns a modified value for examination later
    std::array<std::vector<FloatWithTolerance>, numChannels_> processCallArgs_input;
    std::vector<FloatWithTolerance> processCallArgs_param;
    ON_CALL(player_.getProcessor(), process(_, _))
        .WillByDefault([&](float inputsAndOutputs[numChannels_], const MockProcessor::Parameters& params) {
            for (size_t ch = 0; ch < numChannels_; ch++)
            {
                processCallArgs_input[ch].push_back(inputsAndOutputs[ch]);
                inputsAndOutputs[ch] += 100;
            }
            processCallArgs_param.push_back(params.value);
        });

    // expect processor to be called once for two output samples.
    // linear interpolation requires samples i and i+1, even if fractional is 0 (and i+1 isn't actually used)
    // that's why an additional process() call is made at the end
    EXPECT_CALL(player_.getProcessor(), process(_, _)).Times(numSamplesToPlay / 2 + 1);

    // play the full buffer
    player_.startPlaying(numSamples_);

    AudioBuffer<numChannels_, numSamplesToPlay, float> outputBuffer;
    outputBuffer.fill(0.0f);
    // process in two blocks
    player_.process(0.5f, // speed
                    0.0f, // speed modulation amount
                    Direction::backwards,
                    expectedPostGain,
                    { 0.0f }, // processor params
                    outputBuffer.subBlock(0, numSamplesToPlay / 2 + 1), // first half of the buffer
                    ExponentialSmoother::TimeConstant::instantaneous(), // post gain time constant
                    ExponentialSmoother::TimeConstant::instantaneous()); // speed time constant
    player_.process(0.5f, // speed
                    0.0f, // speed modulation amount
                    Direction::backwards,
                    expectedPostGain,
                    { 1.0f }, // processor params (changed this time)
                    outputBuffer.subBlock(numSamplesToPlay / 2 + 1), // second half of the buffer
                    ExponentialSmoother::TimeConstant::instantaneous(), // post gain time constant
                    ExponentialSmoother::TimeConstant::instantaneous()); // speed time constant

    // expect correct raw samples to be passed to the processor. Since the processor is
    // processed before the resampling, we expect numSamples/2 + 1 calls.
    std::array<std::vector<FloatWithTolerance>, numChannels_> expectedProcessorCallArgs_input = {
        // left channel
        std::vector<FloatWithTolerance>(
            { 5.0f,
              4.0f,
              3.0f,
              2.0f,
              1.0f, // here the playback buffer must wrap around to the beginning (just 5 samples long!)
              5.0f }), // this is the additonal sample from the linear interpolation
        // right channel
        std::vector<FloatWithTolerance>(
            { 105.0f,
              104.0f,
              103.0f,
              102.0f,
              101.0f, // here the playback buffer must wrap around to the beginning (just 5 samples long!)
              105.0f }) // this is the additonal sample from the linear interpolation
    };
    EXPECT_EQ(processCallArgs_input, expectedProcessorCallArgs_input);
    // expect correct parameters to be passed to the processor
    std::vector<FloatWithTolerance> expectedProcessorCallArgs_param(
        { 0.0f,
          0.0f,
          0.0f,
          0.0f, /* start of second block with changed parameters. This sample was already processed during
                   the first block (as required for linear interpolation) hence it has the "old value" */
          1.0f,
          1.0f }); // as before - this is the additonal sample from the linear interpolation
    EXPECT_EQ(processCallArgs_param, expectedProcessorCallArgs_param);

    // expect return values to be what the processor returned, with the gain added
    std::array<std::vector<FloatWithTolerance>, numChannels_> expectedOutputSamples = {
        // left channel
        std::vector<FloatWithTolerance>(
            {
                (100 + 5.0f) * expectedPostGain,
                (100 + 4.5f) * expectedPostGain, // interpolated between 4.0f and 5.0f due to half speed
                (100 + 4.0f) * expectedPostGain,
                (100 + 3.5f) * expectedPostGain, // interpolated
                (100 + 3.0f) * expectedPostGain,
                (100 + 2.5f) * expectedPostGain, // interpolated
                (100 + 2.0f) * expectedPostGain,
                (100 + 1.5f) * expectedPostGain, // interpolated
                (100 + 1.0f) * expectedPostGain,
                (100 + 3.0f) * expectedPostGain // interpolated between 1.0f and 5.0f
            }),
        // right channel
        std::vector<FloatWithTolerance>(
            {
                (100 + 105.0f) * expectedPostGain,
                (100 + 104.5f) * expectedPostGain, // interpolated between 4.0f and 5.0f due to half speed
                (100 + 104.0f) * expectedPostGain,
                (100 + 103.5f) * expectedPostGain, // interpolated
                (100 + 103.0f) * expectedPostGain,
                (100 + 102.5f) * expectedPostGain, // interpolated
                (100 + 102.0f) * expectedPostGain,
                (100 + 101.5f) * expectedPostGain, // interpolated
                (100 + 101.0f) * expectedPostGain,
                (100 + 103.0f) * expectedPostGain // interpolated between 1.0f and 5.0f
            }),
    };
    EXPECT_EQ(toArrOfVecs(outputBuffer), expectedOutputSamples);
}

TEST_F(DSP_Player, h_playBackwardsDoubleSpeed)
{
    const auto expectedPostGain = 0.89563f;
    const auto numSamplesToPlay = 6;

    // the processor mock stores its argument values and
    // returns a modified value for examination later
    std::array<std::vector<FloatWithTolerance>, numChannels_> processCallArgs_input;
    std::vector<FloatWithTolerance> processCallArgs_param;
    ON_CALL(player_.getProcessor(), process(_, _))
        .WillByDefault([&](float inputsAndOutputs[numChannels_], const MockProcessor::Parameters& params) {
            for (size_t ch = 0; ch < numChannels_; ch++)
            {
                processCallArgs_input[ch].push_back(inputsAndOutputs[ch]);
                inputsAndOutputs[ch] += 100;
            }
            processCallArgs_param.push_back(params.value);
        });

    // expect processor to be called twice for each output sample.
    // linear interpolation requires samples i and i+1, even if fractional is 0 (and i+1 isn't actually used)
    // that's why we also expect two raw samples read for the last output sample
    EXPECT_CALL(player_.getProcessor(), process(_, _)).Times(numSamplesToPlay * 2);

    // play the full buffer
    player_.startPlaying(numSamples_);

    AudioBuffer<numChannels_, numSamplesToPlay, float> outputBuffer;
    outputBuffer.fill(0.0f);
    // process in two blocks
    player_.process(2.0f, // speed
                    0.0f, // speed modulation amount
                    Direction::backwards,
                    expectedPostGain,
                    { 0.0f }, // processor params
                    outputBuffer.subBlock(0, numSamplesToPlay / 2 + 1), // first half of the buffer
                    ExponentialSmoother::TimeConstant::instantaneous(), // post gain time constant
                    ExponentialSmoother::TimeConstant::instantaneous()); // speed time constant
    player_.process(2.0f, // speed
                    0.0f, // speed modulation amount
                    Direction::backwards,
                    expectedPostGain,
                    { 1.0f }, // processor params (changed this time)
                    outputBuffer.subBlock(numSamplesToPlay / 2 + 1), // second half of the buffer
                    ExponentialSmoother::TimeConstant::instantaneous(), // post gain time constant
                    ExponentialSmoother::TimeConstant::instantaneous()); // speed time constant

    // expect correct raw samples to be passed to the processor. Since the processor is
    // processed before the resampling, we expect numSamples*2 calls.
    std::array<std::vector<FloatWithTolerance>, numChannels_> expectedProcessorCallArgs_input = {
        // left channel
        std::vector<FloatWithTolerance>(
            { 5.0f,
              4.0f,
              3.0f,
              2.0f,
              1.0f, // here the playback buffer must wrap around to the beginning (just 5 samples long!)
              5.0f,
              4.0f,
              3.0f,
              2.0f,
              1.0f, // here the playback buffer must wrap around to the beginning (just 5 samples long!)
              5.0f,
              4.0f }), // this is the additonal sample from the linear interpolation
        // right channel
        std::vector<FloatWithTolerance>(
            { 105.0f,
              104.0f,
              103.0f,
              102.0f,
              101.0f, // here the playback buffer must wrap around to the beginning (just 5 samples long!)
              105.0f,
              104.0f,
              103.0f,
              102.0f,
              101.0f, // here the playback buffer must wrap around to the beginning (just 5 samples long!)
              105.0f,
              104.0f }), // this is the additonal sample from the linear interpolation
    };
    EXPECT_EQ(processCallArgs_input, expectedProcessorCallArgs_input);
    // expect correct parameters to be passed to the processor
    std::vector<FloatWithTolerance> expectedProcessorCallArgs_param(
        { 0.0f,
          0.0f,
          0.0f,
          0.0f,
          0.0f,
          0.0f,
          0.0f,
          0.0f, /* start of second block with changed parameters. This sample was already processed during
                   the first block (as required for linear interpolation) hence it has the "old value" */
          1.0f,
          1.0f,
          1.0f,
          1.0f }); // as before - this is the additonal sample from the linear interpolation
    EXPECT_EQ(processCallArgs_param, expectedProcessorCallArgs_param);

    // expect return values to be what the processor returned, with the gain added
    std::array<std::vector<FloatWithTolerance>, numChannels_> expectedOutputSamples = {
        // left channel
        std::vector<FloatWithTolerance>(
            { (100 + 5.0f) * expectedPostGain,
              (100 + 3.0f) * expectedPostGain,
              (100 + 1.0f) * expectedPostGain,
              (100 + 4.0f) * expectedPostGain,
              (100 + 2.0f) * expectedPostGain,
              (100 + 5.0f) * expectedPostGain }),
        // right channel
        std::vector<FloatWithTolerance>(
            { (100 + 105.0f) * expectedPostGain,
              (100 + 103.0f) * expectedPostGain,
              (100 + 101.0f) * expectedPostGain,
              (100 + 104.0f) * expectedPostGain,
              (100 + 102.0f) * expectedPostGain,
              (100 + 105.0f) * expectedPostGain })
    };
    EXPECT_EQ(toArrOfVecs(outputBuffer), expectedOutputSamples);
}

TEST_F(DSP_Player, i_applySmoothingToPostGain)
{
    const auto numSamplesToPlay = 10;

    // the processor mock always returns 1.0f
    ON_CALL(player_.getProcessor(), process(_, _))
        .WillByDefault([&](float inputsAndOutputs[numChannels_], const MockProcessor::Parameters&) {
            inputsAndOutputs[0] = 1.0f;
            inputsAndOutputs[1] = 10.0f;
        });
    // add this expectation to silence gmock warning.
    EXPECT_CALL(player_.getProcessor(), process(_, _)).Times(::testing::AnyNumber());

    // play the full buffer
    player_.startPlaying(numSamples_);

    // the smoothing we'll use
    const auto timeConstant = ExponentialSmoother::TimeConstant::fromRawValue(0.5f);
    const auto targetGain = 1.0f;

    AudioBuffer<numChannels_, numSamplesToPlay, float> outputBuffer;
    outputBuffer.fill(0.0f);
    // process block (ramping the gain up)
    player_.process(1.0f, // speed
                    0.0f, // speed modulation amount
                    Direction::forwards,
                    targetGain, // post gain
                    { 0.0f }, // processor params
                    outputBuffer.subBlock(0, numSamplesToPlay / 2), // first half of the buffer
                    timeConstant, // post gain time constant
                    ExponentialSmoother::TimeConstant::instantaneous()); // speed time constant
    // process block (ramping the gain back down)
    player_.process(1.0f, // speed
                    0.0f, // speed modulation amount
                    Direction::forwards,
                    0.0f, // target post gain
                    { 0.0f }, // processor params
                    outputBuffer.subBlock(numSamplesToPlay / 2), // second half of the buffer
                    timeConstant, // post gain time constant
                    ExponentialSmoother::TimeConstant::instantaneous()); // speed time constant

    // we expect the gain to ramp up/down exponentially with the supplied smoothing constant
    std::array<std::vector<FloatWithTolerance>, numChannels_> expectedOutputSamples = {
        // left channel
        std::vector<FloatWithTolerance>(
            {
                targetGain * 0.5f,
                targetGain * 0.75f,
                targetGain * 0.875f,
                targetGain * 0.9375f,
                targetGain * 0.96875f,
                // ramping down again
                targetGain * 0.484375f,
                targetGain * 0.2421875f,
                targetGain * 0.12109375f,
                targetGain * 0.060546875f,
                targetGain * 0.0302734375f,
            }),
        // right channel
        std::vector<FloatWithTolerance>(
            {
                10.0f * targetGain * 0.5f,
                10.0f * targetGain * 0.75f,
                10.0f * targetGain * 0.875f,
                10.0f * targetGain * 0.9375f,
                10.0f * targetGain * 0.96875f,
                // ramping down again
                10.0f * targetGain * 0.484375f,
                10.0f * targetGain * 0.2421875f,
                10.0f * targetGain * 0.12109375f,
                10.0f * targetGain * 0.060546875f,
                10.0f * targetGain * 0.0302734375f,
            })
    };
    EXPECT_EQ(toArrOfVecs(outputBuffer), expectedOutputSamples);
}

TEST_F(DSP_Player, j_applySmoothingToSpeed)
{
    const auto numSamplesToPlay = 6;

    // the processor mock leaves the input samples untouched so
    // that we can easily see the read position incrementing
    ON_CALL(player_.getProcessor(), process(_, _))
        .WillByDefault([&](float[], const MockProcessor::Parameters&) {});
    // add this expectation to silence gmock warning.
    EXPECT_CALL(player_.getProcessor(), process(_, _)).Times(::testing::AnyNumber());

    // play the full buffer
    player_.startPlaying(numSamples_);

    // the smoothing we'll use
    const auto timeConstant = ExponentialSmoother::TimeConstant::fromRawValue(0.5f);
    const auto targetSpeed1 = 0.5f;
    const auto targetSpeed2 = 1.0f;

    AudioBuffer<numChannels_, numSamplesToPlay, float> outputBuffer;
    outputBuffer.fill(0.0f);
    // process block (ramping the speed down)
    player_.process(targetSpeed1,
                    0.0f, // speed modulation amount
                    Direction::forwards,
                    1.0f, // post gain
                    { 0.0f }, // processor params
                    outputBuffer.subBlock(0, numSamplesToPlay / 2), // first half of the buffer
                    ExponentialSmoother::TimeConstant::instantaneous(), // post gain time constant
                    timeConstant); // speed time constant
    // process block (ramping the speed back up)
    player_.process(targetSpeed2,
                    0.0f, // speed modulation amount
                    Direction::forwards,
                    1.0f, // post gain
                    { 0.0f }, // processor params
                    outputBuffer.subBlock(numSamplesToPlay / 2), // second half of the buffer
                    ExponentialSmoother::TimeConstant::instantaneous(), // post gain time constant
                    timeConstant); // speed time constant

    // we expect the speed to ramp up/down exponentially with the supplied smoothing constant
    // we examine this from the output samples - since the raw playback buffer contains
    // increasing numbers 1, 2, 3, 4, ... we can easily see the phase increments here.
    AudioBuffer<numChannels_, numSamplesToPlay, float> expectedOutputSamples;
    for (size_t ch = 0; ch < numChannels_; ch++)
    {
        float currentSpeed = 0.0f;
        float expectedReadPos = 0.0f;
        for (int i = 0; i < numSamplesToPlay; i++)
        {
            expectedOutputSamples[ch][i] = float(100 * ch + expectedReadPos + 1);
            const auto targetSpeed = (i >= (numSamplesToPlay / 2)) ? targetSpeed2 : targetSpeed1;
            currentSpeed = (targetSpeed - currentSpeed) * timeConstant.value_ + currentSpeed;
            expectedReadPos += currentSpeed;
        }
    }
    EXPECT_EQ(toArrOfVecs(outputBuffer), toArrOfVecs(expectedOutputSamples));
}

TEST_F(DSP_Player, k_applyNoSmoothingToProcessorParameters)
{
    const auto numSamplesToPlay = 10;

    // the processor mock stores its argument values
    std::vector<FloatWithTolerance> processCallArgs_param;
    ON_CALL(player_.getProcessor(), process(_, _))
        .WillByDefault([&](float[], const MockProcessor::Parameters& params) {
            processCallArgs_param.push_back(params.value);
        });
    // add this expectation to silence gmock warning.
    EXPECT_CALL(player_.getProcessor(), process(_, _)).Times(::testing::AnyNumber());

    // play the full buffer
    player_.startPlaying(numSamples_);

    // the smoothing we'll (not) use
    const auto timeConstant = ExponentialSmoother::TimeConstant::fromRawValue(0.5f);
    const auto targetParamValue1 = 1.0f;
    const auto targetParamValue2 = 0.0f;

    AudioBuffer<numChannels_, numSamplesToPlay, float> outputBuffer;
    outputBuffer.fill(0.0f);
    // process block with one parameter value
    player_.process(1.0f, // speed
                    0.0f, // speed modulation amount
                    Direction::forwards,
                    0.0f, // post gain
                    { targetParamValue1 },
                    outputBuffer.subBlock(0, numSamplesToPlay / 2), // first half of the buffer
                    timeConstant, // post gain time constant
                    timeConstant); // speed time constant
    // process block with another parameter value
    player_.process(1.0f, // speed
                    0.0f, // speed modulation amount
                    Direction::forwards,
                    0.0f, // post gain
                    { targetParamValue2 },
                    outputBuffer.subBlock(numSamplesToPlay / 2), // second half of the buffer
                    timeConstant, // post gain time constant
                    timeConstant); // speed time constant

    // we expect the gain to ramp up/down exponentially with the supplied smoothing constant
    std::vector<FloatWithTolerance> expectedParameterValues = {
        targetParamValue1,
        targetParamValue1,
        targetParamValue1,
        targetParamValue1,
        targetParamValue1,
        targetParamValue2,
        targetParamValue2,
        targetParamValue2,
        targetParamValue2,
        targetParamValue2,
    };
    EXPECT_EQ(processCallArgs_param, expectedParameterValues);
}

TEST_F(DSP_Player, l_useSpeedModulator)
{
    const auto numSamplesToPlay = 6;

    // the processor mock leaves the input samples untouched so
    // that we can easily see the read position incrementing
    ON_CALL(player_.getProcessor(), process(_, _))
        .WillByDefault([&](float[], const MockProcessor::Parameters&) {});
    // add this expectation to silence gmock warning.
    EXPECT_CALL(player_.getProcessor(), process(_, _)).Times(::testing::AnyNumber());

    // play the full buffer
    player_.startPlaying(numSamples_);

    AudioBuffer<numChannels_, numSamplesToPlay, float> outputBuffer;
    outputBuffer.fill(0.0f);
    // process block with speed modulator returning 1
    EXPECT_CALL(player_.getSpeedModulator(), getAndAdvance())
        .Times(::testing::Exactly(3))
        .WillRepeatedly(::testing::Return(1));
    player_.process(1.0f, // speed
                    1.0f, // speed modulation amount
                    Direction::forwards,
                    1.0f, // post gain
                    { 0.0f }, // processor params
                    outputBuffer.subBlock(0, numSamplesToPlay / 2), // first half of the buffer
                    ExponentialSmoother::TimeConstant::instantaneous(), // post gain time constant
                    ExponentialSmoother::TimeConstant::instantaneous()); // speed time constant
    // process block with speed modulator returning 0.5, but mod amount set to 2.0
    EXPECT_CALL(player_.getSpeedModulator(), getAndAdvance())
        .Times(::testing::Exactly(3))
        .WillRepeatedly(::testing::Return(0.5));
    player_.process(1.0f, // speed
                    2.0f, // speed modulation amount
                    Direction::forwards,
                    1.0f, // post gain
                    { 0.0f }, // processor params
                    outputBuffer.subBlock(numSamplesToPlay / 2), // second half of the buffer
                    ExponentialSmoother::TimeConstant::instantaneous(), // post gain time constant
                    // this time, we have a smoothing time constant to make sure that the modulator is not smoothed
                    ExponentialSmoother::TimeConstant::fromRawValue(0.5f)); // speed time constant

    std::array<std::vector<FloatWithTolerance>, numChannels_> expectedOutputSamples = {
        // left channel
        std::vector<FloatWithTolerance>(
            { 1, 3, 5, 2, 4, 1 }),
        // right channel
        std::vector<FloatWithTolerance>(
            { 101, 103, 105, 102, 104, 101 }),
    };
    EXPECT_EQ(toArrOfVecs(outputBuffer), expectedOutputSamples);
}