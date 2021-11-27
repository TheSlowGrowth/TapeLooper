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

#include <daisy.h>

#include "../constants.h"
#include "../hardware/Leds.h"

// defined in main.cpp
extern PeakMeter<blockSize, sampleRateHz> peakMeterInL;
extern PeakMeter<blockSize, sampleRateHz> peakMeterInR;
extern PeakMeter<blockSize, sampleRateHz> peakMeterOutL;
extern PeakMeter<blockSize, sampleRateHz> peakMeterOutR;

class BaseUiPage : public daisy::UiPage
{
public:
    BaseUiPage()
    {
    }

    virtual ~BaseUiPage() {}

    void Draw(const daisy::UiCanvasDescriptor& canvas) override
    {
        if (canvas.id_ == ledCanvasId)
        {
            LedDriverType& leds = *((LedDriverType*) canvas.handle_);

            // update peak meter leds
            float red, green;
            peakMeterInL.getRedAndGreenValues(red, green);
            leds.SetLed(0, green);
            leds.SetLed(1, red);
            peakMeterInR.getRedAndGreenValues(red, green);
            leds.SetLed(2, green);
            leds.SetLed(3, red);
            peakMeterOutL.getRedAndGreenValues(red, green);
            leds.SetLed(6, green);
            leds.SetLed(7, red);
            peakMeterOutR.getRedAndGreenValues(red, green);
            leds.SetLed(8, green);
            leds.SetLed(9, red);
        }
    }

    bool OnButton(uint16_t, uint8_t, bool) override
    {
        // TODO
        return true;
    }
};