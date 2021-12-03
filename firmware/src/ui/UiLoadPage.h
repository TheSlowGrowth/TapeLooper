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
class UiLoadPage : public daisy::UiPage
{
public:
    UiLoadPage(LooperControllerType& looperController) :
        looperController_(looperController)
    {
    }

    virtual ~UiLoadPage() {}

    bool IsOpaque(const daisy::UiCanvasDescriptor&) override { return false; }

    void Draw(const daisy::UiCanvasDescriptor& canvas) override
    {
        UiHardwareType& hardware = *((UiHardwareType*) canvas.handle_);

        // light up the load page led
        hardware.setLed(Led::load, LedColour::pulsingRed);

        // update channel octave LEDs to show the currently selected slot
        // TODO
        const auto updateChannelOctaveLeds = [&](size_t looperChannel, std::array<Led, 4> ledIds)
        {
            (void) (looperChannel);
            hardware.setLed(ledIds[0], LedColour::off);
            hardware.setLed(ledIds[1], LedColour::off);
            hardware.setLed(ledIds[2], LedColour::off);
            hardware.setLed(ledIds[3], LedColour::off);
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
            case Button::load:
                Close(); // close this page
                break;
            case Button::record:
            case Button::save:
            case Button::settings:
                Close(); // close this page
                return false; // pass event to the page below to open the respective page
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
    UiLoadPage(const UiLoadPage&) = delete;
    UiLoadPage& operator=(const UiLoadPage&) = delete;

    LooperControllerType& looperController_;
};