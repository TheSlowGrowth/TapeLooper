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

#include <functional>
#include <gtest/gtest.h>

#include <../tests/TestIsolator.h> // from libDaisy
#include <sys/system.h> // from libDaisy
#include "ui/TapeLooperUi.h"

#include "mocks/LooperController_mock.h"
#include "mocks/ParameterProvider_mock.h"
#include "mocks/PeakMeter_mock.h"
#include "mocks/UiHardware_mock.h"

// custom expecation macros for googletest
// we use macros not functions, to that the line numbers in the googletest
// console log are still pointing to the correct place.

#define EXPECT_LED_COLOUR(led, expectedColour) \
    { \
        EXPECT_FALSE(uiHardware_.leds_[size_t(led)].setToRawValue); \
        EXPECT_EQ(uiHardware_.leds_[size_t(led)].colour, expectedColour); \
    }
#define EXPECT_LED_RAW(led, green, red) \
    { \
        EXPECT_TRUE(uiHardware_.leds_[size_t(led)].setToRawValue); \
        EXPECT_EQ(uiHardware_.leds_[size_t(led)].brightnessGreen, green); \
        EXPECT_EQ(uiHardware_.leds_[size_t(led)].brightnessRed, red); \
    }
#define EXPECT_OCTAVE_LED_PATTERN(channel, ledm2, ledm1, ledp1, ledp2) \
    { \
        const std::array<LedColour, 4> expectedPattern({ ledm2, ledm1, ledp1, ledp2 }); \
        const auto values = getChannelOctaveLeds(channel); \
        std::array<LedColour, 4> observedPattern; \
        std::transform(values.begin(), \
                       values.end(), \
                       observedPattern.begin(), \
                       [](UiHardwareMock::LedState led) \
                       { \
                           EXPECT_FALSE(led.setToRawValue) << "expect led to set by colour"; \
                           return led.colour; \
                       }); \
        EXPECT_EQ(observedPattern, expectedPattern); \
    }

// pretty printing for googletest
std::ostream& operator<<(std::ostream& out, LedColour colour)
{
    switch (colour)
    {
        case LedColour::off:
            return out << "LedColour::off";
        case LedColour::green:
            return out << "LedColour::green";
        case LedColour::yellow:
            return out << "LedColour::yellow";
        case LedColour::red:
            return out << "LedColour::red";
        case LedColour::pulsingGreen:
            return out << "LedColour::pulsingGreen";
        case LedColour::pulsingYellow:
            return out << "LedColour::pulsingYellow";
        case LedColour::pulsingRed:
            return out << "LedColour::pulsingRed";
    }
}

// the clear and flush functions of the UI cancas must be function pointers.
// to isolate tests that run in parallel, we use the daisy TestIsolator class
// to hold a implementation of these two functions for each test case.
struct UiTestMockFunctions
{
    std::function<void(const daisy::UiCanvasDescriptor& desc)> clearCanvasFunction;
    std::function<void(const daisy::UiCanvasDescriptor& desc)> flushCanvasFunction;
};
TestIsolator<UiTestMockFunctions> uiTestMockFunctionIsolator;

void uiTestClearFunction(const daisy::UiCanvasDescriptor& desc)
{
    auto& callbackForThisTest =
        uiTestMockFunctionIsolator.GetStateForCurrentTest()->clearCanvasFunction;
    if (callbackForThisTest)
        callbackForThisTest(desc);
}
void uiTestFlushFunction(const daisy::UiCanvasDescriptor& desc)
{
    auto& callbackForThisTest =
        uiTestMockFunctionIsolator.GetStateForCurrentTest()->flushCanvasFunction;
    if (callbackForThisTest)
        callbackForThisTest(desc);
}

// basic test fixture that sets up mocked versions of the
// LooperController, UiHardware, PeakMeters and LooperParaneterProvider
class UiFixture : public ::testing::Test
{
protected:
    UiFixture() :
        ui_(uiHardware_,
            &uiTestClearFunction,
            &uiTestFlushFunction,
            eventQueue_,
            peakMeters_,
            looperController_,
            looperParameterProvider_)
    {
        uiTestMockFunctionIsolator.GetStateForCurrentTest()->clearCanvasFunction =
            [&](const daisy::UiCanvasDescriptor&)
        {
            uiHardware_.clearLeds();
        };
    }

    // A little helper struct to simulate user input events
    struct Event
    {
        enum class Type
        {
            buttonPressedAndReleased,
            buttonPressed,
            buttonReleased,
            potTurned,
        };

        Event(Type type, Pot potId, float position) :
            type(type),
            id(uint16_t(potId)),
            position(position)
        {
        }

        template <typename IdType>
        Event(Type type, IdType id) :
            type(type),
            id(uint16_t(id)),
            position(0.0f)
        {
        }

        Type type;
        uint16_t id;
        float position;
    };

