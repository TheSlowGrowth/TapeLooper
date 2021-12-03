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

#include <array>
#include <memory>

#include <ui/UI.h>
#include <dsp/TapeLooper.h>

#include "../constants.h"
#include "../hardware/UiHardwareTypes.h"
#include "LooperParameterProvider.h"

template <typename LooperControllerType, typename UiHardwareType>
class UiRecordingPage : public daisy::UiPage
{
public:
    UiRecordingPage(LooperControllerType& looperController) :
        looperController_(looperController)
    {
    }

    virtual ~UiRecordingPage() {}

    bool IsOpaque(const daisy::UiCanvasDescriptor&) override { return false; }

    void Draw(const daisy::UiCanvasDescriptor& canvas) override
    {
        UiHardwareType& hardware = *((UiHardwareType*) canvas.handle_);

        // light up the record page led
        hardware.setLed(Led::record, LedColour::pulsingRed);

        // update channel octave LEDs to reflect the recording state
        const auto updateChannelOctaveLeds = [&](size_t looperChannel, std::array<Led, 4> ledIds)
        {
            const auto colourToUse = looperController_.getLooperState(looperChannel)
                                             == LooperState::recording
                                         ? LedColour::pulsingRed
                                         : LedColour::off;
            hardware.setLed(ledIds[0], colourToUse);
            hardware.setLed(ledIds[1], colourToUse);
            hardware.setLed(ledIds[2], colourToUse);
            hardware.setLed(ledIds[3], colourToUse);
        };
        updateChannelOctaveLeds(0, { Led::chA_m2, Led::chA_m1, Led::chA_p1, Led::chA_p2 });
        updateChannelOctaveLeds(1, { Led::chB_m2, Led::chB_m1, Led::chB_p1, Led::chB_p2 });
        updateChannelOctaveLeds(2, { Led::chC_m2, Led::chC_m1, Led::chC_p1, Led::chC_p2 });
        updateChannelOctaveLeds(3, { Led::chD_m2, Led::chD_m1, Led::chD_p1, Led::chD_p2 });
    }

    bool OnButton(uint16_t buttonID, uint8_t numberOfPresses, bool isRetriggering) override
    {
        (void) (isRetriggering); // ignore this argument

        // swallow but ignore button-up messages
        if (numberOfPresses < 1)
            return true;

        const Button button = Button(buttonID);
        switch (button)
        {
            // channel play buttons
            case Button::chA_play:
                onPlayButton(0);
                break;
            case Button::chB_play:
                onPlayButton(1);
                break;
            case Button::chC_play:
                onPlayButton(2);
                break;
            case Button::chD_play:
                onPlayButton(3);
                break;
            case Button::record:
                Close(); // close this page
                break;
            case Button::save:
            case Button::load:
            case Button::settings:
                Close(); // close this page
                return false; // pass event to the page below to open the respective page
                break;
            default:
                break;
        }
        return true;
    }

    bool OnPotMoved(uint16_t, float) override
    {
        return false; // passthrough pot events to the base page
    }

private:
    UiRecordingPage(const UiRecordingPage&) = delete;
    UiRecordingPage& operator=(const UiRecordingPage&) = delete;

    void onPlayButton(size_t looperChannel)
    {
        const auto currentState = looperController_.getLooperState(looperChannel);
        if (currentState != LooperState::recording)
            looperController_.setLooperState(looperChannel, LooperState::recording);
        else
            looperController_.setLooperState(looperChannel, LooperState::stopped);
    }

    LooperControllerType& looperController_;
};