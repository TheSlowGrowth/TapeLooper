#pragma once
#include <sprout/config.hpp>
#include <sprout/math/pow.hpp>
#include <sprout/math/tan.hpp>
#include <sprout/math/sin.hpp>
#include <sprout/math/cos.hpp>
#include <sprout/math/sqrt.hpp>
#include <sprout/math/constants.hpp>

template <typename FloatType>
class Biquad
{
public:
    struct Coefficients
    {
        FloatType a1 = FloatType(0);
        FloatType a2 = FloatType(0);
        FloatType b0 = FloatType(0);
        FloatType b1 = FloatType(0);
        FloatType b2 = FloatType(0);

        enum class MathType
        {
            accurate,
            fast
        };

        /** calculates the coefficients for a 1pole lowpass */
        template <MathType mathType = MathType::accurate>
        static constexpr Coefficients createLowPass1(FloatType sampleRate, FloatType frequency)
        {
            // according to U.Zoelzer - DAFX, second edition, sec. 2.2, p.49
            const FloatType K = (mathType == MathType::accurate)
                                    ? FloatType(sprout::math::tan(sprout::math::pi<FloatType>() * frequency / sampleRate))
                                    : FloatType(fastTan(sprout::math::pi<FloatType>() * frequency / sampleRate));
            const FloatType Kp1 = K + 1;
            Coefficients result;
            result.b0 = K / Kp1;
            result.b1 = result.b0;
            result.b2 = FloatType(0);
            result.a1 = (K - 1) / Kp1;
            result.a2 = FloatType(0);
            return result;
        }

        /** calculates the coefficients for a 2pole lowpass */
        template <MathType mathType = MathType::accurate>
        static constexpr Coefficients createLowPass2(FloatType sampleRate, FloatType frequency, FloatType Q = sqrt2Inv)
        {
            // according to U.Zoelzer - DAFX, second edition, sec. 2.2, p.49
            const FloatType K = (mathType == MathType::accurate)
                                    ? FloatType(sprout::math::tan(sprout::math::pi<FloatType>() * frequency / sampleRate))
                                    : FloatType(fastTan(sprout::math::pi<FloatType>() * frequency / sampleRate));
            const FloatType K2 = K * K;
            const FloatType K2Q = K2 * Q;
            const FloatType denom = K2Q + K + Q;
            Coefficients result;
            result.b0 = K2Q / denom;
            result.b1 = 2 * result.b0;
            result.b2 = result.b0;
            result.a1 = 2 * Q * (K2 - 1) / denom;
            result.a2 = (K2Q - K + Q) / denom;
            return result;
        }

        /** calculates the coefficients for a 1pole lowpass */
        template <MathType mathType = MathType::accurate>
        static constexpr Coefficients createHighPass1(FloatType sampleRate, FloatType frequency)
        {
            // according to U.Zoelzer - DAFX, second edition, sec. 2.2, p.49
            const FloatType K = (mathType == MathType::accurate)
                                    ? FloatType(sprout::math::tan(sprout::math::pi<FloatType>() * frequency / sampleRate))
                                    : FloatType(fastTan(sprout::math::pi<FloatType>() * frequency / sampleRate));
            const FloatType Kp1 = K + 1;
            Coefficients result;
            result.b0 = 1 / Kp1;
            result.b1 = -result.b0;
            result.b2 = FloatType(0);
            result.a1 = (K - 1) / Kp1;
            result.a2 = FloatType(0);
            return result;
        }

