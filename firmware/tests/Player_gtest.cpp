#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <random>
#include "dsp/Player.h"

using ::testing::_;

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
        MOCK_METHOD(float, process, (float input, const Parameters& params), ());
    };

    DSP_Player() :
        player_(buffer_.data(), numSamples_)
    {
        fillInputSequence();
    }

    void fillInputSequence()
    {
        for (size_t i = 0; i < numSamples_; i++)
            buffer_[i] = float(i);
    }

    static constexpr size_t numSamples_ = 5;
    std::array<float, numSamples_> buffer_;
    Player<MockProcessor> player_;
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
    const auto expectedGain = 0.6734258f;
    const auto numSamplesToPlay = 2 * numSamples_;

    // the processor mock stores its argument values and
    // returns a modified value for examination later
    std::vector<float> processCallArgs_input;
    std::vector<float> processCallArgs_param;
    ON_CALL(player_.getProcessor(), process(_, _))
        .WillByDefault([&](float input, const MockProcessor::Parameters& params) {
            processCallArgs_input.push_back(input);
            processCallArgs_param.push_back(params.value);
            return input + 100;
        });

    // expect processor to be called the correct number of times.
    // linear interpolation requires samples i and i+1, even if fractional is 0 (and i+1 isn't actually used)
    // that's why an additional process() call is made
    EXPECT_CALL(player_.getProcessor(), process(_, _)).Times(numSamplesToPlay + 1);

    // play the full buffer
    player_.startPlaying(numSamples_);

    std::array<float, numSamplesToPlay> outputBuffer;
    outputBuffer.fill(0.0f);
    // process in two blocks
    player_.process(1.0f, // speed
                    Direction::forwards,
                    expectedGain,
                    { 0.0f }, // processor parameters
                    outputBuffer.data(),
                    numSamplesToPlay / 2 + 1,
                    ExponentialSmoother::TimeConstant::instantaneous(), // gain time constant
                    ExponentialSmoother::TimeConstant::instantaneous()); // speed time constant
    player_.process(1.0f, // speed
                    Direction::forwards,
                    expectedGain,
                    { 1.0f }, // processor parameters (changed this time)
                    outputBuffer.data() + numSamplesToPlay / 2 + 1, // offset to fill second part of the buffer
                    numSamplesToPlay / 2 - 1,
                    ExponentialSmoother::TimeConstant::instantaneous(), // gain time constant
                    ExponentialSmoother::TimeConstant::instantaneous()); // speed time constant

    // expect correct raw samples to be passed to the processor
    std::vector<float> expectedProcessorCallArgs_input(
        { 0.0f,
          1.0f,
          2.0f,
          3.0f,
          4.0f,
          0.0f, // here the playback buffer must wrap around to the beginning (just 5 samples long!)
          1.0f,
          2.0f,
          3.0f,
          4.0f, // last sample of second buffer
          // this is the additonal sample from the linear interpolation
          0.0f });
    EXPECT_EQ(processCallArgs_input, expectedProcessorCallArgs_input);
    // expect correct raw samples to be passed to the processor
    std::vector<float> expectedProcessorCallArgs_param(
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
    std::array<float, numSamplesToPlay> expectedOutputSamples = {
        (100 + 0) * expectedGain,
        (100 + 1) * expectedGain,
        (100 + 2) * expectedGain,
        (100 + 3) * expectedGain,
        (100 + 4) * expectedGain,
        (100 + 0) * expectedGain, // this is where the buffer wrapped around
        (100 + 1) * expectedGain,
        (100 + 2) * expectedGain,
        (100 + 3) * expectedGain,
        (100 + 4) * expectedGain
    };
    EXPECT_EQ(outputBuffer, expectedOutputSamples);
}

