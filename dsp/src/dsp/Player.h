#pragma once

#include "DspHelpers.h"

#ifndef MANUAL_INLINE
#    define MANUAL_INLINE inline __attribute__((always_inline))
#endif

enum class Direction
{
    forwards,
    backwards
};

template <typename ProcessorType, int sampleRateHz = 48000>
class Player
{
public:
    Player(const float* buffer, size_t bufferSize) :
        sampleBuffer_(buffer),
        sampleBufferSize_(bufferSize),
        playbackLength_(0),
        isPlaying_(false),
        playPos_(0.0f),
        lastProcessedSampleIdx_(-1),
        interpolationBuffer_({ 0, 0 })
    {
    }

    void reset()
    {
        processor_.reset();
        gainSmoother_.reset(0);
        speedSmoother_.reset(0);
        playbackLength_ = 0;
        isPlaying_ = false;
        playPos_ = 0.0f;
        lastProcessedSampleIdx_ = -1;
        interpolationBuffer_[0] = interpolationBuffer_[1] = 0.0f;
    }

    void startPlaying(size_t loopLengthInSamples)
    {
        playbackLength_ = std::min(loopLengthInSamples, sampleBufferSize_);
        isPlaying_ = true;
        playPos_ = 0.0f;
        lastProcessedSampleIdx_ = wrapUpToPlaybackLength(-1);
        interpolationBuffer_[0] = interpolationBuffer_[1] = 0.0f;
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
                 Direction direction,
                 float paramGain,
                 const typename ProcessorType::Parameters& processorParameters,
                 float* outputToAddTo,
                 size_t numSamples,
                 ExponentialSmoother::TimeConstant gainSmootherTimeConstant =
                     ExponentialSmoother::TimeConstant(0.05f, sampleRateHz, 1),
                 ExponentialSmoother::TimeConstant speedSmootherTimeConstant =
                     ExponentialSmoother::TimeConstant(0.05f, sampleRateHz, 1))
    {
        const auto gainTarget = (isPlaying_) ? paramGain : 0.0f;
        const auto speedTarget = limit(paramSpeed, minSpeed_, maxSpeed_);

        if (playbackLength_ < 1)
            return;

        for (size_t i = 0; i < numSamples; i++)
        {
            const auto gain = gainSmoother_.smooth(gainTarget, gainSmootherTimeConstant);
            const auto speed = speedSmoother_.smooth(speedTarget, speedSmootherTimeConstant);

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
                interpolationBuffer_[1] = interpolationBuffer_[0];
                const auto indexToRead = (direction == Direction::forwards) ? currentSampleIdx
                                                                            : playbackLength_ - 1 - currentSampleIdx;
                interpolationBuffer_[0] = processor_.process(sampleBuffer_[indexToRead], processorParameters);
            }

            // interpolate
            const auto outSample = fractional * interpolationBuffer_[0] + (1.0f - fractional) * interpolationBuffer_[1];
            lastProcessedSampleIdx_ = currentSampleIdx;

            // advance read position
            playPos_ += speed;
            playPos_ = wrapDownToPlaybackLength(playPos_);

            outputToAddTo[i] += outSample * gain;
        }
    }

    ProcessorType& getProcessor() { return processor_; }

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

    ExponentialSmoother gainSmoother_;
    ExponentialSmoother speedSmoother_;

    const float* sampleBuffer_;
    const size_t sampleBufferSize_;
    size_t playbackLength_;

    bool isPlaying_;
    float playPos_;
    int lastProcessedSampleIdx_;
    std::array<float, 2> interpolationBuffer_;

    ProcessorType processor_;
};