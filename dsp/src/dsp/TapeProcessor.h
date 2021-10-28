#pragma once

#include "DspHelpers.h"
#include "EmphasisEq.h"
#include "TapeEq.h"
#include "TapeGrain.h"
#include "TapeSaturator.h"

#ifndef MANUAL_INLINE
#    define MANUAL_INLINE inline __attribute__((always_inline))
#endif

template <size_t sampleRate>
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
        emphasisEq_.reset();
        tapeEq_.reset();
        grainProcessor_.reset();
    }

    MANUAL_INLINE float process(float input,
                                const Parameters& parameters)
    {
        constexpr auto smootherTimeConstant_ =
            ExponentialSmoother::TimeConstant(0.05f, sampleRate, 1);
        const auto driveGain = driveGainSmoother_.smooth(parameters.driveGain,
                                                         smootherTimeConstant_);
        const auto grainAmt = grainAmtSmoother_.smooth(parameters.grainAmt,
                                                       smootherTimeConstant_);

        auto sample = input;
        // tape saturation
        sample = emphasisEq_.processPreEmphasis(sample);
        sample *= driveGain;
        sample = TapeSaturator<float>::saturate(sample);
        sample /= driveGain;
        sample = emphasisEq_.processDeEmphasis(sample);
        // tape grain
        sample = grainProcessor_.process(grainAmt, sample);
        // apply tape EQ
        sample = tapeEq_.process(sample);

        return sample;
    }

private:
    ExponentialSmoother driveGainSmoother_;
    ExponentialSmoother grainAmtSmoother_;

    EmphasisEq<float, sampleRate> emphasisEq_;
    TapeEq<sampleRate> tapeEq_;
    TapeGrainProcessor<float, sampleRate> grainProcessor_;
};
