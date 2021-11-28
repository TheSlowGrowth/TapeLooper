/**	
 * Copyright (C) Johannes Elliesen, 2021
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *  
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

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
    specs.numChannels = juce::uint32(getMainBusNumInputChannels());
    specs.maximumBlockSize = juce::uint32(samplesPerBlock) * 2u; // safety for some weird hosts
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

    loopers_.processBlock(buffer);
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
    try
    {
        loopers_.saveState(destData);
    }
    catch (const std::exception& e)
    {
        juce::NativeMessageBox::showAsync(
            juce::MessageBoxOptions()
                .withTitle(juce::String("Exception saving state"))
                .withMessage(juce::String("Exception saving state: ") + e.what())
                .withIconType(juce::MessageBoxIconType::WarningIcon),
            [](int) {});
    }
}

void TapeLooperPluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    try
    {
        loopers_.recallState(data, size_t(sizeInBytes));
    }
    catch (const std::exception& e)
    {
        juce::NativeMessageBox::showAsync(
            juce::MessageBoxOptions()
                .withTitle(juce::String("Exception recalling state"))
                .withMessage(juce::String("Exception recalling state: ") + e.what())
                .withIconType(juce::MessageBoxIconType::WarningIcon),
            [](int) {});
    }
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
                                                               1.0f, // default value
                                                               "", // label
                                                               juce::AudioProcessorParameter::genericParameter, // category
                                                               [](float value, int) { return juce::String(value, 2); }) // value-to-string function
        );
        juce::NormalisableRange<float> driveRange(0.25f, 4.0f);
        driveRange.setSkewForCentre(1.0f);
        result.add(std::make_unique<juce::AudioParameterFloat>(juce::String(dspdefs::paramIds::chDrive) + chStr, // parameterID
                                                               "Drive " + chStr, // parameter name
                                                               driveRange, // mapping
                                                               1.0f, // default value
                                                               "", // label
                                                               juce::AudioProcessorParameter::genericParameter, // category
                                                               [](float value, int) { return juce::String(value, 2); }) // value-to-string function
        );
        result.add(std::make_unique<juce::AudioParameterFloat>(juce::String(dspdefs::paramIds::chGrainAmt) + chStr, // parameterID
                                                               "GrainAmt " + chStr, // parameter name
                                                               0.0f, // minimum value
                                                               1.0f, // maximum value
                                                               0.25f) // default value
        );
        result.add(std::make_unique<juce::AudioParameterFloat>(juce::String(dspdefs::paramIds::chWowAndFlutterAmt) + chStr, // parameterID
                                                               "WowAndFlutterAmt " + chStr, // parameter name
                                                               0.0f, // minimum value
                                                               1.0f, // maximum value
                                                               0.25f) // default value
        );
        result.add(std::make_unique<juce::AudioParameterFloat>(juce::String(dspdefs::paramIds::chPostGain) + chStr, // parameterID
                                                               "PostGain " + chStr, // parameter name
                                                               0.0f, // minimum value
                                                               2.0f, // maximum value
                                                               1.0f) // default value
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
