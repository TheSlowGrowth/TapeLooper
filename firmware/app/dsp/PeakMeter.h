#include <math.h>

template <int blockSize, int sampleRate>
class PeakMeter
{
public:
    void init(float releaseTimeInS)
    {
        lastPeak_ = 0.0f;
        // calculate time constants for the smoother
        // according to Udo Zoelzer, DAFX, sec. 4.2, p. 109
        timeConstant_ = 0.0217597649487899548f; //1.0f - std::expf(-2.2f * float(blockSize) / float(sampleRate) / releaseTimeInS);
    }

    void readPeaks(const float* samples)
    {
        // find peaks
        for (int i = 0; i < blockSize; i++)
        {
            const float absSample = abs(samples[i]);
            if (absSample > lastPeak_)
                lastPeak_ = absSample;
        }
        // apply smoothing
        lastPeak_ = -lastPeak_ * timeConstant_ + lastPeak_;
    }

    void getRedAndGreenValues(float& red, float& green) const
    {
        float decibelValue = -120.0f;
        if (lastPeak_ > 0.000001f)
            decibelValue = 20.0f * log10f(lastPeak_);

        // < -60dB: all black
        if (decibelValue <= -60.0f)
        {
            red = 0.0f;
            green = 0.0f;
            return;
        }
        // -60dB .. -20dB: green comes on
        else if (decibelValue <= -20.0f)
        {
            red = 0.0f;
            green = (decibelValue + 60.0f) / 40.0f;
        }
        // -20dB .. -5dB: green .. yellow
        else if (decibelValue <= -10.0f)
        {
            red = (decibelValue + 20.0f) / 10.0f;
            green = 1.0f;
        }
        // -10dB .. -5dB: yellow .. orange
        else if (decibelValue <= -5.0f)
        {
            red = 1.0f;
            green = 1.0f - (decibelValue + 10.0f) / 5.0f;
            green /= 2.0f; // only go down to 50% to have a visible step when we go to full red
        }
        // above -5dB: red
        else
        {
            red = 1.0f;
            green = 0.0f;
        }
    }

private:
    __attribute__((always_inline)) float fastLog10f(float inputValue)
    {
        return log10f(inputValue); // TODO
    }

    float lastPeak_;
    float timeConstant_;
};
