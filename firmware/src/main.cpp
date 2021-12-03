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

#define MANUAL_INLINE inline __attribute__((always_inline))

#include "daisy_seed.h"

#include "constants.h"
#include "dsp/PeakMeter.h"
#include "hardware/UiHardware.h"
#include "ui/TapeLooperUi.h"
#include "dsp/TapeLooper.h"
#include "util/LateInitializedObject.h"
#include "LooperController.h"
#include "ui/LooperParameterProvider.h"

#define EXTERNAL_SDRAM_SECTION __attribute__((section(".sdram_bss")))

// type definitions
using LooperParameterProviderType = LooperParameterProvider<numLoopers>;
struct LooperTypes
{
    using MonoLooperType = TapeLooper<sampleRateHz, 1>;
    using StereoLooperType = TapeLooper<sampleRateHz, 2>;
    using ParameterProvider = LooperParameterProviderType;
};
using LooperControllerType = LooperController<LooperTypes, numLoopers>;
using TapeLooperUiType = TapeLooperUi<UiHardware,
                                      PeakMeter<blockSize, sampleRateHz>,
                                      LooperControllerType,
                                      LooperParameterProviderType>;
using LooperStoragesType = std::array<MonoOrStereoLooperStorage<looperSamplesPerChannel>, numLoopers>;

// hardware static objects
daisy::DaisySeed seed;

// ui static objects
daisy::UiEventQueue uiEventQueue;
UiHardware::LedDmaBufferType DMA_BUFFER_MEM_SECTION ledDmaBufferA, ledDmaBufferB;
LateInitializedObject<UiHardware> uiHardware;
LateInitializedObject<TapeLooperUiType> ui;

// dsp static objects
std::array<PeakMeter<blockSize, sampleRateHz>, 4> peakMeters;
#define EXTERNAL_SDRAM_SECTION __attribute__((section(".sdram_bss")))
LateInitializedObject<LooperStoragesType> EXTERNAL_SDRAM_SECTION looperStorages;
LateInitializedObject<LooperParameterProviderType> looperParameterProvider;
AudioBuffer<1, blockSize> monoDownmixBuffer;
AudioBuffer<1, blockSize> temporaryBuffer;
LateInitializedObject<LooperControllerType> looperController;

// misc
daisy::CpuLoadMeter cpuLoadMeter;

// ===================================================================
// UI
// ===================================================================

void clearLedDisplay(const daisy::UiCanvasDescriptor&)
{
    uiHardware->clearLeds();
}

void flushLedDisplay(const daisy::UiCanvasDescriptor&)
{
    uiHardware->transmitLeds();
}

void initUi()
{
    // init the UI hardware
    auto& hardware = *uiHardware.create(uiEventQueue,
                                        ledDmaBufferA,
                                        ledDmaBufferB);

    // init the UI
    ui.create(
        hardware,
        &clearLedDisplay,
        &flushLedDisplay,
        uiEventQueue,
        peakMeters,
        looperController,
        looperParameterProvider);
}

// ===================================================================
// DSP Init
// ===================================================================

void initDsp()
{
    // init the parameter provider
    looperParameterProvider.create();

    // initialize the looper storage in SDRAM
    const auto& rawStorages = *looperStorages.create();
    // build an array of storage pointers
    std::array<MonoOrStereoLooperStoragePtr, numLoopers> arrayOfStoragePtrs;
    for (size_t i = 0; i < numLoopers; i++)
        arrayOfStoragePtrs[i] = rawStorages[i];

    // initialize the looper controller that contains/controls the loopers
    looperController.create(
        arrayOfStoragePtrs,
        monoDownmixBuffer,
        temporaryBuffer,
        looperParameterProvider);

    // init peak meters
    for (auto& peakMeter : peakMeters)
        peakMeter.init(0.5f);

    // init CPU load meter
    cpuLoadMeter.Init(seed.AudioSampleRate(), seed.AudioBlockSize());
}

// ===================================================================
// ===================================================================

void configurePlatform()
{
    seed.Configure();
    seed.Init(true /* enable 480MHz boost */);
    seed.SetAudioBlockSize(blockSize);
    seed.SetAudioSampleRate(sampleRate);
}

void audioCallback(const float* const* in, float** out, size_t size)
{
    cpuLoadMeter.OnBlockStart();

    // peak meters
    peakMeters[0].readPeaks(in[0]);
    peakMeters[1].readPeaks(in[1]);

    // process
    AudioBufferPtr<numChannelsPerLooper, const float> inputs(in, size);
    AudioBufferPtr<numChannelsPerLooper, float> outputs(out, size);
    outputs.fill(0.0f);
    looperController->process(inputs, outputs);

    // peak meters
    peakMeters[2].readPeaks(out[0]);
    peakMeters[3].readPeaks(out[1]);

    // clip outputs
    // TODO

    cpuLoadMeter.OnBlockEnd();
    // Set breakpoint here to check load
    const auto load = cpuLoadMeter.GetAvgCpuLoad();
    (void) load;
}

int main(void)
{
    configurePlatform();

    initDsp();

    initUi();

    // start audio
    seed.StartAudio(&audioCallback);

    // UI loop
    for (;;)
    {
        uiHardware->processControls();
        ui->process();
    }
}
