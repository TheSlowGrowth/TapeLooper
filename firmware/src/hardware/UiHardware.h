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
#include <stdint.h>
#include <assert.h>
#include <memory>

#include <daisy.h>

#include <dsp/TapeProcessor.h>

#include "../constants.h"
#include "UiHardwareTypes.h"

extern "C"
{
#include "util/hal_map.h"
}

class UiHardware
{
public:
    using LedDriverType = daisy::LedDriverPca9685<3, false>;
    using LedDmaBufferType = LedDriverType::DmaBuffer;

    UiHardware(daisy::UiEventQueue& eventQueue,
               LedDmaBufferType bufferA,
               LedDmaBufferType bufferB)
    {
        potMonitor_.Init(eventQueue, *this);
        buttonMonitor_.Init(eventQueue,
                            *this,
                            10, // debounce time ms
                            500, // double click time ms
                            0); // retriggering disabled

        initControls();
        initLeds(bufferA, bufferB);
    }

    void setLedFromRawValues(Led led, float brightnessRed, float brightnessGreen)
    {
        auto& cfg = ledCfgs_[size_t(led)];
        cfg.mode = LedSettings::Mode::rawRG;
        cfg.r = 0.0f;
        cfg.r = brightnessRed;
        cfg.g = brightnessGreen;
    }

    void setLed(Led led, LedColour colour)
    {
        auto& cfg = ledCfgs_[size_t(led)];
        cfg.mode = LedSettings::Mode::colour;
        cfg.colour = colour;
    }

    void clearLeds()
    {
        for (size_t i = 0; i < ledCfgs_.size(); i++)
            ledCfgs_[i].clear();
    }

    void transmitLeds()
    {
        updateLedBrightnessValues();
        ledDriver_.SwapBuffersAndTransmit();
    }

    float getCvVolts(CvInput cv) const
    {
        switch (cv)
        {
            case CvInput::chA_speed:
                return 0.0f; // TODO
            case CvInput::chA_volume:
                return 0.0f; // TODO
            case CvInput::chB_speed:
                return 0.0f; // TODO
            case CvInput::chB_volume:
                return 0.0f; // TODO
            case CvInput::chC_speed:
                return 0.0f; // TODO
            case CvInput::chC_volume:
                return 0.0f; // TODO
            case CvInput::chD_speed:
                return 0.0f; // TODO
            case CvInput::chD_volume:
                return 0.0f; // TODO
            case CvInput::NUM_CVS:
                break;
        }
        return 0.0f;
    }

    // ===================================================================
    // implements the button reader interface for the libDaisy UI system
    // ===================================================================

    bool IsButtonPressed(uint16_t buttonId) const
    {
        assert(buttonId < uint16_t(Button::NUM_BUTTONS)); // TODO: write my own assert macro with bkpt();
        switch (Button(buttonId))
        {
            case Button::save:
                return false; // TODO
            case Button::load:
                return false; // TODO
            case Button::settings:
                return false; // TODO
            case Button::record:
                return false; // TODO
            case Button::chA_play:
                return false; // TODO
            case Button::chA_up:
                return false; // TODO
            case Button::chA_down:
                return false; // TODO
            case Button::chB_play:
                return false; // TODO
            case Button::chB_up:
                return false; // TODO
            case Button::chB_down:
                return false; // TODO
            case Button::chC_play:
                return false; // TODO
            case Button::chC_up:
                return false; // TODO
            case Button::chC_down:
                return false; // TODO
            case Button::chD_play:
                return false; // TODO
            case Button::chD_up:
                return false; // TODO
            case Button::chD_down:
                return false; // TODO
            case Button::NUM_BUTTONS:
                break;
        }
        return false;
    }

    // ===================================================================
    // implements the pot reader interface for the libDaisy UI system
    // ===================================================================

    float GetPotValue(uint16_t potId) const
    {
        assert(potId < uint16_t(Pot::NUM_POTS)); // TODO: write my own assert macro with bkpt();
        switch (Pot(potId))
        {
            case Pot::chA_speed:
                return 0.0f; // TODO
            case Pot::chA_warbleAmt:
                return 0.0f; // TODO
            case Pot::chA_grainAmt:
                return 0.0f; // TODO
            case Pot::chA_driveAmt:
                return 0.0f; // TODO
            case Pot::chA_volume:
                return 0.0f; // TODO
            case Pot::chB_speed:
                return 0.0f; // TODO
            case Pot::chB_warbleAmt:
                return 0.0f; // TODO
            case Pot::chB_grainAmt:
                return 0.0f; // TODO
            case Pot::chB_driveAmt:
                return 0.0f; // TODO
            case Pot::chB_volume:
                return 0.0f; // TODO
            case Pot::chC_speed:
                return 0.0f; // TODO
            case Pot::chC_warbleAmt:
                return 0.0f; // TODO
            case Pot::chC_grainAmt:
                return 0.0f; // TODO
            case Pot::chC_driveAmt:
                return 0.0f; // TODO
            case Pot::chC_volume:
                return 0.0f; // TODO
            case Pot::chD_speed:
                return 0.0f; // TODO
            case Pot::chD_warbleAmt:
                return 0.0f; // TODO
            case Pot::chD_grainAmt:
                return 0.0f; // TODO
            case Pot::chD_driveAmt:
                return 0.0f; // TODO
            case Pot::chD_volume:
                return 0.0f; // TODO
            case Pot::NUM_POTS:
                break;
        }
        return 0.0f;
    }

private:
    UiHardware(const UiHardware&) = delete;
    UiHardware& operator=(const UiHardware&) = delete;

