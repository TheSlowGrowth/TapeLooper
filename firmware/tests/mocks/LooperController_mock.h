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

#include <functional>

#include <gtest/gtest.h>

#include <dsp/TapeLooper.h>
#include "LooperController.h"

template <size_t numLoopers>
class LooperControllerMock
{
public:
    std::string events;
    std::string pullEvents()
    {
        const auto result = events;
        events.clear();
        return result;
    }

    void setLooperState(size_t looperIdx, LooperState state)
    {
        std::ostringstream stringStream;
        stringStream << "setLooperState(" << looperIdx << ", ";
        switch (state)
        {
            case LooperState::playing:
                stringStream << "LooperState::playing)";
                break;
            case LooperState::stopped:
                stringStream << "LooperState::stopped)";
                break;
            case LooperState::recording:
                stringStream << "LooperState::recording)";
                break;
        };
        events += stringStream.str() + "\n";
    }

    std::function<LooperState(size_t looperIdx)> getLooperStateMock_;
    LooperState getLooperState(size_t looperIdx) const
    {
        if (getLooperStateMock_)
            return getLooperStateMock_(looperIdx);
        else
            return LooperState::stopped;
    }

    void saveTo(size_t looperIdx, size_t slot)
    {
        std::ostringstream stringStream;
        stringStream << "saveTo(" << looperIdx << ", " << slot << ")";
        events += stringStream.str() + "\n";
    }

    void loadFrom(size_t looperIdx, size_t slot)
    {
        std::ostringstream stringStream;
        stringStream << "loadFrom(" << looperIdx << ", " << slot << ")";
        events += stringStream.str() + "\n";
    }

    void setChannelLayout(size_t looperIdx, ChannelLayout channelLayout)
    {
        std::ostringstream stringStream;
        stringStream << "setChannelLayout("
                     << looperIdx << ", ChannelLayout::"
                     << (channelLayout == ChannelLayout::stereo ? "stereo)" : "mono)");
        events += stringStream.str() + "\n";
    }

    std::function<ChannelLayout(size_t looperIdx)> getChannelLayoutMock_;
    ChannelLayout getChannelLayout(size_t looperIdx) const
    {
        if (getChannelLayoutMock_)
            return getChannelLayoutMock_(looperIdx);
        else
            return ChannelLayout::stereo;
    }

    void setMotorAcceleration(size_t looperIdx, MotorAcceleration acceleration)
    {
        std::ostringstream stringStream;
        stringStream << "setMotorAcceleration(" << looperIdx << ", ";
        switch (acceleration)
        {
            case MotorAcceleration::instantaneous:
                stringStream << "MotorAcceleration::instantaneous)";
                break;
            case MotorAcceleration::verySlow:
                stringStream << "MotorAcceleration::verySlow)";
                break;
            case MotorAcceleration::slow:
                stringStream << "MotorAcceleration::slow)";
                break;
            case MotorAcceleration::medium:
                stringStream << "MotorAcceleration::medium)";
                break;
            case MotorAcceleration::fast:
                stringStream << "MotorAcceleration::fast)";
                break;
            case MotorAcceleration::veryFast:
                stringStream << "MotorAcceleration::veryFast)";
                break;
        };
        events += stringStream.str() + "\n";
    }

    std::function<MotorAcceleration(size_t looperIdx)> getMotorAccelerationMock_;
    MotorAcceleration getMotorAcceleration(size_t looperIdx) const
    {
        if (getMotorAccelerationMock_)
            return getMotorAccelerationMock_(looperIdx);
        else
            return MotorAcceleration::medium;
    }

    void setDirection(size_t looperIdx, Direction direction)
    {
        std::ostringstream stringStream;
        stringStream << "setDirection("
                     << looperIdx << ", Direction::"
                     << (direction == Direction::forwards ? "forwards)" : "backwards)");
        events += stringStream.str() + "\n";
    }

    std::function<Direction(size_t looperIdx)> getDirectionMock_;
    Direction getDirection(size_t looperIdx) const
    {
        if (getDirectionMock_)
            return getDirectionMock_(looperIdx);
        else
            return Direction::forwards;
    }
};