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

#pragma once

class FatFsFileIo
{
public:
    void ensureVolumeMounted() {}

    bool makeFolderIfNotExistent(const char* folderName)
    {
        (void) (folderName);
        return false;
    }

    bool hasFile(const char* fileName)
    {
        (void) (fileName);
        return false;
    }

    bool openForReading(const char* fileName)
    {
        (void) (fileName);
        return false;
    }

    bool openForWriting(const char* fileName)
    {
        (void) (fileName);
        return false;
    }

    bool write(void* data, size_t size)
    {
        return false;
    }

    int32_t read(void* data, size_t maxSize)
    {
        return -1;
    }

    void closeFile()
    {
    }
};