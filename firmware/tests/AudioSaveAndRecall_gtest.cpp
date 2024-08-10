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

#include <gtest/gtest.h>

// substitutes for the stuff provided by the daisy library
#define FBIPMAX 0.999985f /**< close to 1.0f-LSB at 16 bit */
#define FBIPMIN (-FBIPMAX) /**< - (1 - LSB) */
#define F2S32_SCALE 2147483647.f /**< (2 ** 31) - 1 */
int32_t f2s32(float x)
{
    x = x <= FBIPMIN ? FBIPMIN : x;
    x = x >= FBIPMAX ? FBIPMAX : x;
    return (int32_t) (x * F2S32_SCALE);
}

#include "AudioSaveAndRecall.h"

class TestFileIo
{
public:
    bool openForReading(const AudioFileName& fileName)
    {
        lastFileName_ = fileName;

        if (simulateFileOpenError_)
            return false;

        return true;
    }

    bool openForWriting(const AudioFileName& fileName)
    {
        lastFileName_ = fileName;

        if (simulateFileOpenError_)
            return false;

        dataWritten_.clear();
        return true;
    }

    bool write(void* data, size_t size)
    {
        if (simulateFileWriteError_)
            return false;

        dataWritten_.insert(dataWritten_.end(),
                            static_cast<std::byte*>(data),
                            static_cast<std::byte*>(data) + size);
        return true;
    }

    void closeFile()
    {
        closeFileCalled_ = true;
    }

    AudioFileName lastFileName_;
    std::vector<std::byte> dataWritten_;
    bool closeFileCalled_ = false;

    bool simulateFileOpenError_ = false;
    bool simulateFileWriteError_ = false;
};

class AudioSaveAndRecallFixture : public ::testing::Test
{
public:
    AudioSaveAndRecallFixture() :
        monoLooper_(monoBuffer_),
        stereoLooper_(stereoBuffer_)
    {
        fillBuffersWithSineWaves();
        monoLooper_.setPlaybackLength(kNumSamples);
        stereoLooper_.setPlaybackLength(kNumSamples);
    }

    static constexpr auto kSampleRate = 48000;
    static constexpr auto kNumSamples = 48000;
    LooperStorage<kNumSamples, 1> monoBuffer_;
    LooperStorage<kNumSamples, 2> stereoBuffer_;
    TapeLooper<kSampleRate, 1> monoLooper_;
    TapeLooper<kSampleRate, 2> stereoLooper_;

    AudioSaveAndRecallResult doneCallbackResult_ =
        AudioSaveAndRecallResult(-1); // invalid

    static void doneCallback(void* contextPtr, AudioSaveAndRecallResult result)
    {
        auto* resultFlag = reinterpret_cast<AudioSaveAndRecallResult*>(contextPtr);
        *resultFlag = result;
    };

private:
    void fillBuffersWithSineWaves()
    {
        const auto freq1 = 480.0f;
        const auto phaseIncrement1 = float(kSampleRate) / freq1;
        const auto freq2 = 640.0f;
        const auto phaseIncrement2 = float(kSampleRate) / freq2;

        auto phase1 = 0.0f;
        auto phase2 = 0.0f;

        for (size_t i = 0; i < kNumSamples; i++)
        {
            stereoBuffer_.data[0][i] = std::sin(phase1 * 2.0f * M_PI);
            stereoBuffer_.data[1][i] = std::sin(phase2 * 2.0f * M_PI);
            monoBuffer_.data[0][i] = stereoBuffer_.data[0][i];

            phase1 += phaseIncrement1;
            if (phase1 >= 1.0f)
                phase1 -= 1.0f;
            phase2 += phaseIncrement2;
            if (phase2 >= 1.0f)
                phase2 -= 1.0f;
        }
    }
};

TEST_F(AudioSaveAndRecallFixture, a_saveToFile_failWhenNoDataInLooper)
{
    AudioSaveAndRecall<TestFileIo> sut;

    monoLooper_.setPlaybackLength(0);

    sut.getFileIoProviderForTesting().simulateFileOpenError_ = true;
    sut.startSavingToFile("dummyFilename",
                          monoLooper_,
                          &doneCallback,
                          &doneCallbackResult_);

    // doesn't even open the file
    EXPECT_STREQ(sut.getFileIoProviderForTesting().lastFileName_, "");
    EXPECT_FALSE(sut.getFileIoProviderForTesting().closeFileCalled_);

    EXPECT_EQ(doneCallbackResult_, AudioSaveAndRecallResult::error);
    EXPECT_EQ(sut.getCurrentProgress(), -1.0f);
}

TEST_F(AudioSaveAndRecallFixture, b_saveToFile_failWhenFileDoesntOpen)
{
    AudioSaveAndRecall<TestFileIo> sut;

    sut.getFileIoProviderForTesting().simulateFileOpenError_ = true;
    sut.startSavingToFile("dummyFilename",
                          monoLooper_,
                          &doneCallback,
                          &doneCallbackResult_);

    EXPECT_STREQ(sut.getFileIoProviderForTesting().lastFileName_, "dummyFilename");
    EXPECT_EQ(doneCallbackResult_, AudioSaveAndRecallResult::error);
    EXPECT_EQ(sut.getCurrentProgress(), -1.0f);
    EXPECT_TRUE(sut.getFileIoProviderForTesting().closeFileCalled_);
}

