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

#include "LooperController.h"
#include "../libDaisy/tests/TestIsolator.h"

using ::testing::_;

class StereoLooperMock;
class MonoLooperMock;
struct MockInstanceRegistry
{
    std::vector<StereoLooperMock*> stereoLooperMockInstances;
    std::vector<MonoLooperMock*> monoLooperMockInstances;
};
TestIsolator<MockInstanceRegistry> mockInstances;

struct ProcessorParametersMock
{
    float param1 = 0.0f;
    float param2 = 0.0f;
    bool operator==(const ProcessorParametersMock& other) const
    {
        return param1 == other.param1 && param2 == other.param2;
    }
};

class LooperMock
{
public:
    // MOCK_METHOD(ReturnType, MethodName, (Args...), (Specs...));
    MOCK_METHOD(void, switchState, (LooperState state), ());
    MOCK_METHOD(LooperState, getState, (), (const));
};

class StereoLooperMock : public LooperMock
{
public:
    StereoLooperMock(LooperStoragePtr<2> storageToUse) :
        storage_(storageToUse)
    {
        auto& registry = mockInstances.GetStateForCurrentTest()->stereoLooperMockInstances;
        registry.push_back(this);
    }
    ~StereoLooperMock()
    {
        auto& registry = mockInstances.GetStateForCurrentTest()->stereoLooperMockInstances;
        const auto foundInRegistry = std::find(registry.begin(), registry.end(), this);
        if (foundInRegistry != registry.end())
            registry.erase(foundInRegistry);
    }

    // clang-format off
    MOCK_METHOD(void, process, (
        float speedParam, 
        float wowAndFlutterParam, 
        Direction direction, 
        ProcessorParametersMock processorParams, 
        float gainParam, 
        (AudioBufferPtr<2, const float>) input, 
        AudioBufferPtr<2> output, 
        ExponentialSmoother::TimeConstant postGainSmootherTimeConstant, 
        ExponentialSmoother::TimeConstant speedSmootherTimeConstant), ());
    // clang-format on

    const LooperStoragePtr<2> storage_;
};
class MonoLooperMock : public LooperMock
{
public:
    MonoLooperMock(LooperStoragePtr<1> storageToUse) :
        storage_(storageToUse)
    {
        auto& registry = mockInstances.GetStateForCurrentTest()->monoLooperMockInstances;
        registry.push_back(this);
    }
    ~MonoLooperMock()
    {
        auto& registry = mockInstances.GetStateForCurrentTest()->monoLooperMockInstances;
        const auto regEntry = std::find(registry.begin(), registry.end(), this);
        if (regEntry != registry.end())
            registry.erase(regEntry);
    }

    // clang-format off
    MOCK_METHOD(void, process, (
        float speedParam, 
        float wowAndFlutterParam, 
        Direction direction, 
        ProcessorParametersMock processorParams, 
        float gainParam, 
        (AudioBufferPtr<1, const float>) input, 
        AudioBufferPtr<1> output, 
        ExponentialSmoother::TimeConstant postGainSmootherTimeConstant, 
        ExponentialSmoother::TimeConstant speedSmootherTimeConstant), ());
    // clang-format on

    const LooperStoragePtr<1> storage_;
};

class ParameterProviderMock
{
public:
    // MOCK_METHOD(ReturnType, MethodName, (Args...), (Specs...));
    MOCK_METHOD(float, getSpeedParameter, (size_t looperChannel), (const));
    MOCK_METHOD(float, getWowAndFlutterAmtParameter, (size_t looperChannel), (const));
    MOCK_METHOD(float, getGainParameter, (size_t looperChannel), (const));
    MOCK_METHOD(ProcessorParametersMock, getProcessorParameters, (size_t looperChannel), (const));
};

struct LooperMockTypes
{
    using MonoLooperType = ::testing::NiceMock<MonoLooperMock>;
    using StereoLooperType = ::testing::NiceMock<StereoLooperMock>;
    using ParameterProvider = ::testing::NiceMock<ParameterProviderMock>;
};

