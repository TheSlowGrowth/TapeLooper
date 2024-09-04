/**
 * Copyright (C) Johannes Elliesen, 2024
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

#include "../constants.h"
#include "../hardware/UiHardwareTypes.h"
#include "LooperParameterProvider.h"

template <typename UiHardwareType>
class UiCalibrationPage : public daisy::UiPage
{
public:
    UiCalibrationPage(UiHardwareType& uiHardware) :
        uiHardware_(uiHardware)
    {
    }

    virtual ~UiCalibrationPage() {}

    bool IsOpaque(const daisy::UiCanvasDescriptor&) override { return false; }

    void OnShow() override
    {
        state_ = State::idle;
        recordedMin_ = 0.0f;
        selectedChannel_ = -1;
    }

    void Draw(const daisy::UiCanvasDescriptor& canvas) override
    {
        UiHardwareType& hardware = *((UiHardwareType*) canvas.handle_);

        // light up the load & save page leds to indicate calibration
        hardware.setLed(Led::load, LedColour::pulsingRed);
        hardware.setLed(Led::save, LedColour::pulsingRed);

        // clear all channel octave leds
        for (auto led : { Led::chA_m2,
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
                          Led::chD_p2 })
        {
            hardware.setLed(led, LedColour::off);
        }

        // still selecting an input ?
        if (selectedChannel_ < 0)
        {
            hardware.setLed(Led::chA_m2, LedColour::pulsingYellow);
            hardware.setLed(Led::chA_p2, LedColour::pulsingYellow);
            hardware.setLed(Led::chB_m2, LedColour::pulsingYellow);
            hardware.setLed(Led::chB_p2, LedColour::pulsingYellow);
            hardware.setLed(Led::chC_m2, LedColour::pulsingYellow);
            hardware.setLed(Led::chC_p2, LedColour::pulsingYellow);
            hardware.setLed(Led::chD_m2, LedColour::pulsingYellow);
            hardware.setLed(Led::chD_p2, LedColour::pulsingYellow);
        }
        else
        {
            constexpr Led lowestLeds[4] = { Led::chA_m2, Led::chB_m2, Led::chC_m2, Led::chD_m2 };
            constexpr Led highestLeds[4] = { Led::chA_p2, Led::chB_p2, Led::chC_p2, Led::chD_p2 };

            switch (state_)
            {
                case State::readMinSpeed:
                    hardware.setLed(lowestLeds[selectedChannel_], LedColour::pulsingRed);
                    break;
                case State::readMaxSpeed:
                    hardware.setLed(highestLeds[selectedChannel_], LedColour::pulsingRed);
                    break;
                case State::readMinVolume:
                    hardware.setLed(lowestLeds[selectedChannel_], LedColour::pulsingGreen);
                    break;
                case State::readMaxVolume:
                    hardware.setLed(highestLeds[selectedChannel_], LedColour::pulsingGreen);
                    break;
                default:
                    break;
            }
        }
    }

    bool OnButton(uint16_t buttonID, uint8_t numberOfPresses, bool isRetriggering) override
    {
        (void) (isRetriggering); // ignore

        // consume but ignore button-up messages
        if (numberOfPresses < 1)
            return true;

        const Button button = Button(buttonID);
        switch (button)
        {
            // channel play buttons
            case Button::chA_play:
                channelButtonPressed(0);
                break;
            case Button::chB_play:
                channelButtonPressed(1);
                break;
            case Button::chC_play:
                channelButtonPressed(2);
                break;
            case Button::chD_play:
                channelButtonPressed(3);
                break;

            case Button::save:
                uiHardware_.getKnobsAndCv().getCalibrationData() = newCalibrationData_;
                uiHardware_.getKnobsAndCv().saveCalibrationData();
                Close();
                break;

            case Button::record:
            case Button::load:
            case Button::settings:
                Close(); // close without saving
                break;

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
    UiCalibrationPage(const UiCalibrationPage&) = delete;
    UiCalibrationPage& operator=(const UiCalibrationPage&) = delete;

    void channelButtonPressed(const int channel)
    {
        // a channel button was pressed.
        // If it's the same channel, advance the calibration procedure.
        if (channel == selectedChannel_)
        {
            constexpr CvInput speedCvForChannel[4] = { CvInput::chA_speed,
                                                       CvInput::chB_speed,
                                                       CvInput::chC_speed,
                                                       CvInput::chD_speed };
            constexpr CvInput volumeCvForChannel[4] = { CvInput::chA_volume,
                                                        CvInput::chB_volume,
                                                        CvInput::chC_volume,
                                                        CvInput::chD_volume };
            switch (state_)
            {
                case State::readMinSpeed:
                {
                    recordedMin_ = uiHardware_.getKnobsAndCv().getCvRaw(speedCvForChannel[selectedChannel_]);
                    state_ = State::readMaxSpeed;
                    break;
                }
                case State::readMaxSpeed:
                {
                    const auto recordedMax = uiHardware_.getKnobsAndCv().getCvRaw(speedCvForChannel[selectedChannel_]);
                    const auto coeffs = calculateCoefficients(recordedMin_, 0.0f, recordedMax, 2.0f);
                    newCalibrationData_[int(speedCvForChannel[selectedChannel_])] = coeffs;
                    state_ = State::readMinVolume;
                    break;
                }
                case State::readMinVolume:
                {
                    recordedMin_ = uiHardware_.getKnobsAndCv().getCvRaw(volumeCvForChannel[selectedChannel_]);
                    state_ = State::readMaxVolume;
                    break;
                }
                case State::readMaxVolume:
                {
                    const auto recordedMax = uiHardware_.getKnobsAndCv().getCvRaw(volumeCvForChannel[selectedChannel_]);
                    const auto coeffs = calculateCoefficients(recordedMin_, 0.0f, recordedMax, 5.0f);
                    newCalibrationData_[int(volumeCvForChannel[selectedChannel_])] = coeffs;
                    selectedChannel_ = -1;
                    state_ = State::idle;
                    break;
                }
                default:
                    break;
            }
        }
        // if it's a different channel, select that channel and start the calibration readings
        else
        {
            selectedChannel_ = channel;
            state_ = State::readMinSpeed;
        }
    }

    CvInCoefficients calculateCoefficients(float rawMin,
                                           float realMin,
                                           float rawMax,
                                           float realMax)
    {
        const auto scale = (realMax - realMin) / (rawMax - rawMin);
        const auto offset = realMax - scale * (rawMax);
        return { scale, offset };
    }

    enum class State
    {
        readMinSpeed,
        readMaxSpeed,
        readMinVolume,
        readMaxVolume,
        idle
    };

    UiHardwareType& uiHardware_;
    int selectedChannel_ = -1;
    State state_ = State::idle;
    float recordedMin_;
    CvCalibrationData newCalibrationData_;
};