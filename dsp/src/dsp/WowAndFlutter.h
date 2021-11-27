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
#include "DspHelpers.h"
#include <sprout/math/ceil.hpp>

template <typename FloatType, int sampleRate>
class WowAndFlutterOscillator
{
public:
    WowAndFlutterOscillator() :
        flutterCarrierOsc_(flutterCarrierSpeedInHz_),
        flutterModulatorOsc_(flutterModulatorSpeedInHz_),
        wowOsc_(wowSpeedInHz_)
    {
    }

    void reset() {}

    MANUAL_INLINE float getAndAdvance()
    {
        auto flutterSample = flutterCarrierOsc_.process();
        const auto flutterModSample = flutterModulatorOsc_.process();
        flutterSample *= FloatType(1) - flutterModulationDepth_ + flutterModulationDepth_ * flutterModSample;

        const auto wowSample = wowOsc_.process();

        const FloatType result = flutterAmplitude_ * flutterSample
                                 + wowAmplitude_ * wowSample;
        return result;
    }

private:
    static constexpr FloatType flutterCarrierSpeedInHz_ = FloatType(6.234);
    static constexpr FloatType flutterModulatorSpeedInHz_ = FloatType(4.8994);
    static constexpr FloatType flutterModulationDepth_ = FloatType(0.23);
    static constexpr FloatType flutterAmplitude_ = FloatType(0.6);

    static constexpr FloatType wowSpeedInHz_ = FloatType(0.4);
    static constexpr FloatType wowAmplitude_ = FloatType(0.4);

public:
    static constexpr FloatType maxOutput_ = 1.0f;

private:
    class SinOscillator
    {
    public:
        SinOscillator(FloatType frequencyInHz = FloatType(5)) :
            phase_(FloatType(0.0))
        {
            setFrequency(frequencyInHz);
        }

        void setFrequency(FloatType newFrequencyInHz)
        {
            phaseIncrement_ = std::max(FloatType(0.0), newFrequencyInHz) / float(sampleRate);
        }

        void setPhase(FloatType phase)
        {
            phase_ = limit(FloatType(-0.5), FloatType(0.5), phase);
        }

        FloatType process()
        {
            const auto output = cheapSinShaper(phase_);
            phase_ += phaseIncrement_;
            if (phase_ >= FloatType(0.5))
                phase_ -= FloatType(1.0);
            return output;
        }

    private:
        FloatType cheapSinShaper(FloatType phase)
        {
            return phase * (FloatType(8.0) - FloatType(16.0) * std::abs(phase));
        }

        FloatType phase_;
        FloatType phaseIncrement_;
    };

    SinOscillator flutterCarrierOsc_;
    SinOscillator flutterModulatorOsc_;
    SinOscillator wowOsc_;
};
