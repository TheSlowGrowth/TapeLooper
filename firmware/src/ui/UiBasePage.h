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