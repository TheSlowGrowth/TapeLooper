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
#include <dsp/TapeLooper.h>

class TapeLooperFixture : public ::testing::Test
{
public:
    TapeLooperFixture() :
        looper_(storage_) {}

    void processOneBuffer()
    {
        TapeProcessorParameters processorParameters;
        processorParameters.driveGain = 1.0f;
        processorParameters.grainAmt = 0.0f;
        AudioBuffer<1, kNumSamples> inBuffer;
        AudioBuffer<1, kNumSamples> outBuffer;
        looper_.process(1.0f, 0.0f, Direction::forwards, processorParameters, 1.0f, inBuffer, outBuffer);
    }

    void recordSomeAudio()
    {
        looper_.switchState(LooperState::recording);
        processOneBuffer();
        looper_.switchState(LooperState::stopped);
        processOneBuffer();
        EXPECT_FALSE(looper_.isRecording());
    }

    static constexpr auto kSampleRate = 100;
    static constexpr auto kNumSamples = 100;

    LooperStorage<kNumSamples, 1> storage_;
    TapeLooper<kSampleRate, 1> looper_;
};

TEST_F(TapeLooperFixture, a_preventRecordingStopsCurrentRecording)
{
    looper_.switchState(LooperState::recording);
    EXPECT_EQ(looper_.getState(), LooperState::recording);

    looper_.preventRecording();
    // looper aborts the recording
    EXPECT_EQ(looper_.getState(), LooperState::stopped);
    // recorder is still crossfading out
    EXPECT_TRUE(looper_.isRecording());

    // after processing for a while
    processOneBuffer();
    // recorder has stopped
    EXPECT_FALSE(looper_.isRecording());
}

TEST_F(TapeLooperFixture, b_preventRecordingImmediatelyStopsCurrentRecordingWithoutCrossfade)
{
    looper_.switchState(LooperState::recording);
    EXPECT_EQ(looper_.getState(), LooperState::recording);

    looper_.preventRecording(true);
    // looper aborts the recording
    EXPECT_EQ(looper_.getState(), LooperState::stopped);
    // recorder has stopped immediately
    EXPECT_FALSE(looper_.isRecording());
}

TEST_F(TapeLooperFixture, c_preventRecordingPreventsStartingARecording)
{
    looper_.switchState(LooperState::stopped);
    EXPECT_EQ(looper_.getState(), LooperState::stopped);

    looper_.preventRecording();
    looper_.switchState(LooperState::recording);
    EXPECT_EQ(looper_.getState(), LooperState::stopped);

    looper_.liftRestrictions();
    looper_.switchState(LooperState::recording);
    EXPECT_EQ(looper_.getState(), LooperState::recording);
}

TEST_F(TapeLooperFixture, d_preventRecordingStillAllowsPlayback)
{
    recordSomeAudio();

    looper_.switchState(LooperState::stopped);
    EXPECT_EQ(looper_.getState(), LooperState::stopped);

    looper_.preventRecording();
    looper_.switchState(LooperState::playing);
    EXPECT_EQ(looper_.getState(), LooperState::playing);
}

TEST_F(TapeLooperFixture, e_preventPlaybackAndRecordingStopsPlayback)
{
    recordSomeAudio();

    looper_.switchState(LooperState::playing);
    EXPECT_EQ(looper_.getState(), LooperState::playing);

    looper_.preventPlaybackAndRecording();
    // looper aborts the playback
    EXPECT_EQ(looper_.getState(), LooperState::stopped);
}

TEST_F(TapeLooperFixture, f_preventPlaybackAndRecordingStopsCurrentRecording)
{
    looper_.switchState(LooperState::recording);
    EXPECT_EQ(looper_.getState(), LooperState::recording);

    looper_.preventPlaybackAndRecording();
    // looper aborts the recording
    EXPECT_EQ(looper_.getState(), LooperState::stopped);
    // recorder is still crossfading out
    EXPECT_TRUE(looper_.isRecording());

    // after processing for a while
    processOneBuffer();
    // recorder has stopped
    EXPECT_FALSE(looper_.isRecording());
}

TEST_F(TapeLooperFixture, g_preventPlaybackAndRecordingImmediatelyStopsCurrentRecordingWithoutCrossfade)
{
    looper_.switchState(LooperState::recording);
    EXPECT_EQ(looper_.getState(), LooperState::recording);

    looper_.preventPlaybackAndRecording(true);
    // looper aborts the recording
    EXPECT_EQ(looper_.getState(), LooperState::stopped);
    // recorder has stopped
    EXPECT_FALSE(looper_.isRecording());
}

TEST_F(TapeLooperFixture, h_preventPlaybackAndRecordingPreventsStartingARecording)
{
    looper_.switchState(LooperState::stopped);
    EXPECT_EQ(looper_.getState(), LooperState::stopped);

    looper_.preventPlaybackAndRecording();
    looper_.switchState(LooperState::recording);
    EXPECT_EQ(looper_.getState(), LooperState::stopped);

    looper_.liftRestrictions();
    looper_.switchState(LooperState::recording);
    EXPECT_EQ(looper_.getState(), LooperState::recording);
}

TEST_F(TapeLooperFixture, i_preventPlaybackAndRecordingPreventsStartingPlayback)
{
    recordSomeAudio();

    looper_.switchState(LooperState::stopped);
    EXPECT_EQ(looper_.getState(), LooperState::stopped);

    looper_.preventPlaybackAndRecording();
    looper_.switchState(LooperState::playing);
    EXPECT_EQ(looper_.getState(), LooperState::stopped);

    looper_.liftRestrictions();
    looper_.switchState(LooperState::playing);
    EXPECT_EQ(looper_.getState(), LooperState::playing);
}