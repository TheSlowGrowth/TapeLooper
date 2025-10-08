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
#include <gmock/gmock-matchers.h>
#include <math.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

// uncomment to update the test references
// #define WRITE_REFERENCES

#include "libDaisyCombined.h"
#include "AudioSaveAndRecall.h"
namespace
{
    static constexpr auto kSampleRate = 48000;

    std::vector<float> makeSawWave(double frequency, size_t numSamples)
    {
        const float kIncrement = float(2.0f * frequency / double(kSampleRate));
        float value = 0.0f;

        std::vector<float> result(numSamples);
        for (auto& sample : result)
        {
            sample = value;
            value += kIncrement;
            if (value >= 0.9999f)
            {
                value = -1.0f;
            }
        }

        return result;
    }

    std::vector<std::byte> readReferenceFile(const char* filename)
    {
        std::ifstream referenceFile(filename, std::ios::binary);
        EXPECT_TRUE(referenceFile.is_open());

        std::vector<std::byte> contents;

        char b;
        while (referenceFile.read(&b, sizeof(b)))
        {
            contents.push_back(std::byte(b));
        }
        referenceFile.close();

        return contents;
    }

#ifdef WRITE_REFERENCES
    void writeReferenceFile(const char* filename, const std::vector<std::byte>& data)
    {
        std::ofstream referenceFile(filename, std::ios::binary);
        EXPECT_TRUE(referenceFile.is_open());

        for (std::byte byte : data)
        {
            referenceFile << std::ofstream::char_type(byte);
        }
        referenceFile.close();
    }
#endif
} // namespace

class TestFileIo
{
public:
    bool openForReading(const AudioFileName& fileName)
    {
        lastFileName_ = fileName;

        if (simulateFileOpenError_)
            return false;

        readIndex_ = 0;
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

    int32_t read(void* data, size_t maxSize)
    {
        if (simulateFileOpenError_)
        {
            return -1;
        }

        uint8_t* outPtr = reinterpret_cast<uint8_t*>(data);

        const auto numRead = std::min(maxSize, dataToRead_.size() - readIndex_);
        for (size_t i = 0; i < numRead; i++)
        {
            *outPtr = uint8_t(dataToRead_[readIndex_ + i]);
            outPtr++;
        }

        readIndex_ += numRead;
        return numRead;
    }

    void closeFile()
    {
        closeFileCalled_ = true;
    }

    AudioFileName lastFileName_;
    std::vector<std::byte> dataWritten_;
    std::vector<std::byte> dataToRead_;
    size_t readIndex_ = 0;
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
        fillBuffersWithSilence();
        monoLooper_.setPlaybackLength(kNumSamples);
        stereoLooper_.setPlaybackLength(kNumSamples);
    }

    static constexpr auto kNumSamples = 48000;
    LooperStorage<kNumSamples, 1> monoBuffer_;
    LooperStorage<kNumSamples, 2> stereoBuffer_;
    TapeLooper<kSampleRate, 1> monoLooper_;
    TapeLooper<kSampleRate, 2> stereoLooper_;
    TestFileIo fileIo_;

    AudioSaveAndRecallResult doneCallbackResult_ =
        AudioSaveAndRecallResult(-1); // invalid

    static void doneCallback(void* contextPtr, AudioSaveAndRecallResult result)
    {
        auto* resultFlag = reinterpret_cast<AudioSaveAndRecallResult*>(contextPtr);
        *resultFlag = result;
    };

    void fillBuffersWithSilence()
    {
        for (size_t i = 0; i < kNumSamples; i++)
        {
            stereoBuffer_.data[0][i] = 0.0f;
            stereoBuffer_.data[1][i] = 0.0f;
            monoBuffer_.data[0][i] = 0.0f;
        }
    }

