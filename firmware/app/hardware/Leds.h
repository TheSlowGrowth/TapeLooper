#pragma once

#include <daisy_seed.h>

using LedDriverType = daisy::LedDriverPca9685<1>;
static constexpr uint8_t ledCanvasId = 0;