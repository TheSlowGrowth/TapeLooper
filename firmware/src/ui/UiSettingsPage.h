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
#include <memory>

#include <ui/UI.h>
#include <dsp/TapeLooper.h>

#include "../constants.h"
#include "../LooperController.h"
#include "../hardware/UiHardwareTypes.h"
#include "LooperParameterProvider.h"

template <typename LooperControllerType, typename UiHardwareType, typename LooperParameterProviderType>
class UiSettingsPage : public daisy::UiPage
{
public:
    UiSettingsPage(LooperControllerType& looperController,
                   LooperParameterProviderType& looperParameterProvider) :
        looperController_(looperController),
        looperParameterProvider_(looperParameterProvider)
    {
    }

    virtual ~UiSettingsPage() {}

    bool IsOpaque(const daisy::UiCanvasDescriptor&) override { return false; }

    void Draw(const daisy::UiCanvasDescriptor& canvas) override
    {
        UiHardwareType& hardware = *((UiHardwareType*) canvas.handle_);

        // light up the settings page led
        if (currentSetting_ == Setting::direction)
            hardware.setLed(Led::settings, LedColour::pulsingRed);
        else if (currentSetting_ == Setting::numChannels)
            hardware.setLed(Led::settings, LedColour::pulsingYellow);
        else
            hardware.setLed(Led::settings, LedColour::pulsingGreen);

        // update channel octave LEDs to show the currently selected setting
        const auto updateChannelOctaveLeds = [&](size_t looperChannel, std::array<Led, 4> ledIds)
        {
            if (currentSetting_ == Setting::direction)
            {
                if (looperController_.getDirection(looperChannel) == Direction::forwards)
                {
                    hardware.setLed(ledIds[0], LedColour::off);
                    hardware.setLed(ledIds[1], LedColour::off);
                    hardware.setLed(ledIds[2], LedColour::red);
                    hardware.setLed(ledIds[3], LedColour::red);
                }
                else
                {
                    hardware.setLed(ledIds[0], LedColour::red);
                    hardware.setLed(ledIds[1], LedColour::red);
                    hardware.setLed(ledIds[2], LedColour::off);
                    hardware.setLed(ledIds[3], LedColour::off);
                }
            }
            else if (currentSetting_ == Setting::numChannels)
            {
                hardware.setLed(ledIds[0], LedColour::yellow);
                hardware.setLed(ledIds[1], LedColour::yellow);
                if (looperController_.getChannelLayout(looperChannel) == ChannelLayout::stereo)
                {
                    hardware.setLed(ledIds[2], LedColour::yellow);
                    hardware.setLed(ledIds[3], LedColour::yellow);
                }
                else
                {
                    hardware.setLed(ledIds[2], LedColour::off);
                    hardware.setLed(ledIds[3], LedColour::off);
                }
            }
            else
            {
                const auto acceleration = looperController_.getMotorAcceleration(looperChannel);
                hardware.setLed(ledIds[0],
                                acceleration == MotorAcceleration::verySlow
                                    ? LedColour::green
                                    : LedColour::off);
                hardware.setLed(ledIds[1],
                                (acceleration == MotorAcceleration::slow) || (acceleration == MotorAcceleration::medium)
                                    ? LedColour::green
                                    : LedColour::off);
                hardware.setLed(ledIds[2],
                                (acceleration == MotorAcceleration::medium) || (acceleration == MotorAcceleration::fast)
                                    ? LedColour::green
                                    : LedColour::off);
                hardware.setLed(ledIds[3],
                                acceleration == MotorAcceleration::veryFast
                                    ? LedColour::green
                                    : LedColour::off);
            }
        };
        updateChannelOctaveLeds(0, { Led::chA_m2, Led::chA_m1, Led::chA_p1, Led::chA_p2 });
        updateChannelOctaveLeds(1, { Led::chB_m2, Led::chB_m1, Led::chB_p1, Led::chB_p2 });
        updateChannelOctaveLeds(2, { Led::chC_m2, Led::chC_m1, Led::chC_p1, Led::chC_p2 });
        updateChannelOctaveLeds(3, { Led::chD_m2, Led::chD_m1, Led::chD_p1, Led::chD_p2 });
    }

    void OnFocusGained() override
    {
        currentSetting_ = Setting::direction;
    }

