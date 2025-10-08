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

#include <per/sdmmc.h>
#include <fatfs.h>

class FatFsFileIo
{
public:
    FatFsFileIo()
    {
        daisy::SdmmcHandler::Config sdCfg;
        sdCfg.Defaults();
        sdCfg.speed = daisy::SdmmcHandler::Speed::STANDARD;
        sd_.Init(sdCfg);
        fileSystem_.Init(daisy::FatFSInterface::Config::MEDIA_SD);
    }

    void ensureVolumeMounted()
    {
        isMounted_ = f_mount(&fileSystem_.GetSDFileSystem(), fileSystem_.GetSDPath(), 1) == FR_OK;
    }

    bool makeFolderIfNotExistent(const char* folderName)
    {
        return f_mkdir(folderName) == FR_OK;
    }

    bool hasFile(const char* fileName)
    {
        const auto canOpenForReading = openForReading(fileName);
        if (canOpenForReading)
        {
            closeFile();
        }
        return canOpenForReading;
    }

    bool openForReading(const char* fileName)
    {
        const auto result = f_open(&file_, fileName, FA_OPEN_EXISTING | FA_READ);
        return result == FR_OK;
    }

    bool openForWriting(const char* fileName)
    {
        const auto result = f_open(&file_, fileName, FA_CREATE_ALWAYS | FA_WRITE);
        return result == FR_OK;
    }

    int32_t write(const void* data, size_t size)
    {
        UINT bytesWritten = 0;
        const auto result = f_write(&file_, data, UINT(size), &bytesWritten);
        return result == FR_OK ? int32_t(bytesWritten) : -1;
    }

    int32_t read(void* data, size_t maxSize)
    {
        UINT bytesRead = 0;
        const auto result = f_read(&file_, data, UINT(maxSize), &bytesRead);
        return result == FR_OK ? int32_t(bytesRead) : -1;
    }

    bool closeFile()
    {
        const auto result = f_close(&file_);
        return result == FR_OK;
    }

private:
    bool isMounted_ = false;
    daisy::SdmmcHandler sd_;
    daisy::FatFSInterface fileSystem_;
    FIL file_;
};