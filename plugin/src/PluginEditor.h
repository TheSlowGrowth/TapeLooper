#pragma once

#include "PluginProcessor.h"
#include "TapeLooperComponent.h"

//==============================================================================
class TapeLooperPluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit TapeLooperPluginAudioProcessorEditor(TapeLooperPluginAudioProcessor&);
    ~TapeLooperPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TapeLooperPluginAudioProcessor& processorRef;

    juce::OwnedArray<TapeLooperComponent> looperComponents_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TapeLooperPluginAudioProcessorEditor)
};