    bool OnButton(uint16_t buttonID, uint8_t numberOfPresses, bool isRetriggering) override
    {
        (void) (isRetriggering); // ignore this argument

        // swallow but ignore button-up messages
        if (numberOfPresses < 1)
            return true;

        const Button button = Button(buttonID);
        switch (button)
        {
            // channel up rocker switch
            case Button::chA_up:
                onUpButton(0);
                break;
            case Button::chB_up:
                onUpButton(1);
                break;
            case Button::chC_up:
                onUpButton(2);
                break;
            case Button::chD_up:
                onUpButton(3);
                break;
            // channel down rocker switch
            case Button::chA_down:
                onDownButton(0);
                break;
            case Button::chB_down:
                onDownButton(1);
                break;
            case Button::chC_down:
                onDownButton(2);
                break;
            case Button::chD_down:
                onDownButton(3);
                break;
            // cycle through settings; then return to base page
            case Button::settings:
                if (currentSetting_ == Setting::direction)
                    currentSetting_ = Setting::numChannels;
                else if (currentSetting_ == Setting::numChannels)
                    currentSetting_ = Setting::motorLag;
                else if (currentSetting_ == Setting::motorLag)
                    Close(); // close this page
                break;
            case Button::record:
            case Button::load:
            case Button::save:
                Close(); // close this page
                return false; // pass event to the page below to open the respective page
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
    UiSettingsPage(const UiSettingsPage&) = delete;
    UiSettingsPage& operator=(const UiSettingsPage&) = delete;

    void onUpButton(size_t looperChannel)
    {
        if (currentSetting_ == Setting::direction)
        {
            if (looperController_.getDirection(looperChannel) != Direction::forwards)
                looperController_.setDirection(looperChannel, Direction::forwards);
        }
        else if (currentSetting_ == Setting::numChannels)
        {
            if (looperController_.getChannelLayout(looperChannel) != ChannelLayout::stereo)
                looperController_.setChannelLayout(looperChannel, ChannelLayout::stereo);
        }
        else
        {
            auto currentAcceleration = looperController_.getMotorAcceleration(looperChannel);
            switch (currentAcceleration)
            {
                case MotorAcceleration::verySlow:
                    looperController_.setMotorAcceleration(looperChannel, MotorAcceleration::slow);
                    break;
                case MotorAcceleration::slow:
                    looperController_.setMotorAcceleration(looperChannel, MotorAcceleration::medium);
                    break;
                case MotorAcceleration::medium:
                    looperController_.setMotorAcceleration(looperChannel, MotorAcceleration::fast);
                    break;
                case MotorAcceleration::fast:
                    looperController_.setMotorAcceleration(looperChannel, MotorAcceleration::veryFast);
                    break;
                default:
                    break;
            }
        }
    }

    void onDownButton(size_t looperChannel)
    {
        if (currentSetting_ == Setting::direction)
        {
            if (looperController_.getDirection(looperChannel) != Direction::backwards)
                looperController_.setDirection(looperChannel, Direction::backwards);
        }
        else if (currentSetting_ == Setting::numChannels)
        {
            if (looperController_.getChannelLayout(looperChannel) != ChannelLayout::mono)
                looperController_.setChannelLayout(looperChannel, ChannelLayout::mono);
        }
        else
        {
            auto currentAcceleration = looperController_.getMotorAcceleration(looperChannel);
            switch (currentAcceleration)
            {
                case MotorAcceleration::veryFast:
                    looperController_.setMotorAcceleration(looperChannel, MotorAcceleration::fast);
                    break;
                case MotorAcceleration::fast:
                    looperController_.setMotorAcceleration(looperChannel, MotorAcceleration::medium);
                    break;
                case MotorAcceleration::medium:
                    looperController_.setMotorAcceleration(looperChannel, MotorAcceleration::slow);
                    break;
                case MotorAcceleration::slow:
                    looperController_.setMotorAcceleration(looperChannel, MotorAcceleration::verySlow);
                    break;
                default:
                    break;
            }
        }
    }

    enum class Setting
    {
        direction,
        numChannels,
        motorLag
    };
    Setting currentSetting_ = Setting::direction;

    LooperControllerType& looperController_;
    LooperParameterProviderType& looperParameterProvider_;
};