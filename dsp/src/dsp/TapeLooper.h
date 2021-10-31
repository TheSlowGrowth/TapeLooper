#pragma once

#include "DspHelpers.h"
#include "Player.h"
#include "Recorder.h"
#include "TapeProcessor.h"
#include "WowAndFlutter.h"

template <size_t numChannels>
struct LooperStoragePtr
{
    float* data[numChannels];
    size_t numSamples;
};

template <size_t size, size_t numChannels>
class LooperStorage : public LooperStoragePtr<numChannels>
{
public:
    LooperStorage()
    {
        for (size_t ch = 0; ch < numChannels; ch++)
        {
            storage_[ch].fill(0.0f);
            this->data[ch] = storage_[ch];
        }
        this->numSamples = size;
    }

private:
    std::array<std::array<float, size>, numChannels> storage_;
};

enum class LooperState
{
    stopped,
    playing,
    recording
};

template <size_t sampleRate, size_t numChannels>
class TapeLooper
{
public:
    using ProcessorType = TapeProcessor<sampleRate, numChannels>;
    using SpeedModulatorType = WowAndFlutterOscillator<float, sampleRate>;

    TapeLooper(LooperStoragePtr<numChannels> storageToUse) :
        storage_(storageToUse),
        state_(LooperState::stopped),
        player_(AudioBufferPtr<numChannels, const float>(storage_.data, storage_.numSamples)),
        recorder_(AudioBufferPtr<numChannels, float>(storage_.data, storage_.numSamples))
    {
    }

    void switchState(LooperState state)
    {
        switch (state)
        {
            default:
            case LooperState::stopped:
                if (state_ == LooperState::recording)
                    recorder_.stopRecording();
                else if (state_ == LooperState::playing)
                    player_.stopPlaying();
                break;
            case LooperState::playing:
                if (state_ == LooperState::recording)
                    recorder_.stopRecording();
                player_.startPlaying(recorder_.getCurrentRecordingLength());
                break;
            case LooperState::recording:
                if (state_ == LooperState::playing)
                    player_.stopPlaying();
                recorder_.startRecording();
                break;
        }
        state_ = state;
    }

    LooperState getState() const { return state_; }

    void reset()
    {
        player_.reset();
        recorder_.reset();
    }

    void process(float paramSpeed,
                 float wowAndFlutterAmt,
                 Direction direction,
                 const typename ProcessorType::Parameters& processorParameters,
                 float paramPreGain,
                 float paramPostGain,
                 AudioBufferPtr<numChannels, const float> input,
                 AudioBufferPtr<numChannels, float> outputToAddTo)
    {
        const auto mappedWowAndFlutterAmt = wowAndFlutterAmt * wowAndFlutterAmt;
        player_.process(paramSpeed,
                        mappedWowAndFlutterAmt * maxWowAndFlutterAmt_,
                        direction,
                        paramPreGain,
                        paramPostGain,
                        processorParameters,
                        outputToAddTo);
        recorder_.process(input);

        const auto recordingStopped = state_ == LooperState::recording && !recorder_.isRecording();
        if (recordingStopped)
            switchState(LooperState::playing);
    }

private:
    static constexpr float maxWowAndFlutterAmt_ = 0.0125f;
    const LooperStoragePtr<numChannels> storage_;
    LooperState state_;
    Player<ProcessorType, SpeedModulatorType, sampleRate, numChannels> player_;
    Recorder<sampleRate, numChannels> recorder_;
};
