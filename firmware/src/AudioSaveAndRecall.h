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

#include <util/FixedCapStr.h>
#include <dsp/TapeLooper.h>
#include "util/WavFileFormat.h"

extern int32_t f2s32(float x);
extern float s322f(int32_t x);
extern float s242f(int32_t x);
extern float s162f(int16_t x);

using AudioFileName = daisy::FixedCapStr<32>;

enum class AudioSaveAndRecallResult
{
    ok,
    error
};

enum class StorageBank
{
    green,
    yellow,
    red,

    count
};

typedef void (*AudioSaveAndRecallDoneCallbackPtr)(void* context, AudioSaveAndRecallResult result);

namespace loop_library
{
    static auto kLibraryBaseFolder = "loops/";

    AudioFileName makeFileNameWithoutExtensionFor(const StorageBank bank,
                                                  const int slot);
    AudioFileName makeWavFileNameFor(const StorageBank bank,
                                     const int slot);

    template <typename FileIoProvider>
    class Library
    {
    public:
        Library(FileIoProvider& fileIo) :
            fileIo_(fileIo)
        {
            for (int i = 0; i < int(StorageBank::count); i++)
            {
                isStorageSlotUsedBitfield_[i] = 0;
            }
        }

        void scanOrInitLibrary()
        {
            fileIo_.ensureVolumeMounted();

            fileIo_.makeFolderIfNotExistent(loop_library::kLibraryBaseFolder);

            for (size_t bank = 0; bank < kNumBanks; bank++)
            {
                for (size_t slot = 0; slot < kNumSlots; slot++)
                {
                    const auto filename = loop_library::makeWavFileNameFor(StorageBank(bank), slot);

                    if (const auto isInUse = fileIo_.hasFile(filename))
                        isStorageSlotUsedBitfield_[bank] |= (1 << slot);
                    else
                        isStorageSlotUsedBitfield_[bank] &= ~(1 << slot);
                }
            }
        }

        bool hasLoop(StorageBank bank, int slot)
        {
            if (slot < 0 || slot >= kNumSlots)
                return false;

            return isStorageSlotUsedBitfield_[int(bank)] & (1 << slot);
        }

    private:
        FileIoProvider& fileIo_;

        static constexpr auto kNumSlots = 16;
        static constexpr auto kNumBanks = int(StorageBank::count);
        int16_t isStorageSlotUsedBitfield_[kNumBanks];
    };
}; // namespace loop_library

template <typename FileIoProvider>
class AudioSaveAndRecall
{
public:
    AudioSaveAndRecall(FileIoProvider& fileIo) :
        fileIo_(fileIo)
    {
    }

    template <typename LooperType>
    void startSavingToFile(StorageBank bank,
                           int slot,
                           LooperType& looper,
                           AudioSaveAndRecallDoneCallbackPtr doneCallback,
                           void* doneCallbackContext)
    {
        startSavingToFile(loop_library::makeWavFileNameFor(bank, slot),
                          looper,
                          doneCallback,
                          doneCallbackContext);
    }