TEST_F(DSP_Player, d_playForwardsHalfSpeed)
{
    const auto expectedGain = 0.6734258f;
    const auto numSamplesToPlay = 2 * numSamples_;

    // the processor mock stores its argument values and
    // returns a modified value for examination later
    std::vector<float> processCallArgs_input;
    std::vector<float> processCallArgs_param;
    ON_CALL(player_.getProcessor(), process(_, _))
        .WillByDefault([&](float input, const MockProcessor::Parameters& params) {
            processCallArgs_input.push_back(input);
            processCallArgs_param.push_back(params.value);
            return input + 100;
        });

    // expect processor to be called once for two output samples.
    // linear interpolation requires samples i and i+1, even if fractional is 0 (and i+1 isn't actually used)
    // that's why an additional process() call is made at the end
    EXPECT_CALL(player_.getProcessor(), process(_, _)).Times(numSamplesToPlay / 2 + 1);

    // play the full buffer
    player_.startPlaying(numSamples_);

    std::array<float, numSamplesToPlay> outputBuffer;
    outputBuffer.fill(0.0f);
    // process in two blocks
    player_.process(0.5f, // speed
                    Direction::forwards,
                    expectedGain,
                    { 0.0f }, // processor params
                    outputBuffer.data(),
                    numSamplesToPlay / 2 + 1,
                    ExponentialSmoother::TimeConstant::instantaneous(), // gain time constant
                    ExponentialSmoother::TimeConstant::instantaneous()); // speed time constant
    player_.process(0.5f, // speed
                    Direction::forwards,
                    expectedGain,
                    { 1.0f }, // processor params (changed this time)
                    outputBuffer.data() + numSamplesToPlay / 2 + 1, // offset to fill second part of the buffer
                    numSamplesToPlay / 2 - 1,
                    ExponentialSmoother::TimeConstant::instantaneous(), // gain time constant
                    ExponentialSmoother::TimeConstant::instantaneous()); // speed time constant

    // expect correct raw samples to be passed to the processor. Since the processor is
    // processed before the resampling, we expect numSamples/2 + 1 calls.
    std::vector<float> expectedProcessorCallArgs_input(
        { 0.0f,
          1.0f,
          2.0f,
          3.0f,
          4.0f, // here the playback buffer must wrap around to the beginning (just 5 samples long!)
          0.0f }); // this is the additonal sample from the linear interpolation
    EXPECT_EQ(processCallArgs_input, expectedProcessorCallArgs_input);
    // expect correct raw samples to be passed to the processor
    std::vector<float> expectedProcessorCallArgs_param(
        { 0.0f,
          0.0f,
          0.0f,
          0.0f, /* start of second block with changed parameters. This sample was already processed during
                   the first block (as required for linear interpolation) hence it has the "old value" */
          1.0f,
          1.0f }); // as before - this is the additonal sample from the linear interpolation
    EXPECT_EQ(processCallArgs_param, expectedProcessorCallArgs_param);

    // expect return values to be what the processor returned, with the gain added
    std::array<float, numSamplesToPlay> expectedOutputSamples = {
        (100 + 0.0f) * expectedGain,
        (100 + 0.5f) * expectedGain, // interpolated between 0.0f and 1.0f due to half speed
        (100 + 1.0f) * expectedGain,
        (100 + 1.5f) * expectedGain, // interpolated
        (100 + 2.0f) * expectedGain,
        (100 + 2.5f) * expectedGain, // interpolated
        (100 + 3.0f) * expectedGain,
        (100 + 3.5f) * expectedGain, // interpolated
        (100 + 4.0f) * expectedGain,
        (100 + 2.0f) * expectedGain // interpolated between 4.0f and 0.0f
    };
    EXPECT_EQ(outputBuffer, expectedOutputSamples);
}

