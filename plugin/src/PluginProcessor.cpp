#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "DspDefinitions.h"

//==============================================================================
TapeLooperPluginAudioProcessor::TapeLooperPluginAudioProcessor() :
    AudioProcessor(BusesProperties()
                       .withInput("Input", juce::AudioChannelSet::stereo(), true)
                       .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
    parameters_(*this, nullptr, juce::Identifier("TapeLooper"), getParameterLayout()),
    loopers_(parameters_)
{
}

TapeLooperPluginAudioProcessor::~TapeLooperPluginAudioProcessor()
{
}

//==============================================================================
const juce::String TapeLooperPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TapeLooperPluginAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool TapeLooperPluginAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool TapeLooperPluginAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double TapeLooperPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TapeLooperPluginAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
        // so this should be at least 1, even if you're not really implementing programs.
}

int TapeLooperPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TapeLooperPluginAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String TapeLooperPluginAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void TapeLooperPluginAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void TapeLooperPluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec specs;
    specs.numChannels = size_t(getMainBusNumInputChannels());
    specs.maximumBlockSize = size_t(samplesPerBlock * 2); // safety for some weird hosts
    specs.sampleRate = sampleRate;

    loopers_.prepareToPlay(specs);
}

void TapeLooperPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool TapeLooperPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    // This is the place where you check if the layout is supported.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
    return true;
}

void TapeLooperPluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                                  juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;

    loopers_.processBlock(buffer, buffer);
}

//==============================================================================
bool TapeLooperPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TapeLooperPluginAudioProcessor::createEditor()
{
    return new TapeLooperPluginAudioProcessorEditor(*this);
}

//==============================================================================
void TapeLooperPluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused(destData);
}

void TapeLooperPluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused(data, sizeInBytes);
}

juce::AudioProcessorValueTreeState::ParameterLayout TapeLooperPluginAudioProcessor::getParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout result;

    for (size_t i = 0; i < dspdefs::numLoopers; i++)
    {
        const juce::String chStr(i);

        result.add(std::make_unique<juce::AudioParameterChoice>(juce::String(dspdefs::paramIds::chState) + chStr, // parameterID
                                                                "Playback State " + chStr, // parameter name
                                                                juce::StringArray({ "<", "stop", ">", "rec" }), // choices
                                                                1) // default index
        );
        juce::NormalisableRange<float> speedRange(dspdefs::speedMin, dspdefs::speedMax);
        speedRange.setSkewForCentre(1.0f);
        result.add(std::make_unique<juce::AudioParameterFloat>(juce::String(dspdefs::paramIds::chSpeed) + chStr, // parameterID
                                                               "Speed " + chStr, // parameter name
                                                               speedRange, // mapping
                                                               1.0f) // default value
        );
        result.add(std::make_unique<juce::AudioParameterFloat>(juce::String(dspdefs::paramIds::chGain) + chStr, // parameterID
                                                               "Gain " + chStr, // parameter name
                                                               0.0f, // minimum value
                                                               1.0f, // maximum value
                                                               0.5f) // default value
        );
    }

    return result;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TapeLooperPluginAudioProcessor();
}
