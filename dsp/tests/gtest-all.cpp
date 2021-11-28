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

// The .cpp => .o rule relies on the cpp files being in the exact same folder hierarchy as the .o files in the build directory.
// I'm no makefile guru - how can we add files in ../lib/googletest/src to the makefile directly without this ugly include here?!
#include "googletest/src/gtest-all.cc"
#include "googlemock/src/gmock-all.cc"