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
#include "Biquad.h"

template <int sampleRate>
class TapeEq
{
public:
    TapeEq()
    {
        reset();
    }

    void reset()
    {
        highPass_.reset();
        gapLossLowpass_.reset();
        gapLossCompensationBell_.reset();
        headBumpDelayLine_.reset();
        headBumpCombLp_.reset();
    }

    MANUAL_INLINE float process(float sample)
    {
        constexpr auto hpCoeffs = Biquad<float>::Coefficients::createHighPass1(float(sampleRate), 50.0f);
        constexpr auto gapLossLpCoeffs = Biquad<float>::Coefficients::createLowPass1(float(sampleRate), 1000.0f);
        constexpr auto gapLossCompCoeffs = Biquad<float>::Coefficients::createBellEq(float(sampleRate), 5000.0f, 8.0f, 1.2f);
        sample = highPass_.processAsFirstOrder(sample, hpCoeffs);
        sample = gapLossLowpass_.processAsFirstOrder(sample, gapLossLpCoeffs);
        sample = gapLossCompensationBell_.process(sample, gapLossCompCoeffs);

        // head bump model (lowpass filtered comb filter).
        constexpr auto headBumpCombLpCoeffs = Biquad<float>::Coefficients::createLowPass2(float(sampleRate), headBumpFreqInHz_);
        auto combFilterSample = headBumpDelayLine_.process(sample);
        combFilterSample = headBumpCombLp_.process(combFilterSample, headBumpCombLpCoeffs);
        sample -= combFilterSample * headBumpAmount_;

        return sample;
    }

private:
    static constexpr float headBumpFreqInHz_ = 50.0f;
    static constexpr float headBumpAmount_ = 0.8f;
    static constexpr size_t headBumpCombDelayTimeInSamples_ = size_t(float(sampleRate) * 0.5f / headBumpFreqInHz_);

    template <size_t size>
    class StaticDelayLine
    {
    public:
        void reset()
        {
            for (size_t i = 0; i < size; i++)
                buffer_[i] = 0.0f;
            readPos_ = 0;
        }

        MANUAL_INLINE float process(float input)
        {
            const auto result = buffer_[readPos_];
            buffer_[readPos_] = input;
            if (++readPos_ >= size)
                readPos_ = 0;
            return result;
        }

    private:
        float buffer_[size];
        size_t readPos_;
    };

    Biquad<float> highPass_;
    Biquad<float> gapLossLowpass_;
    Biquad<float> gapLossCompensationBell_;
    StaticDelayLine<headBumpCombDelayTimeInSamples_> headBumpDelayLine_;
    Biquad<float> headBumpCombLp_;
};