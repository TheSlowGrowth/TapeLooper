#pragma once

template <typename FloatType>
class TapeSaturator
{
public:
    static FloatType saturate(FloatType input)
    {
        // Tape ...
        // - has a strong 2nd harmonic
        // - has almost no odd harmonics
        // - the higher order even harmonics drop off in amplitude very quickly.
        // This simple polynomial is a decent approximation.
        if (input >= FloatType(4))
            return FloatType(2.66666666666666666);
        else if (input <= FloatType(-4))
            return FloatType(-2.66666666666666666);
        else
            return input - FloatType(0.02083333333333333333) * input * input * input;
    }
};