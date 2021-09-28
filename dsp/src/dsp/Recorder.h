#pragma once

#include "DspHelpers.h"

template <size_t samplerate>
class Recorder
{
public:
    Recorder(float* buffer, size_t bufferSize) :
        bufferStart_(buffer),
        bufferEnd_(buffer + bufferSize),
        bufferSize_(bufferSize),
        currentLength_(0),
        isRecording_(false),
        isFadingOut_(false),
        xfadePhase_(0.0f),
        recHead_(bufferStart_)
    {
        setCrossfadeLength(size_t(defaultXFadeLengthInS_ * float(samplerate)));
    }

    void reset()
    {
        currentLength_ = 0;
        isRecording_ = false;
        isFadingOut_ = false;
        xfadePhase_ = 0.0f;
        recHead_ = bufferStart_;
    }

    void startRecording()
    {
        recHead_ = bufferStart_;
        isRecording_ = true;
        isFadingOut_ = false;
        currentLength_ = 0;
    }

    void stopRecording()
    {
        isRecording_ = false;
        isFadingOut_ = true;
        xfadePhase_ = 0.0f;
        currentLength_ = size_t(recHead_ - bufferStart_);
        recHead_ = bufferStart_;
    }

    void setCrossfadeLength(size_t crossfadeLengthInSamples)
    {
        xFadeLengthInSamples_ = std::min(crossfadeLengthInSamples, bufferSize_);
        xFadeIncrement_ = 1.0f / float(xFadeLengthInSamples_);
    }

    void process(const float* input, size_t numSamples)
    {
        size_t i = 0;
        if (isRecording_)
        {
            for (; i < numSamples; i++)
            {
                *recHead_ = input[i];
                recHead_++;
                if (recHead_ == bufferEnd_)
                {
                    stopRecording();
                    break;
                }
            }
        }
        if (isFadingOut_)
        {
            for (; i < numSamples; i++)
            {
                xfadePhase_ = std::min(xfadePhase_ + xFadeIncrement_, 1.0f);
                const auto xFadedSample = linMap(xfadePhase_, input[i], *recHead_);
                *recHead_ = xFadedSample;
                recHead_++;
                if ((recHead_ == bufferEnd_) || (xfadePhase_ > 1.0f - xFadeIncrement_))
                {
                    isFadingOut_ = false;
                    break;
                }
            }
        }
    }

    size_t getCurrentRecordingLength() const { return currentLength_; }
    bool isRecording() const { return isRecording_ || isFadingOut_; }

private:
    static constexpr float defaultXFadeLengthInS_ = 0.1f;

    float* bufferStart_;
    float* bufferEnd_;
    size_t bufferSize_;
    size_t currentLength_;
    bool isRecording_;
    bool isFadingOut_;
    float xfadePhase_;
    float* recHead_;
    size_t xFadeLengthInSamples_;
    float xFadeIncrement_;
};