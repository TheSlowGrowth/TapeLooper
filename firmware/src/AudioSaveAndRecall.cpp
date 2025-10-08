/**
 * Copyright (C) Johannes Elliesen, 2024
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

#include "AudioSaveAndRecall.h"

namespace loop_library
{
    AudioFileName makeFileNameWithoutExtensionFor(const StorageBank bank,
                                                  const int slot)
    {
        auto filename = AudioFileName(kLibraryBaseFolder);
        switch (bank)
        {
            case StorageBank::green:
                filename.Append("green-");
                break;
            case StorageBank::yellow:
                filename.Append("yellow-");
                break;
            case StorageBank::red:
                filename.Append("red-");
                break;
            default:
                return "";
        }
        switch (slot / 4)
        {
            case 0:
                filename.Append("A");
                break;
            case 1:
                filename.Append("B");
                break;
            case 2:
                filename.Append("C");
                break;
            case 3:
                filename.Append("D");
                break;
            default:
                return "";
        }
        switch (slot % 4)
        {
            case 0:
                filename.Append("0");
                break;
            case 1:
                filename.Append("1");
                break;
            case 2:
                filename.Append("2");
                break;
            case 3:
                filename.Append("3");
                break;
            default:
                return "";
        }
        return filename;
    }

    AudioFileName makeWavFileNameFor(const StorageBank bank,
                                     const int slot)
    {
        auto name = makeFileNameWithoutExtensionFor(bank, slot);
        name.Append(".wav");
        return name;
    }
} // namespace loop_library