    template <typename LooperType>
    void startSavingToFile(const AudioFileName& filename,
                           LooperType& looper,
                           AudioSaveAndRecallDoneCallbackPtr doneCallback,
                           void* doneCallbackContext)
    {
        static_assert(LooperType::getNumChannels() <= kMaxNumChannels_);

        fileIo_.ensureVolumeMounted();

        looperPtr_ = &looper;
        doneCallback_ = doneCallback;
        doneCallbackContext_ = doneCallbackContext;

        looper.preventRecording();
        liftRestrictionsFunc_ = [](void* looperPtr)
        {
            LooperType* looper = reinterpret_cast<LooperType*>(looperPtr);
            looper->liftRestrictions();
        };

        totalNumFrames_ = looper.getPlaybackLength();

        if (totalNumFrames_ == 0)
        {
            abort(false);
            return;
        }

        if (
            !fileIo_.openForWriting(filename)
            || !writeWavHeader(totalNumFrames_, looper.getNumChannels(), looper.getSampleRate()))
        {
            abort(true);
            return;
        }

        writeOrReadFunc_ = [](AudioSaveAndRecall* storage, void* looperPtr)
        {
            LooperType* looper = reinterpret_cast<LooperType*>(looperPtr);

            // wait while the current recording is still crossfading out
            if (looper->isRecording())
                return ReadOrWriteResult::inProgress;

            const auto numSamplesLeft = storage->totalNumFrames_ - storage->numFramesDone_;

            if (numSamplesLeft <= 0)
            {
                return ReadOrWriteResult::error;
            }

            const auto numFramesInThisChunk = std::min(size_t(numSamplesLeft), kNumFramesPerChunk_);
            const auto writeSuccessful = storage->writeBuffer(looper->getSampleStoragePtr(),
                                                              storage->numFramesDone_,
                                                              numFramesInThisChunk);
            if (!writeSuccessful)
            {
                return ReadOrWriteResult::error;
            }

            storage->numFramesDone_ += numFramesInThisChunk;

            const auto isNowDone = storage->numFramesDone_ == storage->totalNumFrames_;
            if (isNowDone)
            {
                storage->fileIo_.closeFile();
                return ReadOrWriteResult::completed;
            }
            return ReadOrWriteResult::inProgress;
        };
    }

    size_t getNumChannelsInFile(StorageBank bank,
                                int slot)
    {
        return getNumChannelsInFile(loop_library::makeWavFileNameFor(bank, slot));
    }

    size_t getNumChannelsInFile(const AudioFileName& filename)
    {
        if (isSavingOrRecalling())
        {
            return 0;
        }

        if (!fileIo_.openForReading(filename))
        {
            return 0;
        }

        if (!readWavHeader() || !isCompatibleFormat())
        {
            fileIo_.closeFile();
            return 0;
        }

        fileIo_.closeFile();
        return size_t(wavHeader_.numAudioChannels);
    }

    template <typename LooperType>
    void startReadingFromFile(StorageBank bank,
                              int slot,
                              LooperType& looper,
                              AudioSaveAndRecallDoneCallbackPtr doneCallback,
                              void* doneCallbackContext)
    {
        startReadingFromFile(loop_library::makeWavFileNameFor(bank, slot),
                             looper,
                             doneCallback,
                             doneCallbackContext);
    }

    template <typename LooperType>
    void startReadingFromFile(const AudioFileName& filename,
                              LooperType& looper,
                              AudioSaveAndRecallDoneCallbackPtr doneCallback,
                              void* doneCallbackContext)
    {
        static_assert(LooperType::getNumChannels() <= kMaxNumChannels_);

        fileIo_.ensureVolumeMounted();

        looperPtr_ = &looper;
        doneCallback_ = doneCallback;
        doneCallbackContext_ = doneCallbackContext;

        looper.preventPlaybackAndRecording(true);
        liftRestrictionsFunc_ = [](void* looperPtr)
        {
            LooperType* looper = reinterpret_cast<LooperType*>(looperPtr);
            looper->liftRestrictions();
        };

        if (!fileIo_.openForReading(filename))
        {
            abort(false);
            return;
        }

        if (!readWavHeader()
            || !isCompatibleFormat()
            || LooperType::getNumChannels() != wavHeader_.numAudioChannels)
        {
            abort(true);
            return;
        }

        totalNumFrames_ = std::min(looper.getSampleStoragePtr().numSamples,
                                   size_t(getNumSamples(wavHeader_)));

        writeOrReadFunc_ = [](AudioSaveAndRecall* storage, void* looperPtr)
        {
            LooperType* looper = reinterpret_cast<LooperType*>(looperPtr);

            const auto numSamplesLeft = storage->totalNumFrames_ - storage->numFramesDone_;
            const auto numFramesInThisChunk = std::min(numSamplesLeft, kNumFramesPerChunk_);

            const auto numSamplesRead = storage->readBuffer(looper->getSampleStoragePtr(),
                                                            storage->numFramesDone_,
                                                            numFramesInThisChunk);
            if (numSamplesRead < 0)
            {
                return ReadOrWriteResult::error;
            }

            storage->numFramesDone_ += numSamplesRead;

            const auto isNowDone = storage->totalNumFrames_ == storage->numFramesDone_;
            if (isNowDone)
            {
                looper->setPlaybackLength(storage->totalNumFrames_);
                storage->fileIo_.closeFile();
                return ReadOrWriteResult::completed;
            }
            return ReadOrWriteResult::inProgress;
        };
    }