        /** calculates the coefficients for a 2pole highpass */
        template <MathType mathType = MathType::accurate>
        static constexpr Coefficients createHighPass2(FloatType sampleRate, FloatType frequency, FloatType Q = sqrt2Inv)
        {
            // according to U.Zoelzer - DAFX, second edition, sec. 2.2, p.49
            const FloatType K = (mathType == MathType::accurate)
                                    ? FloatType(sprout::math::tan(sprout::math::pi<FloatType>() * frequency / sampleRate))
                                    : FloatType(fastTan(sprout::math::pi<FloatType>() * frequency / sampleRate));
            const FloatType K2 = K * K;
            Coefficients result;
            result.b0 = Q / (K2 * Q + K + Q);
            result.b1 = -2 * Q / (K2 * Q + K + Q);
            result.b2 = Q / (K2 * Q + K + Q);
            result.a1 = 2 * Q * (K2 - 1) / (K2 * Q + K + Q);
            result.a2 = (K2 * Q - K + Q) / (K2 * Q + K + Q);
            return result;
        }

        /** calculates the coefficients for a 2pole bandpass */
        template <MathType mathType = MathType::accurate>
        static constexpr Coefficients createBandPass2(FloatType sampleRate, FloatType frequency, FloatType Q = sqrt2Inv)
        {
            // according to U.Zoelzer - DAFX, second edition, sec. 2.2, p.49
            const FloatType K = (mathType == MathType::accurate)
                                    ? FloatType(sprout::math::tan(sprout::math::pi<FloatType>() * frequency / sampleRate))
                                    : FloatType(fastTan(sprout::math::pi<FloatType>() * frequency / sampleRate));
            const FloatType K2 = K * K;
            Coefficients result;
            result.b0 = K / (K2 * Q + K + Q);
            result.b1 = 0;
            result.b2 = -K / (K2 * Q + K + Q);
            result.a1 = 2 * Q * (K2 - 1) / (K2 * Q + K + Q);
            result.a2 = (K2 * Q - K + Q) / (K2 * Q + K + Q);
            return result;
        }

        /** calculates the coefficients for a 2pole notch */
        template <MathType mathType = MathType::accurate>
        static constexpr Coefficients createNotch2(FloatType sampleRate, FloatType frequency, FloatType Q = sqrt2Inv)
        {
            // according to U.Zoelzer - DAFX, second edition, sec. 2.2, p.49
            const FloatType K = (mathType == MathType::accurate)
                                    ? FloatType(sprout::math::tan(sprout::math::pi<FloatType>() * frequency / sampleRate))
                                    : FloatType(fastTan(sprout::math::pi<FloatType>() * frequency / sampleRate));
            const FloatType K2 = K * K;
            Coefficients result;
            result.b0 = Q * (1 + K2) / (K2 * Q + K + Q);
            result.b1 = 2 * Q * (K2 - 1) / (K2 * Q + K + Q);
            result.b2 = Q * (1 + K2) / (K2 * Q + K + Q);
            result.a1 = 2 * Q * (K2 - 1) / (K2 * Q + K + Q);
            result.a2 = (K2 * Q - K + Q) / (K2 * Q + K + Q);
            return result;
        }

