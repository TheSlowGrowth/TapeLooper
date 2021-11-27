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