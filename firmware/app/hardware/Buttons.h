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