TEST_F(AudioSaveAndRecallFixture, c_saveToFile_failWhenErrorWhileWriting)
{
    AudioSaveAndRecall<TestFileIo> sut;

    sut.startSavingToFile("dummyFilename",
                          monoLooper_,
                          &doneCallback,
                          &doneCallbackResult_);

    EXPECT_STREQ(sut.getFileIoProviderForTesting().lastFileName_, "dummyFilename");
    EXPECT_EQ(doneCallbackResult_, AudioSaveAndRecallResult(-1)); // not called yet
    EXPECT_EQ(sut.getCurrentProgress(), 0.0f);
    EXPECT_FALSE(sut.getFileIoProviderForTesting().closeFileCalled_);

    // make some progress
    sut.readOrWriteNextChunk();
    EXPECT_GT(sut.getCurrentProgress(), 0.0f);
    EXPECT_LT(sut.getCurrentProgress(), 1.0f);

    // simulate file IO error
    sut.getFileIoProviderForTesting().simulateFileWriteError_ = true;
    sut.readOrWriteNextChunk();

    // operation was aborted
    EXPECT_EQ(doneCallbackResult_, AudioSaveAndRecallResult::error);
    EXPECT_EQ(sut.getCurrentProgress(), -1.0f);
    EXPECT_TRUE(sut.getFileIoProviderForTesting().closeFileCalled_);
}

TEST_F(AudioSaveAndRecallFixture, d_saveToFile_saveFileButWaitUntilRecordingDoneAndPreventNewRecording)
{
    AudioSaveAndRecall<TestFileIo> sut;

    monoLooper_.switchState(LooperState::recording);
    monoLooper_.recorderForTesting().setRecordingProgressForTesting(2000);

    sut.startSavingToFile("dummyFilename",
                          monoLooper_,
                          &doneCallback,
                          &doneCallbackResult_);

    EXPECT_STREQ(sut.getFileIoProviderForTesting().lastFileName_, "dummyFilename");
    EXPECT_EQ(doneCallbackResult_, AudioSaveAndRecallResult(-1)); // not called yet
    EXPECT_EQ(sut.getCurrentProgress(), 0.0f);
    EXPECT_FALSE(sut.getFileIoProviderForTesting().closeFileCalled_);

    // the looper was stopped when saving started...
    EXPECT_EQ(monoLooper_.getState(), LooperState::stopped);
    // the looper should still be recording (crossfading out)
    EXPECT_TRUE(monoLooper_.isRecording());

    // attempt to write
    sut.readOrWriteNextChunk();
    // nothing happened yet
    EXPECT_EQ(sut.getCurrentProgress(), 0.0f);

    // simulate that crossfading is now complete by brutally aborting the recording
    monoLooper_.recorderForTesting().stopRecordingImmediately();
    monoLooper_.setPlaybackLength(kNumSamples);

    // attempt to write again
    sut.readOrWriteNextChunk();
    // now we have some progress
    EXPECT_GT(sut.getCurrentProgress(), 0.0f);
}

TEST_F(AudioSaveAndRecallFixture, e_saveToFile_saveFileAndAllowPlaybackButPreventNewRecording)
{
    AudioSaveAndRecall<TestFileIo> sut;

    sut.startSavingToFile("dummyFilename",
                          monoLooper_,
                          &doneCallback,
                          &doneCallbackResult_);

    EXPECT_STREQ(sut.getFileIoProviderForTesting().lastFileName_, "dummyFilename");
    EXPECT_EQ(doneCallbackResult_, AudioSaveAndRecallResult(-1)); // not called yet
    EXPECT_EQ(sut.getCurrentProgress(), 0.0f);
    EXPECT_FALSE(sut.getFileIoProviderForTesting().closeFileCalled_);

    // attempt to start a new recording while we're saving
    monoLooper_.switchState(LooperState::recording);
    // but new recordings should be prevented
    EXPECT_EQ(monoLooper_.getState(), LooperState::stopped);
    EXPECT_FALSE(monoLooper_.isRecording());

    // but we can still play the loop while we're saving
    monoLooper_.switchState(LooperState::playing);
    EXPECT_EQ(monoLooper_.getState(), LooperState::playing);

    // complete the saving operation
    int timeout = 1000;
    while (timeout-- > 0 && doneCallbackResult_ != AudioSaveAndRecallResult::ok)
    {
        sut.readOrWriteNextChunk();
    }
    EXPECT_EQ(doneCallbackResult_, AudioSaveAndRecallResult::ok);

    // attempt a recording again
    monoLooper_.switchState(LooperState::recording);
    // and new recordings should be allowed again
    EXPECT_EQ(monoLooper_.getState(), LooperState::recording);
    EXPECT_TRUE(monoLooper_.isRecording());
}

TEST_F(AudioSaveAndRecallFixture, f_saveToFile_saveFileAndReportProgress)
{
    AudioSaveAndRecall<TestFileIo> sut;

    sut.startSavingToFile("dummyFilename",
                          monoLooper_,
                          &doneCallback,
                          &doneCallbackResult_);

    EXPECT_STREQ(sut.getFileIoProviderForTesting().lastFileName_, "dummyFilename");
    EXPECT_EQ(doneCallbackResult_, AudioSaveAndRecallResult(-1)); // not called yet
    EXPECT_EQ(sut.getCurrentProgress(), 0.0f);
    EXPECT_FALSE(sut.getFileIoProviderForTesting().closeFileCalled_);

    // write until completed
    int timeout = 1000;
    auto lastProgress = 0.0f;
    while (timeout-- > 0 && doneCallbackResult_ != AudioSaveAndRecallResult::ok)
    {
        sut.readOrWriteNextChunk();
        EXPECT_GT(sut.getCurrentProgress(), lastProgress);
        lastProgress = sut.getCurrentProgress();
    }
    EXPECT_EQ(sut.getCurrentProgress(), 1.0f);
    EXPECT_EQ(doneCallbackResult_, AudioSaveAndRecallResult::ok);
    EXPECT_TRUE(sut.getFileIoProviderForTesting().closeFileCalled_);
}
