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
#include <memory.h>

#include <dsp/TapeLooper.h>

#include "constants.h"
#include "util/LateInitializedObject.h"

enum class ChannelLayout
{
    mono,
    stereo
};

enum class MotorAcceleration
{
    instantaneous = 0,
    veryFast,
    fast,
    medium,
    slow,
    verySlow
};

template <typename LooperTypes, size_t numLoopers>
class LooperController
{
public:
    using MonoLooperType = typename LooperTypes::MonoLooperType;
    using StereoLooperType = typename LooperTypes::StereoLooperType;
    using ParameterProvider = typename LooperTypes::ParameterProvider;

    LooperController(std::array<MonoOrStereoLooperStoragePtr, numLoopers> looperStorage,
                     AudioBufferPtr<1> monoDownmixBuffer,
                     AudioBufferPtr<1> temporaryBuffer,
                     const ParameterProvider& paramProvider) :
        looperStorage_(looperStorage),
        monoDownmixBuffer_(monoDownmixBuffer),
        temporaryBuffer_(temporaryBuffer),
        paramProvider_(paramProvider)
    {
        for (size_t i = 0; i < numLoopers; i++)
            loopers_[i].initializeToLayout(ChannelLayout::stereo, looperStorage_[i]);
    }

    void setLooperState(size_t looperIdx, LooperState state)
    {
        if (loopers_[looperIdx].layout == ChannelLayout::mono)
            loopers_[looperIdx].looper.template as<MonoLooperType>().switchState(state);
        else
            loopers_[looperIdx].looper.template as<StereoLooperType>().switchState(state);
    }

    LooperState getLooperState(size_t looperIdx) const
    {
        if (loopers_[looperIdx].layout == ChannelLayout::mono)
            return loopers_[looperIdx].looper.template as<MonoLooperType>().getState();
        else
            return loopers_[looperIdx].looper.template as<StereoLooperType>().getState();
    }

    void saveTo(size_t looperIdx, size_t slot)
    {
        (void) (looperIdx);
        (void) (slot);
        // TODO
    }

    void loadFrom(size_t looperIdx, size_t slot)
    {
        (void) (looperIdx);
        (void) (slot);
        // TODO
    }

    void setChannelLayout(size_t looperIdx, ChannelLayout channelLayout)
    {
        loopers_[looperIdx].initializeToLayout(channelLayout, looperStorage_[looperIdx]);
    }
    ChannelLayout getChannelLayout(size_t looperIdx) const
    {
        return loopers_[looperIdx].layout;
    }

    void setMotorAcceleration(size_t looperIdx, MotorAcceleration acceleration)
    {
        loopers_[looperIdx].acceleration = acceleration;
    }
    MotorAcceleration getMotorAcceleration(size_t looperIdx) const
    {
        return loopers_[looperIdx].acceleration;
    }

    void setDirection(size_t looperIdx, Direction direction)
    {
        loopers_[looperIdx].direction = direction;
    }
    Direction getDirection(size_t looperIdx) const
    {
        return loopers_[looperIdx].direction;
    }

    void process(AudioBufferPtr<2, const float> inputBuffer, AudioBufferPtr<2> outputBuffer)
    {
        outputBuffer.fill(0.0f);
        
        bool hasProcessedMonoDownmix = false;
        for (size_t ch = 0; ch < numLoopers; ch++)
        {
            auto& looperChannel = loopers_[ch];

            const auto processorParameters =
                paramProvider_.getProcessorParameters(ch);

            if (looperChannel.layout == ChannelLayout::mono)
            {
                // input downmix
                if (!hasProcessedMonoDownmix)
                {
                    monoDownmixBuffer_.copyToChannel(0, inputBuffer[0], inputBuffer.size_);
                    monoDownmixBuffer_.addToChannel(0, inputBuffer[1], inputBuffer.size_);
                    monoDownmixBuffer_.applyGain(0.5f);
                    hasProcessedMonoDownmix = true;
                }
                // processing
                temporaryBuffer_.fill(0.0f);
                looperChannel.looper.template as<MonoLooperType>().process(
                    paramProvider_.getSpeedParameter(ch),
                    paramProvider_.getWowAndFlutterAmtParameter(ch),
                    looperChannel.direction,
                    processorParameters,
                    paramProvider_.getGainParameter(ch),
                    monoDownmixBuffer_.subBlock(0, inputBuffer.size_),
                    temporaryBuffer_,
                    postGainSmootherTimeConstant_,
                    getMotorTimeConstant(looperChannel.acceleration));

                // output upmix
                outputBuffer.addToChannel(0, temporaryBuffer_[0], temporaryBuffer_.size_);
                outputBuffer.addToChannel(1, temporaryBuffer_[0], temporaryBuffer_.size_);
            }
            else // stereo
            {
                looperChannel.looper.template as<StereoLooperType>().process(
                    paramProvider_.getSpeedParameter(ch),
                    paramProvider_.getWowAndFlutterAmtParameter(ch),
                    looperChannel.direction,
                    processorParameters,
                    paramProvider_.getGainParameter(ch),
                    inputBuffer,
                    outputBuffer,
                    postGainSmootherTimeConstant_,
                    getMotorTimeConstant(looperChannel.acceleration));
            }
        }
    }

private:
    LooperController(const LooperController&) = delete;
    LooperController& operator=(const LooperController&) {};