    // pushes a bunch of user input events into the event queue and makes
    // the SUT process them
    void simulateEvents(std::initializer_list<Event> events)
    {
        for (const auto& event : events)
        {
            switch (event.type)
            {
                case Event::Type::buttonPressed:
                    eventQueue_.AddButtonPressed(event.id, 1, false);
                    break;
                case Event::Type::buttonReleased:
                    eventQueue_.AddButtonReleased(event.id);
                    break;
                case Event::Type::buttonPressedAndReleased:
                    eventQueue_.AddButtonPressed(event.id, 1, false);
                    eventQueue_.AddButtonReleased(event.id);
                    break;
                case Event::Type::potTurned:
                    eventQueue_.AddPotMoved(event.id, event.position);
                    break;
                default:
                    FAIL();
            }
            while (!eventQueue_.IsQueueEmpty())
                ui_.process();
        }
    }

    // processes the SUT until the canvas was redrawn once.
    void waitUntilDisplayDrawn()
    {
        // hook into the display flush function
        auto& flushFunction = uiTestMockFunctionIsolator.GetStateForCurrentTest()->flushCanvasFunction;
        auto originalFunction = flushFunction;
        bool flushFunctionCalled = false;
        flushFunction = [&](const daisy::UiCanvasDescriptor& desc)
        {
            if (originalFunction)
                originalFunction(desc);
            flushFunctionCalled = true;
        };

        // wait until the flush function is called once.
        // This tells us that the canvas was redrawn
        size_t numCycles = 0;
        while (!flushFunctionCalled)
        {
            if (numCycles++ > 100)
                FAIL() << "Display not redrawn after simulated 1000ms";
            ui_.process();
            // simulate 10ms passed
            daisy::System::SetUsForUnitTest(daisy::System::GetUs() + 10000);
        }

        flushFunction = originalFunction;
    }

    // some helpers to get easier access to the buttons IDs for each channel
    struct ChannelButtons
    {
        Button up;
        Button down;
        Button play;
    };
    ChannelButtons getChannelButtons(size_t channel) const
    {
        return std::array {
            ChannelButtons { Button::chA_up, Button::chA_down, Button::chA_play },
            ChannelButtons { Button::chB_up, Button::chB_down, Button::chB_play },
            ChannelButtons { Button::chC_up, Button::chC_down, Button::chC_play },
            ChannelButtons { Button::chD_up, Button::chD_down, Button::chD_play },
        }[channel];
    }

    // some helpers to get easier access to the potentiometer IDs for each channel
    struct ChannelPots
    {
        Pot drive;
        Pot grain;
        Pot warble;
        Pot speed;
        Pot volume;
    };
    ChannelPots getChannelPots(size_t channel) const
    {
        return std::array {
            ChannelPots { Pot::chA_driveAmt, Pot::chA_grainAmt, Pot::chA_warbleAmt, Pot::chA_speed, Pot::chA_volume },
            ChannelPots { Pot::chB_driveAmt, Pot::chB_grainAmt, Pot::chB_warbleAmt, Pot::chB_speed, Pot::chB_volume },
            ChannelPots { Pot::chC_driveAmt, Pot::chC_grainAmt, Pot::chC_warbleAmt, Pot::chC_speed, Pot::chC_volume },
            ChannelPots { Pot::chD_driveAmt, Pot::chD_grainAmt, Pot::chD_warbleAmt, Pot::chD_speed, Pot::chD_volume },
        }[channel];
    }

    // for easier access to the octave LED IDs
    const std::array<std::array<size_t, 4>, 4> channelOctaveLeds_ = {
        std::array<size_t, 4>({ size_t(Led::chA_m2), size_t(Led::chA_m1), size_t(Led::chA_p1), size_t(Led::chA_p2) }),
        std::array<size_t, 4>({ size_t(Led::chB_m2), size_t(Led::chB_m1), size_t(Led::chB_p1), size_t(Led::chB_p2) }),
        std::array<size_t, 4>({ size_t(Led::chC_m2), size_t(Led::chC_m1), size_t(Led::chC_p1), size_t(Led::chC_p2) }),
        std::array<size_t, 4>({ size_t(Led::chD_m2), size_t(Led::chD_m1), size_t(Led::chD_p1), size_t(Led::chD_p2) }),
    };

    std::array<UiHardwareMock::LedState, 4> getChannelOctaveLeds(size_t channel)
    {
        std::array<UiHardwareMock::LedState, 4> result;
        for (size_t led = 0; led < 4; led++)
            result[led] = uiHardware_.leds_[channelOctaveLeds_[channel][led]];
        return result;
    }

    // shared tests (used for multiple pages)

