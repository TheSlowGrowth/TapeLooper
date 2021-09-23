#pragma once
#include <stdint.h>
#include <assert.h>
#include <per/gpio.h>
#include <per/adc.h>

/** Potentiometer identifiers */
enum class PotsAndCVs
{
    speed = 0,
    lofi,
    NUM_POTS
};

/** A backend to read potentiometers and CV inputs for the PotMonitor class.
 */
class PotReader
{
public:
    PotReader() {}

    void init()
    {
        const dsy_gpio_pin adcPin[numAdcChannels_] = {
            { DSY_GPIOC, 0 }, // Daisy Seed pin 16
            { DSY_GPIOA, 3 }, // Daisy Seed pin 17
        };

        daisy::AdcChannelConfig channelCfg[numAdcChannels_];
        for (size_t i = 0; i < numAdcChannels_; i++)
            channelCfg[i].InitSingle(adcPin[i]);
        adc_.Init(channelCfg, numAdcChannels_, daisy::AdcHandle::OverSampling::OVS_128);
        adc_.Start();
    }

    float GetPotValue(uint16_t potId) const
    {
        assert(potId < uint16_t(PotsAndCVs::NUM_POTS)); // TODO: write my own assert macro with bkpt();
        return adc_.GetFloat(potId);
    }

private:
    static constexpr size_t numAdcChannels_ = 2;

    PotReader(const PotReader&) = delete;
    PotReader& operator=(const PotReader&) = delete;

    daisy::AdcHandle adc_;
};
