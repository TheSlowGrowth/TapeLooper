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
#include <util/hal_map.h>

extern SD_HandleTypeDef hsd1;

class FatFsFileIo
{
public:
    FatFsFileIo()
    {
        Init();
    }

    void ensureVolumeMounted()
    {
        isMounted_ = f_mount(&fileSystem_.GetSDFileSystem(), fileSystem_.GetSDPath(), 1) == FR_OK;

        // In case of an error, reinitialize the peripheral and try again.
        // Someone could have removed the SD and plugged it back in
        if (!isMounted_)
        {
            // unmount
            f_mount(nullptr, fileSystem_.GetSDPath(), 0);
            fileSystem_.DeInit();

            HAL_SD_DeInit(&hsd1);
            memset(&hsd1, 0, sizeof(hsd1));

            daisy::System::DelayUs(10000);

            Init();

            isMounted_ = f_mount(&fileSystem_.GetSDFileSystem(), fileSystem_.GetSDPath(), 1) == FR_OK;
        }
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
    void Init()
    {
        daisy::SdmmcHandler::Config sdCfg;
        sdCfg.Defaults();
        sdCfg.speed = daisy::SdmmcHandler::Speed::STANDARD;
        sd_.Init(sdCfg);
        fileSystem_.Init(daisy::FatFSInterface::Config::MEDIA_SD);
    }

    bool isMounted_ = false;
    daisy::SdmmcHandler sd_;
    daisy::FatFSInterface fileSystem_;
    FIL file_;
};