    void runPotentiometerUpdateTest()
    {
        // checks if all potentiometers update their respective destination values
        // in the LooperParameterProvider

        const auto expectPotUpdatesDestination = [&](Pot pot, float& destination)
        {
            for (float value : { 0.351f, 0.934f, 0.845f })
            {
                simulateEvents({ Event(Event::Type::potTurned, pot, value) });
                EXPECT_EQ(destination, value);
            }
        };

        for (size_t ch = 0; ch < numLoopers_; ch++)
        {
            expectPotUpdatesDestination(getChannelPots(ch).drive, looperParameterProvider_.controlInputs_[ch].driveAmt);
            expectPotUpdatesDestination(getChannelPots(ch).grain, looperParameterProvider_.controlInputs_[ch].grainAmt);
            expectPotUpdatesDestination(getChannelPots(ch).speed, looperParameterProvider_.controlInputs_[ch].fineTunePosition);
            expectPotUpdatesDestination(getChannelPots(ch).warble, looperParameterProvider_.controlInputs_[ch].wowAndFlutterAmt);
            expectPotUpdatesDestination(getChannelPots(ch).volume, looperParameterProvider_.controlInputs_[ch].volumeSliderPosition);
        }
    }

    void runPeakMeterTest()
    {
        // peak meter LEDs should display the peak meter values

        // let peak meter mocks produce some non-zero values
        peakMeters_[0].green_ = 0.10f;
        peakMeters_[0].red_ = 0.20f;
        peakMeters_[1].green_ = 0.11f;
        peakMeters_[1].red_ = 0.21f;
        peakMeters_[2].green_ = 0.12f;
        peakMeters_[2].red_ = 0.22f;
        peakMeters_[3].green_ = 0.13f;
        peakMeters_[3].red_ = 0.23f;

        waitUntilDisplayDrawn();

        // expect peak meter values to be drawn to the UI
        EXPECT_EQ(uiHardware_.leds_[size_t(Led::peakMeterInL)].brightnessGreen, 0.10f);
        EXPECT_EQ(uiHardware_.leds_[size_t(Led::peakMeterInL)].brightnessRed, 0.20f);
        EXPECT_EQ(uiHardware_.leds_[size_t(Led::peakMeterInR)].brightnessGreen, 0.11f);
        EXPECT_EQ(uiHardware_.leds_[size_t(Led::peakMeterInR)].brightnessRed, 0.21f);
        EXPECT_EQ(uiHardware_.leds_[size_t(Led::peakMeterOutL)].brightnessGreen, 0.12f);
        EXPECT_EQ(uiHardware_.leds_[size_t(Led::peakMeterOutL)].brightnessRed, 0.22f);
        EXPECT_EQ(uiHardware_.leds_[size_t(Led::peakMeterOutR)].brightnessGreen, 0.13f);
        EXPECT_EQ(uiHardware_.leds_[size_t(Led::peakMeterOutR)].brightnessRed, 0.23f);
    }

    static constexpr size_t numLoopers_ = 4;
    UiHardwareMock uiHardware_;
    daisy::UiEventQueue eventQueue_;
    std::array<PeakMeterMock, numLoopers_> peakMeters_;
    LooperControllerMock<numLoopers_> looperController_;
    ParameterProviderMock<numLoopers_> looperParameterProvider_;
    TapeLooperUi<UiHardwareMock,
                 PeakMeterMock,
                 LooperControllerMock<numLoopers_>,
                 ParameterProviderMock<numLoopers_>>
        ui_;
};

// =======================================================================
// base page tests
// =======================================================================

TEST_F(UiFixture, basePage_shouldDisplayPeakMeters)
{
    runPeakMeterTest();
}

TEST_F(UiFixture, basePage_commonSectionLedsShouldBeOff)
{
    waitUntilDisplayDrawn();

    EXPECT_LED_COLOUR(Led::settings, LedColour::off);
    EXPECT_LED_COLOUR(Led::save, LedColour::off);
    EXPECT_LED_COLOUR(Led::load, LedColour::off);
    EXPECT_LED_COLOUR(Led::record, LedColour::off);
}

TEST_F(UiFixture, basePage_shouldDisplayOctave)
{
    // Octave LEDs should display the current octave setting

    // modify mock to simulate that all loopers are in a certain state
    LooperState stateOfAllLoopers = LooperState::stopped;
    looperController_.getLooperStateMock_ = [&](size_t)
    {
        return stateOfAllLoopers;
    };

    // a helper lambda to check if the octave led pattern has the correct colour in each looper state
    const auto expectOctaveLedPatternsInColour = [&](LedColour onColour)
    {
        for (size_t ch = 0; ch < numLoopers_; ch++)
        {
            const auto off = LedColour::off;
            const auto on = onColour;
            looperParameterProvider_.controlInputs_[ch].octave = -2;
            waitUntilDisplayDrawn();
            EXPECT_OCTAVE_LED_PATTERN(ch, on, off, off, off);

            looperParameterProvider_.controlInputs_[ch].octave = -1;
            waitUntilDisplayDrawn();
            EXPECT_OCTAVE_LED_PATTERN(ch, off, on, off, off);

            looperParameterProvider_.controlInputs_[ch].octave = 0;
            waitUntilDisplayDrawn();
            EXPECT_OCTAVE_LED_PATTERN(ch, off, on, on, off);

            looperParameterProvider_.controlInputs_[ch].octave = 1;
            waitUntilDisplayDrawn();
            EXPECT_OCTAVE_LED_PATTERN(ch, off, off, on, off);

            looperParameterProvider_.controlInputs_[ch].octave = 2;
            waitUntilDisplayDrawn();
            EXPECT_OCTAVE_LED_PATTERN(ch, off, off, off, on);
        }
    };

    // expect octave leds to show the octave in green while stopped
    stateOfAllLoopers = LooperState::stopped;
    expectOctaveLedPatternsInColour(LedColour::green);
    // expect octave leds to show the octave in orange while playing
    stateOfAllLoopers = LooperState::playing;
    expectOctaveLedPatternsInColour(LedColour::yellow);
    // expect octave leds to show the octave in green while stopped
    stateOfAllLoopers = LooperState::recording;
    expectOctaveLedPatternsInColour(LedColour::pulsingRed);
}

