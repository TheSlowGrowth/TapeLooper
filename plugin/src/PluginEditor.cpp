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
TapeLooperPluginAudioProcessorEditor::TapeLooperPluginAudioProcessorEditor(TapeLooperPluginAudioProcessor& p) :
    AudioProcessorEditor(&p), processorRef(p)
{
    juce::ignoreUnused(processorRef);

    for (size_t i = 0; i < dspdefs::numLoopers; i++)
    {
        const juce::String chStr(i);
        auto comp = std::make_unique<TapeLooperComponent>(
            *(juce::AudioParameterChoice*) p.parameters_.getParameter(dspdefs::paramIds::chState + chStr),
            *(juce::AudioParameterFloat*) p.parameters_.getParameter(dspdefs::paramIds::chSpeed + chStr),
            *(juce::AudioParameterFloat*) p.parameters_.getParameter(dspdefs::paramIds::chDrive + chStr),
            *(juce::AudioParameterFloat*) p.parameters_.getParameter(dspdefs::paramIds::chGrainAmt + chStr),
            *(juce::AudioParameterFloat*) p.parameters_.getParameter(dspdefs::paramIds::chWowAndFlutterAmt + chStr),
            *(juce::AudioParameterFloat*) p.parameters_.getParameter(dspdefs::paramIds::chPostGain + chStr));
        addAndMakeVisible(*comp);
        looperComponents_.add(std::move(comp));
    }

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 600);
}

TapeLooperPluginAudioProcessorEditor::~TapeLooperPluginAudioProcessorEditor()
{
}

//==============================================================================
void TapeLooperPluginAudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void TapeLooperPluginAudioProcessorEditor::resized()
{
    const auto numComponents = looperComponents_.size();
    const auto looperCompWidth = float(getWidth()) / float(numComponents);
    for (int i = 0; i < numComponents; i++)
    {
        const auto rect = juce::Rectangle<float>(0.0f, 0.0f, looperCompWidth, float(getHeight()));
        looperComponents_[i]->setBounds(rect.withPosition(looperCompWidth * float(i), 0.0f).toNearestInt());
    }
}
