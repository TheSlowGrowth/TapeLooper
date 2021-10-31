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

template <size_t samplerate>
class TapeLooperProcessor : public ITapeLooperProcessor
{
public:
    TapeLooperProcessor(juce::AudioParameterChoice& stateParameter,
                        juce::AudioParameterFloat& speedParameter,
                        juce::AudioParameterFloat& driveParameter,
                        juce::AudioParameterFloat& grainAmtParameter,
                        juce::AudioParameterFloat& wowAndFlutterAmtParameter,
                        juce::AudioParameterFloat& postGainParameter,
                        const LooperStoragePtr<2>& storage) :
        stateParameter_(stateParameter),
        speedParameter_(speedParameter),
        driveParameter_(driveParameter),
        grainAmtParameter_(grainAmtParameter),
        wowAndFlutterAmtParameter_(wowAndFlutterAmtParameter),
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
        const auto drive = driveParameter_.get();
        const auto grainAmt = grainAmtParameter_.get();
        const auto wowAndFlutterAmt = wowAndFlutterAmtParameter_.get();
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

        typename TapeLooper<samplerate, 2>::ProcessorType::Parameters params;
        params.driveGain = drive;
        params.grainAmt = grainAmt;

        const float* inChPtrs[2] = { input.getChannelPointer(0),
                                     input.getChannelPointer(1) };
        float* outChPtrs[2] = { outputToAddTo.getChannelPointer(0),
                                outputToAddTo.getChannelPointer(1) };
        looper_.process(speed,
                        wowAndFlutterAmt,
                        direction,
                        params,
                        1.0f, // pre gain
                        postGain,
                        AudioBufferPtr<2, const float>(
                            inChPtrs, input.getNumSamples()),
                        AudioBufferPtr<2, float>(
                            outChPtrs, outputToAddTo.getNumSamples()));
    }

private:
    juce::AudioParameterChoice& stateParameter_;
    juce::AudioParameterFloat& speedParameter_;
    juce::AudioParameterFloat& driveParameter_;
    juce::AudioParameterFloat& grainAmtParameter_;
    juce::AudioParameterFloat& wowAndFlutterAmtParameter_;
    juce::AudioParameterFloat& postGainParameter_;

    TapeLooper<samplerate, 2> looper_;
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
        inputTmpBuffer_.setSize(2, specs.maximumBlockSize);
        inputTmpBuffer_.clear();

        createInstancesFor(specs.sampleRate);

        for (auto processor : processors_)
            processor->prepareToPlay();
    }

    void processBlock(juce::dsp::AudioBlock<float> buffer)
    {
        for (size_t ch = 0; ch < std::min(buffer.getNumChannels(), size_t(inputTmpBuffer_.getNumChannels())); ch++)
            inputTmpBuffer_.copyFrom(int(ch), 0, buffer.getChannelPointer(ch), int(buffer.getNumSamples()));
        const auto inputBuffer = juce::dsp::AudioBlock<float>(
            inputTmpBuffer_.getArrayOfWritePointers(),
            buffer.getNumChannels(),
            buffer.getNumSamples());

        // process loopers
        buffer.clear();
        for (auto processor : processors_)
            processor->processBlock(inputBuffer, buffer);
    }

private:
    void createInstancesFor(double sampleRate)
    {
        static constexpr auto supportedSampleRates = { 44100, 48000, 88200, 96000 };

        if (sampleRate != currentSampleRate_)
        {
            // init storage
            const int numSamples = int(std::ceil(dspdefs::loopLengthInS * sampleRate));
            storage_.setSize(int(dspdefs::numLoopers) * 2 /* stereo */,
                             numSamples,
                             false,
                             true);

            // init processors
            const auto sampleRateToUse = getClosestSampleRate(supportedSampleRates, sampleRate);
            switch (sampleRateToUse)
            {
                default:
                case 44100u:
                    createProcessorInstances<TapeLooperProcessor<44100u>>();
                    break;
                case 48000u:
                    createProcessorInstances<TapeLooperProcessor<48000u>>();
                    break;
                case 88200u:
                    createProcessorInstances<TapeLooperProcessor<88200u>>();
                    break;
                case 96000u:
                    createProcessorInstances<TapeLooperProcessor<96000u>>();
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
            LooperStoragePtr<2> storagePtr;
            storagePtr.data[0] = storage_.getWritePointer(int(2 * i));
            storagePtr.data[1] = storage_.getWritePointer(int(2 * i + 1));
            storagePtr.numSamples = size_t(storage_.getNumSamples());

            const juce::String chStr(i);
            auto& stateParameter = *(juce::AudioParameterChoice*) parameters_.getParameter(dspdefs::paramIds::chState + chStr);
            auto& speedParameter = *(juce::AudioParameterFloat*) parameters_.getParameter(dspdefs::paramIds::chSpeed + chStr);
            auto& driveParameter = *(juce::AudioParameterFloat*) parameters_.getParameter(dspdefs::paramIds::chDrive + chStr);
            auto& grainAmtParameter = *(juce::AudioParameterFloat*) parameters_.getParameter(dspdefs::paramIds::chGrainAmt + chStr);
            auto& wowAndFlutterAmtParameter = *(juce::AudioParameterFloat*) parameters_.getParameter(dspdefs::paramIds::chWowAndFlutterAmt + chStr);
            auto& postGainParameter = *(juce::AudioParameterFloat*) parameters_.getParameter(dspdefs::paramIds::chPostGain + chStr);

            processors_.add(new ProcessorType(stateParameter,
                                              speedParameter,
                                              driveParameter,
                                              grainAmtParameter,
                                              wowAndFlutterAmtParameter,
                                              postGainParameter,
                                              storagePtr));
        }
    }

    juce::AudioProcessorValueTreeState& parameters_;

    double currentSampleRate_;
    juce::AudioBuffer<float> storage_;
    juce::AudioBuffer<float> inputTmpBuffer_;
    juce::OwnedArray<ITapeLooperProcessor> processors_;
};