class LooperController_Test : public ::testing::Test
{
protected:
    LooperController_Test() :
        controller_(storage_,
                    monoDownmixBuffer_,
                    temporaryBuffer_,
                    parameterProvider_)
    {
    }

    ~LooperController_Test()
    {
        mockInstances.CleanupCurrentTestState();
    }

    void reconfigureAllLoopersAs(ChannelLayout layout)
    {
        for (size_t i = 0; i < numLoopers_; i++)
            controller_.setChannelLayout(i, layout);
    }

    static constexpr size_t numLoopers_ = 2;
    static constexpr size_t numSamples_ = 10;
    static constexpr size_t numChannels_ = 2;
    std::array<MonoOrStereoLooperStoragePtr, numLoopers_> storage_ = {
        MonoOrStereoLooperStorage<100>(),
        MonoOrStereoLooperStorage<100>(),
    };
    AudioBuffer<1, numSamples_> monoDownmixBuffer_;
    AudioBuffer<1, numSamples_> temporaryBuffer_;
    ::testing::NiceMock<ParameterProviderMock> parameterProvider_;
    LooperController<LooperMockTypes, numLoopers_> controller_;
    MockInstanceRegistry& looperInstanceRegistry_ = *mockInstances.GetStateForCurrentTest();
};

TEST_F(LooperController_Test, ctor_shouldInitLoopersToStereo)
{
    // expect: stereo looper instances were created
    EXPECT_EQ(looperInstanceRegistry_.monoLooperMockInstances.size(), 0ul);
    EXPECT_EQ(looperInstanceRegistry_.stereoLooperMockInstances.size(), numLoopers_);
    // expect: the correct storages were passed during the creation
    for (size_t i = 0; i < looperInstanceRegistry_.stereoLooperMockInstances.size(); i++)
        EXPECT_EQ(looperInstanceRegistry_.stereoLooperMockInstances[i]->storage_.data[0],
                  storage_[i].LooperStoragePtr<2>::data[0]);
}

TEST_F(LooperController_Test, setLooperState_shouldCallLooperWithCorrectSetting)
{
    // configure first looper to be mono; so that we have both
    // a stereo and a mono looper in the test
    controller_.setChannelLayout(0, ChannelLayout::mono);

    auto& mock0 = *looperInstanceRegistry_.monoLooperMockInstances[0];
    auto& mock1 = *looperInstanceRegistry_.stereoLooperMockInstances[0];

    // This variable ensures the sequence order
    ::testing::InSequence s;

    EXPECT_CALL(mock0, switchState(LooperState::recording)).Times(1);
    EXPECT_CALL(mock1, switchState(LooperState::playing)).Times(1);
    EXPECT_CALL(mock0, switchState(LooperState::stopped)).Times(1);

    controller_.setLooperState(0, LooperState::recording);
    controller_.setLooperState(1, LooperState::playing);
    controller_.setLooperState(0, LooperState::stopped);
}

TEST_F(LooperController_Test, getLooperState_shouldGetResultFromCorrectLooper)
{
    // configure first looper to be mono; so that we have both
    // a stereo and a mono looper in the test
    controller_.setChannelLayout(0, ChannelLayout::mono);

    auto& mock0 = *looperInstanceRegistry_.monoLooperMockInstances[0];
    auto& mock1 = *looperInstanceRegistry_.stereoLooperMockInstances[0];

    // This variable ensures the sequence order
    ::testing::InSequence s;

    EXPECT_CALL(mock0, getState()).WillOnce(::testing::Return(LooperState::playing));
    EXPECT_CALL(mock1, getState()).WillOnce(::testing::Return(LooperState::stopped));

    EXPECT_EQ(controller_.getLooperState(0), LooperState::playing);
    EXPECT_EQ(controller_.getLooperState(1), LooperState::stopped);
}

