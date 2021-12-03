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
        buttonMonitor_.Init(eventQueue, *this);

        initControls();
        initLeds(bufferA, bufferB);
    }

    void setLedFromRawValues(Led led, float brightnessRed, float brightnessGreen)
    {
        auto& cfg = ledCfgs_[size_t(led)];
        cfg.mode = LedSettings::Mode::rawRG;
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

    void processControls()
    {
        // read and write values from/to MAX11300
        max11300AB_.Update();
        max11300CD_.Update();

        // transform raw inputs into UI Events
        potMonitor_.Process();
        buttonMonitor_.Process();
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
                return max11300AB_.ReadAnalogPinVolts(daisy::MAX11300::PIN_0);
            case CvInput::chA_volume:
                return max11300AB_.ReadAnalogPinVolts(daisy::MAX11300::PIN_2);
            case CvInput::chB_speed:
                return max11300AB_.ReadAnalogPinVolts(daisy::MAX11300::PIN_1);
            case CvInput::chB_volume:
                return max11300AB_.ReadAnalogPinVolts(daisy::MAX11300::PIN_3);
            case CvInput::chC_speed:
                return max11300CD_.ReadAnalogPinVolts(daisy::MAX11300::PIN_0);
            case CvInput::chC_volume:
                return max11300CD_.ReadAnalogPinVolts(daisy::MAX11300::PIN_2);
            case CvInput::chD_speed:
                return max11300CD_.ReadAnalogPinVolts(daisy::MAX11300::PIN_1);
            case CvInput::chD_volume:
                return max11300CD_.ReadAnalogPinVolts(daisy::MAX11300::PIN_3);
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
                return !dsy_gpio_read(&commonGpios_.save);
            case Button::load:
                return !dsy_gpio_read(&commonGpios_.load);
            case Button::settings:
                return !dsy_gpio_read(&commonGpios_.settings);
            case Button::record:
                return !dsy_gpio_read(&commonGpios_.rec);
            case Button::chA_play:
                return !dsy_gpio_read(&chanAGpios_.play);
            case Button::chA_up:
                return !dsy_gpio_read(&chanAGpios_.up);
            case Button::chA_down:
                return !dsy_gpio_read(&chanAGpios_.down);
            case Button::chB_play:
                return !max11300AB_.ReadDigitalPin(daisy::MAX11300::PIN_17);
            case Button::chB_up:
                return !max11300AB_.ReadDigitalPin(daisy::MAX11300::PIN_15);
            case Button::chB_down:
                return !max11300AB_.ReadDigitalPin(daisy::MAX11300::PIN_16);
            case Button::chC_play:
                return !max11300CD_.ReadDigitalPin(daisy::MAX11300::PIN_18);
            case Button::chC_up:
                return !max11300CD_.ReadDigitalPin(daisy::MAX11300::PIN_14);
            case Button::chC_down:
                return !max11300CD_.ReadDigitalPin(daisy::MAX11300::PIN_16);
            case Button::chD_play:
                return !max11300CD_.ReadDigitalPin(daisy::MAX11300::PIN_19);
            case Button::chD_up:
                return !max11300CD_.ReadDigitalPin(daisy::MAX11300::PIN_15);
            case Button::chD_down:
                return !max11300CD_.ReadDigitalPin(daisy::MAX11300::PIN_17);
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
                return float(max11300AB_.ReadAnalogPinRaw(daisy::MAX11300::PIN_4)) / 4095.0f;
            case Pot::chA_warbleAmt:
                return float(max11300AB_.ReadAnalogPinRaw(daisy::MAX11300::PIN_12)) / 4095.0f;
            case Pot::chA_grainAmt:
                return float(max11300AB_.ReadAnalogPinRaw(daisy::MAX11300::PIN_10)) / 4095.0f;
            case Pot::chA_driveAmt:
                return float(max11300AB_.ReadAnalogPinRaw(daisy::MAX11300::PIN_8)) / 4095.0f;
            case Pot::chA_volume:
                return float(max11300AB_.ReadAnalogPinRaw(daisy::MAX11300::PIN_6)) / 4095.0f;
            case Pot::chB_speed:
                return float(max11300AB_.ReadAnalogPinRaw(daisy::MAX11300::PIN_5)) / 4095.0f;
            case Pot::chB_warbleAmt:
                return float(max11300AB_.ReadAnalogPinRaw(daisy::MAX11300::PIN_14)) / 4095.0f;
            case Pot::chB_grainAmt:
                return float(max11300AB_.ReadAnalogPinRaw(daisy::MAX11300::PIN_11)) / 4095.0f;
            case Pot::chB_driveAmt:
                return float(max11300AB_.ReadAnalogPinRaw(daisy::MAX11300::PIN_9)) / 4095.0f;
            case Pot::chB_volume:
                return float(max11300AB_.ReadAnalogPinRaw(daisy::MAX11300::PIN_7)) / 4095.0f;
            case Pot::chC_speed:
                return float(max11300CD_.ReadAnalogPinRaw(daisy::MAX11300::PIN_4)) / 4095.0f;
            case Pot::chC_warbleAmt:
                return float(max11300CD_.ReadAnalogPinRaw(daisy::MAX11300::PIN_12)) / 4095.0f;
            case Pot::chC_grainAmt:
                return float(max11300CD_.ReadAnalogPinRaw(daisy::MAX11300::PIN_10)) / 4095.0f;
            case Pot::chC_driveAmt:
                return float(max11300CD_.ReadAnalogPinRaw(daisy::MAX11300::PIN_8)) / 4095.0f;
            case Pot::chC_volume:
                return float(max11300CD_.ReadAnalogPinRaw(daisy::MAX11300::PIN_6)) / 4095.0f;
            case Pot::chD_speed:
                return float(max11300CD_.ReadAnalogPinRaw(daisy::MAX11300::PIN_5)) / 4095.0f;
            case Pot::chD_warbleAmt:
                return float(max11300CD_.ReadAnalogPinRaw(daisy::MAX11300::PIN_13)) / 4095.0f;
            case Pot::chD_grainAmt:
                return float(max11300CD_.ReadAnalogPinRaw(daisy::MAX11300::PIN_11)) / 4095.0f;
            case Pot::chD_driveAmt:
                return float(max11300CD_.ReadAnalogPinRaw(daisy::MAX11300::PIN_9)) / 4095.0f;
            case Pot::chD_volume:
                return float(max11300CD_.ReadAnalogPinRaw(daisy::MAX11300::PIN_7)) / 4095.0f;
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
        daisy::MAX11300::Config config;
        config.transport_config.spi_config.periph = daisy::SpiHandle::Config::Peripheral::SPI_1;
        config.transport_config.spi_config.mode = daisy::SpiHandle::Config::Mode::MASTER;
        config.transport_config.spi_config.direction = daisy::SpiHandle::Config::Direction::TWO_LINES;
        config.transport_config.spi_config.datasize = 8;
        config.transport_config.spi_config.clock_polarity = daisy::SpiHandle::Config::ClockPolarity::LOW;
        config.transport_config.spi_config.clock_phase = daisy::SpiHandle::Config::ClockPhase::ONE_EDGE;
        config.transport_config.spi_config.nss = daisy::SpiHandle::Config::NSS::SOFT;
        config.transport_config.spi_config.baud_prescaler = daisy::SpiHandle::Config::BaudPrescaler::PS_4;
        config.transport_config.spi_config.pin_config.nss = { DSY_GPIOC, 1 };
        config.transport_config.spi_config.pin_config.sclk = { DSY_GPIOA, 5 };
        config.transport_config.spi_config.pin_config.miso = { DSY_GPIOA, 6 };
        config.transport_config.spi_config.pin_config.mosi = { DSY_GPIOA, 7 };
        max11300AB_.Init(config);
        config.transport_config.spi_config.pin_config.nss = { DSY_GPIOC, 4 };
        //max11300CD_.Init(config);

        // Pitch CVs
        max11300AB_.ConfigurePinAsAnalogRead(daisy::MAX11300::PIN_0, daisy::MAX11300::AdcVoltageRange::NEGATIVE_5_TO_5);
        max11300AB_.ConfigurePinAsAnalogRead(daisy::MAX11300::PIN_1, daisy::MAX11300::AdcVoltageRange::NEGATIVE_5_TO_5);
        max11300CD_.ConfigurePinAsAnalogRead(daisy::MAX11300::PIN_0, daisy::MAX11300::AdcVoltageRange::NEGATIVE_5_TO_5);
        max11300CD_.ConfigurePinAsAnalogRead(daisy::MAX11300::PIN_1, daisy::MAX11300::AdcVoltageRange::NEGATIVE_5_TO_5);
        // Volume CVs
        max11300AB_.ConfigurePinAsAnalogRead(daisy::MAX11300::PIN_2, daisy::MAX11300::AdcVoltageRange::ZERO_TO_10);
        max11300AB_.ConfigurePinAsAnalogRead(daisy::MAX11300::PIN_3, daisy::MAX11300::AdcVoltageRange::ZERO_TO_10);
        max11300CD_.ConfigurePinAsAnalogRead(daisy::MAX11300::PIN_2, daisy::MAX11300::AdcVoltageRange::ZERO_TO_10);
        max11300CD_.ConfigurePinAsAnalogRead(daisy::MAX11300::PIN_3, daisy::MAX11300::AdcVoltageRange::ZERO_TO_10);
        // Pitch pots
        max11300AB_.ConfigurePinAsAnalogRead(daisy::MAX11300::PIN_4, daisy::MAX11300::AdcVoltageRange::ZERO_TO_2P5);
        max11300AB_.ConfigurePinAsAnalogRead(daisy::MAX11300::PIN_5, daisy::MAX11300::AdcVoltageRange::ZERO_TO_2P5);
        max11300CD_.ConfigurePinAsAnalogRead(daisy::MAX11300::PIN_4, daisy::MAX11300::AdcVoltageRange::ZERO_TO_2P5);
        max11300CD_.ConfigurePinAsAnalogRead(daisy::MAX11300::PIN_5, daisy::MAX11300::AdcVoltageRange::ZERO_TO_2P5);
        // Volume sliders
        max11300AB_.ConfigurePinAsAnalogRead(daisy::MAX11300::PIN_6, daisy::MAX11300::AdcVoltageRange::ZERO_TO_2P5);
        max11300AB_.ConfigurePinAsAnalogRead(daisy::MAX11300::PIN_7, daisy::MAX11300::AdcVoltageRange::ZERO_TO_2P5);
        max11300CD_.ConfigurePinAsAnalogRead(daisy::MAX11300::PIN_6, daisy::MAX11300::AdcVoltageRange::ZERO_TO_2P5);
        max11300CD_.ConfigurePinAsAnalogRead(daisy::MAX11300::PIN_7, daisy::MAX11300::AdcVoltageRange::ZERO_TO_2P5);
        // Drive pots
        max11300AB_.ConfigurePinAsAnalogRead(daisy::MAX11300::PIN_8, daisy::MAX11300::AdcVoltageRange::ZERO_TO_2P5);
        max11300AB_.ConfigurePinAsAnalogRead(daisy::MAX11300::PIN_9, daisy::MAX11300::AdcVoltageRange::ZERO_TO_2P5);
        max11300CD_.ConfigurePinAsAnalogRead(daisy::MAX11300::PIN_8, daisy::MAX11300::AdcVoltageRange::ZERO_TO_2P5);
        max11300CD_.ConfigurePinAsAnalogRead(daisy::MAX11300::PIN_9, daisy::MAX11300::AdcVoltageRange::ZERO_TO_2P5);
        // Grain pots
        max11300AB_.ConfigurePinAsAnalogRead(daisy::MAX11300::PIN_10, daisy::MAX11300::AdcVoltageRange::ZERO_TO_2P5);
        max11300AB_.ConfigurePinAsAnalogRead(daisy::MAX11300::PIN_11, daisy::MAX11300::AdcVoltageRange::ZERO_TO_2P5);
        max11300CD_.ConfigurePinAsAnalogRead(daisy::MAX11300::PIN_10, daisy::MAX11300::AdcVoltageRange::ZERO_TO_2P5);
        max11300CD_.ConfigurePinAsAnalogRead(daisy::MAX11300::PIN_11, daisy::MAX11300::AdcVoltageRange::ZERO_TO_2P5);
        // Warble pots
        max11300AB_.ConfigurePinAsAnalogRead(daisy::MAX11300::PIN_12, daisy::MAX11300::AdcVoltageRange::ZERO_TO_2P5);
        max11300AB_.ConfigurePinAsAnalogRead(daisy::MAX11300::PIN_14, daisy::MAX11300::AdcVoltageRange::ZERO_TO_2P5);
        max11300CD_.ConfigurePinAsAnalogRead(daisy::MAX11300::PIN_12, daisy::MAX11300::AdcVoltageRange::ZERO_TO_2P5);
        max11300CD_.ConfigurePinAsAnalogRead(daisy::MAX11300::PIN_13, daisy::MAX11300::AdcVoltageRange::ZERO_TO_2P5);
        // Buttons for ch A
        chanAGpios_.up.pin = { DSY_GPIOB, 14 };
        chanAGpios_.up.mode = DSY_GPIO_MODE_INPUT;
        chanAGpios_.up.pull = DSY_GPIO_NOPULL;
        dsy_gpio_init(&chanAGpios_.up);
        chanAGpios_.down.pin = { DSY_GPIOA, 2 };
        chanAGpios_.down.mode = DSY_GPIO_MODE_INPUT;
        chanAGpios_.down.pull = DSY_GPIO_NOPULL;
        dsy_gpio_init(&chanAGpios_.down);
        chanAGpios_.play.pin = { DSY_GPIOB, 15 };
        chanAGpios_.play.mode = DSY_GPIO_MODE_INPUT;
        chanAGpios_.play.pull = DSY_GPIO_NOPULL;
        dsy_gpio_init(&chanAGpios_.play);
        // Buttons for ch B
        max11300AB_.ConfigurePinAsDigitalRead(daisy::MAX11300::PIN_15, 1.5f); // up B
        max11300AB_.ConfigurePinAsDigitalRead(daisy::MAX11300::PIN_16, 1.5f); // down B
        max11300AB_.ConfigurePinAsDigitalRead(daisy::MAX11300::PIN_17, 1.5f); // play B
        // Buttons for ch C / D
        max11300CD_.ConfigurePinAsDigitalRead(daisy::MAX11300::PIN_14, 1.5f); // up C
        max11300CD_.ConfigurePinAsDigitalRead(daisy::MAX11300::PIN_15, 1.5f); // up D
        max11300CD_.ConfigurePinAsDigitalRead(daisy::MAX11300::PIN_16, 1.5f); // down C
        max11300CD_.ConfigurePinAsDigitalRead(daisy::MAX11300::PIN_17, 1.5f); // down D
        max11300CD_.ConfigurePinAsDigitalRead(daisy::MAX11300::PIN_18, 1.5f); // play C
        max11300CD_.ConfigurePinAsDigitalRead(daisy::MAX11300::PIN_19, 1.5f); // play D
        // common section buttons
        commonGpios_.settings.pin = { DSY_GPIOB, 7 };
        commonGpios_.settings.mode = DSY_GPIO_MODE_INPUT;
        commonGpios_.settings.pull = DSY_GPIO_NOPULL;
        dsy_gpio_init(&commonGpios_.settings);
        commonGpios_.save.pin = { DSY_GPIOG, 11 };
        commonGpios_.save.mode = DSY_GPIO_MODE_INPUT;
        commonGpios_.save.pull = DSY_GPIO_NOPULL;
        dsy_gpio_init(&commonGpios_.save);
        commonGpios_.load.pin = { DSY_GPIOG, 10 };
        commonGpios_.load.mode = DSY_GPIO_MODE_INPUT;
        commonGpios_.load.pull = DSY_GPIO_NOPULL;
        dsy_gpio_init(&commonGpios_.load);
        commonGpios_.rec.pin = { DSY_GPIOB, 12 };
        commonGpios_.rec.mode = DSY_GPIO_MODE_INPUT;
        commonGpios_.rec.pull = DSY_GPIO_NOPULL;
        dsy_gpio_init(&commonGpios_.rec);
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

    daisy::MAX11300 max11300AB_;
    daisy::MAX11300 max11300CD_;
    struct
    {
        dsy_gpio up;
        dsy_gpio down;
        dsy_gpio play;
    } chanAGpios_;
    struct
    {
        dsy_gpio settings;
        dsy_gpio save;
        dsy_gpio load;
        dsy_gpio rec;
    } commonGpios_;
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