TEST_F(UiFixture, basePage_rockerSwitchesShouldModifyOctave)
{
    // use rocker switches; UI should modify the octave setting on the
    // LooperParameterProvider

    for (size_t ch = 0; ch < numLoopers_; ch++)
    {
        const auto& buttons = getChannelButtons(ch);
        const auto upButtonPressed = { Event(Event::Type::buttonPressedAndReleased, buttons.up) };
        const auto downButtonPressed = { Event(Event::Type::buttonPressedAndReleased, buttons.down) };

        looperParameterProvider_.controlInputs_[ch].octave = -2;
        simulateEvents(upButtonPressed);
        EXPECT_EQ(looperParameterProvider_.controlInputs_[ch].octave, -1);
        simulateEvents(upButtonPressed);
        EXPECT_EQ(looperParameterProvider_.controlInputs_[ch].octave, 0);
        simulateEvents(upButtonPressed);
        EXPECT_EQ(looperParameterProvider_.controlInputs_[ch].octave, 1);
        simulateEvents(upButtonPressed);
        EXPECT_EQ(looperParameterProvider_.controlInputs_[ch].octave, 2);
        simulateEvents(upButtonPressed);
        EXPECT_EQ(looperParameterProvider_.controlInputs_[ch].octave, 2); // limited at the top
        simulateEvents(downButtonPressed);
        EXPECT_EQ(looperParameterProvider_.controlInputs_[ch].octave, 1);
        simulateEvents(downButtonPressed);
        EXPECT_EQ(looperParameterProvider_.controlInputs_[ch].octave, 0);
        simulateEvents(downButtonPressed);
        EXPECT_EQ(looperParameterProvider_.controlInputs_[ch].octave, -1);
        simulateEvents(downButtonPressed);
        EXPECT_EQ(looperParameterProvider_.controlInputs_[ch].octave, -2);
        simulateEvents(downButtonPressed);
        EXPECT_EQ(looperParameterProvider_.controlInputs_[ch].octave, -2); // limited at the bottom
    }
}

TEST_F(UiFixture, basePage_playButtonShouldStartAndStopPlayback)
{
    // the playback button should start/stop playback

    // make looper controller mock return values from an array
    std::array<LooperState, numLoopers_> looperStates = { LooperState::stopped };
    looperController_.getLooperStateMock_ = [&](size_t channel)
    {
        return looperStates[channel];
    };

    for (size_t ch = 0; ch < numLoopers_; ch++)
    {
        std::string chStr = std::to_string(ch);
        const auto playButtonPressed = { Event(Event::Type::buttonPressedAndReleased, getChannelButtons(ch).play) };

        // start playback when stopped
        looperStates[ch] = LooperState::stopped;
        simulateEvents(playButtonPressed);
        EXPECT_EQ(looperController_.pullEvents(), "setLooperState(" + chStr + ", LooperState::playing)\n");

        // stop playback when playing
        looperStates[ch] = LooperState::playing;
        simulateEvents(playButtonPressed);
        EXPECT_EQ(looperController_.pullEvents(), "setLooperState(" + chStr + ", LooperState::stopped)\n");

        // start playback when recording
        looperStates[ch] = LooperState::recording;
        simulateEvents(playButtonPressed);
        EXPECT_EQ(looperController_.pullEvents(), "setLooperState(" + chStr + ", LooperState::playing)\n");
    }
}

TEST_F(UiFixture, basePage_shouldUpdateParameterProviderWithPotValues)
{
    runPotentiometerUpdateTest();
}

// =======================================================================
// record page tests
// =======================================================================

TEST_F(UiFixture, recordingPage_shouldUpdateParameterProviderWithPotValues)
{
    // enter recording page
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::record) });
    // potentiometer updates should still get through
    runPotentiometerUpdateTest();
}

TEST_F(UiFixture, recordingPage_shouldDisplayPeakMeters)
{
    // enter recording page
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::record) });
    // peak meters are still functional
    runPeakMeterTest();
}

TEST_F(UiFixture, recordingPage_onlyRecordLedShouldBeOn)
{
    // enter recording page
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::record) });

    waitUntilDisplayDrawn();
    EXPECT_LED_COLOUR(Led::settings, LedColour::off);
    EXPECT_LED_COLOUR(Led::save, LedColour::off);
    EXPECT_LED_COLOUR(Led::load, LedColour::off);
    EXPECT_LED_COLOUR(Led::record, LedColour::pulsingRed);
}