TEST_F(LooperController_Test, setChannelLayout_shouldReconfigureToMonoOrStereo)
{
    // initial state: both channels are stereo
    EXPECT_EQ(controller_.getChannelLayout(0), ChannelLayout::stereo);
    EXPECT_EQ(controller_.getChannelLayout(1), ChannelLayout::stereo);

    controller_.setChannelLayout(0, ChannelLayout::mono);
    EXPECT_EQ(controller_.getChannelLayout(0), ChannelLayout::mono);
    // expect: one of the two loopers was replaced with a mono looper
    EXPECT_EQ(looperInstanceRegistry_.monoLooperMockInstances.size(), 1ul);
    EXPECT_EQ(looperInstanceRegistry_.stereoLooperMockInstances.size(), 1ul);

    controller_.setChannelLayout(1, ChannelLayout::mono);
    EXPECT_EQ(controller_.getChannelLayout(1), ChannelLayout::mono);
    // expect: both loopers were replaced with a mono looper
    EXPECT_EQ(looperInstanceRegistry_.monoLooperMockInstances.size(), 2ul);
    EXPECT_EQ(looperInstanceRegistry_.stereoLooperMockInstances.size(), 0ul);

    controller_.setChannelLayout(0, ChannelLayout::stereo);
    EXPECT_EQ(controller_.getChannelLayout(0), ChannelLayout::stereo);
    // expect: one of the two loopers was replaced with a stereo looper again
    EXPECT_EQ(looperInstanceRegistry_.monoLooperMockInstances.size(), 1ul);
    EXPECT_EQ(looperInstanceRegistry_.stereoLooperMockInstances.size(), 1ul);
}

TEST_F(LooperController_Test, process_shouldPassthroughParameters)
{
    controller_.setChannelLayout(0, ChannelLayout::mono);
    controller_.setChannelLayout(1, ChannelLayout::stereo);

    // set parameters (we ignore the motor acceleration)
    EXPECT_CALL(parameterProvider_, getSpeedParameter(0)).WillOnce(::testing::Return(0.01f));
    EXPECT_CALL(parameterProvider_, getSpeedParameter(1)).WillOnce(::testing::Return(0.11f));
    EXPECT_CALL(parameterProvider_, getWowAndFlutterAmtParameter(0)).WillOnce(::testing::Return(0.02f));
    EXPECT_CALL(parameterProvider_, getWowAndFlutterAmtParameter(1)).WillOnce(::testing::Return(0.12f));
    EXPECT_CALL(parameterProvider_, getGainParameter(0)).WillOnce(::testing::Return(0.03f));
    EXPECT_CALL(parameterProvider_, getGainParameter(1)).WillOnce(::testing::Return(0.13f));
    EXPECT_CALL(parameterProvider_, getProcessorParameters(0)).WillOnce(::testing::Return<ProcessorParametersMock>({ 0.04f, 0.05f }));
    EXPECT_CALL(parameterProvider_, getProcessorParameters(1)).WillOnce(::testing::Return<ProcessorParametersMock>({ 0.14f, 0.15f }));
    controller_.setDirection(0, Direction::backwards);
    controller_.setDirection(1, Direction::forwards);

    auto& mock0 = *looperInstanceRegistry_.monoLooperMockInstances[0];
    auto& mock1 = *looperInstanceRegistry_.stereoLooperMockInstances[0];

    // expect process() to be called with the correct parameters
    EXPECT_CALL(mock0, process(0.01f, 0.02f, Direction::backwards, ProcessorParametersMock { 0.04f, 0.05f }, 0.03f, _, _, _, _));
    EXPECT_CALL(mock1, process(0.11f, 0.12f, Direction::forwards, ProcessorParametersMock { 0.14f, 0.15f }, 0.13f, _, _, _, _));

    AudioBuffer<2, 10> input;
    AudioBuffer<2, 10> output;
    controller_.process(input, output);
}

