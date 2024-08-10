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
#include <util/wav_format.h>

using AudioFileName = daisy::FixedCapStr<32>;
enum class AudioSaveAndRecallResult
{
    ok,
    error
};

template <typename FileIoProvider>
class AudioSaveAndRecall
{
public:
    typedef void (*DoneCallbackPtr)(void* context, AudioSaveAndRecallResult result);

    AudioSaveAndRecall()
    {
    }

    template <typename LooperType>
    void startSavingToFile(const AudioFileName& filename,
                           LooperType& looper,
                           DoneCallbackPtr doneCallback,
                           void* doneCallbackContext)
    {
        static_assert(LooperType::getNumChannels() <= kMaxNumChannels_);

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

    template <typename LooperType>
    void startReadingFromFile(const AudioFileName& filename,
                              LooperType& looper,
                              DoneCallbackPtr doneCallback,
                              void* doneCallbackContext)
    {
        static_assert(LooperType::getNumChannels() <= kMaxNumChannels_);

        looperPtr_ = &looper;
        doneCallback_ = doneCallback;
        doneCallbackContext_ = doneCallbackContext;

        looper.preventPlaybackAndRecording();
        looper.stopRecordingImmediately();
        liftRestrictionsFunc_ = [](void* looperPtr)
        {
            LooperType* looper = reinterpret_cast<LooperType*>(looperPtr);
            looper->liftRestrictions();
        };

        if (!fileIo_.openForReading(filename))
        {
            abort(true);
            return;
        }
        // TODO: read file header

        totalNumFrames_ = 1000; // TODO

        writeOrReadFunc_ = [](AudioSaveAndRecall* storage, void* looperPtr)
        {
            constexpr auto kChunkSize = 1000u;

            LooperType* looper = reinterpret_cast<LooperType*>(looperPtr);

            const auto numSamplesLeft = storage->totalNumFrames_ - storage->numFramesDone_;
            const auto numFramesInThisChunk = std::min(numSamplesLeft, kChunkSize);

            // TODO: read a chunk

            const auto isNowDone = numSamplesLeft == numFramesInThisChunk;
            if (isNowDone)
            {
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
                doneCallback_(doneCallbackContext_, AudioSaveAndRecallResult::ok);

                doneCallback_ = nullptr;
                doneCallbackContext_ = nullptr;
                liftRestrictionsFunc_ = nullptr;
                looperPtr_ = nullptr;
            }
        }
    }

    FileIoProvider& getFileIoProviderForTesting() { return fileIo_; }

private:
    void abort(bool shouldCloseFile)
    {
        if (shouldCloseFile)
            fileIo_.closeFile();

        liftRestrictionsFunc_(looperPtr_);

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
        const auto byteRate = sampleRate * numChannelsPerFrame * kBitsPerSample_ / 8;

        wavHeader_.ChunkId = daisy::kWavFileChunkId; /** "RIFF" */
        wavHeader_.FileFormat = daisy::kWavFileWaveId; /** "WAVE" */
        wavHeader_.SubChunk1ID = daisy::kWavFileSubChunk1Id; /** "fmt " */
        wavHeader_.SubChunk1Size = 16; // for PCM
        wavHeader_.AudioFormat = daisy::WAVE_FORMAT_PCM;
        wavHeader_.NbrChannels = numChannelsPerFrame;
        wavHeader_.SampleRate = sampleRate;
        wavHeader_.ByteRate = byteRate;
        wavHeader_.BlockAlign = numChannelsPerFrame * kBitsPerSample_ / 8;
        wavHeader_.BitPerSample = kBitsPerSample_;
        wavHeader_.SubChunk2ID = daisy::kWavFileSubChunk2Id; /** "data" */
        wavHeader_.SubCHunk2Size = totalNumFrames * numChannelsPerFrame * kBitsPerSample_ / 8;
        wavHeader_.FileSize = 36 + wavHeader_.SubCHunk2Size;

        return fileIo_.write(&wavHeader_, sizeof(wavHeader_));
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

        return fileIo_.write(writeBuffer_, sizeof(writeBuffer_));
    }

    static constexpr auto kNumFramesPerChunk_ = 1000ul;
    static constexpr auto kBitsPerSample_ = 32u;
    static constexpr auto kMaxNumChannels_ = 2u;
    static constexpr auto kWriteBufferSize_ = kNumFramesPerChunk_
                                              * kMaxNumChannels_
                                              * kBitsPerSample_ / 8u;

    daisy::WAV_FormatTypeDef wavHeader_;

    enum class ReadOrWriteResult
    {
        inProgress,
        completed,
        error
    };
    typedef ReadOrWriteResult (*WriteOrReadFuncPtr)(AudioSaveAndRecall* storage, void* looper);
    typedef void (*LiftRestirctionsFuncPtr)(void* looper);

    FileIoProvider fileIo_;

    size_t totalNumFrames_ = 0;
    size_t numFramesDone_ = 0;
    WriteOrReadFuncPtr writeOrReadFunc_ = nullptr;
    LiftRestirctionsFuncPtr liftRestrictionsFunc_ = nullptr;
    void* looperPtr_ = nullptr;
    DoneCallbackPtr doneCallback_ = nullptr;
    void* doneCallbackContext_ = nullptr;

    int32_t writeBuffer_[kWriteBufferSize_];
};