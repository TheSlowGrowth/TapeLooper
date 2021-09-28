#include <gtest/gtest.h>
#include <random>
#include "dsp/Recorder.h"

class DSP_Recorder : public ::testing::Test
{
protected:
    DSP_Recorder() :
        recorder_(storage_.data(), numSamples_)
    {
        recorder_.setCrossfadeLength(xFadeLengthInSamples_);
    }

    void fillInputSequence()
    {
        // generate sequence of pseudo random samples
        std::mt19937 generator(1234 /* seed */);
        std::uniform_int_distribution<> distrib(-1000, 1000);
        for (size_t i = 0; i < numSamples_; i++)
            inputSequence_[i] = float(distrib(generator)) / 1000.0f;
    }

    static constexpr size_t xFadeLengthInSamples_ = 10;
    static constexpr float xFadeIncrement_ = 1.0f / float(xFadeLengthInSamples_);
    static constexpr size_t blockSize_ = 5;
    static constexpr float absError_ = 10e-4;

    static constexpr size_t numSamples_ = 2 * xFadeLengthInSamples_ + 10;
    std::array<float, numSamples_> storage_;
    std::array<float, numSamples_> inputSequence_;
    Recorder<48000> recorder_;
};

TEST_F(DSP_Recorder, a_stateAfterInit)
{
    EXPECT_FALSE(recorder_.isRecording());
    EXPECT_EQ(recorder_.getCurrentRecordingLength(), 0u);
}

TEST_F(DSP_Recorder, b_recordAndStop)
{
    fillInputSequence();

    recorder_.startRecording();

    // record fade-in
    size_t inputSampleIdx;
    for (inputSampleIdx = 0; inputSampleIdx < xFadeLengthInSamples_; inputSampleIdx += blockSize_)
        recorder_.process(inputSequence_.data() + inputSampleIdx, blockSize_);
    // record half a block before stopping
    recorder_.process(inputSequence_.data() + inputSampleIdx, blockSize_ / 2);
    inputSampleIdx += blockSize_ / 2;

    recorder_.stopRecording();
    EXPECT_TRUE(recorder_.isRecording()); // recording has not yet stopped!

    // continue until the rest of the input data has been processed
    while (inputSampleIdx < numSamples_)
    {
        const size_t remainingSamples = numSamples_ - inputSampleIdx;
        const size_t sizeOfNextBlock = std::min(blockSize_, remainingSamples);
        recorder_.process(inputSequence_.data() + inputSampleIdx, blockSize_);
        inputSampleIdx += sizeOfNextBlock;
    }

    EXPECT_FALSE(recorder_.isRecording()); // recording has stopped!

    // check total length
    // we recorded until the crossfade was over, then another half block
    const size_t expectedTotalNumSamples = xFadeLengthInSamples_ + blockSize_ / 2;
    EXPECT_EQ(recorder_.getCurrentRecordingLength(), expectedTotalNumSamples);

    // compare the samples in the fade-in portion
    float xfade = 0.0f;
    for (size_t i = 0; i < xFadeLengthInSamples_; i++)
    {
        xfade += xFadeIncrement_;
        const float expectedSample = xfade * inputSequence_[i]
                                     + (1.0f - xfade) * inputSequence_[i + expectedTotalNumSamples];
        EXPECT_NEAR(expectedSample, storage_[i], absError_) << "at i=" << i;
    }
    // compare the samples in the non-crossfaded portion
    for (size_t i = xFadeLengthInSamples_; i < expectedTotalNumSamples; i++)
        EXPECT_NEAR(inputSequence_[i], storage_[i], absError_) << "at i=" << i;

    if (HasFailure())
    {
        // print the raw input sequence
        std::cout << "input (raw)   : ";
        for (const auto i : inputSequence_)
            std::cout << i << ", ";
        std::cout << "\n";
        // print the input rest of the inoput sequence that will be crossfaded into the beginning of the recording buffer
        std::cout << "input overlap : ";
        for (size_t i = expectedTotalNumSamples; i < expectedTotalNumSamples + xFadeLengthInSamples_; i++)
            std::cout << inputSequence_[i] << ", ";
        std::cout << "\n";
        // print the recording buffer, whose first elements will contain a crossfade from "input overlap" to "input (raw)"
        std::cout << "recBuffer     : ";
        for (const auto i : storage_)
            std::cout << i << ", ";
        std::cout << "\n";
    }
}