TEST_F(UiFixture, recordingPage_exitRecordPage)
{
    // enter recording page
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::record) });
    waitUntilDisplayDrawn();
    EXPECT_LED_COLOUR(Led::settings, LedColour::off);
    EXPECT_LED_COLOUR(Led::save, LedColour::off);
    EXPECT_LED_COLOUR(Led::load, LedColour::off);
    EXPECT_LED_COLOUR(Led::record, LedColour::pulsingRed);

    // leave towards settings page
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::settings) });
    waitUntilDisplayDrawn();
    EXPECT_LED_COLOUR(Led::settings, LedColour::pulsingRed);
    EXPECT_LED_COLOUR(Led::save, LedColour::off);
    EXPECT_LED_COLOUR(Led::load, LedColour::off);
    EXPECT_LED_COLOUR(Led::record, LedColour::off);

    // enter recording page again
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::record) });
    waitUntilDisplayDrawn();
    EXPECT_LED_COLOUR(Led::settings, LedColour::off);
    EXPECT_LED_COLOUR(Led::save, LedColour::off);
    EXPECT_LED_COLOUR(Led::load, LedColour::off);
    EXPECT_LED_COLOUR(Led::record, LedColour::pulsingRed);

    // leave towards save page
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::save) });
    waitUntilDisplayDrawn();
    EXPECT_LED_COLOUR(Led::settings, LedColour::off);
    EXPECT_LED_COLOUR(Led::save, LedColour::pulsingRed);
    EXPECT_LED_COLOUR(Led::load, LedColour::off);
    EXPECT_LED_COLOUR(Led::record, LedColour::off);

    // enter recording page again
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::record) });
    waitUntilDisplayDrawn();
    EXPECT_LED_COLOUR(Led::settings, LedColour::off);
    EXPECT_LED_COLOUR(Led::save, LedColour::off);
    EXPECT_LED_COLOUR(Led::load, LedColour::off);
    EXPECT_LED_COLOUR(Led::record, LedColour::pulsingRed);

    // leave towards load page
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::load) });
    waitUntilDisplayDrawn();
    EXPECT_LED_COLOUR(Led::settings, LedColour::off);
    EXPECT_LED_COLOUR(Led::save, LedColour::off);
    EXPECT_LED_COLOUR(Led::load, LedColour::pulsingRed);
    EXPECT_LED_COLOUR(Led::record, LedColour::off);

    // enter recording page again
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::record) });
    waitUntilDisplayDrawn();
    EXPECT_LED_COLOUR(Led::settings, LedColour::off);
    EXPECT_LED_COLOUR(Led::save, LedColour::off);
    EXPECT_LED_COLOUR(Led::load, LedColour::off);
    EXPECT_LED_COLOUR(Led::record, LedColour::pulsingRed);

    // leave towards base page
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::record) });
    waitUntilDisplayDrawn();
    EXPECT_LED_COLOUR(Led::settings, LedColour::off);
    EXPECT_LED_COLOUR(Led::save, LedColour::off);
    EXPECT_LED_COLOUR(Led::load, LedColour::off);
    EXPECT_LED_COLOUR(Led::record, LedColour::off);
}

TEST_F(UiFixture, recordingPage_playButtonShouldStartAndStopRecording)
{
    // the playback button should start/stop recording

    // enter recording page
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::record) });

    // make looper controller mock return values from an array
    std::array<LooperState, numLoopers_> looperStates = { LooperState::stopped };
    looperController_.getLooperStateMock_ = [&](size_t channel)
    {
        return looperStates[channel];
    };

    for (size_t ch = 0; ch < numLoopers_; ch++)
    {
        std::string chStr = std::to_string(ch);
        const auto playButtonPressed = { Event(Event::Type::buttonPressedAndReleased, getChannelButtons(ch).play) };

        // start recording when stopped
        looperStates[ch] = LooperState::stopped;
        simulateEvents(playButtonPressed);
        EXPECT_EQ(looperController_.pullEvents(), "setLooperState(" + chStr + ", LooperState::recording)\n");

        // start recording when playing
        looperStates[ch] = LooperState::playing;
        simulateEvents(playButtonPressed);
        EXPECT_EQ(looperController_.pullEvents(), "setLooperState(" + chStr + ", LooperState::recording)\n");

        // stop recording when recording
        looperStates[ch] = LooperState::recording;
        simulateEvents(playButtonPressed);
        EXPECT_EQ(looperController_.pullEvents(), "setLooperState(" + chStr + ", LooperState::stopped)\n");
    }
}

TEST_F(UiFixture, recordingPage_leavingPageDoesntStopRecording)
{
    // the playback button should start/stop recording

    // enter recording page
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::record) });

    // make looper controller mock return LooperState::recording for all channels
    looperController_.getLooperStateMock_ = [&](size_t)
    {
        return LooperState::recording;
    };

    // leave recording page
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::record) });

    // nothing happened, recording continues
    EXPECT_EQ(looperController_.pullEvents(), "");
}

