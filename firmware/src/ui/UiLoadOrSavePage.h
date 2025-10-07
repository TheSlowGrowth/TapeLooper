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

template <typename LooperControllerType, typename UiHardwareType, Led PageLed>
class UiLoadOrSavePage : public daisy::UiPage
{
public:
    UiLoadOrSavePage(LooperControllerType& looperController) :
        looperController_(looperController)
    {
    }

    virtual ~UiLoadOrSavePage() {}

    bool IsOpaque(const daisy::UiCanvasDescriptor&) override { return false; }

    void OnShow() override
    {
        stage_ = Stage::selectChannel;
        selectedChannel_ = -1;
    }

    void Draw(const daisy::UiCanvasDescriptor& canvas) override
    {
        UiHardwareType& hardware = *((UiHardwareType*) canvas.handle_);

        // light up the page led
        hardware.setLed(PageLed, LedColour::pulsingRed);

        switch (stage_)
        {
            case Stage::selectChannel:
                drawSelectChannel(hardware);
                break;
            case Stage::selectBankAndSlot:
                drawSelectBankAndSlot(hardware);
                break;
            case Stage::saveOrLoadInProgress:
                drawProgress(hardware);
                break;
            case Stage::displayError:
                drawResult(hardware, AudioSaveAndRecallResult::error);
                break;
            case Stage::displaySuccess:
                drawResult(hardware, AudioSaveAndRecallResult::ok);
                break;
            default:
                break;
        }
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
            case Button::record:
                [[fallthrough]];
            case Button::settings:
                looperController_.abortLoadOrSaveOperation();
                Close(); // close this page
                return false; // pass event to the page below to open the respective page
            case Button::save:
                return handleSaveButton();
            case Button::load:
                return handleLoadButton();
            default:
                break;
        }

        switch (stage_)
        {
            case Stage::selectChannel:
                return onButtonInChannelSelection(button);
            case Stage::selectBankAndSlot:
                return onButtonInBankAndSlotSelection(button);
            default:
                // any button press while showing
                if (stage_ == Stage::displaySuccess || stage_ == Stage::displayError)
                {
                    Close();
                }
                break;
        }

        return true;
    }

    bool OnPotMoved(uint16_t, float) override
    {
        return false; // passthrough pot events to the base page
    }

protected:
    UiLoadOrSavePage(const UiLoadOrSavePage&) = delete;
    UiLoadOrSavePage& operator=(const UiLoadOrSavePage&) = delete;

    static LedColour getBankColourPulsing(StorageBank bank)
    {
        if (bank == StorageBank::green)
            return LedColour::pulsingGreen;
        if (bank == StorageBank::yellow)
            return LedColour::pulsingYellow;
        if (bank == StorageBank::red)
            return LedColour::pulsingRed;

        return LedColour::off;
    };

    static LedColour getBankColour(StorageBank bank)
    {
        if (bank == StorageBank::green)
            return LedColour::green;
        if (bank == StorageBank::yellow)
            return LedColour::yellow;
        if (bank == StorageBank::red)
            return LedColour::red;

        return LedColour::off;
    };

    void drawSelectChannel(UiHardwareType& hardware)
    {
        hardware.setLed(Led::chA_p2, LedColour::pulsingYellow);
        hardware.setLed(Led::chA_p1, LedColour::pulsingYellow);
        hardware.setLed(Led::chA_m1, LedColour::pulsingYellow);
        hardware.setLed(Led::chA_m2, LedColour::pulsingYellow);

        hardware.setLed(Led::chB_p2, LedColour::pulsingYellow);
        hardware.setLed(Led::chB_p1, LedColour::pulsingYellow);
        hardware.setLed(Led::chB_m1, LedColour::pulsingYellow);
        hardware.setLed(Led::chB_m2, LedColour::pulsingYellow);

        hardware.setLed(Led::chC_p2, LedColour::pulsingYellow);
        hardware.setLed(Led::chC_p1, LedColour::pulsingYellow);
        hardware.setLed(Led::chC_m1, LedColour::pulsingYellow);
        hardware.setLed(Led::chC_m2, LedColour::pulsingYellow);

        hardware.setLed(Led::chD_p2, LedColour::pulsingYellow);
        hardware.setLed(Led::chD_p1, LedColour::pulsingYellow);
        hardware.setLed(Led::chD_m1, LedColour::pulsingYellow);
        hardware.setLed(Led::chD_m2, LedColour::pulsingYellow);
    }

    void drawSelectBankAndSlot(UiHardwareType& hardware)
    {
        const auto bankColour = this->getBankColour(this->selectedBank_);
        const auto bankColourPulsing = this->getBankColourPulsing(this->selectedBank_);

        hardware.setLed(Led::settings, bankColourPulsing);

        const auto allLeds = UiHardwareType::allChannelLeds();
        for (size_t i = 0; i < allLeds.size(); i++)
        {
            hardware.setLed(allLeds[i], this->selectedSlot_ == int(i) ? bankColorPulsing : LedColour::off);
        }
    }

    void drawProgress(UiHardwareType& hardware)
    {
        const auto allLeds = UiHardwareType::allChannelLeds();
        for (size_t i = 0; i < allLeds.size(); i++)
        {
            const auto shouldBeOn = looperController_.getCurrentSaveOrLoadProgress()
                                    > float(i) / float(allLeds.size() - 1);
            hardware.setLed(allLeds[i], shouldBeOn ? LedColour::pulsingYellow : LedColour::off);
        }
    }

    void drawResult(UiHardwareType& hardware, AudioSaveAndRecallResult result)
    {
        const auto color = result == AudioSaveAndRecallResult::ok ? LedColour::pulsingGreen : LedColour::pulsingRed;
        for (auto led : UiHardwareType::allChannelLeds())
        {
            hardware.setLed(led, color);
        }

        if (daisy::System::GetNow() >= closeAfterTime_)
        {
            Close();
        }
    }

    bool onButtonInChannelSelection(Button button)
    {
        switch (button)
        {
            case Button::chA_play:
                selectedChannel_ = 0;
                stage_ = Stage::selectBankAndSlot;
                break;
            case Button::chB_play:
                selectedChannel_ = 1;
                stage_ = Stage::selectBankAndSlot;
                break;
            case Button::chC_play:
                selectedChannel_ = 2;
                stage_ = Stage::selectBankAndSlot;
                break;
            case Button::chD_play:
                selectedChannel_ = 3;
                stage_ = Stage::selectBankAndSlot;
                break;
            default:
                break;
        }

        return true;
    }

    bool onButtonInBankAndSlotSelection(Button button)
    {
        const auto handleChannelUp = [&](int channel)
        {
            if (selectedSlot_ / 4 != channel)
            {
                selectedSlot_ = selectedSlot_ % 4 + channel * 4;
                return;
            }
            selectedSlot_ = std::min(selectedSlot_ + 1, channel * 4 + 3);
        };
        const auto handleChannelDown = [&](int channel)
        {
            if (selectedSlot_ / 4 != channel)
            {
                selectedSlot_ = selectedSlot_ % 4 + channel * 4;
                return;
            }
            selectedSlot_ = std::max(selectedSlot_ - 1, channel * 4);
        };

        switch (button)
        {
            case Button::chA_down:
                handleChannelDown(0);
                break;
            case Button::chA_up:
                handleChannelUp(0);
                break;
            case Button::chB_down:
                handleChannelDown(1);
                break;
            case Button::chB_up:
                handleChannelUp(1);
                break;
            case Button::chC_down:
                handleChannelDown(2);
                break;
            case Button::chC_up:
                handleChannelUp(2);
                break;
            case Button::chD_down:
                handleChannelDown(3);
                break;
            case Button::chD_up:
                handleChannelUp(3);
                break;
            case Button::settings:
                selectedBank_ = StorageBank((int(selectedBank_) + 1) % int(StorageBank::count));
                break;
            default:
                break;
        }

        return true;
    }

    virtual bool handleSaveButton() = 0;
    virtual bool handleLoadButton() = 0;

    static void doneCallback(void* context, AudioSaveAndRecallResult result)
    {
        const auto page = static_cast<UiLoadOrSavePage*>(context);
        page->stage_ = result == AudioSaveAndRecallResult::ok ? Stage::displaySuccess : Stage::displayError;
        page->closeAfterTime_ = daisy::System::GetNow() + 2000;
    }

    enum class Stage
    {
        selectChannel,
        selectBankAndSlot,
        saveOrLoadInProgress,
        displaySuccess,
        displayError,
    };

    Stage stage_ = Stage::selectChannel;
    StorageBank selectedBank_ = StorageBank::green;
    int selectedChannel_ = -1;
    int selectedSlot_ = 0;
    LooperControllerType& looperController_;
    uint32_t closeAfterTime_ = -1;
};

