#pragma once

#include "DspDefinitions.h"
#include <juce_audio_plugin_client/juce_audio_plugin_client.h>
#include <juce_dsp/juce_dsp.h>

#include <dsp/TapeLooper.h>

class ITapeLooperProcessor
{
public:
    virtual ~ITapeLooperProcessor() {}

    virtual void prepareToPlay() = 0;
    virtual void processBlock(juce::dsp::AudioBlock<const float> input,
                              juce::dsp::AudioBlock<float> outputToAddTo) = 0;
};

template <int samplerate>
class TapeLooperProcessor : public ITapeLooperProcessor
{
public:
    TapeLooperProcessor(juce::AudioParameterChoice& stateParameter,
                        juce::AudioParameterFloat& speedParameter,
                        juce::AudioParameterFloat& preGainParameter,
                        juce::AudioParameterFloat& postGainParameter,
                        const LooperStorage& storage) :
        stateParameter_(stateParameter),
        speedParameter_(speedParameter),
        preGainParameter_(preGainParameter),
        postGainParameter_(postGainParameter),
        looper_(storage)
    {
    }

    void prepareToPlay() override
    {
        looper_.reset();
    }

    void processBlock(juce::dsp::AudioBlock<const float> input,
                      juce::dsp::AudioBlock<float> outputToAddTo) override
    {
        const auto speed = speedParameter_.get();
        const auto preGain = preGainParameter_.get();
        const auto postGain = postGainParameter_.get();

        Direction direction = Direction::forwards;
        LooperState nextState = LooperState::stopped;
        const auto stateIndex = stateParameter_.getIndex();
        if (stateIndex == 0)
        {
            nextState = LooperState::playing;
            direction = Direction::backwards;
        }
        else if (stateIndex == 1)
            nextState = LooperState::stopped;
        else if (stateIndex == 2)
        {
            nextState = LooperState::playing;
            direction = Direction::forwards;
        }
        else if (stateIndex == 3)
            nextState = LooperState::recording;

        if (nextState != looper_.getState())
            looper_.switchState(nextState);

        typename TapeLooper<samplerate>::ProcessorType::Parameters params;
        params.lofi = 0.0f; // TODO

        looper_.process(speed,
                        direction,
                        params,
                        preGain,
                        postGain,
                        input.getChannelPointer(0),
                        outputToAddTo.getChannelPointer(0),
                        outputToAddTo.getNumSamples());
    }

private:
    juce::AudioParameterChoice& stateParameter_;
    juce::AudioParameterFloat& speedParameter_;
    juce::AudioParameterFloat& preGainParameter_;
    juce::AudioParameterFloat& postGainParameter_;

    TapeLooper<samplerate> looper_;
};

class TapeLooperBank
{
public:
    TapeLooperBank(juce::AudioProcessorValueTreeState& parameters) :
        parameters_(parameters),
        currentSampleRate_(0)
    {
    }

    void prepareToPlay(const juce::dsp::ProcessSpec& specs)
    {
        createInstancesFor(specs.sampleRate);

        for (auto processor : processors_)
            processor->prepareToPlay();

        monoDownmixBuffer_.setSize(1, int(specs.maximumBlockSize));
    }

    void processBlock(juce::dsp::AudioBlock<const float> input,
                      juce::dsp::AudioBlock<float> output)
    {
        // input mono downmix
        input
            .getSubsetChannelBlock(0, 1)
            .copyTo(monoDownmixBuffer_);
        for (size_t ch = 1; ch < input.getNumChannels(); ch++)
            monoDownmixBuffer_.addFrom(0, 0, input.getChannelPointer(ch), int(input.getNumSamples()));

        // process loopers
        output.clear();
        for (auto processor : processors_)
            processor->processBlock(monoDownmixBuffer_, output);

        // expand mono output to stereo
        for (size_t ch = 1; ch < output.getNumChannels(); ch++)
            output
                .getSubsetChannelBlock(ch, 1)
                .copyFrom(output.getSubsetChannelBlock(0, 1));
    }

private:
    void createInstancesFor(double sampleRate)
    {
        static constexpr auto supportedSampleRates = { 44100, 48000, 88200, 96000 };

        if (sampleRate != currentSampleRate_)
        {
            // init storage
            const int numSamples = int(std::ceil(dspdefs::loopLengthInS * sampleRate));
            storage_.setSize(int(dspdefs::numLoopers), numSamples, false, true);

            // init processors
            const auto sampleRateToUse = getClosestSampleRate(supportedSampleRates, sampleRate);
            switch (sampleRateToUse)
            {
                default:
                case 44100:
                    createProcessorInstances<TapeLooperProcessor<44100>>();
                    break;
                case 48000:
                    createProcessorInstances<TapeLooperProcessor<48000>>();
                    break;
                case 88200:
                    createProcessorInstances<TapeLooperProcessor<88200>>();
                    break;
                case 96000:
                    createProcessorInstances<TapeLooperProcessor<96000>>();
                    break;
            }
        }

        currentSampleRate_ = sampleRate;
    }

    size_t getClosestSampleRate(const std::initializer_list<int>& supportedSampleRates, double sampleRate)
    {
        size_t result = 0;
        double currentDistance = 10.0e12;
        for (const auto sr : supportedSampleRates)
        {
            const auto distance = std::abs(double(sr) - sampleRate);
            if (distance < currentDistance)
            {
                result = size_t(sr);
                currentDistance = distance;
            }
        }
        return result;
    }

    template <typename ProcessorType>
    void createProcessorInstances()
    {
        processors_.clear();
        for (size_t i = 0; i < dspdefs::numLoopers; i++)
        {
            LooperStorage storagePtr;
            storagePtr.data = storage_.getWritePointer(int(i));
            storagePtr.numSamples = size_t(storage_.getNumSamples());

            const juce::String chStr(i);
            auto& stateParameter = *(juce::AudioParameterChoice*) parameters_.getParameter(dspdefs::paramIds::chState + chStr);
            auto& speedParameter = *(juce::AudioParameterFloat*) parameters_.getParameter(dspdefs::paramIds::chSpeed + chStr);
            auto& preGainParameter = *(juce::AudioParameterFloat*) parameters_.getParameter(dspdefs::paramIds::chPreGain + chStr);
            auto& postGainParameter = *(juce::AudioParameterFloat*) parameters_.getParameter(dspdefs::paramIds::chPostGain + chStr);

            processors_.add(new ProcessorType(stateParameter,
                                              speedParameter,
                                              preGainParameter,
                                              postGainParameter,
                                              storagePtr));
        }
    }

    juce::AudioProcessorValueTreeState& parameters_;

    double currentSampleRate_;
    juce::AudioBuffer<float> monoDownmixBuffer_;
    juce::AudioBuffer<float> storage_;
    juce::OwnedArray<ITapeLooperProcessor> processors_;
};