TEST_F(LooperController_Test, process_shouldUseCorrectMotorSpeedTimeConstants)
{
    controller_.setChannelLayout(0, ChannelLayout::mono);
    controller_.setChannelLayout(1, ChannelLayout::stereo);

    auto& mock0 = *looperInstanceRegistry_.monoLooperMockInstances[0];
    auto& mock1 = *looperInstanceRegistry_.stereoLooperMockInstances[0];

    // setup the looper mocks to capture the smoothing time using lambda functions
    float monoSpeedTimeConstant = 0.0f;
    float stereoSpeedTimeConstant = 0.0f;
    const auto captureFuncArgumentsMono =
        [&](float,
            float,
            Direction,
            const ProcessorParametersMock&,
            float,
            AudioBufferPtr<1, const float>,
            AudioBufferPtr<1, float>,
            ExponentialSmoother::TimeConstant,
            ExponentialSmoother::TimeConstant speedSmootherTimeConstant)
    {
        monoSpeedTimeConstant = speedSmootherTimeConstant.value_;
    };
    const auto captureFuncArgumentsStereo =
        [&](float,
            float,
            Direction,
            const ProcessorParametersMock&,
            float,
            AudioBufferPtr<2, const float>,
            AudioBufferPtr<2, float>,
            ExponentialSmoother::TimeConstant,
            ExponentialSmoother::TimeConstant speedSmootherTimeConstant)
    {
        stereoSpeedTimeConstant = speedSmootherTimeConstant.value_;
    };
    EXPECT_CALL(mock0, process(_, _, _, _, _, _, _, _, _)).WillRepeatedly(testing::Invoke(captureFuncArgumentsMono));
    EXPECT_CALL(mock1, process(_, _, _, _, _, _, _, _, _)).WillRepeatedly(testing::Invoke(captureFuncArgumentsStereo));

    // a lambda to capture the smoother time constants for a given setting
    const auto captureTimeConstant = [&](MotorAcceleration acceleration)
    {
        controller_.setMotorAcceleration(0, acceleration);
        controller_.setMotorAcceleration(1, acceleration);

        // process (mocks will capture the smoothing time)
        AudioBuffer<2, 10> input;
        AudioBuffer<2, 10> output;
        controller_.process(input, output);

        // both loopers should have seen the same smoothing time
        EXPECT_EQ(monoSpeedTimeConstant, stereoSpeedTimeConstant);
        return monoSpeedTimeConstant;
    };

    // a lambda to transform captured time constants to smoothing times
    const auto getSmoothingTimeFromTimeConstant = [](float timeConstant)
    {
        // those two come from the constants.h file
        constexpr float expectedBlockSize = float(blockSize);
        constexpr float expectedSampleRate = sampleRateHz;
        // according to Udo Zoelzer:
        //   timeConstant = 1.0f - sprout::math::exp(-2.2f * float(expectedBlockSize) / expectedSampleRate / smoothingTimeInS);
        // hence:
        //   ln(1.0f - timeConstant) =  -2.2f * float(expectedBlockSize) / expectedSampleRate / smoothingTimeInS
        //   smoothingTime = -2.2f * float(expectedBlockSize) / expectedSampleRate * log(e) / log(1.0f - timeConstant)
        return -2.2f * float(expectedBlockSize) / expectedSampleRate
               * sprout::math::log(2.718281828f) / sprout::math::log(1.0f - timeConstant);
    };

    // finally capture the time constants for each acceleration setting
    const auto accelerations = { MotorAcceleration::instantaneous,
                                 MotorAcceleration::veryFast,
                                 MotorAcceleration::fast,
                                 MotorAcceleration::medium,
                                 MotorAcceleration::slow,
                                 MotorAcceleration::verySlow };
    std::map<MotorAcceleration, float> measuredSmoothingTimes;
    for (const auto acceleration : accelerations)
        measuredSmoothingTimes[acceleration] = getSmoothingTimeFromTimeConstant(captureTimeConstant(acceleration));

    const float toleranceAmt = 0.01f; // 1% tolerance
    // instantaneous setting should yield no smoothing at all
    EXPECT_FLOAT_EQ(measuredSmoothingTimes[MotorAcceleration::instantaneous], 0.0f);
    // expect minimum and maximum values
    EXPECT_NEAR(measuredSmoothingTimes[MotorAcceleration::veryFast], 0.01f, 0.01f * toleranceAmt);
    EXPECT_NEAR(measuredSmoothingTimes[MotorAcceleration::verySlow], 5.0f, 5.0f * toleranceAmt);
    // expect the other times to be scaled exponentially in between (== constant factor)
    constexpr float totalRange = 5.0f / 0.01f;
    constexpr size_t numSettings = 5;
    constexpr float expectedFactor = sprout::pow(totalRange, 1.0f / float(numSettings - 1));
    EXPECT_NEAR(measuredSmoothingTimes[MotorAcceleration::fast]
                    / measuredSmoothingTimes[MotorAcceleration::veryFast],
                expectedFactor,
                expectedFactor * toleranceAmt);
    EXPECT_NEAR(measuredSmoothingTimes[MotorAcceleration::medium]
                    / measuredSmoothingTimes[MotorAcceleration::fast],
                expectedFactor,
                expectedFactor * toleranceAmt);
    EXPECT_NEAR(measuredSmoothingTimes[MotorAcceleration::slow]
                    / measuredSmoothingTimes[MotorAcceleration::medium],
                expectedFactor,
                expectedFactor * toleranceAmt);
    EXPECT_NEAR(measuredSmoothingTimes[MotorAcceleration::verySlow]
                    / measuredSmoothingTimes[MotorAcceleration::slow],
                expectedFactor,
                expectedFactor * toleranceAmt);
}