    float getCurrentProgress() const
    {
        if (totalNumFrames_ <= 0)
            return -1.0f;
        return float(numFramesDone_) / float(totalNumFrames_);
    }

    bool isSavingOrRecalling() const
    {
        return totalNumFrames_ > 0;
    }

    void readOrWriteNextChunk()
    {
        if (writeOrReadFunc_)
        {
            const auto result = writeOrReadFunc_(this, looperPtr_);

            if (result == ReadOrWriteResult::error)
            {
                abort(true);
                return;
            }
            else if (result == ReadOrWriteResult::completed)
            {
                liftRestrictionsFunc_(looperPtr_);
                if (doneCallback_)
                {
                    doneCallback_(doneCallbackContext_, AudioSaveAndRecallResult::ok);
                }

                doneCallback_ = nullptr;
                doneCallbackContext_ = nullptr;
                liftRestrictionsFunc_ = nullptr;
                looperPtr_ = nullptr;
            }
        }
    }

    FileIoProvider& getFileIoProviderForTesting() { return fileIo_; }

    void abort()
    {
        if (isSavingOrRecalling())
        {
            abort(true);
        }
    }

private:
    void abort(bool shouldCloseFile)
    {
        if (shouldCloseFile)
            fileIo_.closeFile();

        if (liftRestrictionsFunc_)
        {
            liftRestrictionsFunc_(looperPtr_);
        }

        if (doneCallback_)
        {
            doneCallback_(doneCallbackContext_, AudioSaveAndRecallResult::error);
        }

        doneCallback_ = nullptr;
        doneCallbackContext_ = nullptr;
        liftRestrictionsFunc_ = nullptr;
        looperPtr_ = nullptr;
        totalNumFrames_ = 0;
        numFramesDone_ = 0;
    }

    bool writeWavHeader(size_t totalNumFrames, size_t numChannelsPerFrame, int sampleRate)
    {
        wavHeader_ = makeWavFileHeader32bitInt(sampleRate, numChannelsPerFrame, totalNumFrames);

        return fileIo_.write(&wavHeader_, sizeof(wavHeader_)) == int32_t(sizeof(wavHeader_));
    }

    bool readWavHeader()
    {
        const auto skipForwardUntil = [&](int32_t value)
        {
            int32_t last4Bytes = 0;
            while (last4Bytes != value)
            {
                uint8_t nextByte;
                if (fileIo_.read(&nextByte, 1) != 1)
                {
                    return false;
                }

                last4Bytes = ((last4Bytes >> 8) & 0x00FFFFFF)
                             | (nextByte << 24);
            }
            return true;
        };

        // read Master RIFF chunk
        {
            const auto kMasterRiffChunkSize = 3 * sizeof(int32_t);
            const auto numBytesRead = fileIo_.read(&wavHeader_.fileTypeBlocId, kMasterRiffChunkSize);

            // invalid Master RIFF chunk
            if (numBytesRead != kMasterRiffChunkSize
                || wavHeader_.fileTypeBlocId != 0x46464952 // "RIFF"
                || wavHeader_.fileFormatId != 0x45564157 // "WAVE"
            )
            {
                return false;
            }
        }

        // read fmt chunk
        {
            if (!skipForwardUntil(0x20746d66)) // "fmt"
            {
                return false;
            }
            wavHeader_.formatBlocId = 0x20746d66;

            const auto kFmtChunkContentSize = 5 * sizeof(int32_t);
            if (fileIo_.read(&wavHeader_.formatBlocSize, kFmtChunkContentSize) != kFmtChunkContentSize)
            {
                return false;
            }

            if (wavHeader_.formatBlocSize < 0x10) // fmt chunk size can never be <16bytes
            {
                return false;
            }
        }

        // there can be additional sections in the file. Skip forward if our
        // data section doesn't point to the right data yet.
        {
            if (!skipForwardUntil(0x61746164)) // "data"
            {
                return false;
            }

            wavHeader_.dataBlocId = 0x61746164;
            if (fileIo_.read(&wavHeader_.dataBlocSize, sizeof(wavHeader_.dataBlocSize)) != sizeof(wavHeader_.dataBlocSize))
            {
                return false;
            }
        }

        return true;
    }