    struct LooperWithSettings
    {
        LooperWithSettings() {}
        ~LooperWithSettings()
        {
            if (layout == ChannelLayout::mono)
                looper.template destroy<typename LooperTypes::MonoLooperType>();
            else
                looper.template destroy<typename LooperTypes::StereoLooperType>();
        }

        ChannelLayout layout = ChannelLayout::stereo;
        Direction direction = Direction::forwards;
        MotorAcceleration acceleration = MotorAcceleration::medium;
        LateInitializedObject<MonoLooperType, StereoLooperType> looper;

        void initializeToLayout(const ChannelLayout newLayout, MonoOrStereoLooperStoragePtr& storage)
        {
            layout = newLayout;
            if (layout == ChannelLayout::mono)
            {
                looper.template destroy<typename LooperTypes::StereoLooperType>();
                looper.template create<typename LooperTypes::MonoLooperType>(storage);
            }
            else
            {
                looper.template destroy<typename LooperTypes::MonoLooperType>();
                looper.template create<typename LooperTypes::StereoLooperType>(storage);
            }
        }
    };

    static constexpr float getMotorSmoothingTimeInSFor(MotorAcceleration acceleration)
    {
        constexpr float minSpeed = 0.01f;
        constexpr float maxSpeed = 5.0f;
        constexpr size_t numSteps = 5;

        // f(x) = a * e^(b * x)
        // f(0) = minSpeed;
        // f(numSteps-1) = maxSpeed;
        // -------------------------
        //      minSpeed = a * e^(b * 0) = a
        //      maxSpeed = a * e^(b * (numSteps-1))
        //      ln(maxSpeed / minSpeed) = (numSteps-1) * b
        //      ln(maxSpeed / minSpeed) / (numSteps-1) = b
        //      log(maxSpeed / minSpeed) / ((numSteps-1) * log(e)) = b
        constexpr auto a = minSpeed;
        constexpr auto b = sprout::math::log(maxSpeed / minSpeed) / (float(numSteps - 1) * sprout::math::log(2.718281828f));

        switch (acceleration)
        {
            case MotorAcceleration::instantaneous:
                // not expected!
            case MotorAcceleration::veryFast:
                return a;
            case MotorAcceleration::fast:
                return a * sprout::exp(b);
            case MotorAcceleration::medium:
                return a * sprout::exp(2 * b);
            case MotorAcceleration::slow:
                return a * sprout::exp(3 * b);
            case MotorAcceleration::verySlow:
                return a * sprout::exp(4 * b);
        }
    }

    static constexpr ExponentialSmoother::TimeConstant
        postGainSmootherTimeConstant_ = ExponentialSmoother::TimeConstant(0.1f, sampleRateHz, blockSize);
    static constexpr ExponentialSmoother::TimeConstant
        motorTimeConstantVeryFast_ = ExponentialSmoother::TimeConstant(
            getMotorSmoothingTimeInSFor(MotorAcceleration::veryFast),
            sampleRateHz,
            blockSize);
    static constexpr ExponentialSmoother::TimeConstant
        motorTimeConstantFast_ = ExponentialSmoother::TimeConstant(
            getMotorSmoothingTimeInSFor(MotorAcceleration::fast),
            sampleRateHz,
            blockSize);
    static constexpr ExponentialSmoother::TimeConstant
        motorTimeConstantMedium_ = ExponentialSmoother::TimeConstant(
            getMotorSmoothingTimeInSFor(MotorAcceleration::medium),
            sampleRateHz,
            blockSize);
    static constexpr ExponentialSmoother::TimeConstant
        motorTimeConstantSlow_ = ExponentialSmoother::TimeConstant(
            getMotorSmoothingTimeInSFor(MotorAcceleration::slow),
            sampleRateHz,
            blockSize);
    static constexpr ExponentialSmoother::TimeConstant
        motorTimeConstantVerySlow_ = ExponentialSmoother::TimeConstant(
            getMotorSmoothingTimeInSFor(MotorAcceleration::verySlow),
            sampleRateHz,
            blockSize);

    static ExponentialSmoother::TimeConstant getMotorTimeConstant(MotorAcceleration acceleration)
    {
        switch (acceleration)
        {
            case MotorAcceleration::instantaneous:
                return ExponentialSmoother::TimeConstant::instantaneous();
            case MotorAcceleration::veryFast:
                return motorTimeConstantVeryFast_;
            case MotorAcceleration::fast:
                return motorTimeConstantFast_;
            default:
            case MotorAcceleration::medium:
                return motorTimeConstantMedium_;
            case MotorAcceleration::slow:
                return motorTimeConstantSlow_;
            case MotorAcceleration::verySlow:
                return motorTimeConstantVerySlow_;
        }
    }

    std::array<MonoOrStereoLooperStoragePtr, numLoopers> looperStorage_;
    std::array<LooperWithSettings, numLoopers> loopers_;
    AudioBufferPtr<1> monoDownmixBuffer_;
    AudioBufferPtr<1> temporaryBuffer_;
    const ParameterProvider& paramProvider_;
};