#pragma once

#include "DspHelpers.h"
#include "AudioBuffer.h"

#ifndef MANUAL_INLINE
#    define MANUAL_INLINE inline __attribute__((always_inline))
#endif

enum class Direction
{
    forwards,
    backwards
};

template <typename ProcessorType,
          typename SpeedModulatorType,
          size_t sampleRateHz,
          size_t numChannels>
class Player
{
public:
    Player(AudioBufferPtr<numChannels, const float> bufferPtr) :
        sampleBuffer_(bufferPtr),
        playbackLength_(0),
        isPlaying_(false),
        playPos_(0.0f),
        lastProcessedSampleIdx_(-1)
    {
    }

    void reset()
    {
        processor_.reset();
        speedModulator_.reset();
        preGainSmoother_.reset(0);
        postGainSmoother_.reset(0);
        speedSmoother_.reset(0);
        playbackLength_ = 0;
        isPlaying_ = false;
        playPos_ = 0.0f;
        lastProcessedSampleIdx_ = -1;
        interpolationBuffer_[0].fill(0.0f);
        interpolationBuffer_[1].fill(0.0f);
    }

    void startPlaying(size_t loopLengthInSamples)
    {
        playbackLength_ = std::min(loopLengthInSamples, sampleBuffer_.size_);
        isPlaying_ = true;
        playPos_ = 0.0f;
        lastProcessedSampleIdx_ = wrapUpToPlaybackLength(-1);
        interpolationBuffer_[0].fill(0.0f);
        interpolationBuffer_[1].fill(0.0f);
    }

    void stopPlaying()
    {
        isPlaying_ = false;
    }

    bool isPlaying() const
    {
        return isPlaying_;
    }

    void process(float paramSpeed,
                 float speedModulationAmt,
                 Direction direction,
                 float paramPreProcessorGain,
                 float paramPostProcessorGain,
                 const typename ProcessorType::Parameters& processorParameters,
                 AudioBufferPtr<numChannels, float> outputToAddTo,
                 ExponentialSmoother::TimeConstant preGainSmootherTimeConstant =
                     ExponentialSmoother::TimeConstant(0.05f, sampleRateHz, 1),
                 ExponentialSmoother::TimeConstant postGainSmootherTimeConstant =
                     ExponentialSmoother::TimeConstant(0.05f, sampleRateHz, 1),
                 ExponentialSmoother::TimeConstant speedSmootherTimeConstant =
                     ExponentialSmoother::TimeConstant(0.5f, sampleRateHz, 1))
    {
        const auto preGainTarget = paramPreProcessorGain;
        const auto postGainTarget = (isPlaying_) ? paramPostProcessorGain : 0.0f;
        const auto speedTarget = limit(paramSpeed, minSpeed_, maxSpeed_);

        if (playbackLength_ < 1)
            return;

        for (size_t i = 0; i < outputToAddTo.size_; i++)
        {
            const auto preGain = preGainSmoother_.smooth(preGainTarget, preGainSmootherTimeConstant);
            const auto postGain = postGainSmoother_.smooth(postGainTarget, postGainSmootherTimeConstant);
            const auto speedModulation = speedModulator_.getAndAdvance() * speedModulationAmt;
            const auto speed = speedModulation + speedSmoother_.smooth(speedTarget, speedSmootherTimeConstant);

            // the final two samples we need to interpolate for the current output sample
            const auto sampleAIdx = int(playPos_);
            const auto sampleBIdx = int(wrapDownToPlaybackLength(sampleAIdx + 1));
            // the interpolation point between the two samples
            const auto fractional = playPos_ - float(sampleAIdx);

            // process the raw, uninterpolated samples using the processor until both samples
            // are in the interpolation buffer
            int currentSampleIdx = lastProcessedSampleIdx_;
            while (currentSampleIdx != sampleBIdx)
            {
                lastProcessedSampleIdx_ = currentSampleIdx;
                currentSampleIdx = wrapDownToPlaybackLength(currentSampleIdx + 1);
                const auto indexToRead = (direction == Direction::forwards)
                                             ? currentSampleIdx
                                             : int(playbackLength_) - 1 - currentSampleIdx;

                for (size_t ch = 0; ch < numChannels; ch++)
                {
                    interpolationBuffer_[1][ch] = interpolationBuffer_[0][ch];
                    interpolationBuffer_[0][ch] = sampleBuffer_[ch][indexToRead] * preGain;
                }
                processor_.process(interpolationBuffer_[0].data(), processorParameters);
            }

            for (size_t ch = 0; ch < numChannels; ch++)
            {
                const auto outSample = fractional * interpolationBuffer_[0][ch]
                                       + (1.0f - fractional) * interpolationBuffer_[1][ch];
                outputToAddTo[ch][i] += outSample * postGain;
            }
            lastProcessedSampleIdx_ = currentSampleIdx;

            // advance read position
            playPos_ += speed;
            playPos_ = wrapDownToPlaybackLength(playPos_);
        }
    }

    ProcessorType& getProcessor() { return processor_; }
    SpeedModulatorType& getSpeedModulator() { return speedModulator_; }

private:
    static constexpr float minSpeed_ = 0.25f;
    static constexpr float maxSpeed_ = 4.0f;

    template <typename T>
    MANUAL_INLINE T wrapDownToPlaybackLength(T value)
    {
        if (value >= T(playbackLength_))
            value -= T(playbackLength_);
        return value;
    }

    template <typename T>
    MANUAL_INLINE T wrapUpToPlaybackLength(T value)
    {
        if (value < T(0))
            value += T(playbackLength_);
        return value;
    }

    ExponentialSmoother preGainSmoother_;
    ExponentialSmoother postGainSmoother_;
    ExponentialSmoother speedSmoother_;

    AudioBufferPtr<numChannels, const float> sampleBuffer_;
    size_t playbackLength_;

    bool isPlaying_;
    float playPos_;
    int lastProcessedSampleIdx_;
    std::array<std::array<float, numChannels>, 2> interpolationBuffer_;

    SpeedModulatorType speedModulator_;
    ProcessorType processor_;
};