    void fillBuffersWithSineWaves()
    {
        const auto freq1 = 480.0f;
        const auto phaseIncrement1 = freq1 / float(kSampleRate);
        const auto freq2 = 640.0f;
        const auto phaseIncrement2 = freq2 / float(kSampleRate);

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
    AudioSaveAndRecall<TestFileIo> sut(fileIo_);

    monoLooper_.setPlaybackLength(0);

    fileIo_.simulateFileOpenError_ = true;
    sut.startSavingToFile("dummyFilename",
                          monoLooper_,
                          &doneCallback,
                          &doneCallbackResult_);

    // doesn't even open the file
    EXPECT_STREQ(fileIo_.lastFileName_, "");
    EXPECT_FALSE(fileIo_.closeFileCalled_);

    EXPECT_EQ(doneCallbackResult_, AudioSaveAndRecallResult::error);
    EXPECT_EQ(sut.getCurrentProgress(), -1.0f);
}

TEST_F(AudioSaveAndRecallFixture, b_saveToFile_failWhenFileDoesntOpen)
{
    AudioSaveAndRecall<TestFileIo> sut(fileIo_);

    fileIo_.simulateFileOpenError_ = true;
    sut.startSavingToFile("dummyFilename",
                          monoLooper_,
                          &doneCallback,
                          &doneCallbackResult_);

    EXPECT_STREQ(fileIo_.lastFileName_, "dummyFilename");
    EXPECT_EQ(doneCallbackResult_, AudioSaveAndRecallResult::error);
    EXPECT_EQ(sut.getCurrentProgress(), -1.0f);
    EXPECT_TRUE(fileIo_.closeFileCalled_);
}

TEST_F(AudioSaveAndRecallFixture, c_saveToFile_failWhenErrorWhileWriting)
{
    AudioSaveAndRecall<TestFileIo> sut(fileIo_);

    sut.startSavingToFile("dummyFilename",
                          monoLooper_,
                          &doneCallback,
                          &doneCallbackResult_);

    EXPECT_STREQ(fileIo_.lastFileName_, "dummyFilename");
    EXPECT_EQ(doneCallbackResult_, AudioSaveAndRecallResult(-1)); // not called yet
    EXPECT_EQ(sut.getCurrentProgress(), 0.0f);
    EXPECT_FALSE(fileIo_.closeFileCalled_);

    // make some progress
    sut.readOrWriteNextChunk();
    EXPECT_GT(sut.getCurrentProgress(), 0.0f);
    EXPECT_LT(sut.getCurrentProgress(), 1.0f);

    // simulate file IO error
    fileIo_.simulateFileWriteError_ = true;
    sut.readOrWriteNextChunk();

    // operation was aborted
    EXPECT_EQ(doneCallbackResult_, AudioSaveAndRecallResult::error);
    EXPECT_EQ(sut.getCurrentProgress(), -1.0f);
    EXPECT_TRUE(fileIo_.closeFileCalled_);
}

TEST_F(AudioSaveAndRecallFixture, d_saveToFile_saveFileButWaitUntilRecordingDoneAndPreventNewRecording)
{
    AudioSaveAndRecall<TestFileIo> sut(fileIo_);

    monoLooper_.switchState(LooperState::recording);
    monoLooper_.recorderForTesting().setRecordingProgressForTesting(2000);

    sut.startSavingToFile("dummyFilename",
                          monoLooper_,
                          &doneCallback,
                          &doneCallbackResult_);

    EXPECT_STREQ(fileIo_.lastFileName_, "dummyFilename");
    EXPECT_EQ(doneCallbackResult_, AudioSaveAndRecallResult(-1)); // not called yet
    EXPECT_EQ(sut.getCurrentProgress(), 0.0f);
    EXPECT_FALSE(fileIo_.closeFileCalled_);

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
    AudioSaveAndRecall<TestFileIo> sut(fileIo_);

    sut.startSavingToFile("dummyFilename",
                          monoLooper_,
                          &doneCallback,
                          &doneCallbackResult_);

    EXPECT_STREQ(fileIo_.lastFileName_, "dummyFilename");
    EXPECT_EQ(doneCallbackResult_, AudioSaveAndRecallResult(-1)); // not called yet
    EXPECT_EQ(sut.getCurrentProgress(), 0.0f);
    EXPECT_FALSE(fileIo_.closeFileCalled_);

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
    AudioSaveAndRecall<TestFileIo> sut(fileIo_);

    sut.startSavingToFile("dummyFilename",
                          monoLooper_,
                          &doneCallback,
                          &doneCallbackResult_);

    EXPECT_STREQ(fileIo_.lastFileName_, "dummyFilename");
    EXPECT_EQ(doneCallbackResult_, AudioSaveAndRecallResult(-1)); // not called yet
    EXPECT_EQ(sut.getCurrentProgress(), 0.0f);
    EXPECT_FALSE(fileIo_.closeFileCalled_);

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
    EXPECT_TRUE(fileIo_.closeFileCalled_);
}

TEST_F(AudioSaveAndRecallFixture, g_saveToFile_producesValidMonoWavFile)
{
    AudioSaveAndRecall<TestFileIo> sut(fileIo_);

    constexpr auto kNumTestSamples = 4800;
    constexpr auto kReferenceFilename = "data/monoSineWaves.wav";

    fillBuffersWithSineWaves();
    monoLooper_.setPlaybackLength(kNumTestSamples);

    sut.startSavingToFile("dummyFilename",
                          monoLooper_,
                          &doneCallback,
                          &doneCallbackResult_);

    // write until completed
    int timeout = 1000;
    while (timeout-- > 0 && doneCallbackResult_ != AudioSaveAndRecallResult::ok)
    {
        sut.readOrWriteNextChunk();
    }
    EXPECT_EQ(sut.getCurrentProgress(), 1.0f);
    EXPECT_EQ(doneCallbackResult_, AudioSaveAndRecallResult::ok);
    EXPECT_TRUE(fileIo_.closeFileCalled_);

#ifdef WRITE_REFERENCES
    writeReferenceFile(kReferenceFilename, fileIo_.dataWritten_);
#endif

    const auto expectedFileContents = readReferenceFile(kReferenceFilename);
    EXPECT_EQ(fileIo_.dataWritten_, expectedFileContents);
}

TEST_F(AudioSaveAndRecallFixture, h_saveToFile_producesValidStereoWavFile)
{
    AudioSaveAndRecall<TestFileIo> sut(fileIo_);

    constexpr auto kNumTestSamples = 4800;
    constexpr auto kReferenceFilename = "data/stereoSineWaves.wav";

    fillBuffersWithSineWaves();
    stereoLooper_.setPlaybackLength(kNumTestSamples);

    sut.startSavingToFile("dummyFilename",
                          stereoLooper_,
                          &doneCallback,
                          &doneCallbackResult_);

    // write until completed
    int timeout = 1000;
    while (timeout-- > 0 && doneCallbackResult_ != AudioSaveAndRecallResult::ok)
    {
        sut.readOrWriteNextChunk();
    }
    EXPECT_EQ(sut.getCurrentProgress(), 1.0f);
    EXPECT_EQ(doneCallbackResult_, AudioSaveAndRecallResult::ok);
    EXPECT_TRUE(fileIo_.closeFileCalled_);

#ifdef WRITE_REFERENCES
    writeReferenceFile(kReferenceFilename, fileIo_.dataWritten_);
#endif

    const auto expectedFileContents = readReferenceFile(kReferenceFilename);
    EXPECT_EQ(fileIo_.dataWritten_, expectedFileContents);
}

class AudioSaveAndRecallParameterizedFileFormatFixture :
    public AudioSaveAndRecallFixture,
    public ::testing::WithParamInterface<std::tuple<const char*, const char*, float>>
{
};

TEST_P(AudioSaveAndRecallParameterizedFileFormatFixture, canReadMonoWav)
{
    const auto [referenceFileNameMono, referenceFileNameStereo, epsilon] = GetParam();

    AudioSaveAndRecall<TestFileIo> sut(fileIo_);

    constexpr auto kExpectedNumSamples = 48;

    fileIo_.dataToRead_ = readReferenceFile(referenceFileNameMono);

    sut.startReadingFromFile("dummyFilename",
                             monoLooper_,
                             &doneCallback,
                             &doneCallbackResult_);

    // read until completed
    int timeout = 1000;
    while (timeout-- > 0 && doneCallbackResult_ != AudioSaveAndRecallResult::ok)
    {
        sut.readOrWriteNextChunk();
    }

    const auto kExpectedSawWav = makeSawWave(1000.0, kExpectedNumSamples);

    const auto actualSamples = std::vector<float>(monoBuffer_.data[0],
                                                  monoBuffer_.data[0] + monoLooper_.getPlaybackLength());

    EXPECT_THAT(actualSamples,
                ::testing::Pointwise(::testing::FloatNear(epsilon),
                                     kExpectedSawWav));
}

TEST_P(AudioSaveAndRecallParameterizedFileFormatFixture, canReadStereoWav)
{
    const auto [referenceFileNameMono, referenceFileNameStereo, epsilon] = GetParam();

    AudioSaveAndRecall<TestFileIo> sut(fileIo_);

    constexpr auto kExpectedNumSamples = 48;

    fileIo_.dataToRead_ = readReferenceFile(referenceFileNameStereo);

    sut.startReadingFromFile("dummyFilename",
                             stereoLooper_,
                             &doneCallback,
                             &doneCallbackResult_);

    // read until completed
    int timeout = 1000;
    while (timeout-- > 0 && doneCallbackResult_ != AudioSaveAndRecallResult::ok)
    {
        sut.readOrWriteNextChunk();
    }

    const auto kExpectedSawWavLeft = makeSawWave(1000.0, kExpectedNumSamples);
    const auto kExpectedSawWavRight = makeSawWave(2000.0, kExpectedNumSamples);

    const auto actualSamplesLeft = std::vector<float>(stereoBuffer_.data[0],
                                                      stereoBuffer_.data[0] + stereoLooper_.getPlaybackLength());
    const auto actualSamplesRight = std::vector<float>(stereoBuffer_.data[1],
                                                       stereoBuffer_.data[1] + stereoLooper_.getPlaybackLength());

    EXPECT_THAT(actualSamplesLeft,
                ::testing::Pointwise(::testing::FloatNear(epsilon),
                                     kExpectedSawWavLeft));
    EXPECT_THAT(actualSamplesRight,
                ::testing::Pointwise(::testing::FloatNear(epsilon),
                                     kExpectedSawWavRight));
}

INSTANTIATE_TEST_SUITE_P(
    AudioSaveAndRecall_loadFromFile_canReadWavs,
    AudioSaveAndRecallParameterizedFileFormatFixture,
    ::testing::Values(
        std::make_tuple("data/monoSaw_int16.wav", "data/stereoSaw_int16.wav", 3e-4f),
        std::make_tuple("data/monoSaw_int24.wav", "data/stereoSaw_int24.wav", 1e-6f),
        std::make_tuple("data/monoSaw_int32.wav", "data/stereoSaw_int32.wav", 1e-6f),
        std::make_tuple("data/monoSaw_float.wav", "data/stereoSaw_float.wav", 1e-6f),
        // Ableton uses the JUNK segment to encode a messsage in the file.
        std::make_tuple("data/monoSaw_float.wav", "data/stereoSaw_float_Ableton.wav", 1e-6f)));
