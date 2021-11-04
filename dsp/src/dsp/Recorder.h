#pragma once

#include "DspHelpers.h"
#include "AudioBuffer.h"
#include "../util/Memory.h"

template <size_t samplerate, size_t numChannels>
class Recorder
{
public:
    Recorder(AudioBufferPtr<numChannels> bufferPtr) :
        buffer_(bufferPtr),
        currentLength_(0),
        isRecording_(false),
        isFadingOut_(false),
        xfadePhase_(0.0f),
        recHeadIdx_(0)
    {
        setCrossfadeLength(size_t(defaultXFadeLengthInS_ * float(samplerate)));
    }

    void reset()
    {
        currentLength_ = 0;
        isRecording_ = false;
        isFadingOut_ = false;
        xfadePhase_ = 0.0f;
        recHeadIdx_ = 0;
    }

    void startRecording()
    {
        recHeadIdx_ = 0;
        isRecording_ = true;
        isFadingOut_ = false;
        currentLength_ = 0;
    }

    void stopRecording()
    {
        isRecording_ = false;
        isFadingOut_ = true;
        xfadePhase_ = 0.0f;
        currentLength_ = recHeadIdx_;
        recHeadIdx_ = 0;
    }

    void setCrossfadeLength(size_t crossfadeLengthInSamples)
    {
        xFadeLengthInSamples_ = std::min(crossfadeLengthInSamples, buffer_.size_);
        xFadeIncrement_ = 1.0f / float(xFadeLengthInSamples_);
    }

    void process(AudioBufferPtr<numChannels, const float> input)
    {
        size_t i = 0;
        if (isRecording_)
        {
            for (; i < input.size_; i++)
            {
                for (size_t ch = 0; ch < numChannels; ch++)
                    buffer_[ch][recHeadIdx_] = input.buffer_[ch][i];
                recHeadIdx_++;
                if (recHeadIdx_ == buffer_.size_)
                {
                    stopRecording();
                    break;
                }
            }
        }
        if (isFadingOut_)
        {
            for (; i < input.size_; i++)
            {
                xfadePhase_ = std::min(xfadePhase_ + xFadeIncrement_, 1.0f);
                for (size_t ch = 0; ch < numChannels; ch++)
                {
                    const auto xFadedSample = linMap(xfadePhase_,
                                                     input.buffer_[ch][i],
                                                     buffer_[ch][recHeadIdx_]);
                    buffer_[ch][recHeadIdx_] = xFadedSample;
                }
                recHeadIdx_++;
                if ((recHeadIdx_ == buffer_.size_) || (xfadePhase_ > 1.0f - xFadeIncrement_))
                {
                    isFadingOut_ = false;
                    break;
                }
            }
        }
    }

    size_t getSaveAndRecallStorageSize() const
    {
        return sizeof(uint32_t); // recordingLengthInSamples
    }

    template <typename StorageType>
    bool save(WritableMemory<StorageType>& mem) const
    {
        const auto recordingLengthInSamples = uint32_t(currentLength_);
        return mem.writeItems(recordingLengthInSamples);
    }

    template <typename StorageType>
    bool restore(ReadableMemory<StorageType>& mem)
    {
        reset();
        uint32_t recordingLengthInSamples = 0;
        if (!mem.readItems(recordingLengthInSamples))
            return false;

        currentLength_ = std::min(size_t(recordingLengthInSamples), buffer_.size_);
        return true;
    }

    size_t getCurrentRecordingLength() const { return currentLength_; }
    bool isRecording() const { return isRecording_ || isFadingOut_; }

private:
    static constexpr float defaultXFadeLengthInS_ = 0.1f;

    AudioBufferPtr<numChannels> buffer_;
    size_t currentLength_;
    bool isRecording_;
    bool isFadingOut_;
    float xfadePhase_;
    size_t recHeadIdx_;
    size_t xFadeLengthInSamples_;
    float xFadeIncrement_;
};