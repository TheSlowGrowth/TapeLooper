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

class KnobsAndCvMock
{
public:
    float getCvRaw(CvInput cv) { return cvRawValues_[int(cv)]; }
    CvCalibrationData& getCalibrationData() { return calibrationData_; }
    void saveCalibrationData() { savedCalibrationData_ = calibrationData_; }

    float cvRawValues_[int(CvInput::NUM_CVS)] = { 0 };
    CvCalibrationData calibrationData_;
    CvCalibrationData savedCalibrationData_;
};

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

    static const std::array<Led, 16> allChannelLeds()
    {
        return {
            Led::chA_m2,
            Led::chA_m1,
            Led::chA_p1,
            Led::chA_p2,
            Led::chB_m2,
            Led::chB_m1,
            Led::chB_p1,
            Led::chB_p2,
            Led::chC_m2,
            Led::chC_m1,
            Led::chC_p1,
            Led::chC_p2,
            Led::chD_m2,
            Led::chD_m1,
            Led::chD_p1,
            Led::chD_p2,
        };
    }

    KnobsAndCvMock& getKnobsAndCv() { return knobsAndCv_; };

    KnobsAndCvMock knobsAndCv_;
};