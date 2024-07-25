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

class ButtonReader
{
public:
    void init(uint16_t* dmaBuffer)
    {
        dmaBuffer_ = dmaBuffer;

        // shift register for the channel buttons
        shiftRegisterLoadPin_.pin = { DSY_GPIOA, 4 };
        shiftRegisterLoadPin_.mode = DSY_GPIO_MODE_OUTPUT_PP;
        shiftRegisterLoadPin_.pull = DSY_GPIO_NOPULL;
        dsy_gpio_init(&shiftRegisterLoadPin_);
        dsy_gpio_write(&shiftRegisterLoadPin_, 1);

        daisy::SpiHandle::Config config;
        config.periph = daisy::SpiHandle::Config::Peripheral::SPI_1;
        config.mode = daisy::SpiHandle::Config::Mode::MASTER;
        config.direction = daisy::SpiHandle::Config::Direction::TWO_LINES_RX_ONLY;
        config.datasize = 8;
        config.clock_polarity = daisy::SpiHandle::Config::ClockPolarity::LOW;
        config.clock_phase = daisy::SpiHandle::Config::ClockPhase::ONE_EDGE;
        config.nss = daisy::SpiHandle::Config::NSS::SOFT;
        config.baud_prescaler = daisy::SpiHandle::Config::BaudPrescaler::PS_16;
        config.pin_config.sclk = { DSY_GPIOA, 5 };
        config.pin_config.miso = { DSY_GPIOG, 9 };
        shiftRegisterSpi_.Init(config);

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

    bool getButtonState(Button buttonId) const
    {
        assert(uint16_t(buttonId) < uint16_t(Button::NUM_BUTTONS));
        switch (buttonId)
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
                return !getBit<2>();
            case Button::chA_up:
                return !getBit<1>();
            case Button::chA_down:
                return !getBit<0>();
            case Button::chB_play:
                return !getBit<5>();
            case Button::chB_up:
                return !getBit<3>();
            case Button::chB_down:
                return !getBit<4>();
            case Button::chC_play:
                return !getBit<10>();
            case Button::chC_up:
                return !getBit<8>();
            case Button::chC_down:
                return !getBit<9>();
            case Button::chD_play:
                return !getBit<13>();
            case Button::chD_up:
                return !getBit<11>();
            case Button::chD_down:
                return !getBit<12>();
            case Button::NUM_BUTTONS:
                break;
        }
        return false;
    }

    void triggerReadout()
    {
        dsy_gpio_write(&shiftRegisterLoadPin_, 0);

        constexpr auto kNumBytes = 2;
        shiftRegisterSpi_.DmaReceive((uint8_t*) dmaBuffer_, kNumBytes, nullptr, nullptr, nullptr);

        dsy_gpio_write(&shiftRegisterLoadPin_, 1);
    }

private:
    template <size_t bitIndex>
    bool getBit() const
    {
        return *dmaBuffer_ & (1 << bitIndex);
    }

    struct
    {
        dsy_gpio settings;
        dsy_gpio save;
        dsy_gpio load;
        dsy_gpio rec;
    } commonGpios_;

    uint16_t* dmaBuffer_ = nullptr;
    daisy::SpiHandle shiftRegisterSpi_;
    dsy_gpio shiftRegisterLoadPin_;
};

class KnobAndCvReader
{
public:
    void init()
    {
        const dsy_gpio_pin mux0 = { DSY_GPIOA, 1 };
        const dsy_gpio_pin mux1 = { DSY_GPIOA, 0 };
        const dsy_gpio_pin mux2 = { DSY_GPIOD, 11 };

        constexpr auto kNumChannelsPerAdc = 7;
        constexpr auto kNumAdcs = 4;

        daisy::AdcChannelConfig channelConfigs[kNumAdcs];
        channelConfigs[0].InitMux({ DSY_GPIOC, 0 }, kNumChannelsPerAdc, mux0, mux1, mux2);
        channelConfigs[1].InitMux({ DSY_GPIOA, 3 }, kNumChannelsPerAdc, mux0, mux1, mux2);
        channelConfigs[2].InitMux({ DSY_GPIOB, 1 }, kNumChannelsPerAdc, mux0, mux1, mux2);
        channelConfigs[3].InitMux({ DSY_GPIOA, 7 }, kNumChannelsPerAdc, mux0, mux1, mux2);

        adc_.Init(channelConfigs, kNumAdcs);
        adc_.Start();
    }