TEST_F(UiFixture, recordingPage_rockerSwitchesShouldHaveNoEffect)
{
    // the up/down switches don't do anything

    // enter recording page
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::record) });

    // activate the rocker switches a bunch of times
    simulateEvents({
        Event(Event::Type::buttonPressedAndReleased, Button::chA_up),
        Event(Event::Type::buttonPressedAndReleased, Button::chA_down),
        Event(Event::Type::buttonPressedAndReleased, Button::chA_up),
        Event(Event::Type::buttonPressedAndReleased, Button::chA_down),

        Event(Event::Type::buttonPressedAndReleased, Button::chB_up),
        Event(Event::Type::buttonPressedAndReleased, Button::chB_down),
        Event(Event::Type::buttonPressedAndReleased, Button::chB_up),
        Event(Event::Type::buttonPressedAndReleased, Button::chB_down),

        Event(Event::Type::buttonPressedAndReleased, Button::chC_up),
        Event(Event::Type::buttonPressedAndReleased, Button::chC_down),
        Event(Event::Type::buttonPressedAndReleased, Button::chC_up),
        Event(Event::Type::buttonPressedAndReleased, Button::chC_down),

        Event(Event::Type::buttonPressedAndReleased, Button::chD_up),
        Event(Event::Type::buttonPressedAndReleased, Button::chD_down),
        Event(Event::Type::buttonPressedAndReleased, Button::chD_up),
        Event(Event::Type::buttonPressedAndReleased, Button::chD_down),
    });

    // nothing happened
    EXPECT_EQ(looperController_.pullEvents(), "");
}

// =======================================================================
// settings page tests
// =======================================================================

TEST_F(UiFixture, settingsPage_shouldUpdateParameterProviderWithPotValues)
{
    // enter settings page
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::settings) });
    // potentiometer updates should still get through
    runPotentiometerUpdateTest();
}

TEST_F(UiFixture, settingsPage_shouldDisplayPeakMeters)
{
    // enter settings page
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::settings) });
    // peak meters are still functional
    runPeakMeterTest();
}

TEST_F(UiFixture, settingsPage_settingsLedShouldCycleThroughColours)
{
    // enter settings page
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::settings) });
    waitUntilDisplayDrawn();
    EXPECT_LED_COLOUR(Led::settings, LedColour::pulsingRed);
    EXPECT_LED_COLOUR(Led::save, LedColour::off);
    EXPECT_LED_COLOUR(Led::load, LedColour::off);
    EXPECT_LED_COLOUR(Led::record, LedColour::off);

    // cycle to next setting
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::settings) });
    waitUntilDisplayDrawn();
    EXPECT_LED_COLOUR(Led::settings, LedColour::pulsingYellow);
    EXPECT_LED_COLOUR(Led::save, LedColour::off);
    EXPECT_LED_COLOUR(Led::load, LedColour::off);
    EXPECT_LED_COLOUR(Led::record, LedColour::off);

    // cycle to next setting
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::settings) });
    waitUntilDisplayDrawn();
    EXPECT_LED_COLOUR(Led::settings, LedColour::pulsingGreen);
    EXPECT_LED_COLOUR(Led::save, LedColour::off);
    EXPECT_LED_COLOUR(Led::load, LedColour::off);
    EXPECT_LED_COLOUR(Led::record, LedColour::off);

    // return to the base page
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::settings) });
    waitUntilDisplayDrawn();
    EXPECT_LED_COLOUR(Led::settings, LedColour::off);
    EXPECT_LED_COLOUR(Led::save, LedColour::off);
    EXPECT_LED_COLOUR(Led::load, LedColour::off);
    EXPECT_LED_COLOUR(Led::record, LedColour::off);
}

