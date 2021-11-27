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

#include <cstdio>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#ifdef __CYGWIN__
#    include <string>
#    include <regex>
#    include <vector>
std::vector<std::vector<char>> replacedStringStorage;
std::string printResult;
void replaceWinPathWithCygwinPath(int argc, char** argv)
{
    // find gtest_output argument
    for (int i = 0; i < argc; i++)
    {
        auto str = std::string(argv[i]);
        if (str.rfind("--gtest_output", 0) == 0)
        {
            // this is the path we need to modify
            // replace drive letter, then replace all backslashes
            str = std::regex_replace(
                str, std::regex("([A-Z]):\\\\"), "/cygdrive/$1/");
            str = std::regex_replace(str, std::regex("\\\\"), "/");

            printResult = str;
            // push into the storage so that it persists after this function
            replacedStringStorage.push_back({ str.begin(), str.end() });
            // replace original argument
            argv[i] = replacedStringStorage.back().data();
        }
    }
}
#endif

int main(int argc, char** argv)
{
#ifdef __CYGWIN__
    // cygwin-build binaries want their paths to be cygwin-style paths.
    // This collides with google-test adapters for IDEs, which will pass
    // Windows-style paths. We need to translate these paths.
    replaceWinPathWithCygwinPath(argc, argv);
#endif
    testing::InitGoogleMock(&argc, argv); // also inits google test
    return RUN_ALL_TESTS();
}