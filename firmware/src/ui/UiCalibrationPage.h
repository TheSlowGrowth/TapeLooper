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

private:
    UiCalibrationPage(const UiCalibrationPage&) = delete;
    UiCalibrationPage& operator=(const UiCalibrationPage&) = delete;

    UiHardwareType& uiHardware_;
};