TEST_F(UiFixture, settingsPage_exitSettingsPage)
{
    // enter settings page
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::settings) });
    waitUntilDisplayDrawn();
    EXPECT_LED_COLOUR(Led::settings, LedColour::pulsingRed);
    EXPECT_LED_COLOUR(Led::save, LedColour::off);
    EXPECT_LED_COLOUR(Led::load, LedColour::off);
    EXPECT_LED_COLOUR(Led::record, LedColour::off);

    // leave towards save page
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::save) });
    waitUntilDisplayDrawn();
    EXPECT_LED_COLOUR(Led::settings, LedColour::off);
    EXPECT_LED_COLOUR(Led::save, LedColour::pulsingRed);
    EXPECT_LED_COLOUR(Led::load, LedColour::off);
    EXPECT_LED_COLOUR(Led::record, LedColour::off);

    // enter settings page again
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::settings) });
    waitUntilDisplayDrawn();
    EXPECT_LED_COLOUR(Led::settings, LedColour::pulsingRed);
    EXPECT_LED_COLOUR(Led::save, LedColour::off);
    EXPECT_LED_COLOUR(Led::load, LedColour::off);
    EXPECT_LED_COLOUR(Led::record, LedColour::off);

    // leave towards load page
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::load) });
    waitUntilDisplayDrawn();
    EXPECT_LED_COLOUR(Led::settings, LedColour::off);
    EXPECT_LED_COLOUR(Led::save, LedColour::off);
    EXPECT_LED_COLOUR(Led::load, LedColour::pulsingRed);
    EXPECT_LED_COLOUR(Led::record, LedColour::off);

    // enter settings page again
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::settings) });
    waitUntilDisplayDrawn();
    EXPECT_LED_COLOUR(Led::settings, LedColour::pulsingRed);
    EXPECT_LED_COLOUR(Led::save, LedColour::off);
    EXPECT_LED_COLOUR(Led::load, LedColour::off);
    EXPECT_LED_COLOUR(Led::record, LedColour::off);

    // leave towards recording page
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::record) });
    waitUntilDisplayDrawn();
    EXPECT_LED_COLOUR(Led::settings, LedColour::off);
    EXPECT_LED_COLOUR(Led::save, LedColour::off);
    EXPECT_LED_COLOUR(Led::load, LedColour::off);
    EXPECT_LED_COLOUR(Led::record, LedColour::pulsingRed);

    // enter settings page again
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::settings) });
    waitUntilDisplayDrawn();
    EXPECT_LED_COLOUR(Led::settings, LedColour::pulsingRed);
    EXPECT_LED_COLOUR(Led::save, LedColour::off);
    EXPECT_LED_COLOUR(Led::load, LedColour::off);
    EXPECT_LED_COLOUR(Led::record, LedColour::off);

    // leave towards base page (cycle through settings, then return to base page)
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::settings) });
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::settings) });
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::settings) });
    waitUntilDisplayDrawn();
    EXPECT_LED_COLOUR(Led::settings, LedColour::off);
    EXPECT_LED_COLOUR(Led::save, LedColour::off);
    EXPECT_LED_COLOUR(Led::load, LedColour::off);
    EXPECT_LED_COLOUR(Led::record, LedColour::off);
}

TEST_F(UiFixture, settingsPage_adjustDirection)
{
    // enter settings page
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::settings) });

    // make looper controler mock return specific settings
    std::array<Direction, numLoopers_> looperDirections;
    looperController_.getDirectionMock_ = [&](size_t ch)
    {
        return looperDirections[ch];
    };

    // check if rocker switches modify the value
    for (size_t ch = 0; ch < numLoopers_; ch++)
    {
        const auto chStr = std::to_string(ch);

        looperDirections[ch] = Direction::forwards;
        // nothing happens when pressing up while direction is already forwards
        simulateEvents({ Event(Event::Type::buttonPressedAndReleased, getChannelButtons(ch).up) });
        EXPECT_EQ(looperController_.pullEvents(), "");
        // pressing down while direction is forward reverses playback
        simulateEvents({ Event(Event::Type::buttonPressedAndReleased, getChannelButtons(ch).down) });
        EXPECT_EQ(looperController_.pullEvents(), "setDirection(" + chStr + ", Direction::backwards)\n");

        looperDirections[ch] = Direction::backwards;
        // nothing happens when pressing down while direction is already backwards
        simulateEvents({ Event(Event::Type::buttonPressedAndReleased, getChannelButtons(ch).down) });
        EXPECT_EQ(looperController_.pullEvents(), "");
        // pressing up while direction is backwards reverses playback
        simulateEvents({ Event(Event::Type::buttonPressedAndReleased, getChannelButtons(ch).up) });
        EXPECT_EQ(looperController_.pullEvents(), "setDirection(" + chStr + ", Direction::forwards)\n");
    }
}

TEST_F(UiFixture, settingsPage_adjustChannelLayout)
{
    // enter settings page
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::settings) });
    // cycle to the channel layout setting
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::settings) });

    // make looper controler mock return specific settings
    std::array<ChannelLayout, numLoopers_> looperChannelLayouts;
    looperController_.getChannelLayoutMock_ = [&](size_t ch)
    {
        return looperChannelLayouts[ch];
    };

    // check if rocker switches modify the value
    for (size_t ch = 0; ch < numLoopers_; ch++)
    {
        const auto chStr = std::to_string(ch);

        looperChannelLayouts[ch] = ChannelLayout::stereo;
        // nothing happens when pressing up while layout is already stereo
        simulateEvents({ Event(Event::Type::buttonPressedAndReleased, getChannelButtons(ch).up) });
        EXPECT_EQ(looperController_.pullEvents(), "");
        // pressing down while layout is stereo switches to mono
        simulateEvents({ Event(Event::Type::buttonPressedAndReleased, getChannelButtons(ch).down) });
        EXPECT_EQ(looperController_.pullEvents(), "setChannelLayout(" + chStr + ", ChannelLayout::mono)\n");

        looperChannelLayouts[ch] = ChannelLayout::mono;
        // nothing happens when pressing down while layout is already mono
        simulateEvents({ Event(Event::Type::buttonPressedAndReleased, getChannelButtons(ch).down) });
        EXPECT_EQ(looperController_.pullEvents(), "");
        // pressing up while layout is mono switches to stereo
        simulateEvents({ Event(Event::Type::buttonPressedAndReleased, getChannelButtons(ch).up) });
        EXPECT_EQ(looperController_.pullEvents(), "setChannelLayout(" + chStr + ", ChannelLayout::stereo)\n");
    }
}