    void initControls()
    {
    }

    void initLeds(LedDmaBufferType bufferA,
                  LedDmaBufferType bufferB)
    {
        daisy::I2CHandle i2c;
        daisy::I2CHandle::Config i2cCfg;
        i2cCfg.periph = daisy::I2CHandle::Config::Peripheral::I2C_1;
        i2cCfg.pin_config.sda = { DSY_GPIOB, 9 };
        i2cCfg.pin_config.scl = { DSY_GPIOB, 8 };
        i2cCfg.speed = daisy::I2CHandle::Config::Speed::I2C_400KHZ;
        i2cCfg.mode = daisy::I2CHandle::Config::Mode::I2C_MASTER;
        i2c.Init(i2cCfg);

        ledDriver_.Init(i2c, { 0b00, 0b01, 0b10 }, bufferA, bufferB, { DSY_GPIOB, 6 });
    }

    void updateLedBrightnessValues()
    {
        const auto pulseIntensity = getIntensityForCurrentSystemTime();
        for (size_t i = 0; i < ledCfgs_.size(); i++)
        {
            const auto cfg = ledCfgs_[i];
            const auto ledGreenIdx = getLedGreenIndex(Led(i));
            if (cfg.mode == LedSettings::Mode::rawRG)
            {
                ledDriver_.SetLed(ledGreenIdx, cfg.g);
                ledDriver_.SetLed(ledGreenIdx + 1, cfg.r);
            }
            else
            {
                auto g = 0.0f;
                auto r = 0.0f;
                switch (cfg.colour)
                {
                    case LedColour::off:
                        break;
                    case LedColour::red:
                        r = 1.0f;
                        break;
                    case LedColour::green:
                        g = 1.0f;
                        break;
                    case LedColour::yellow:
                        g = r = 1.0f;
                        break;
                    case LedColour::pulsingRed:
                        r = pulseIntensity;
                        break;
                    case LedColour::pulsingGreen:
                        g = pulseIntensity;
                        break;
                    case LedColour::pulsingYellow:
                        g = r = pulseIntensity;
                        break;
                }
                ledDriver_.SetLed(ledGreenIdx, g);
                ledDriver_.SetLed(ledGreenIdx + 1, r);
            }
        }
    }

    float getIntensityForCurrentSystemTime()
    {
        const auto time = daisy::System::GetNow();
        constexpr uint32_t ledCycleTimeMs = 1000;
        const float phase = float(time % ledCycleTimeMs) / float(ledCycleTimeMs);
        return phase < 0.5f ? 2.0f * phase : (2.0f - 2.0f * phase);
    }

    int getLedGreenIndex(Led led)
    {
        switch (led)
        {
            case Led::peakMeterInL:
                return 46;
            case Led::peakMeterInR:
                return 44;
            case Led::peakMeterOutL:
                return 42;
            case Led::peakMeterOutR:
                return 40;
            case Led::save:
                return 36;
            case Led::load:
                return 34;
            case Led::settings:
                return 38;
            case Led::record:
                return 32;
            case Led::chA_m2:
                return 0;
            case Led::chA_m1:
                return 2;
            case Led::chA_p1:
                return 4;
            case Led::chA_p2:
                return 6;
            case Led::chB_m2:
                return 14;
            case Led::chB_m1:
                return 12;
            case Led::chB_p1:
                return 10;
            case Led::chB_p2:
                return 8;
            case Led::chC_m2:
                return 16;
            case Led::chC_m1:
                return 18;
            case Led::chC_p1:
                return 20;
            case Led::chC_p2:
                return 22;
            case Led::chD_m2:
                return 30;
            case Led::chD_m1:
                return 28;
            case Led::chD_p1:
                return 26;
            case Led::chD_p2:
                return 24;
            default:
                return 0;
        }
    }

    void processControls()
    {
        // transform raw inputs into UI Events
        potMonitor_.Process();
        buttonMonitor_.Process();
    }

    static void updateComplete(void* context)
    {
        UiHardware* sender = reinterpret_cast<UiHardware*>(context);
        sender->processControls();
    }

    LedDriverType ledDriver_;

    struct LedSettings
    {
        LedSettings()
        {
            clear();
        }

        void clear()
        {
            mode = Mode::rawRG;
            r = 0.0f;
            g = 0.0f;
            colour = LedColour::off;
        }

        enum class Mode
        {
            rawRG,
            colour
        } mode;
        float r, g;
        LedColour colour;
    };
    std::array<LedSettings, size_t(Led::NUM_LEDS)> ledCfgs_;

    daisy::PotMonitor<UiHardware, int(Pot::NUM_POTS)> potMonitor_;
    daisy::ButtonMonitor<UiHardware, int(Button::NUM_BUTTONS)> buttonMonitor_;
};