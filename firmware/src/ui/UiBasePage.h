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
#include "../hardware/UiHardwareTypes.h"
#include "LooperParameterProvider.h"

template <typename PeakMeterType, typename LooperControllerType, typename UiHardwareType, typename ParameterProviderType>
class UiBasePage : public daisy::UiPage
{
public:
    UiBasePage(const std::array<PeakMeterType, 4>& peakMeters,
               LooperControllerType& looperController,
               ParameterProviderType& looperParameterProvider,
               daisy::UiPage& settingsPage,
               daisy::UiPage& savePage,
               daisy::UiPage& loadPage,
               daisy::UiPage& recordPage) :
        peakMeters_(peakMeters),
        looperController_(looperController),
        looperParameterProvider_(looperParameterProvider),
        settingsPage_(settingsPage),
        savePage_(savePage),
        loadPage_(loadPage),
        recordPage_(recordPage)
    {
    }

    virtual ~UiBasePage() {}

    void Draw(const daisy::UiCanvasDescriptor& canvas) override
    {
        UiHardwareType& hardware = *((UiHardwareType*) canvas.handle_);

        // update peak meter LEDs
        float red, green;
        peakMeters_[0].getRedAndGreenValues(red, green);
        hardware.setLedFromRawValues(Led::peakMeterInL, red, green);
        peakMeters_[1].getRedAndGreenValues(red, green);
        hardware.setLedFromRawValues(Led::peakMeterInR, red, green);
        peakMeters_[2].getRedAndGreenValues(red, green);
        hardware.setLedFromRawValues(Led::peakMeterOutL, red, green);
        peakMeters_[3].getRedAndGreenValues(red, green);
        hardware.setLedFromRawValues(Led::peakMeterOutR, red, green);

        // update channel octave LEDs
        const auto updateChannelOctaveLeds = [&](size_t looperChannel, std::array<Led, 4> ledIds)
        {
            // select colour
            const auto looperState = looperController_.getLooperState(looperChannel);
            auto colourToUse = LedColour::green;
            if (looperState == LooperState::recording)
                colourToUse = LedColour::pulsingRed;
            else if (looperState == LooperState::playing)
                colourToUse = LedColour::yellow;

            const auto octave = looperParameterProvider_.controlInputs_[looperChannel].octave;
            hardware.setLed(ledIds[0],
                            octave == -2 ? colourToUse : LedColour::off);
            hardware.setLed(ledIds[1],
                            (octave == -1) || (octave == 0) ? colourToUse : LedColour::off);
            hardware.setLed(ledIds[2],
                            (octave == 1) || (octave == 0) ? colourToUse : LedColour::off);
            hardware.setLed(ledIds[3],
                            octave == 2 ? colourToUse : LedColour::off);
        };
        updateChannelOctaveLeds(0, { Led::chA_m2, Led::chA_m1, Led::chA_p1, Led::chA_p2 });
        updateChannelOctaveLeds(1, { Led::chB_m2, Led::chB_m1, Led::chB_p1, Led::chB_p2 });
        updateChannelOctaveLeds(2, { Led::chC_m2, Led::chC_m1, Led::chC_p1, Led::chC_p2 });
        updateChannelOctaveLeds(3, { Led::chD_m2, Led::chD_m1, Led::chD_p1, Led::chD_p2 });
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
            // channel play buttons
            case Button::chA_play:
                onPlayButton(0);
                break;
            case Button::chB_play:
                onPlayButton(1);
                break;
            case Button::chC_play:
                onPlayButton(2);
                break;
            case Button::chD_play:
                onPlayButton(3);
                break;
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
            // switch to sub pages
            case Button::settings:
                GetParentUI()->OpenPage(settingsPage_);
                break;
            case Button::save:
                GetParentUI()->OpenPage(savePage_);
                break;
            case Button::load:
                GetParentUI()->OpenPage(loadPage_);
                break;
            case Button::record:
                GetParentUI()->OpenPage(recordPage_);
                break;
            default:
                break;
        }
        return true;
    }

    bool OnPotMoved(uint16_t potID, float newPosition) override
    {
        // write raw user inputs to the looperParameterProvider

        const Pot pot = Pot(potID);
        switch (pot)
        {
            // speed / pitch
            case Pot::chA_speed:
                looperParameterProvider_.controlInputs_[0].fineTunePosition = newPosition;
                break;
            case Pot::chB_speed:
                looperParameterProvider_.controlInputs_[1].fineTunePosition = newPosition;
                break;
            case Pot::chC_speed:
                looperParameterProvider_.controlInputs_[2].fineTunePosition = newPosition;
                break;
            case Pot::chD_speed:
                looperParameterProvider_.controlInputs_[3].fineTunePosition = newPosition;
                break;
            // grain
            case Pot::chA_grainAmt:
                looperParameterProvider_.controlInputs_[0].grainAmt = newPosition;
                break;
            case Pot::chB_grainAmt:
                looperParameterProvider_.controlInputs_[1].grainAmt = newPosition;
                break;
            case Pot::chC_grainAmt:
                looperParameterProvider_.controlInputs_[2].grainAmt = newPosition;
                break;
            case Pot::chD_grainAmt:
                looperParameterProvider_.controlInputs_[3].grainAmt = newPosition;
                break;
            // warble
            case Pot::chA_warbleAmt:
                looperParameterProvider_.controlInputs_[0].wowAndFlutterAmt = newPosition;
                break;
            case Pot::chB_warbleAmt:
                looperParameterProvider_.controlInputs_[1].wowAndFlutterAmt = newPosition;
                break;
            case Pot::chC_warbleAmt:
                looperParameterProvider_.controlInputs_[2].wowAndFlutterAmt = newPosition;
                break;
            case Pot::chD_warbleAmt:
                looperParameterProvider_.controlInputs_[3].wowAndFlutterAmt = newPosition;
                break;
            // drive
            case Pot::chA_driveAmt:
                looperParameterProvider_.controlInputs_[0].driveAmt = newPosition;
                break;
            case Pot::chB_driveAmt:
                looperParameterProvider_.controlInputs_[1].driveAmt = newPosition;
                break;
            case Pot::chC_driveAmt:
                looperParameterProvider_.controlInputs_[2].driveAmt = newPosition;
                break;
            case Pot::chD_driveAmt:
                looperParameterProvider_.controlInputs_[3].driveAmt = newPosition;
                break;
            // volume
            case Pot::chA_volume:
                looperParameterProvider_.controlInputs_[0].volumeSliderPosition = newPosition;
                break;
            case Pot::chB_volume:
                looperParameterProvider_.controlInputs_[1].volumeSliderPosition = newPosition;
                break;
            case Pot::chC_volume:
                looperParameterProvider_.controlInputs_[2].volumeSliderPosition = newPosition;
                break;
            case Pot::chD_volume:
                looperParameterProvider_.controlInputs_[3].volumeSliderPosition = newPosition;
                break;
            default:
                break;
        }

        return true;
    }