TEST_F(UiFixture, settingsPage_adjustMotorLag)
{
    // enter settings page
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::settings) });
    // cycle to the motor lag setting
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::settings) });
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::settings) });

    // make looper controler mock return specific settings
    std::array<MotorAcceleration, numLoopers_> looperMotorAccelerations;
    looperController_.getMotorAccelerationMock_ = [&](size_t ch)
    {
        return looperMotorAccelerations[ch];
    };

    // check if rocker switches modify the value
    for (size_t ch = 0; ch < numLoopers_; ch++)
    {
        const auto chStr = std::to_string(ch);

        looperMotorAccelerations[ch] = MotorAcceleration::verySlow;
        simulateEvents({ Event(Event::Type::buttonPressedAndReleased, getChannelButtons(ch).down) });
        EXPECT_EQ(looperController_.pullEvents(), ""); // nothing happens, already slowest setting
        simulateEvents({ Event(Event::Type::buttonPressedAndReleased, getChannelButtons(ch).up) });
        EXPECT_EQ(looperController_.pullEvents(), "setMotorAcceleration(" + chStr + ", MotorAcceleration::slow)\n");

        looperMotorAccelerations[ch] = MotorAcceleration::slow;
        simulateEvents({ Event(Event::Type::buttonPressedAndReleased, getChannelButtons(ch).down) });
        EXPECT_EQ(looperController_.pullEvents(), "setMotorAcceleration(" + chStr + ", MotorAcceleration::verySlow)\n");
        simulateEvents({ Event(Event::Type::buttonPressedAndReleased, getChannelButtons(ch).up) });
        EXPECT_EQ(looperController_.pullEvents(), "setMotorAcceleration(" + chStr + ", MotorAcceleration::medium)\n");

        looperMotorAccelerations[ch] = MotorAcceleration::medium;
        simulateEvents({ Event(Event::Type::buttonPressedAndReleased, getChannelButtons(ch).down) });
        EXPECT_EQ(looperController_.pullEvents(), "setMotorAcceleration(" + chStr + ", MotorAcceleration::slow)\n");
        simulateEvents({ Event(Event::Type::buttonPressedAndReleased, getChannelButtons(ch).up) });
        EXPECT_EQ(looperController_.pullEvents(), "setMotorAcceleration(" + chStr + ", MotorAcceleration::fast)\n");

        looperMotorAccelerations[ch] = MotorAcceleration::fast;
        simulateEvents({ Event(Event::Type::buttonPressedAndReleased, getChannelButtons(ch).down) });
        EXPECT_EQ(looperController_.pullEvents(), "setMotorAcceleration(" + chStr + ", MotorAcceleration::medium)\n");
        simulateEvents({ Event(Event::Type::buttonPressedAndReleased, getChannelButtons(ch).up) });
        EXPECT_EQ(looperController_.pullEvents(), "setMotorAcceleration(" + chStr + ", MotorAcceleration::veryFast)\n");

        looperMotorAccelerations[ch] = MotorAcceleration::veryFast;
        simulateEvents({ Event(Event::Type::buttonPressedAndReleased, getChannelButtons(ch).down) });
        EXPECT_EQ(looperController_.pullEvents(), "setMotorAcceleration(" + chStr + ", MotorAcceleration::fast)\n");
        simulateEvents({ Event(Event::Type::buttonPressedAndReleased, getChannelButtons(ch).up) });
        EXPECT_EQ(looperController_.pullEvents(), ""); // nothing happens, already fastest setting
    }
}

TEST_F(UiFixture, settingsPage_playButtonsDoNothing)
{
    // enter settings page
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::settings) });

    const auto pressPlayButtons = [&]()
    {
        simulateEvents({
            Event(Event::Type::buttonPressedAndReleased, Button::chA_play),
            Event(Event::Type::buttonPressedAndReleased, Button::chB_play),
            Event(Event::Type::buttonPressedAndReleased, Button::chC_play),
            Event(Event::Type::buttonPressedAndReleased, Button::chD_play),
        });
    };

    // nothing happens when controlling the direction
    pressPlayButtons();
    EXPECT_EQ(looperController_.pullEvents(), "");

    // nothing happens when controlling the channel layout
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::settings) });
    pressPlayButtons();
    EXPECT_EQ(looperController_.pullEvents(), "");

    // nothing happens when controlling the motor lag
    simulateEvents({ Event(Event::Type::buttonPressedAndReleased, Button::settings) });
    pressPlayButtons();
    EXPECT_EQ(looperController_.pullEvents(), "");
}
