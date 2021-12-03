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

#include <ui/UI.h>
#include <dsp/DspHelpers.h>

#include "LooperParameterProvider.h"
#include "UiBasePage.h"
#include "UiSettingsPage.h"
#include "UiSavePage.h"
#include "UiLoadPage.h"
#include "UiRecordingPage.h"

template <typename UiHardwareType,
          typename PeakMeterType,
          typename LooperControllerType,
          typename ParameterProviderType>
class TapeLooperUi
{
public:
    TapeLooperUi(UiHardwareType& uiHardware,
                 daisy::UiCanvasDescriptor::ClearFuncPtr clearFunction,
                 daisy::UiCanvasDescriptor::FlushFuncPtr flushFunction,
                 daisy::UiEventQueue& eventQueue,
                 const std::array<PeakMeterType, 4>& peakMeters,
                 LooperControllerType& looperController,
                 ParameterProviderType& looperParameterProvider) :
        uiHardware_(uiHardware),
        eventQueue_(eventQueue),
        settingsPage_(looperController,
                      looperParameterProvider),
        savePage_(looperController),
        loadPage_(looperController),
        recordingPage_(looperController),
        basePage_(peakMeters,
                  looperController,
                  looperParameterProvider,
                  settingsPage_,
                  savePage_,
                  loadPage_,
                  recordingPage_)
    {
        daisy::UiCanvasDescriptor ledCanvas;
        ledCanvas.clearFunction_ = clearFunction;
        ledCanvas.flushFunction_ = flushFunction;
        ledCanvas.handle_ = &uiHardware_;
        ledCanvas.id_ = 0;
        ledCanvas.updateRateMs_ = 20; // 50Hz

        ui_.Init(eventQueue_,
                 daisy::UI::SpecialControlIds {},
                 { ledCanvas });
        ui_.OpenPage(basePage_);
    }

    void process()
    {
        ui_.Process();
    }

private:
    TapeLooperUi(const TapeLooperUi&) = delete;
    TapeLooperUi& operator=(const TapeLooperUi&) = delete;

    UiHardwareType& uiHardware_;
    daisy::UiEventQueue& eventQueue_;
    daisy::UI ui_;
    UiSettingsPage<LooperControllerType,
                   UiHardwareType,
                   ParameterProviderType>
        settingsPage_;
    UiSavePage<LooperControllerType,
               UiHardwareType>
        savePage_;
    UiLoadPage<LooperControllerType,
               UiHardwareType>
        loadPage_;
    UiRecordingPage<LooperControllerType,
                    UiHardwareType>
        recordingPage_;
    UiBasePage<PeakMeterType,
               LooperControllerType,
               UiHardwareType,
               ParameterProviderType>
        basePage_;
};