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
                        juce::AudioParameterFloat& gainParameter) :
        playStopRecButtons_(playStateParameter),
        speedSliderAttachment_(speedParameter, speedSlider_),
        gainSliderAttachment_(gainParameter, gainSlider_)
    {
        addAndMakeVisible(playStopRecButtons_);

        speedSlider_.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        speedSlider_.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 40, 25);
        addAndMakeVisible(speedSlider_);

        gainSlider_.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        gainSlider_.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 40, 25);
        addAndMakeVisible(gainSlider_);

        speedSliderAttachment_.sendInitialUpdate();
        gainSliderAttachment_.sendInitialUpdate();
    }

    void resized() override
    {
        auto bounds = getLocalBounds().reduced(1);

        const auto buttonHeight = 25;

        playStopRecButtons_.setBounds(bounds.removeFromTop(2 * buttonHeight));
        bounds.removeFromTop(1); // padding
        const auto speedSliderWidth = std::min(bounds.getWidth(), 60);
        speedSlider_.setBounds(bounds.removeFromTop(speedSliderWidth + buttonHeight));
        bounds.removeFromTop(1); // padding
        gainSlider_.setBounds(bounds);
    }

private:
    PlayStopRecButtons playStopRecButtons_;
    juce::Slider speedSlider_;
    juce::SliderParameterAttachment speedSliderAttachment_;
    juce::Slider gainSlider_;
    juce::SliderParameterAttachment gainSliderAttachment_;
};
