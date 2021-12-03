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

#include "hardware/UiHardwareTypes.h"

class UiHardwareMock
{
public:
    struct LedState
    {
        bool setToRawValue = false;
        LedColour colour = LedColour::off;
        float brightnessRed = 0.0f;
        float brightnessGreen = 0.0f;
    };
    std::array<LedState, size_t(Led::NUM_LEDS)> leds_;

    void setLedFromRawValues(Led led, float brightnessRed, float brightnessGreen)
    {
        leds_[size_t(led)].brightnessRed = brightnessRed;
        leds_[size_t(led)].brightnessGreen = brightnessGreen;
        leds_[size_t(led)].setToRawValue = true;
    }

    void setLed(Led led, LedColour colour)
    {
        leds_[size_t(led)].colour = colour;
        leds_[size_t(led)].setToRawValue = false;
    }

    void clearLeds()
    {
        for (auto& led : leds_)
        {
            led.brightnessRed = 0.0f;
            led.brightnessGreen = 0.0f;
            led.setToRawValue = false;
            led.colour = LedColour::off;
        }
    }
};