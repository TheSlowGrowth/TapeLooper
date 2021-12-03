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

template <size_t numLoopers>
class ParameterProviderMock
{
public:
    struct ControlInputs
    {
        // -2 .. 2
        int8_t octave = 0;

        // 0 .. 10
        float volumeCvVolts = 0.0f;
        // -5 .. 5
        float pitchCvVolts = 0.0f;

        // the rest here is 0..1
        float fineTunePosition = 0.5f;
        float wowAndFlutterAmt = 0.0f;
        float volumeSliderPosition = 0.0f;
        float driveAmt = 0.0f;
        float grainAmt = 0.0f;
    };

    /** Set from UI pages */
    std::array<ControlInputs, numLoopers>
        controlInputs_;
};