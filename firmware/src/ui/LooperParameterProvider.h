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

#include <dsp/TapeProcessor.h>

#include "../util/PlateauMap.h"

/**
 * Consumes raw UI control inputs (potentiometer positions, etc.)
 * and converts them to mapped DSP parameters for the LooperController
 */
template <size_t numLoopers>
class LooperParameterProvider
{
public:
    /** Called by the LooperController */
    float getSpeedParameter(size_t looperChannel) const
    {
        return limit(getUnclampedSpeedParameter(looperChannel), minSpeed_, maxSpeed_);
    }
    /** Called by the LooperController */
    float getWowAndFlutterAmtParameter(size_t looperChannel) const
    {
        return controlInputs_[looperChannel].wowAndFlutterAmt;
    }
    /** Called by the LooperController */
    float getGainParameter(size_t looperChannel) const
    {
        return volumeSliderToGainMultiplier(controlInputs_[looperChannel].volumeSliderPosition)
               * volumeCvToGainMultiplier(controlInputs_[looperChannel].volumeCvVolts);
    }
    /** Called by the LooperController */
    TapeProcessorParameters getProcessorParameters(size_t looperChannel) const
    {
        TapeProcessorParameters results;
        results.driveGain = drivePotToDriveGain(controlInputs_[looperChannel].driveAmt);
        results.grainAmt = controlInputs_[looperChannel].grainAmt;
        return results;
    }

    enum class ValueRangeViolation
    {
        tooLow,
        okay,
        tooHigh
    };

    ValueRangeViolation getSpeedParameterRangeViolation(size_t looperChannel) const
    {
        const auto unclampedSpeed = getUnclampedSpeedParameter(looperChannel);
        if (unclampedSpeed > maxSpeed_)
            return ValueRangeViolation::tooHigh;
        else if (unclampedSpeed < minSpeed_)
            return ValueRangeViolation::tooLow;
        return ValueRangeViolation::okay;
    }

    struct ControlInputs
    {
        // -2 .. 2
        int8_t octave = 0;

        // 0 .. 10
        float volumeCvVolts = 0.0f;
        // -5 .. 5
        float pitchCvVolts = 0.0f;

        // the rest here is 0..1
        float fineTunePosition = 0.5f;
        float wowAndFlutterAmt = 0.0f;
        float volumeSliderPosition = 0.0f;
        float driveAmt = 0.0f;
        float grainAmt = 0.0f;
    };

    /** Set from UI pages */
    std::array<ControlInputs, numLoopers>
        controlInputs_;

private:
    static constexpr float minSpeed_ = 0.25f;
    static constexpr float maxSpeed_ = 4.0f;
    static constexpr float minDriveGain_ = 0.25f;
    static constexpr float maxDriveGain_ = 10.0f;
    static constexpr float maxSliderGain_ = 2.0f;
    static constexpr float maxCvGain_ = 2.0f;
    static constexpr size_t numPlateaus_ = 5;
    static constexpr int minSemitones_ = -7;
    static constexpr int maxSemitones_ = 7;

    static constexpr float semitoneToFineTuneKnobPosition(int semitone)
    {
        constexpr int range = maxSemitones_ - minSemitones_;
        return float(semitone - minSemitones_) / float(range);
    }
    static constexpr PlateauMapParameters<float, numPlateaus_> fineTunePlateauParams_ =
        PlateauMapParameters<float, numPlateaus_>({ semitoneToFineTuneKnobPosition(-5),
                                                    semitoneToFineTuneKnobPosition(-3),
                                                    semitoneToFineTuneKnobPosition(0),
                                                    semitoneToFineTuneKnobPosition(3),
                                                    semitoneToFineTuneKnobPosition(5) });

    float pitchCvToSemitones(float cvVolts) const
    {
        (void) (cvVolts);
        return 0.0f; // TODO
    }

    float volumeSliderToGainMultiplier(float sliderValue) const
    {
        constexpr auto maxGain = 2.0f;
        return sliderValue * sliderValue * maxGain;
    }

    float volumeCvToGainMultiplier(float cvVolts) const
    {
        (void) (cvVolts);
        return 1.0f; // TODO
    }

    float getUnclampedSpeedParameter(size_t looperChannel) const
    {
        auto semitones = 12.0f * float(controlInputs_[looperChannel].octave);
        semitones += pitchCvToSemitones(controlInputs_[looperChannel].pitchCvVolts);
        auto mappedFintTuneKnobPosition = plateauMap<numPlateaus_>(
            controlInputs_[looperChannel].fineTunePosition,
            fineTunePlateauParams_);
        semitones += linMap(mappedFintTuneKnobPosition, float(minSemitones_), float(maxSemitones_));
        return semitonesToRatio(semitones);
    }

    float drivePotToDriveGain(float normalizedInput) const
    {
        const auto mappedDriveGain = normalizedInput * normalizedInput;
        return linMap(mappedDriveGain, minDriveGain_, maxDriveGain_);
    }
};