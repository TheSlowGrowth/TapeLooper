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

namespace dspdefs
{
    constexpr size_t numLoopers = 4;
    constexpr double loopLengthInS = 60.0;

    constexpr float speedMin = 0.25f;
    constexpr float speedMax = 4.0f;

    namespace paramIds
    {
        const auto chState = "chState";
        const auto chSpeed = "chSpeed";
        const auto chDrive = "chDrive";
        const auto chGrainAmt = "chGrainAmt";
        const auto chWowAndFlutterAmt = "chWowAndFlutterAmt";
        const auto chPostGain = "chPostGain";
    } // namespace paramIds
} // namespace dspdefs