TEST_F(LooperController_Test, process_shouldProvideCorrectSamples)
{
    controller_.setChannelLayout(0, ChannelLayout::mono);
    controller_.setChannelLayout(1, ChannelLayout::stereo);

    auto& mock0 = *looperInstanceRegistry_.monoLooperMockInstances[0];
    auto& mock1 = *looperInstanceRegistry_.stereoLooperMockInstances[0];

    // setup the looper mocks to capture the incoming audio samples
    std::vector<float> monoSamples;
    std::vector<float> stereoSamplesLeft, stereoSamplesRight;
    const auto captureFuncArgumentsMono =
        [&](float,
            float,
            Direction,
            const ProcessorParametersMock&,
            float,
            AudioBufferPtr<1, const float> input,
            AudioBufferPtr<1, float>,
            ExponentialSmoother::TimeConstant,
            ExponentialSmoother::TimeConstant)
    {
        for (size_t i = 0; i < input.size_; i++)
            monoSamples.push_back(input.getChannelPointers()[0][i]);
    };
    const auto captureFuncArgumentsStereo =
        [&](float,
            float,
            Direction,
            const ProcessorParametersMock&,
            float,
            AudioBufferPtr<2, const float> input,
            AudioBufferPtr<2, float>,
            ExponentialSmoother::TimeConstant,
            ExponentialSmoother::TimeConstant)
    {
        for (size_t i = 0; i < input.size_; i++)
        {
            stereoSamplesLeft.push_back(input.getChannelPointers()[0][i]);
            stereoSamplesRight.push_back(input.getChannelPointers()[1][i]);
        }
    };
    EXPECT_CALL(mock0, process(_, _, _, _, _, _, _, _, _)).WillRepeatedly(testing::Invoke(captureFuncArgumentsMono));
    EXPECT_CALL(mock1, process(_, _, _, _, _, _, _, _, _)).WillRepeatedly(testing::Invoke(captureFuncArgumentsStereo));

    // prepare some dummy samples
    AudioBuffer<2, 9> inputSamples;
    for (size_t i = 0; i < inputSamples.size_; i++)
    {
        inputSamples[0][i] = float(i); // left channel: 0, 1, 2, 3, ...
        inputSamples[1][i] = float(10 + i); // right channel: 10, 11, 12, 13, ...
    }

    // process audio
    AudioBuffer<2, 9> outputSamples;
    controller_.process(inputSamples, outputSamples);

    // stereo samples are passed through
    EXPECT_EQ(stereoSamplesLeft, std::vector<float>(inputSamples[0], inputSamples[0] + inputSamples.size_));
    EXPECT_EQ(stereoSamplesRight, std::vector<float>(inputSamples[1], inputSamples[1] + inputSamples.size_));
    // mono looper gets a downmix
    AudioBuffer<1, 9> downmixedSamples;
    for (size_t i = 0; i < inputSamples.size_; i++)
        downmixedSamples[0][i] = (inputSamples[0][i] + inputSamples[1][i]) / 2.0f;
    EXPECT_EQ(monoSamples, std::vector<float>(downmixedSamples[0], downmixedSamples[0] + downmixedSamples.size_));
}

