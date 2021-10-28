#pragma once
#include "DspHelpers.h"
#include "Biquad.h"

/** A source of clocked noise that can have variable amounts of jitter. */
template <typename FloatType, int sampleRate>
class ClockedNoiseGenerator
{
public:
    ClockedNoiseGenerator(int32_t seed = 0, FloatType frequencyInHz = FloatType(100), FloatType jitter = FloatType(0.5)) :
        jitter_(jitter),
        rnd_(seed),
        phase_(FloatType(0)),
        currentOutputValue_(FloatType(0))
    {
        setFrequency(frequencyInHz);
    }

    void setFrequency(FloatType frequencyInHz)
    {
        frequency_ = frequencyInHz;
        updatePhaseIncrement(frequency_);
    }

    /** Sets the amount of jitter between 0 (none) and 1 (full).
      * At full jitter, the frequency of the individual cycles varies between
      * 0.00001 and 2 times the base frequency.
      */
    void setJitter(FloatType jitterAmount)
    {
        jitter_ = limit(FloatType(0), FloatType(1), jitterAmount);
    }

    void setSeed(int32_t seed)
    {
        rnd_ = seed;
    }

    void reset()
    {
        phase_ = FloatType(0);
    }

    MANUAL_INLINE FloatType process()
    {
        phase_ += phaseIncrement_;
        if (phase_ >= FloatType(1))
        {
            phase_ -= FloatType(1);
            rnd_ = 1125585549 * rnd_ + 49951;
            currentOutputValue_ = FloatType(rnd_) / FloatType(INT32_MAX);
            if (jitter_ > FloatType(0))
            {
                rnd_ = 1098659777 * rnd_ + 48973;
                const auto currentJitterValue = FloatType(rnd_) / FloatType(INT32_MAX);
                const auto nextCycleFrequency = frequency_ * limit(FloatType(0.00001), FloatType(2), (FloatType(1) + jitter_ * currentJitterValue));
                updatePhaseIncrement(nextCycleFrequency);
            }
        }
        return currentOutputValue_;
    }

    FloatType getCurrentOutputValue() const { return currentOutputValue_; }

private:
    void updatePhaseIncrement(FloatType frequency)
    {
        phaseIncrement_ = frequency / FloatType(sampleRate);
    }

    FloatType sampleRate_;
    FloatType frequency_;
    FloatType phaseIncrement_;
    FloatType jitter_;
    int32_t rnd_;
    FloatType phase_;
    FloatType currentOutputValue_;
};

/** 
 * Amplitude-modulates a signal with filtered noise
 * to simulate the grain of magnetic tape.
 */
template <typename FloatType, int sampleRate>
class TapeGrainProcessor
{
public:
    TapeGrainProcessor(int seed = 0) :
        noise_(seed, clockedNoiseFreq_, clockedNoiseJitter_)
    {
    }

    void reset()
    {
        noiseFilter_.reset();
        noiseEq_.reset();
    }

    MANUAL_INLINE FloatType process(FloatType normalizedAmount, FloatType input)
    {
        constexpr auto lpCoeffs = Biquad<FloatType>::Coefficients::createLowPass2(FloatType(sampleRate), FloatType(270));
        constexpr auto lowShelfCoeffs = Biquad<FloatType>::Coefficients::createLowShelf1(FloatType(sampleRate), FloatType(500), FloatType(-36));
        const auto amount = normalizedAmount * maxAmount_;
        const auto noiseSample = noise_.process();
        const auto filteredNoiseSample = noiseFilter_.process(noiseSample, lpCoeffs);
        const auto amContribution = input * amount * filteredNoiseSample;
        const auto filteredAmContribution = noiseEq_.processAsFirstOrder(amContribution, lowShelfCoeffs);
        return input + filteredAmContribution;
    }

private:
    static constexpr auto clockedNoiseFreq_ = FloatType(8000);
    static constexpr auto clockedNoiseJitter_ = FloatType(0.6);
    static constexpr auto maxAmount_ = FloatType(12);

    ClockedNoiseGenerator<FloatType, sampleRate> noise_;
    Biquad<FloatType> noiseFilter_;
    Biquad<FloatType> noiseEq_;
};
