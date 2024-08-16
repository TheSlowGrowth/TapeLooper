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

/** Button identifiers */
enum class Button
{
    save = 0,
    load,
    settings,
    record,
    chA_play,
    chA_up,
    chA_down,
    chB_play,
    chB_up,
    chB_down,
    chC_play,
    chC_up,
    chC_down,
    chD_play,
    chD_up,
    chD_down,
    NUM_BUTTONS
};

enum class Led
{
    peakMeterInL = 0,
    peakMeterInR,
    peakMeterOutL,
    peakMeterOutR,
    save,
    load,
    settings,
    record,
    chA_m2,
    chA_m1,
    chA_p1,
    chA_p2,
    chB_m2,
    chB_m1,
    chB_p1,
    chB_p2,
    chC_m2,
    chC_m1,
    chC_p1,
    chC_p2,
    chD_m2,
    chD_m1,
    chD_p1,
    chD_p2,
    NUM_LEDS
};

enum class Pot
{
    chA_speed = 0,
    chA_warbleAmt,
    chA_grainAmt,
    chA_driveAmt,
    chA_volume,
    chB_speed,
    chB_warbleAmt,
    chB_grainAmt,
    chB_driveAmt,
    chB_volume,
    chC_speed,
    chC_warbleAmt,
    chC_grainAmt,
    chC_driveAmt,
    chC_volume,
    chD_speed,
    chD_warbleAmt,
    chD_grainAmt,
    chD_driveAmt,
    chD_volume,
    NUM_POTS
};

enum class CvInput
{
    chA_speed = 0,
    chA_volume,
    chB_speed,
    chB_volume,
    chC_speed,
    chC_volume,
    chD_speed,
    chD_volume,
    NUM_CVS
};

enum class LedColour
{
    off,
    red,
    yellow,
    green,
    pulsingRed,
    pulsingYellow,
    pulsingGreen,
};