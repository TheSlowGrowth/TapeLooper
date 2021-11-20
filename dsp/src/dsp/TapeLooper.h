#pragma once

#include "DspHelpers.h"
#include "Player.h"
#include "Recorder.h"
#include "TapeProcessor.h"
#include "WowAndFlutter.h"

#include "../util/Memory.h"

template <size_t numChannels_>
struct LooperStoragePtr
{
    float* data[numChannels_];
    size_t numSamples;
    static constexpr size_t numChannels = numChannels_;

    size_t getTotalSizeInBytes() const
    {
        return numChannels * numSamples * sizeof(float);
    }
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
            this->data[ch] = storage_[ch].data();
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
    using PlayerType = Player<ProcessorType, SpeedModulatorType, sampleRate, numChannels>;
    using RecorderType = Recorder<sampleRate, numChannels>;

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

    size_t getSaveAndRecallStorageSize() const
    {
        return recorder_.getSaveAndRecallStorageSize()
               + sizeof(uint8_t) // isPlaying?
               + storage_.getTotalSizeInBytes();
    }

    /** 
     * Saves the state of the looper
     */
    template <typename StorageType>
    bool save(WritableMemory<StorageType>& mem) const
    {
        if (!recorder_.save(mem))
            return false;

        // write sample data
        for (size_t ch = 0; ch < numChannels; ch++)
        {
            if (!mem.writeRaw(storage_.data[ch], sizeof(float) * storage_.numSamples))
                return false;
        }

        const uint8_t isPlaying = state_ == LooperState::playing ? 1 : 0;

        return mem.writeItems(isPlaying);
    }

    /** 
     * Stopps playback or recording and recalls the state of the looper, including
     *    - the length of the current recording
     *    - if the looper is currently playing
     *    - the sample data stored
     */
    template <typename StorageType>
    bool restore(ReadableMemory<StorageType>& mem)
    {
        if (!recorder_.restore(mem))
            return false;

        // read sample data
        for (size_t ch = 0; ch < numChannels; ch++)
        {
            if (!mem.readRaw(storage_.data[ch], sizeof(float) * storage_.numSamples))
                return false;
        }

        uint8_t isPlaying = 0;
        mem.readItems(isPlaying);

        switchState(isPlaying == 1 ? LooperState::playing : LooperState::stopped);
        return true;
    }

    const LooperStoragePtr<numChannels> getSampleStoragePtr() const { return storage_; }

private:
    static constexpr float maxWowAndFlutterAmt_ = 0.0125f;
    const LooperStoragePtr<numChannels> storage_;
    LooperState state_;
    PlayerType player_;
    RecorderType recorder_;
};
