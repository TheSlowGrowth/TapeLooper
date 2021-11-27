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
#include "EmphasisEq.h"
#include "TapeEq.h"
#include "TapeGrain.h"
#include "TapeSaturator.h"

#ifndef MANUAL_INLINE
#    define MANUAL_INLINE inline __attribute__((always_inline))
#endif

template <size_t sampleRate, size_t numChannels>
class TapeProcessor
{
public:
    TapeProcessor()
    {
    }

    struct Parameters
    {
        float driveGain;
        float grainAmt;
    };

    void reset()
    {
        driveGainSmoother_.reset();
        grainAmtSmoother_.reset();
        for (size_t ch = 0; ch < numChannels; ch++)
        {
            emphasisEqs_[ch].reset();
            tapeEqs_[ch].reset();
            grainProcessors_[ch].reset();
        }
    }

    MANUAL_INLINE void process(float inputAndOutput[numChannels],
                               const Parameters& parameters)
    {
        constexpr auto smootherTimeConstant_ =
            ExponentialSmoother::TimeConstant(0.05f, sampleRate, 1);
        const auto driveGain = driveGainSmoother_.smooth(parameters.driveGain,
                                                         smootherTimeConstant_);
        const auto grainAmt = grainAmtSmoother_.smooth(parameters.grainAmt,
                                                       smootherTimeConstant_);

        for (size_t ch = 0; ch < numChannels; ch++)
        {
            // tape saturation
            auto& sample = inputAndOutput[ch];
            sample = emphasisEqs_[ch].processPreEmphasis(sample);
            sample *= driveGain;
            sample = TapeSaturator<float>::saturate(sample);
            sample /= driveGain;
            sample = emphasisEqs_[ch].processDeEmphasis(sample);
            // tape grain
            sample = grainProcessors_[ch].process(grainAmt, sample);
            // apply tape EQ
            sample = tapeEqs_[ch].process(sample);
        }
    }

private:
    ExponentialSmoother driveGainSmoother_;
    ExponentialSmoother grainAmtSmoother_;

    std::array<EmphasisEq<float, sampleRate>, numChannels> emphasisEqs_;
    std::array<TapeEq<sampleRate>, numChannels> tapeEqs_;
    std::array<TapeGrainProcessor<float, sampleRate>, numChannels> grainProcessors_;
};
