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
        shiftRegisterLoadPin_.Init({ daisy::GPIOPort::PORTA, 4 },
                                   daisy::GPIO::Mode::OUTPUT,
                                   daisy::GPIO::Pull::NOPULL,
                                   daisy::GPIO::Speed::MEDIUM);
        shiftRegisterLoadPin_.Write(true);

        daisy::SpiHandle::Config config;
        config.periph = daisy::SpiHandle::Config::Peripheral::SPI_1;
        config.mode = daisy::SpiHandle::Config::Mode::MASTER;
        config.direction = daisy::SpiHandle::Config::Direction::TWO_LINES_RX_ONLY;
        config.datasize = 8;
        config.clock_polarity = daisy::SpiHandle::Config::ClockPolarity::LOW;
        config.clock_phase = daisy::SpiHandle::Config::ClockPhase::ONE_EDGE;
        config.nss = daisy::SpiHandle::Config::NSS::SOFT;
        config.baud_prescaler = daisy::SpiHandle::Config::BaudPrescaler::PS_16;
        config.pin_config.sclk = { daisy::GPIOPort::PORTA, 5 };
        config.pin_config.miso = { daisy::GPIOPort::PORTG, 9 };
        shiftRegisterSpi_.Init(config);

        // common section buttons
        commonGpios_.settings.Init({ daisy::GPIOPort::PORTB, 7 },
                                   daisy::GPIO::Mode::INPUT,
                                   daisy::GPIO::Pull::NOPULL,
                                   daisy::GPIO::Speed::LOW);
        commonGpios_.save.Init({ daisy::GPIOPort::PORTG, 11 },
                               daisy::GPIO::Mode::INPUT,
                               daisy::GPIO::Pull::NOPULL,
                               daisy::GPIO::Speed::LOW);
        commonGpios_.load.Init({ daisy::GPIOPort::PORTG, 10 },
                               daisy::GPIO::Mode::INPUT,
                               daisy::GPIO::Pull::NOPULL,
                               daisy::GPIO::Speed::LOW);
        commonGpios_.rec.Init({ daisy::GPIOPort::PORTB, 12 },
                              daisy::GPIO::Mode::INPUT,
                              daisy::GPIO::Pull::NOPULL,
                              daisy::GPIO::Speed::LOW);
    }

    bool getButtonState(Button buttonId) const
    {
        assert(uint16_t(buttonId) < uint16_t(Button::NUM_BUTTONS));
        switch (buttonId)
        {
            case Button::save:
                return !const_cast<daisy::GPIO*>(&commonGpios_.save)->Read();
            case Button::load:
                return !const_cast<daisy::GPIO*>(&commonGpios_.load)->Read();
            case Button::settings:
                return !const_cast<daisy::GPIO*>(&commonGpios_.settings)->Read();
            case Button::record:
                return !const_cast<daisy::GPIO*>(&commonGpios_.rec)->Read();
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
        shiftRegisterLoadPin_.Write(false);

        constexpr auto kNumBytes = 2;
        shiftRegisterSpi_.DmaReceive((uint8_t*) dmaBuffer_, kNumBytes, nullptr, nullptr, nullptr);

        shiftRegisterLoadPin_.Write(true);
    }

private:
    template <size_t bitIndex>
    bool getBit() const
    {
        return *dmaBuffer_ & (1 << bitIndex);
    }

    struct
    {
        daisy::GPIO settings;
        daisy::GPIO save;
        daisy::GPIO load;
        daisy::GPIO rec;
    } commonGpios_;

    uint16_t* dmaBuffer_ = nullptr;
    daisy::SpiHandle shiftRegisterSpi_;
    daisy::GPIO shiftRegisterLoadPin_;
};

class KnobAndCvReader
{
public:
    KnobAndCvReader(daisy::QSPIHandle& qspi) :
        calibrationStorage_(qspi)
    {
    }

