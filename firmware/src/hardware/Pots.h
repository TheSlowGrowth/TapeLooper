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