    bool isCompatibleFormat() const
    {
        const auto isCompatibleSampleFormat =
            // 16bit PCM or 24bit PCM or 32bit PCM
            (wavHeader_.audioFormat == WavFileFormatCode::pcm
             && (wavHeader_.numBitsPerSample == 16
                 || wavHeader_.numBitsPerSample == 24
                 || wavHeader_.numBitsPerSample == 32))
            // 32bit float
            || (wavHeader_.audioFormat == WavFileFormatCode::ieeeFloat
                && wavHeader_.numBitsPerSample == 32);

        const auto isCompatibleNumChannels =
            wavHeader_.numAudioChannels == 1
            || wavHeader_.numAudioChannels == 2;

        return isCompatibleSampleFormat && isCompatibleNumChannels;
    }

    template <size_t numChannels>
    bool writeBuffer(LooperStoragePtr<numChannels> storage, const size_t startFrame, const size_t numFrames)
    {
        float* inPtr[numChannels];

        for (size_t ch = 0; ch < numChannels; ch++)
            inPtr[ch] = &storage.data[ch][startFrame];

        int32_t* outPtr = &writeBuffer_[0];

        size_t numFramesLeft = numFrames;
        while (numFramesLeft > 0)
        {
            for (size_t ch = 0; ch < numChannels; ch++)
            {
                *outPtr = f2s32(*inPtr[ch]);
                outPtr++;
                inPtr[ch]++;
            }
            numFramesLeft--;
        }

        const auto numBytes = std::distance(&writeBuffer_[0], outPtr) * sizeof(int32_t);
        return fileIo_.write(writeBuffer_, numBytes) == int32_t(numBytes);
    }

