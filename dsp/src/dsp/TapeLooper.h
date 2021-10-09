#pragma once

#include "DspHelpers.h"
#include "Player.h"
#include "Recorder.h"
#include "TapeProcessor.h"
#include "WowAndFlutter.h"

struct LooperStorage
{
    float* data;
    size_t numSamples;
};

enum class LooperState
{
    stopped,
    playing,
    recording
};

template <size_t sampleRate>
class TapeLooper
{
public:
    using ProcessorType = TapeProcessor<sampleRate>;
    using SpeedModulatorType = WowAndFlutterOscillator<float, sampleRate>;

    TapeLooper(LooperStorage storageToUse) :
        storage_(storageToUse),
        state_(LooperState::stopped),
        player_(storage_.data, storage_.numSamples),
        recorder_(storage_.data, storage_.numSamples)
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
                 const float* input,
                 float* outputToAddTo,
                 size_t numSamples)
    {
        const auto mappedWowAndFlutterAmt = wowAndFlutterAmt * wowAndFlutterAmt;
        player_.process(paramSpeed,
                        mappedWowAndFlutterAmt * maxWowAndFlutterAmt_,
                        direction,
                        paramPreGain,
                        paramPostGain,
                        processorParameters,
                        outputToAddTo,
                        numSamples);
        recorder_.process(input, numSamples);

        const auto recordingStopped = state_ == LooperState::recording && !recorder_.isRecording();
        if (recordingStopped)
            switchState(LooperState::playing);
    }

private:
    static constexpr float maxWowAndFlutterAmt_ = 0.0125f;
    const LooperStorage storage_;
    LooperState state_;
    Player<ProcessorType, SpeedModulatorType> player_;
    Recorder<sampleRate> recorder_;
};