        /** calculates the coefficients for a bell eq */
        template <MathType mathType = MathType::accurate>
        static constexpr Coefficients createBellEq(FloatType sampleRate, FloatType frequency, FloatType gain, FloatType Q = sqrt2Inv)
        {
            // according to U.Zoelzer - DAFX, second edition, sec. 2.3.2, p.66, table 2.4
            const FloatType K = (mathType == MathType::accurate)
                                    ? FloatType(sprout::math::tan(sprout::math::pi<FloatType>() * frequency / sampleRate))
                                    : FloatType(fastTan(sprout::math::pi<FloatType>() * frequency / sampleRate));
            const FloatType V0 = sprout::math::pow(tenPow1div20, gain); // == pow(10, gain/20)
            const FloatType K2 = K * K;
            Coefficients result;
            if (gain > 0)
            {
                const FloatType denom = 1 + K / Q + K2;
                result.b0 = (1 + K * V0 / Q + K2) / denom;
                result.b1 = (2 * K2 - 2) / denom;
                result.b2 = (1 - K * V0 / Q + K2) / denom;
                result.a1 = result.b1;
                result.a2 = (1 - K / Q + K2) / denom;
            }
            else
            {
                const FloatType denom = 1 + K / Q / V0 + K2;
                result.b0 = (1 + K / Q + K2) / denom;
                result.b1 = (2 * K2 - 2) / denom;
                result.b2 = (1 - K / Q + K2) / denom;
                result.a1 = result.b1;
                result.a2 = (1 - K / Q / V0 + K2) / denom;
            }
            return result;
        }
        /** calculates the coefficients for a 1st order low shelf eq */
        template <MathType mathType = MathType::accurate>
        static constexpr Coefficients createLowShelf1(FloatType sampleRate, FloatType frequency, FloatType gain)
        {
            // Bilinear transform
            const auto sqrtA = sprout::math::pow(tenPow1div40, gain); // = sqrt(10^(gain/20))
            const auto scaledFreq = frequency / sqrtA;
            const auto warpedFreq = (mathType == MathType::accurate)
                                        ? FloatType(sprout::math::tan(sprout::math::pi<FloatType>() * scaledFreq / sampleRate))
                                        : FloatType(fastTan(sprout::math::pi<FloatType>() * scaledFreq / sampleRate));
            const auto A = sqrtA * sqrtA; // = 10^(gain/20)

            const auto wfp1 = (warpedFreq + FloatType(1.0));
            const auto tmp = (warpedFreq / wfp1) * (A - FloatType(1.0));

            Coefficients result;
            result.a1 = (FloatType(1.0) - warpedFreq) / wfp1;
            result.a2 = FloatType(0);
            result.b0 = tmp + FloatType(1.0);
            result.b1 = tmp - result.a1;
            result.b2 = FloatType(0);
            return result;
        }

        /** calculates the coefficients for a 2nd order low shelf eq */
        template <MathType mathType = MathType::accurate>
        static constexpr Coefficients createLowShelf2(FloatType sampleRate, FloatType frequency, FloatType gain, FloatType Q = sqrt2Inv)
        {
            static_assert(mathType == MathType::accurate, "MathType::fast is not yet supported!");

            const FloatType A = sprout::math::pow(tenPow1div40, gain); // == pow(10, gain/40) == sqrt(pow(10, gain/20))
            const auto aminus1 = A - FloatType(1);
            const auto aplus1 = A + FloatType(1);
            const auto omega = (sprout::math::two_pi<FloatType>() * std::max(frequency, FloatType(2))) / sampleRate;
            const auto coso = sprout::math::cos(omega);
            const auto beta = sprout::math::sin(omega) * sprout::math::sqrt(A) / Q;
            const auto aminus1TimesCoso = aminus1 * coso;

            // coefficients for transposed Form II Biquad
            const auto c1 = A * (aplus1 - aminus1TimesCoso + beta);
            const auto c2 = A * FloatType(2) * (aminus1 - aplus1 * coso);
            const auto c3 = A * (aplus1 - aminus1TimesCoso - beta);
            const auto c4 = aplus1 + aminus1TimesCoso + beta;
            const auto c5 = FloatType(-2) * (aminus1 + aplus1 * coso);
            const auto c6 = aplus1 + aminus1TimesCoso - beta;
            const auto a = FloatType(1) / c4;

            // transform to canonical form
            Coefficients result;
            result.b0 = c1 * a;
            result.b1 = c2 * a;
            result.b2 = c3 * a;
            result.a1 = c5 * a;
            result.a2 = c6 * a;
            return result;
        }

        /** calculates the coefficients for a 1st order high shelf eq */
        template <MathType mathType = MathType::accurate>
        static constexpr Coefficients createHighShelf1(FloatType sampleRate, FloatType frequency, FloatType gain)
        {
            // Bilinear transform
            const auto sqrtA = sprout::math::pow(tenPow1div40, gain); // = sqrt(10^(gain/20))
            const auto scaledFreq = frequency * sqrtA;
            const auto warpedFreq = (mathType == MathType::accurate)
                                        ? FloatType(sprout::math::tan(sprout::math::pi<FloatType>() * scaledFreq / sampleRate))
                                        : FloatType(fastTan(sprout::math::pi<FloatType>() * scaledFreq / sampleRate));
            const auto A = sqrtA * sqrtA; // = 10^(gain/20)

            const auto wfp1 = (warpedFreq + FloatType(1.0));
            const auto tmp = (A - FloatType(1.0)) / wfp1;

            Coefficients result;
            result.a1 = (FloatType(1.0) - warpedFreq) / wfp1;
            result.a2 = FloatType(0);
            result.b0 = tmp + FloatType(1.0);
            result.b1 = -(result.a1 + tmp);
            result.b2 = FloatType(0);
            return result;
        }

