#pragma once

#include "DspHelpers.h"

#ifndef MANUAL_INLINE
#    define MANUAL_INLINE inline __attribute__((always_inline))
#endif

template <size_t samplerate>
class TapeProcessor
{
public:
    TapeProcessor()
    {
    }

    struct Parameters
    {
        float lofi;
    };

    void reset()
    {
    }

    MANUAL_INLINE float process(float input,
                                const Parameters& /* parameters */)
    {
        // TODO: add processing here
        return input;
    }
};
