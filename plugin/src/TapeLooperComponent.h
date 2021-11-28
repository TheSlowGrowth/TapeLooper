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

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_plugin_client/juce_audio_plugin_client.h>

class PlayStopRecButtons : public juce::Component, private juce::Button::Listener
{
public:
    PlayStopRecButtons(juce::AudioParameterChoice& playStateParameter) :
        paramAttachment_(playStateParameter, [this](float val) { updateButtons(val); })
    {
        recButton_.setButtonText("Rec");
        addAndMakeVisible(recButton_);
        recButton_.addListener(this);

        revButton_.setButtonText("<");
        addAndMakeVisible(revButton_);
        revButton_.addListener(this);

        stopButton_.setButtonText("Stop");
        addAndMakeVisible(stopButton_);
        stopButton_.addListener(this);

        forwButton_.setButtonText(">");
        addAndMakeVisible(forwButton_);
        forwButton_.addListener(this);

        paramAttachment_.sendInitialUpdate();
    }

    ~PlayStopRecButtons() override
    {
        recButton_.removeListener(this);
        revButton_.removeListener(this);
        stopButton_.removeListener(this);
        forwButton_.removeListener(this);
    }

    void resized() override
    {
        auto bounds = getLocalBounds();
        const auto quarterOfWidth = int(float(getWidth()) / 4.0f);

        recButton_.setBounds(bounds.removeFromTop(getHeight() / 2));
        revButton_.setBounds(bounds.removeFromLeft(quarterOfWidth));
        forwButton_.setBounds(bounds.removeFromRight(quarterOfWidth));
        stopButton_.setBounds(bounds);
    }

private:
    juce::TextButton recButton_;
    juce::TextButton revButton_;
    juce::TextButton stopButton_;
    juce::TextButton forwButton_;
    juce::ParameterAttachment paramAttachment_;

    void updateButtons(float val)
    {
        const auto choice = int(std::round(val));
        revButton_.setToggleState(choice == 0, juce::NotificationType::dontSendNotification);
        stopButton_.setToggleState(choice == 1, juce::NotificationType::dontSendNotification);
        forwButton_.setToggleState(choice == 2, juce::NotificationType::dontSendNotification);
        recButton_.setToggleState(choice == 3, juce::NotificationType::dontSendNotification);
    }

    void buttonClicked(juce::Button* bttn) override
    {
        if (bttn == &revButton_)
            paramAttachment_.setValueAsCompleteGesture(0.0f);
        else if (bttn == &stopButton_)
            paramAttachment_.setValueAsCompleteGesture(1.0f);
        else if (bttn == &forwButton_)
            paramAttachment_.setValueAsCompleteGesture(2.0f);
        else if (bttn == &recButton_)
            paramAttachment_.setValueAsCompleteGesture(3.0f);
    }
};

class TapeLooperComponent : public juce::Component
{
public:
    TapeLooperComponent(juce::AudioParameterChoice& playStateParameter,
                        juce::AudioParameterFloat& speedParameter,
                        juce::AudioParameterFloat& driveParameter,
                        juce::AudioParameterFloat& grainAmtParameter,
                        juce::AudioParameterFloat& wowAndFlutterAmtParameter,
                        juce::AudioParameterFloat& postGainParameter) :
        playStopRecButtons_(playStateParameter),
        speedSliderAttachment_(speedParameter, speedSlider_),
        driveSliderAttachment_(driveParameter, driveSlider_),
        grainAmtSliderAttachment_(grainAmtParameter, grainAmtSlider_),
        wowAndFlutterAmtSliderAttachment_(wowAndFlutterAmtParameter, wowAndFlutterAmtSlider_),
        postGainSliderAttachment_(postGainParameter, postGainSlider_)
    {
        addAndMakeVisible(playStopRecButtons_);

        speedSlider_.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        speedSlider_.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 40, 25);
        addAndMakeVisible(speedSlider_);

        driveSlider_.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        driveSlider_.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 40, 25);
        addAndMakeVisible(driveSlider_);

        grainAmtSlider_.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        grainAmtSlider_.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 40, 25);
        addAndMakeVisible(grainAmtSlider_);

        wowAndFlutterAmtSlider_.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        wowAndFlutterAmtSlider_.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 40, 25);
        addAndMakeVisible(wowAndFlutterAmtSlider_);

        postGainSlider_.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        postGainSlider_.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 40, 25);
        addAndMakeVisible(postGainSlider_);

        speedSliderAttachment_.sendInitialUpdate();
        driveSliderAttachment_.sendInitialUpdate();
        grainAmtSliderAttachment_.sendInitialUpdate();
        wowAndFlutterAmtSliderAttachment_.sendInitialUpdate();
        postGainSliderAttachment_.sendInitialUpdate();
    }

    void resized() override
    {
        auto bounds = getLocalBounds().reduced(1);

        const auto buttonHeight = 25;

        playStopRecButtons_.setBounds(bounds.removeFromTop(2 * buttonHeight));
        bounds.removeFromTop(1); // padding
        const auto rotarySliderWidth = std::min(bounds.getWidth(), 60);
        speedSlider_.setBounds(bounds.removeFromTop(rotarySliderWidth + buttonHeight));
        bounds.removeFromTop(1); // padding
        driveSlider_.setBounds(bounds.removeFromTop(rotarySliderWidth + buttonHeight));
        bounds.removeFromTop(1); // padding
        grainAmtSlider_.setBounds(bounds.removeFromTop(rotarySliderWidth + buttonHeight));
        bounds.removeFromTop(1); // padding
        wowAndFlutterAmtSlider_.setBounds(bounds.removeFromTop(rotarySliderWidth + buttonHeight));
        bounds.removeFromTop(1); // padding
        postGainSlider_.setBounds(bounds);
    }

    void paint(juce::Graphics& g) override
    {
        const auto drawVerticalText = [&](const juce::String& text, juce::Rectangle<int> boundingBox) {
            g.saveState();
            g.setOrigin(boundingBox.getCentre());
            g.addTransform(juce::AffineTransform().rotated(-juce::MathConstants<float>::halfPi));
            g.drawSingleLineText(text,
                                 0,
                                 0,
                                 juce::Justification::horizontallyCentred);
            g.restoreState();
        };

        g.setColour(findColour(juce::Label::ColourIds::textColourId));
        drawVerticalText("Speed", speedSlider_.getBounds().withWidth(20));
        drawVerticalText("Drive", driveSlider_.getBounds().withWidth(20));
        drawVerticalText("Grain", grainAmtSlider_.getBounds().withWidth(20));
        drawVerticalText("Wow & Flutter", wowAndFlutterAmtSlider_.getBounds().withWidth(20));
        drawVerticalText("Volume", postGainSlider_.getBounds().withWidth(20));
    }

private:
    PlayStopRecButtons playStopRecButtons_;
    juce::Slider speedSlider_;
    juce::SliderParameterAttachment speedSliderAttachment_;
    juce::Slider driveSlider_;
    juce::SliderParameterAttachment driveSliderAttachment_;
    juce::Slider grainAmtSlider_;
    juce::SliderParameterAttachment grainAmtSliderAttachment_;
    juce::Slider wowAndFlutterAmtSlider_;
    juce::SliderParameterAttachment wowAndFlutterAmtSliderAttachment_;
    juce::Slider postGainSlider_;
    juce::SliderParameterAttachment postGainSliderAttachment_;
};
