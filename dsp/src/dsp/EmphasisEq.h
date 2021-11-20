#pragma once
#include <sprout/config.hpp>
#include <sprout/math/pow.hpp>
#include "DspHelpers.h"

enum class OnePoleShelvingEqType
{
    lowShelf,
    highShelf
};

/** 
 * This class serves as a "relay" to allow specifying 
 * float parameters with template arguments
 */
template <typename NumericType,
          int sampleRate,
          OnePoleShelvingEqType type,
          int cutoffFrequency,
          int gainInDecibels>
class FixedOnePoleShelvingEqParameters
{
public:
    using FloatType = NumericType;
    static constexpr bool isLowShelf() { return type == OnePoleShelvingEqType::lowShelf; }
    static constexpr FloatType getCutoffFrequency() { return FloatType(cutoffFrequency); }
    static constexpr FloatType getGainInDecibels() { return FloatType(gainInDecibels); }
    static constexpr FloatType getSampleRate() { return FloatType(sampleRate); }
};

/**
 * A one pole shelving EQ with fixed parameters.
 * Filter coefficients are precomputed at compile time.
 */
template <typename FilterParameters>
class FixedOnePoleShelvingEq
{
public:
    using FloatType = typename FilterParameters::FloatType;

    constexpr FixedOnePoleShelvingEq()
    {
        reset();
    }

    void reset(FloatType valueToResetTo = FloatType(0.0))
    {
        stateO_ = valueToResetTo;
        stateI_ = valueToResetTo;
    }

    MANUAL_INLINE FloatType processSample(FloatType input)
    {
        constexpr Coeffs coeffs = getCoeffs();

        FloatType output = coeffs.b1 * stateI_;
        output += coeffs.b0 * input;
        output += coeffs.a1 * stateO_;
        stateI_ = input;
        stateO_ = output;
        return output;
    }

    struct Coeffs
    {
        FloatType a1 = 0;
        FloatType b0 = 0;
        FloatType b1 = 0;
    };

    static constexpr Coeffs getCoeffs()
    {
        constexpr FloatType sampleRate = FilterParameters::getSampleRate();
        // Bilinear transform
        constexpr auto sqrtA = sprout::math::pow(FloatType(1.05925),
                                                 FilterParameters::getGainInDecibels()); // = sqrt(10^(gain/20))
        constexpr auto scaledFreq = FilterParameters::getCutoffFrequency() * (FilterParameters::isLowShelf() ? (FloatType(1) / sqrtA) : sqrtA);
        constexpr auto clippedFreq = limit(sampleRate / FloatType(24576.0),
                                           sampleRate / FloatType(2.125),
                                           scaledFreq);
        constexpr auto warpedFreq = fastTan(FloatType(3.141592653589793238462643383) / sampleRate * clippedFreq);
        constexpr auto A = sqrtA * sqrtA; // = 10^(gain/20)

        constexpr auto wfp1 = (warpedFreq + FloatType(1.0));

        if (FilterParameters::isLowShelf())
        {
            constexpr auto tmp = (warpedFreq / wfp1) * (A - FloatType(1.0));

            Coeffs coeffs;
            coeffs.a1 = (FloatType(1.0) - warpedFreq) / wfp1;
            coeffs.b0 = tmp + FloatType(1.0);
            coeffs.b1 = tmp - coeffs.a1;
            return coeffs;
        }
        else
        {
            constexpr auto tmp = (A - FloatType(1.0)) / wfp1;

            Coeffs coeffs;
            coeffs.a1 = (FloatType(1.0) - warpedFreq) / wfp1;
            coeffs.b0 = tmp + FloatType(1.0);
            coeffs.b1 = -(coeffs.a1 + tmp);
            return coeffs;
        }
    }

private:
    FloatType stateO_;
    FloatType stateI_;

    /** Taylor aproximation of tan() */
    template <typename FloatType>
    static constexpr FloatType fastTan(FloatType input)
    {
        FloatType input2 = input * input;
        FloatType input3 = input2 * input;
        FloatType input5 = input2 * input3;
        auto output = input;
        output += FloatType(1.0 / 3.0) * input3;
        output += FloatType(1.0 / 7.5) * input5;
        return output;
    }
};

/**
 * An pre-emphasis / de-emphasis EQ consisting of
 * two first order low shelf EQs.
 * The pre-emphasis section boosts bass,
 * while the de-emphasis section cuts it by the same amount.
 */
template <typename FloatType, int sampleRate>
class EmphasisEq
{
public:
    EmphasisEq()
    {
    }

    void reset()
    {
        lowShelfPreEmphasis_.reset();
        lowShelfDeEmphasis_.reset();
    }

    /** Boosts bass by 12dB */
    MANUAL_INLINE FloatType processPreEmphasis(const FloatType inputSample)
    {
        FloatType sample = inputSample;
        sample = lowShelfPreEmphasis_.processSample(sample);
        return sample;
    }

    /** Cuts bass by 12dB */
    MANUAL_INLINE FloatType processDeEmphasis(const FloatType inputSample)
    {
        FloatType sample = inputSample;
        sample = lowShelfDeEmphasis_.processSample(sample);
        return sample;
    }

private:
    static constexpr int lowFrequencyInHz_ = 500;
    static constexpr int gainInDecibels_ = 12;
    FixedOnePoleShelvingEq<FixedOnePoleShelvingEqParameters<FloatType,
                                                            sampleRate,
                                                            OnePoleShelvingEqType::lowShelf,
                                                            lowFrequencyInHz_,
                                                            gainInDecibels_>>
        lowShelfPreEmphasis_;
    FixedOnePoleShelvingEq<FixedOnePoleShelvingEqParameters<FloatType,
                                                            sampleRate,
                                                            OnePoleShelvingEqType::lowShelf,
                                                            lowFrequencyInHz_,
                                                            -gainInDecibels_>>
        lowShelfDeEmphasis_;
};