private:
    UiBasePage(const UiBasePage&) = delete;
    UiBasePage& operator=(const UiBasePage&) = delete;

    void onPlayButton(size_t looperChannel)
    {
        const auto currentState = looperController_.getLooperState(looperChannel);
        if (currentState == LooperState::playing)
            looperController_.setLooperState(looperChannel, LooperState::stopped);
        else if (currentState == LooperState::stopped)
            looperController_.setLooperState(looperChannel, LooperState::playing);
        else if (currentState == LooperState::recording)
            looperController_.setLooperState(looperChannel, LooperState::playing);
    }

    void onUpButton(size_t looperChannel)
    {
        auto& currentOctave = looperParameterProvider_.controlInputs_[looperChannel].octave;
        if (currentOctave < 2)
            currentOctave++;
    }

    void onDownButton(size_t looperChannel)
    {
        auto& currentOctave = looperParameterProvider_.controlInputs_[looperChannel].octave;
        if (currentOctave > -2)
            currentOctave--;
    }

    const std::array<PeakMeterType, 4>& peakMeters_;
    LooperControllerType& looperController_;
    ParameterProviderType& looperParameterProvider_;
    daisy::UiPage& settingsPage_;
    daisy::UiPage& savePage_;
    daisy::UiPage& loadPage_;
    daisy::UiPage& recordPage_;
};