    template <size_t numChannels>
    int32_t readBuffer(LooperStoragePtr<numChannels> storage, const size_t startFrame, const size_t maxNumFrames)
    {
        float* outPtr[numChannels];

        for (size_t ch = 0; ch < numChannels; ch++)
            outPtr[ch] = &storage.data[ch][startFrame];

        uint8_t* inPtr = reinterpret_cast<uint8_t*>(&writeBuffer_[0]);
        int32_t numFramesRead = 0;

        if (wavHeader_.audioFormat == WavFileFormatCode::ieeeFloat)
        {
            constexpr auto kFrameSize = sizeof(float);
            const auto maxNumBytes = maxNumFrames * numChannels * kFrameSize;
            const auto bytesRead = fileIo_.read(writeBuffer_, maxNumBytes);

            if (bytesRead < 0)
            {
                return -1;
            }

            numFramesRead = bytesRead / kFrameSize;
            size_t numFramesLeft = numFramesRead;
            while (numFramesLeft > 0)
            {
                for (size_t ch = 0; ch < numChannels; ch++)
                {
                    *outPtr[ch] = *reinterpret_cast<float*>(inPtr);
                    inPtr += kFrameSize;
                    outPtr[ch]++;
                }
                numFramesLeft--;
            }
        }
        else if (wavHeader_.audioFormat == WavFileFormatCode::pcm && wavHeader_.numBitsPerSample == 32)
        {
            constexpr auto kFrameSize = sizeof(int32_t);
            const auto maxNumBytes = maxNumFrames * numChannels * kFrameSize;
            const auto bytesRead = fileIo_.read(writeBuffer_, maxNumBytes);

            if (bytesRead < 0)
            {
                return -1;
            }

            numFramesRead = bytesRead / kFrameSize;
            size_t numFramesLeft = numFramesRead;
            while (numFramesLeft > 0)
            {
                for (size_t ch = 0; ch < numChannels; ch++)
                {
                    *outPtr[ch] = s322f(*reinterpret_cast<int32_t*>(inPtr));
                    inPtr += kFrameSize;
                    outPtr[ch]++;
                }
                numFramesLeft--;
            }
        }
        else if (wavHeader_.audioFormat == WavFileFormatCode::pcm && wavHeader_.numBitsPerSample == 24)
        {
            constexpr auto kFrameSize = sizeof(int8_t) * 3;
            const auto maxNumBytes = maxNumFrames * numChannels * kFrameSize;
            const auto bytesRead = fileIo_.read(writeBuffer_, maxNumBytes);

            if (bytesRead < 0)
            {
                return -1;
            }

            numFramesRead = bytesRead / kFrameSize;
            size_t numFramesLeft = numFramesRead;
            while (numFramesLeft > 0)
            {
                for (size_t ch = 0; ch < numChannels; ch++)
                {
                    *outPtr[ch] = s242f(*reinterpret_cast<int32_t*>(inPtr) & 0xFFFFFF);
                    inPtr += kFrameSize;
                    outPtr[ch]++;
                }
                numFramesLeft--;
            }
        }
        else if (wavHeader_.audioFormat == WavFileFormatCode::pcm && wavHeader_.numBitsPerSample == 16)
        {
            constexpr auto kFrameSize = sizeof(int16_t);
            const auto maxNumBytes = maxNumFrames * numChannels * kFrameSize;
            const auto bytesRead = fileIo_.read(writeBuffer_, maxNumBytes);

            if (bytesRead < 0)
            {
                return -1;
            }

            numFramesRead = bytesRead / kFrameSize;
            size_t numFramesLeft = numFramesRead;
            while (numFramesLeft > 0)
            {
                for (size_t ch = 0; ch < numChannels; ch++)
                {
                    *outPtr[ch] = s162f(*reinterpret_cast<int16_t*>(inPtr));
                    inPtr += kFrameSize;
                    outPtr[ch]++;
                }
                numFramesLeft--;
            }
        }

        return numFramesRead / numChannels;
    }

    static constexpr size_t kNumFramesPerChunk_ = 1000;
    static constexpr size_t kBitsPerSample_ = 32u;
    static constexpr size_t kMaxNumChannels_ = 2u;
    static constexpr size_t kWriteBufferSizeBytes_ = kNumFramesPerChunk_
                                                     * kMaxNumChannels_
                                                     * kBitsPerSample_ / 8u;

    WavFileHeader wavHeader_;

    enum class ReadOrWriteResult
    {
        inProgress,
        completed,
        error
    };
    typedef ReadOrWriteResult (*WriteOrReadFuncPtr)(AudioSaveAndRecall* storage, void* looper);
    typedef void (*LiftRestrictionsFuncPtr)(void* looper);

    FileIoProvider& fileIo_;

    size_t totalNumFrames_ = 0;
    size_t numFramesDone_ = 0;
    WriteOrReadFuncPtr writeOrReadFunc_ = nullptr;
    LiftRestrictionsFuncPtr liftRestrictionsFunc_ = nullptr;
    void* looperPtr_ = nullptr;
    AudioSaveAndRecallDoneCallbackPtr doneCallback_ = nullptr;
    void* doneCallbackContext_ = nullptr;

    int32_t writeBuffer_[kWriteBufferSizeBytes_ / sizeof(int32_t)];
};
