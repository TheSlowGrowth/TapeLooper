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
#include <daisy_seed.h>

/** Button identifiers */
enum class Buttons
{
    record = 0,
    store,
    load,
    select0,
    select1,
    select2,
    select3,
    playAndDirection0,
    playAndDirection1,
    playAndDirection2,
    playAndDirection3,
    NUM_BUTTONS
};

/** A backend to read buttons for the ButtonMonitor class. */
class ButtonReader
{
public:
    ButtonReader()
    {
    }

    ~ButtonReader()
    {
        // TODO
    }

    void init()
    {
        // TODO
    }

    bool IsButtonPressed(uint16_t buttonId) const
    {
        assert(buttonId < uint16_t(Buttons::NUM_BUTTONS)); // TODO: write my own assert macro with bkpt();
        return false; // TODO
    }

private:
    ButtonReader(const ButtonReader&) = delete;
    ButtonReader& operator=(const ButtonReader&) = delete;
};