TEST_F(LooperController_Test, process_shouldMixTogetherOutputSamples)
{
    controller_.setChannelLayout(0, ChannelLayout::mono);
    controller_.setChannelLayout(1, ChannelLayout::stereo);

    auto& mock0 = *looperInstanceRegistry_.monoLooperMockInstances[0];
    auto& mock1 = *looperInstanceRegistry_.stereoLooperMockInstances[0];

    // generate some dummy output signals for our mocked loopers
    std::array<float, 9> monoSamples;
    std::array<float, 9> stereoSamplesLeft, stereoSamplesRight;
    for (size_t i = 0; i < monoSamples.size(); i++)
    {
        monoSamples[i] = float(i);
        stereoSamplesLeft[i] = float(10 + i);
        stereoSamplesRight[i] = float(20 + i);
    }

    // setup the looper mocks to play back the dummy signals
    const auto playDummySignalMono =
        [&](float,
            float,
            Direction,
            const ProcessorParametersMock&,
            float,
            AudioBufferPtr<1, const float>,
            AudioBufferPtr<1, float> output,
            ExponentialSmoother::TimeConstant,
            ExponentialSmoother::TimeConstant)
    {
        // the signals are added to the output channel! Not overwritten!
        output.addToChannel(0, monoSamples.data(), std::min(monoSamples.size(), output.size_));
    };
    const auto playDummySignalStereo =
        [&](float,
            float,
            Direction,
            const ProcessorParametersMock&,
            float,
            AudioBufferPtr<2, const float>,
            AudioBufferPtr<2, float> output,
            ExponentialSmoother::TimeConstant,
            ExponentialSmoother::TimeConstant)
    {
        // the signals are added to the output channel! Not overwritten!
        output.addToChannel(0, stereoSamplesLeft.data(), std::min(stereoSamplesLeft.size(), output.size_));
        output.addToChannel(1, stereoSamplesRight.data(), std::min(stereoSamplesRight.size(), output.size_));
    };
    EXPECT_CALL(mock0, process(_, _, _, _, _, _, _, _, _)).WillRepeatedly(testing::Invoke(playDummySignalMono));
    EXPECT_CALL(mock1, process(_, _, _, _, _, _, _, _, _)).WillRepeatedly(testing::Invoke(playDummySignalStereo));

    // process audio
    AudioBuffer<2, 9> inputSamples;
    AudioBuffer<2, 9> outputSamples;
    outputSamples.fill(100.0f); // fill with crap - this should be overwritten
    controller_.process(inputSamples, outputSamples);

    AudioBuffer<2, 9> expectedOutputSamples;
    for (size_t i = 0; i < expectedOutputSamples.size_; i++)
    {
        expectedOutputSamples[0][i] = stereoSamplesLeft[i] + monoSamples[i]; // left channel
        expectedOutputSamples[1][i] = stereoSamplesRight[i] + monoSamples[i]; // right channel
    }

    // expect L & R signals to contain the upmixed mono looper signal + stereo looper signal.
    // we use std::vector for the comparison because of the pretty printing in the test log
    EXPECT_EQ(std::vector<float>(outputSamples[0], outputSamples[0] + outputSamples.size_),
              std::vector<float>(expectedOutputSamples[0], expectedOutputSamples[0] + expectedOutputSamples.size_));
    EXPECT_EQ(std::vector<float>(outputSamples[1], outputSamples[1] + outputSamples.size_),
              std::vector<float>(expectedOutputSamples[1], expectedOutputSamples[1] + expectedOutputSamples.size_));
}