    void init()
    {
        calibrationStorage_.Init(getDefaultCalibrationData(), kCalibrationDataOffset);

        const daisy::Pin mux0 = { daisy::GPIOPort::PORTA, 1 };
        const daisy::Pin mux1 = { daisy::GPIOPort::PORTA, 0 };
        const daisy::Pin mux2 = { daisy::GPIOPort::PORTD, 11 };

        constexpr auto kNumChannelsPerAdc = 7;
        constexpr auto kNumAdcs = 4;

        daisy::AdcChannelConfig channelConfigs[kNumAdcs];
        channelConfigs[0].InitMux({ daisy::GPIOPort::PORTC, 0 }, kNumChannelsPerAdc, mux0, mux1, mux2);
        channelConfigs[1].InitMux({ daisy::GPIOPort::PORTA, 3 }, kNumChannelsPerAdc, mux0, mux1, mux2);
        channelConfigs[2].InitMux({ daisy::GPIOPort::PORTB, 1 }, kNumChannelsPerAdc, mux0, mux1, mux2);
        channelConfigs[3].InitMux({ daisy::GPIOPort::PORTA, 7 }, kNumChannelsPerAdc, mux0, mux1, mux2);

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

    float getCvRaw(CvInput cv) const
    {
        switch (cv)
        {
            case CvInput::chA_speed:
                return adc_.GetMuxFloat(0, 5);
            case CvInput::chA_volume:
                return adc_.GetMuxFloat(0, 6);
            case CvInput::chB_speed:
                return adc_.GetMuxFloat(1, 5);
            case CvInput::chB_volume:
                return adc_.GetMuxFloat(1, 6);
            case CvInput::chC_speed:
                return adc_.GetMuxFloat(2, 5);
            case CvInput::chC_volume:
                return adc_.GetMuxFloat(2, 6);
            case CvInput::chD_speed:
                return adc_.GetMuxFloat(3, 5);
            case CvInput::chD_volume:
                return adc_.GetMuxFloat(3, 6);
            case CvInput::NUM_CVS:
                break;
        }
        return 0.0f;
    }

    float getCvVolts(CvInput cv) const
    {
        const auto& coeffs = calibrationStorage_.GetSettings();
        return getCvRaw(cv) * coeffs[int(cv)].scale
               + coeffs[int(cv)].offset;
    }

    CvCalibrationData& getCalibrationData() { return calibrationStorage_.GetSettings(); }
    void saveCalibrationData() { calibrationStorage_.Save(); }

private:
    static constexpr CvCalibrationData getDefaultCalibrationData()
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

        constexpr CvCalibrationData defaults = []() constexpr
        {
            CvCalibrationData result = { { 0.0f, 0.0f } };
            for (size_t i = 0; i < result.size(); i++)
            {
                const auto isSpeedCv = i % 2 == 0;
                result[i] = isSpeedCv ? CvInCoefficients { kScaleSpeed, kOffsetSpeed } : CvInCoefficients { kScaleVolume, kOffsetVolume };
            }
            return result;
        }();

        return defaults;
    }
    static constexpr auto kCalibrationDataOffset = 0;

    mutable daisy::PersistentStorage<CvCalibrationData> calibrationStorage_;

    daisy::AdcHandle adc_;
};

class UiHardware
{
public:
    using LedDriverType = daisy::LedDriverPca9685<3, false>;
    using LedDmaBufferType = LedDriverType::DmaBuffer;

    UiHardware(daisy::QSPIHandle& qspi,
               daisy::UiEventQueue& eventQueue,
               LedDmaBufferType bufferA,
               LedDmaBufferType bufferB,
               uint16_t* shiftRegisterDmaBuffer) :
        knobsAndCv_(qspi)
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

    KnobAndCvReader& getKnobsAndCv() { return knobsAndCv_; }

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
        i2cCfg.pin_config.sda = { daisy::GPIOPort::PORTB, 9 };
        i2cCfg.pin_config.scl = { daisy::GPIOPort::PORTB, 8 };
        i2cCfg.speed = daisy::I2CHandle::Config::Speed::I2C_400KHZ;
        i2cCfg.mode = daisy::I2CHandle::Config::Mode::I2C_MASTER;
        i2c.Init(i2cCfg);

        ledDriver_.Init(i2c, { 0b00, 0b01, 0b10 }, bufferA, bufferB, { daisy::GPIOPort::PORTB, 6 });
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