    float getPotValue(Pot potId) const
    {
        assert(uint16_t(potId) < uint16_t(Pot::NUM_POTS));
        switch (potId)
        {
            case Pot::chA_speed:
                return adc_.GetMuxFloat(0, 0);
            case Pot::chA_warbleAmt:
                return adc_.GetMuxFloat(0, 4);
            case Pot::chA_grainAmt:
                return adc_.GetMuxFloat(0, 3);
            case Pot::chA_driveAmt:
                return adc_.GetMuxFloat(0, 2);
            case Pot::chA_volume:
                return adc_.GetMuxFloat(0, 1);
            case Pot::chB_speed:
                return adc_.GetMuxFloat(1, 0);
            case Pot::chB_warbleAmt:
                return adc_.GetMuxFloat(1, 4);
            case Pot::chB_grainAmt:
                return adc_.GetMuxFloat(1, 3);
            case Pot::chB_driveAmt:
                return adc_.GetMuxFloat(1, 2);
            case Pot::chB_volume:
                return adc_.GetMuxFloat(1, 1);
            case Pot::chC_speed:
                return adc_.GetMuxFloat(2, 0);
            case Pot::chC_warbleAmt:
                return adc_.GetMuxFloat(2, 4);
            case Pot::chC_grainAmt:
                return adc_.GetMuxFloat(2, 3);
            case Pot::chC_driveAmt:
                return adc_.GetMuxFloat(2, 2);
            case Pot::chC_volume:
                return adc_.GetMuxFloat(2, 1);
            case Pot::chD_speed:
                return adc_.GetMuxFloat(3, 0);
            case Pot::chD_warbleAmt:
                return adc_.GetMuxFloat(3, 4);
            case Pot::chD_grainAmt:
                return adc_.GetMuxFloat(3, 3);
            case Pot::chD_driveAmt:
                return adc_.GetMuxFloat(3, 2);
            case Pot::chD_volume:
                return adc_.GetMuxFloat(3, 1);
            case Pot::NUM_POTS:
                break;
        }
        return 0.0f;
    }

    float getCvVolts(CvInput cv) const
    {
        /**
            The general formula for the CV inputs is

                Uout = -Isum * Rf;
                Isum = -10V / Rr + Uin / Rin;
                Uout = -(-10V / Rr + Uin / Rin) * Rf;
                Xadc = Uout / 3.3V
                Uin = -3.3V * Rin/Rf * Xadc - Rin/Rr * (-10V)
                Uin = scale * Xadc + offset

                scale = -3.3V * Rin/Rf
                offset = 10V * Rin/Rr

            For the speed/pitch inputs:

                Rin = 100k
                Rr = 481k
                Rf = 78.7k

                scale = -4.19313850
                offset = 2.079002

            For the volume inputs:

                Rin = 100k
                Rr = 200k
                Rf = 68k

                scale = -4.19313850
                offset = 5.0
        */
        constexpr auto kScaleSpeed = -4.19313850f;
        constexpr auto kOffsetSpeed = 2.079002f;
        constexpr auto kScaleVolume = -4.852941f;
        constexpr auto kOffsetVolume = 5.0f;

        switch (cv)
        {
            case CvInput::chA_speed:
                return adc_.GetMuxFloat(0, 5) * kScaleSpeed + kOffsetSpeed;
            case CvInput::chA_volume:
                return adc_.GetMuxFloat(0, 6) * kScaleVolume + kOffsetVolume;
            case CvInput::chB_speed:
                return adc_.GetMuxFloat(1, 5) * kScaleSpeed + kOffsetSpeed;
            case CvInput::chB_volume:
                return adc_.GetMuxFloat(1, 6) * kScaleVolume + kOffsetVolume;
            case CvInput::chC_speed:
                return adc_.GetMuxFloat(2, 5) * kScaleSpeed + kOffsetSpeed;
            case CvInput::chC_volume:
                return adc_.GetMuxFloat(2, 6) * kScaleVolume + kOffsetVolume;
            case CvInput::chD_speed:
                return adc_.GetMuxFloat(3, 5) * kScaleSpeed + kOffsetSpeed;
            case CvInput::chD_volume:
                return adc_.GetMuxFloat(3, 6) * kScaleVolume + kOffsetVolume;
            case CvInput::NUM_CVS:
                break;
        }
        return 0.0f;
    }

private:
    daisy::AdcHandle adc_;
};

class UiHardware
{
public:
    using LedDriverType = daisy::LedDriverPca9685<3, false>;
    using LedDmaBufferType = LedDriverType::DmaBuffer;

    UiHardware(daisy::UiEventQueue& eventQueue,
               LedDmaBufferType bufferA,
               LedDmaBufferType bufferB,
               uint16_t* shiftRegisterDmaBuffer)
    {
        potMonitor_.Init(eventQueue, *this);
        buttonMonitor_.Init(eventQueue,
                            *this,
                            10, // debounce time ms
                            500, // double click time ms
                            0); // retriggering disabled

        initControls(shiftRegisterDmaBuffer);
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
        return knobsAndCv_.getCvVolts(cv);
    }

    // ===================================================================
    // implements the button reader interface for the libDaisy UI system
    // ===================================================================

    bool IsButtonPressed(uint16_t buttonId) const
    {
        return buttons_.getButtonState(Button(buttonId));
    }

    // ===================================================================
    // implements the pot reader interface for the libDaisy UI system
    // ===================================================================

    float GetPotValue(uint16_t potId) const
    {
        return knobsAndCv_.getPotValue(Pot(potId));
    }

    void processControls()
    {
        // transform raw inputs into UI Events
        potMonitor_.Process();
        buttonMonitor_.Process();

        buttons_.triggerReadout();
    }

private:
    UiHardware(const UiHardware&) = delete;
    UiHardware& operator=(const UiHardware&) = delete;

    void initControls(uint16_t* shiftRegisterDmaBuffer)
    {
        buttons_.init(shiftRegisterDmaBuffer);
        knobsAndCv_.init();
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

    ButtonReader buttons_;
    KnobAndCvReader knobsAndCv_;

    daisy::PotMonitor<UiHardware, int(Pot::NUM_POTS)> potMonitor_;
    daisy::ButtonMonitor<UiHardware, int(Button::NUM_BUTTONS)> buttonMonitor_;
};