TEST_F(DSP_Player, e_playForwardsDoubleSpeed)
{
    const auto expectedGain = 0.6734258f;
    const auto numSamplesToPlay = 6;

    // the processor mock stores its argument values and
    // returns a modified value for examination later
    std::vector<float> processCallArgs_input;
    std::vector<float> processCallArgs_param;
    ON_CALL(player_.getProcessor(), process(_, _))
        .WillByDefault([&](float input, const MockProcessor::Parameters& params) {
            processCallArgs_input.push_back(input);
            processCallArgs_param.push_back(params.value);
            return input + 100;
        });

    // expect processor to be called twice for each output sample.
    // linear interpolation requires samples i and i+1, even if fractional is 0 (and i+1 isn't actually used)
    // that's why we also expect two raw samples read for the last output sample
    EXPECT_CALL(player_.getProcessor(), process(_, _)).Times(numSamplesToPlay * 2);

    // play the full buffer
    player_.startPlaying(numSamples_);

    std::array<float, numSamplesToPlay> outputBuffer;
    outputBuffer.fill(0.0f);
    // process in two blocks
    player_.process(2.0f, // speed
                    Direction::forwards,
                    expectedGain,
                    { 0.0f }, // processor params
                    outputBuffer.data(),
                    numSamplesToPlay / 2 + 1,
                    ExponentialSmoother::TimeConstant::instantaneous(), // gain time constant
                    ExponentialSmoother::TimeConstant::instantaneous()); // speed time constant
    player_.process(2.0f, // speed
                    Direction::forwards,
                    expectedGain,
                    { 1.0f }, // processor params (changed this time)
                    outputBuffer.data() + numSamplesToPlay / 2 + 1, // offset to fill second part of the buffer
                    numSamplesToPlay / 2 - 1,
                    ExponentialSmoother::TimeConstant::instantaneous(), // gain time constant
                    ExponentialSmoother::TimeConstant::instantaneous()); // speed time constant

    // expect correct raw samples to be passed to the processor. Since the processor is
    // processed before the resampling, we expect numSamples*2 calls.
    std::vector<float> expectedProcessorCallArgs_input(
        { 0.0f,
          1.0f,
          2.0f,
          3.0f,
          4.0f, // here the playback buffer must wrap around to the beginning (just 5 samples long!)
          0.0f,
          1.0f,
          2.0f,
          3.0f,
          4.0f, // here the playback buffer must wrap around to the beginning (just 5 samples long!)
          0.0f,
          1.0f }); // this is the additonal sample from the linear interpolation
    EXPECT_EQ(processCallArgs_input, expectedProcessorCallArgs_input);
    // expect correct raw samples to be passed to the processor
    std::vector<float> expectedProcessorCallArgs_param(
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
    std::array<float, numSamplesToPlay> expectedOutputSamples = {
        (100 + 0.0f) * expectedGain,
        (100 + 2.0f) * expectedGain,
        (100 + 4.0f) * expectedGain,
        (100 + 1.0f) * expectedGain,
        (100 + 3.0f) * expectedGain,
        (100 + 0.0f) * expectedGain
    };
    EXPECT_EQ(outputBuffer, expectedOutputSamples);
}

TEST_F(DSP_Player, f_playBackwardsNormalSpeed)
{
    const auto expectedGain = 0.6734258f;
    const auto numSamplesToPlay = 2 * numSamples_;

    // the processor mock stores its argument values and
    // returns a modified value for examination later
    std::vector<float> processCallArgs_input;
    std::vector<float> processCallArgs_param;
    ON_CALL(player_.getProcessor(), process(_, _))
        .WillByDefault([&](float input, const MockProcessor::Parameters& params) {
            processCallArgs_input.push_back(input);
            processCallArgs_param.push_back(params.value);
            return input + 100;
        });

    // expect processor to be called the correct number of times.
    // linear interpolation requires samples i and i+1, even if fractional is 0 (and i+1 isn't actually used)
    // that's why an additional process() call is made
    EXPECT_CALL(player_.getProcessor(), process(_, _)).Times(numSamplesToPlay + 1);

    // play the full buffer
    player_.startPlaying(numSamples_);

    std::array<float, numSamplesToPlay> outputBuffer;
    outputBuffer.fill(0.0f);
    // process in two blocks
    player_.process(1.0f, // speed
                    Direction::backwards,
                    expectedGain,
                    { 0.0f }, // processor params
                    outputBuffer.data(),
                    numSamplesToPlay / 2 + 1,
                    ExponentialSmoother::TimeConstant::instantaneous(), // gain time constant
                    ExponentialSmoother::TimeConstant::instantaneous()); // speed time constant
    player_.process(1.0f, // speed
                    Direction::backwards,
                    expectedGain,
                    { 1.0f }, // processor params (changed this time)
                    outputBuffer.data() + numSamplesToPlay / 2 + 1, // offset to fill second part of the buffer
                    numSamplesToPlay / 2 - 1,
                    ExponentialSmoother::TimeConstant::instantaneous(), // gain time constant
                    ExponentialSmoother::TimeConstant::instantaneous()); // speed time constant

    // expect correct raw samples to be passed to the processor
    std::vector<float> expectedProcessorCallArgs_input(
        { 4.0f,
          3.0f,
          2.0f,
          1.0f,
          0.0f,
          4.0f, // here the playback buffer must wrap around to the beginning (just 5 samples long!)
          3.0f,
          2.0f,
          1.0f,
          0.0f, // last sample of second buffer
          // this is the additonal sample from the linear interpolation
          4.0f });
    EXPECT_EQ(processCallArgs_input, expectedProcessorCallArgs_input);
    // expect correct raw samples to be passed to the processor
    std::vector<float> expectedProcessorCallArgs_param(
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
    std::array<float, numSamplesToPlay> expectedOutputSamples = {
        (100 + 4) * expectedGain,
        (100 + 3) * expectedGain,
        (100 + 2) * expectedGain,
        (100 + 1) * expectedGain,
        (100 + 0) * expectedGain,
        (100 + 4) * expectedGain, // this is where the buffer wrapped around
        (100 + 3) * expectedGain,
        (100 + 2) * expectedGain,
        (100 + 1) * expectedGain,
        (100 + 0) * expectedGain
    };
    EXPECT_EQ(outputBuffer, expectedOutputSamples);
}

TEST_F(DSP_Player, g_playBackwardsHalfSpeed)
{
    const auto expectedGain = 0.6734258f;
    const auto numSamplesToPlay = 2 * numSamples_;

    // the processor mock stores its argument values and
    // returns a modified value for examination later
    std::vector<float> processCallArgs_input;
    std::vector<float> processCallArgs_param;
    ON_CALL(player_.getProcessor(), process(_, _))
        .WillByDefault([&](float input, const MockProcessor::Parameters& params) {
            processCallArgs_input.push_back(input);
            processCallArgs_param.push_back(params.value);
            return input + 100;
        });

    // expect processor to be called once for two output samples.
    // linear interpolation requires samples i and i+1, even if fractional is 0 (and i+1 isn't actually used)
    // that's why an additional process() call is made at the end
    EXPECT_CALL(player_.getProcessor(), process(_, _)).Times(numSamplesToPlay / 2 + 1);

    // play the full buffer
    player_.startPlaying(numSamples_);

    std::array<float, numSamplesToPlay> outputBuffer;
    outputBuffer.fill(0.0f);
    // process in two blocks
    player_.process(0.5f, // speed
                    Direction::backwards,
                    expectedGain,
                    { 0.0f }, // processor params
                    outputBuffer.data(),
                    numSamplesToPlay / 2 + 1,
                    ExponentialSmoother::TimeConstant::instantaneous(), // gain time constant
                    ExponentialSmoother::TimeConstant::instantaneous()); // speed time constant
    player_.process(0.5f, // speed
                    Direction::backwards,
                    expectedGain,
                    { 1.0f }, // processor params (changed this time)
                    outputBuffer.data() + numSamplesToPlay / 2 + 1, // offset to fill second part of the buffer
                    numSamplesToPlay / 2 - 1,
                    ExponentialSmoother::TimeConstant::instantaneous(), // gain time constant
                    ExponentialSmoother::TimeConstant::instantaneous()); // speed time constant

    // expect correct raw samples to be passed to the processor. Since the processor is
    // processed before the resampling, we expect numSamples/2 + 1 calls.
    std::vector<float> expectedProcessorCallArgs_input(
        { 4.0f,
          3.0f,
          2.0f,
          1.0f,
          0.0f, // here the playback buffer must wrap around to the beginning (just 5 samples long!)
          4.0f }); // this is the additonal sample from the linear interpolation
    EXPECT_EQ(processCallArgs_input, expectedProcessorCallArgs_input);
    // expect correct raw samples to be passed to the processor
    std::vector<float> expectedProcessorCallArgs_param(
        { 0.0f,
          0.0f,
          0.0f,
          0.0f, /* start of second block with changed parameters. This sample was already processed during
                   the first block (as required for linear interpolation) hence it has the "old value" */
          1.0f,
          1.0f }); // as before - this is the additonal sample from the linear interpolation
    EXPECT_EQ(processCallArgs_param, expectedProcessorCallArgs_param);

    // expect return values to be what the processor returned, with the gain added
    std::array<float, numSamplesToPlay> expectedOutputSamples = {
        (100 + 4.0f) * expectedGain,
        (100 + 3.5f) * expectedGain, // interpolated between 3.0f and 4.0f due to half speed
        (100 + 3.0f) * expectedGain,
        (100 + 2.5f) * expectedGain, // interpolated
        (100 + 2.0f) * expectedGain,
        (100 + 1.5f) * expectedGain, // interpolated
        (100 + 1.0f) * expectedGain,
        (100 + 0.5f) * expectedGain, // interpolated
        (100 + 0.0f) * expectedGain,
        (100 + 2.0f) * expectedGain // interpolated between 0.0f and 4.0f
    };
    EXPECT_EQ(outputBuffer, expectedOutputSamples);
}
TEST_F(DSP_Player, h_playBackwardsDoubleSpeed)
{
    const auto expectedGain = 0.6734258f;
    const auto numSamplesToPlay = 6;

    // the processor mock stores its argument values and
    // returns a modified value for examination later
    std::vector<float> processCallArgs_input;
    std::vector<float> processCallArgs_param;
    ON_CALL(player_.getProcessor(), process(_, _))
        .WillByDefault([&](float input, const MockProcessor::Parameters& params) {
            processCallArgs_input.push_back(input);
            processCallArgs_param.push_back(params.value);
            return input + 100;
        });

    // expect processor to be called twice for each output sample.
    // linear interpolation requires samples i and i+1, even if fractional is 0 (and i+1 isn't actually used)
    // that's why we also expect two raw samples read for the last output sample
    EXPECT_CALL(player_.getProcessor(), process(_, _)).Times(numSamplesToPlay * 2);

    // play the full buffer
    player_.startPlaying(numSamples_);

    std::array<float, numSamplesToPlay> outputBuffer;
    outputBuffer.fill(0.0f);
    // process in two blocks
    player_.process(2.0f, // speed
                    Direction::backwards,
                    expectedGain,
                    { 0.0f }, // processor params
                    outputBuffer.data(),
                    numSamplesToPlay / 2 + 1,
                    ExponentialSmoother::TimeConstant::instantaneous(), // gain time constant
                    ExponentialSmoother::TimeConstant::instantaneous()); // speed time constant
    player_.process(2.0f, // speed
                    Direction::backwards,
                    expectedGain,
                    { 1.0f }, // processor params (changed this time)
                    outputBuffer.data() + numSamplesToPlay / 2 + 1, // offset to fill second part of the buffer
                    numSamplesToPlay / 2 - 1,
                    ExponentialSmoother::TimeConstant::instantaneous(), // gain time constant
                    ExponentialSmoother::TimeConstant::instantaneous()); // speed time constant

    // expect correct raw samples to be passed to the processor. Since the processor is
    // processed before the resampling, we expect numSamples*2 calls.
    std::vector<float> expectedProcessorCallArgs_input(
        { 4.0f,
          3.0f,
          2.0f,
          1.0f,
          0.0f, // here the playback buffer must wrap around to the beginning (just 5 samples long!)
          4.0f,
          3.0f,
          2.0f,
          1.0f,
          0.0f, // here the playback buffer must wrap around to the beginning (just 5 samples long!)
          4.0f,
          3.0f }); // this is the additonal sample from the linear interpolation
    EXPECT_EQ(processCallArgs_input, expectedProcessorCallArgs_input);
    // expect correct raw samples to be passed to the processor
    std::vector<float> expectedProcessorCallArgs_param(
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
    std::array<float, numSamplesToPlay> expectedOutputSamples = {
        (100 + 4.0f) * expectedGain,
        (100 + 2.0f) * expectedGain,
        (100 + 0.0f) * expectedGain,
        (100 + 3.0f) * expectedGain,
        (100 + 1.0f) * expectedGain,
        (100 + 4.0f) * expectedGain
    };
    EXPECT_EQ(outputBuffer, expectedOutputSamples);
}

TEST_F(DSP_Player, i_applySmoothingToGain)
{
    const auto numSamplesToPlay = 10;

    // the processor mock returns 1.0f so that we can easily see
    // the applied gain.
    ON_CALL(player_.getProcessor(), process(_, _))
        .WillByDefault([&](float, const MockProcessor::Parameters&) {
            return 1.0f;
        });
    // add this expectation to silence gmock warning.
    EXPECT_CALL(player_.getProcessor(), process(_, _)).Times(::testing::AnyNumber());

    // play the full buffer
    player_.startPlaying(numSamples_);

    // the smoothing we'll use
    const auto timeConstant = ExponentialSmoother::TimeConstant::fromRawValue(0.5f);
    const auto targetGain = 1.0f;

    std::array<float, numSamplesToPlay> outputBuffer;
    outputBuffer.fill(0.0f);
    // process block (ramping the gain up)
    player_.process(1.0f, // speed
                    Direction::forwards,
                    targetGain,
                    { 0.0f }, // processor params
                    outputBuffer.data(),
                    numSamplesToPlay / 2,
                    timeConstant, // gain time constant
                    ExponentialSmoother::TimeConstant::instantaneous()); // speed time constant
    // process block (ramping the gain back down)
    player_.process(1.0f, // speed
                    Direction::forwards,
                    0.0f, // target gain
                    { 0.0f }, // processor params
                    outputBuffer.data() + numSamplesToPlay / 2,
                    numSamplesToPlay / 2,
                    timeConstant, // gain time constant
                    ExponentialSmoother::TimeConstant::instantaneous()); // speed time constant

    // we expect the gain to ramp up/down exponentially with the supplied smoothing constant
    std::array<float, numSamplesToPlay> expectedOutputSamples = {
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
    };
    EXPECT_EQ(outputBuffer, expectedOutputSamples);
}

TEST_F(DSP_Player, j_applySmoothingToSpeed)
{
    const auto numSamplesToPlay = 6;

    // the processor mock returns the input value so
    // that we can easily see the read position incrementing
    ON_CALL(player_.getProcessor(), process(_, _))
        .WillByDefault([&](float input, const MockProcessor::Parameters&) {
            return input;
        });
    // add this expectation to silence gmock warning.
    EXPECT_CALL(player_.getProcessor(), process(_, _)).Times(::testing::AnyNumber());

    // play the full buffer
    player_.startPlaying(numSamples_);

    // the smoothing we'll use
    const auto timeConstant = ExponentialSmoother::TimeConstant::fromRawValue(0.5f);
    const auto targetSpeed1 = 0.5f;
    const auto targetSpeed2 = 1.0f;

    std::array<float, numSamplesToPlay> outputBuffer;
    outputBuffer.fill(0.0f);
    // process block (ramping the speed down)
    player_.process(targetSpeed1,
                    Direction::forwards,
                    1.0f, // gain
                    { 0.0f }, // processor params
                    outputBuffer.data(),
                    numSamplesToPlay / 2,
                    ExponentialSmoother::TimeConstant::instantaneous(), // gain time constant
                    timeConstant); // speed time constant
    // process block (ramping the speed back up)
    player_.process(targetSpeed2,
                    Direction::forwards,
                    1.0f, // gain
                    { 0.0f }, // processor params
                    outputBuffer.data() + numSamplesToPlay / 2,
                    numSamplesToPlay / 2,
                    ExponentialSmoother::TimeConstant::instantaneous(), // gain time constant
                    timeConstant); // speed time constant

    // we expect the speed to ramp up/down exponentially with the supplied smoothing constant
    // we examine this from the output samples - since the raw playback buffer contains
    // increasing numbers 0, 1, 2, 3, ... we can easily see the phase increments here.
    std::array<float, numSamplesToPlay> expectedOutputSamples = { 0 };
    float currentSpeed = 0.0f;
    float expectedReadPos = 0.0f;
    for (int i = 0; i < numSamplesToPlay; i++)
    {
        expectedOutputSamples[i] = expectedReadPos;
        const auto targetSpeed = (i >= (numSamplesToPlay / 2)) ? targetSpeed2 : targetSpeed1;
        currentSpeed = (targetSpeed - currentSpeed) * timeConstant.value_ + currentSpeed;
        expectedReadPos += currentSpeed;
    }
    EXPECT_EQ(outputBuffer, expectedOutputSamples);
}

TEST_F(DSP_Player, k_applyNoSmoothingToProcessorParameters)
{
    const auto numSamplesToPlay = 10;

    // the processor mock stores its argument values
    std::vector<float> processCallArgs_param;
    ON_CALL(player_.getProcessor(), process(_, _))
        .WillByDefault([&](float, const MockProcessor::Parameters& params) {
            processCallArgs_param.push_back(params.value);
            return 0.0f; // irrellevant for us
        });
    // add this expectation to silence gmock warning.
    EXPECT_CALL(player_.getProcessor(), process(_, _)).Times(::testing::AnyNumber());

    // play the full buffer
    player_.startPlaying(numSamples_);

    // the smoothing we'll (not) use
    const auto timeConstant = ExponentialSmoother::TimeConstant::fromRawValue(0.5f);
    const auto targetParamValue1 = 1.0f;
    const auto targetParamValue2 = 0.0f;

    std::array<float, numSamplesToPlay> outputBuffer;
    outputBuffer.fill(0.0f);
    // process block with one parameter value
    player_.process(1.0f, // speed
                    Direction::forwards,
                    0.0f, // gain
                    { targetParamValue1 },
                    outputBuffer.data(),
                    numSamplesToPlay / 2,
                    timeConstant, // gain time constant
                    timeConstant); // speed time constant
    // process block with another parameter value
    player_.process(1.0f, // speed
                    Direction::forwards,
                    0.0f, // target gain
                    { targetParamValue2 },
                    outputBuffer.data() + numSamplesToPlay / 2,
                    numSamplesToPlay / 2,
                    timeConstant, // gain time constant
                    timeConstant); // speed time constant

    // we expect the gain to ramp up/down exponentially with the supplied smoothing constant
    std::vector<float> expectedParameterValues = {
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