template <typename LooperControllerType, typename UiHardwareType>
class UiSavePage : public UiLoadOrSavePage<LooperControllerType, UiHardwareType, Led::save>
{
    using ParentType = UiLoadOrSavePage<LooperControllerType, UiHardwareType, Led::save>;

public:
    UiSavePage(LooperControllerType& looperController) :
        ParentType(looperController)
    {
    }

    ~UiSavePage() = default;

    bool handleSaveButton() override
    {
        if (this->stage_ == ParentType::Stage::selectBankAndSlot)
        {
            this->stage_ = ParentType::Stage::saveOrLoadInProgress;
            this->looperController_.saveTo(this->selectedBank_,
                                           this->selectedChannel_,
                                           this->selectedSlot_,
                                           &this->doneCallback,
                                           this);
        }
        else if (this->stage_ == ParentType::Stage::selectChannel
                 || this->stage_ == ParentType::Stage::displaySuccess
                 || this->stage_ == ParentType::Stage::displayError)
        {
            Close();
        }
        return true;
    }

    bool handleLoadButton() override
    {
        this->looperController_.abortLoadOrSaveOperation();
        this->Close();
        return false; // pass event to the page below to open the respective page
    }
};

template <typename LooperControllerType, typename UiHardwareType>
class UiLoadPage : public UiLoadOrSavePage<LooperControllerType, UiHardwareType, Led::load>
{
    using ParentType = UiLoadOrSavePage<LooperControllerType, UiHardwareType, Led::load>;

public:
    UiLoadPage(LooperControllerType& looperController) :
        ParentType(looperController)
    {
    }

    ~UiLoadPage() = default;

    bool handleSaveButton() override
    {
        this->looperController_.abortLoadOrSaveOperation();
        this->Close();
        return false; // pass event to the page below to open the respective page
    }

    bool handleLoadButton() override
    {
        if (this->stage_ == ParentType::Stage::selectBankAndSlot)
        {
            this->stage_ = ParentType::Stage::saveOrLoadInProgress;
            this->looperController_.loadFrom(this->selectedBank_,
                                             this->selectedChannel_,
                                             this->selectedSlot_,
                                             &this->doneCallback,
                                             this);
        }
        else if (this->stage_ == ParentType::Stage::selectChannel
                 || this->stage_ == ParentType::Stage::displaySuccess
                 || this->stage_ == ParentType::Stage::displayError)
        {
            Close();
        }
        return true;
    }
};