        /** calculates the coefficients for a 2nd order high shelf eq */
        template <MathType mathType = MathType::accurate>
        static constexpr Coefficients createHighShelf2(FloatType sampleRate, FloatType frequency, FloatType gain, FloatType Q = sqrt2Inv)
        {
            static_assert(mathType == MathType::accurate, "MathType::fast is not yet supported!");

            const FloatType A = sprout::math::pow(tenPow1div40, gain); // == pow(10, gain/40) == sqrt(pow(10, gain/20))
            const auto aminus1 = A - FloatType(1);
            const auto aplus1 = A + FloatType(1);
            const auto omega = (sprout::math::two_pi<FloatType>() * std::max(frequency, FloatType(2))) / sampleRate;
            const auto coso = sprout::math::cos(omega);
            const auto beta = sprout::math::sin(omega) * sprout::math::sqrt(A) / Q;
            const auto aminus1TimesCoso = aminus1 * coso;

            // coefficients for transposed Form II Biquad
            const auto c1 = A * (aplus1 + aminus1TimesCoso + beta);
            const auto c2 = A * FloatType(-2) * (aminus1 + aplus1 * coso);
            const auto c3 = A * (aplus1 + aminus1TimesCoso - beta);
            const auto c4 = aplus1 - aminus1TimesCoso + beta;
            const auto c5 = FloatType(2) * (aminus1 - aplus1 * coso);
            const auto c6 = aplus1 - aminus1TimesCoso - beta;
            const auto a = FloatType(1) / c4;

            // transform to canonical form
            Coefficients result;
            result.b0 = c1 * a;
            result.b1 = c2 * a;
            result.b2 = c3 * a;
            result.a1 = c5 * a;
            result.a2 = c6 * a;
            return result;
        }

    private:
        static constexpr FloatType tenPow1div20 = FloatType(1.12201845430196343559);
        static constexpr FloatType tenPow1div40 = FloatType(1.05925372517728887881);
        static constexpr FloatType sqrt2Inv = FloatType(0.70710678118654752440);
        /** Taylor series expansion of tan(x) around x=0;
          * Valid for |x| << pi/2 */
        static constexpr FloatType fastTan(FloatType input)
        {
            const FloatType input2 = input * input;
            const FloatType input3 = input2 * input;
            const FloatType input5 = input2 * input3;
            auto output = input;
            output += FloatType(1.0 / 3.0) * input3;
            output += FloatType(1.0 / 7.5) * input5;
            return output;
        }
    };

    Biquad()
    {
        reset();
    }

    void reset()
    {
        state1_ = state2_ = 0.0f;
    }

    MANUAL_INLINE float process(float sample, const Coefficients& coeffs)
    {
        float newState1 = sample - (state1_ * coeffs.a1) - (state2_ * coeffs.a2);
        float result = (coeffs.b2 * state2_) + (coeffs.b1 * state1_) + (newState1 * coeffs.b0);
        state2_ = state1_;
        state1_ = newState1;
        return result;
    }

    MANUAL_INLINE float processAsFirstOrder(float sample, const Coefficients& coeffs)
    {
        float newState1 = sample - (state1_ * coeffs.a1);
        float result = (coeffs.b1 * state1_) + (newState1 * coeffs.b0);
        state1_ = newState1;
        return result;
    }

private:
    float state1_;
    float state2_;
};
