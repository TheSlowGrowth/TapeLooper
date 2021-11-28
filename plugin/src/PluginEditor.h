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
