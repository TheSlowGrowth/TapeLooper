#define MANUAL_INLINE inline __attribute__((always_inline))

#include "daisy_seed.h"

#include "constants.h"
#include "dsp/PeakMeter.h"
#include "hardware/Buttons.h"
#include "hardware/Leds.h"
#include "hardware/Pots.h"
#include "ui/UiBasePage.h"

#define EXTERNAL_SDRAM_SECTION __attribute__((section(".sdram_bss")))

// constants
constexpr uint16_t potIdleTimeoutMs = 500; // TODO: adjust
constexpr uint16_t buttonDebounceTimeoutMs = 50; // TODO: adjust
constexpr uint32_t buttonDoubleClickTimeoutMs = 500; // TODO: adjust
constexpr uint32_t retriggerTimeoutMs = 1000; // TODO: adjust
constexpr uint32_t retriggerPeriodMs = 50; // TODO: adjust

// hardware static objects
daisy::DaisySeed seed;
PotReader potReader;
ButtonReader buttonReader;

// ui static objects
daisy::UiEventQueue uiEventQueue;
daisy::PotMonitor<PotReader, 12> potMonitor;
daisy::ButtonMonitor<ButtonReader, int(Buttons::NUM_BUTTONS)> buttonMonitor;

LedDriverType::DmaBuffer DMA_BUFFER_MEM_SECTION ledDmaBufferA, ledDmaBufferB;
LedDriverType leds;

PeakMeter<blockSize, sampleRateHz> peakMeterInL;
PeakMeter<blockSize, sampleRateHz> peakMeterInR;
PeakMeter<blockSize, sampleRateHz> peakMeterOutL;
PeakMeter<blockSize, sampleRateHz> peakMeterOutR;

// dsp static objects
// TODO

// misc
daisy::CpuLoadMeter cpuLoadMeter;

// ===================================================================
// UI
// ===================================================================

daisy::UI ui;

void clearDisplay(const daisy::UiCanvasDescriptor&)
{
    leds.SetAllToRaw(0);
}

void updateDisplay(const daisy::UiCanvasDescriptor&)
{
    leds.SwapBuffersAndTransmit();
}

void initUi()
{
    // initialize the display
    // TODO

    // add the led display canvas
    daisy::UiCanvasDescriptor ledDisplayDescriptor;
    ledDisplayDescriptor.id_ = 0;
    ledDisplayDescriptor.handle_ = &leds;
    ledDisplayDescriptor.updateRateMs_ = 50;
    ledDisplayDescriptor.clearFunction_ = &clearDisplay;
    ledDisplayDescriptor.flushFunction_ = &updateDisplay;

    // initialize the UI
    ui.Init(uiEventQueue,
            daisy::UI::SpecialControlIds {},
            { ledDisplayDescriptor });
}

// ===================================================================
// UI Pages
// ===================================================================

BaseUiPage basePage;

void initUiPages()
{
    // open the base UI page
    ui.OpenPage(basePage);
}

// ===================================================================
// ===================================================================

void configureHardware()
{
    // Configure and Initialize the Daisy Seed
    // These are separate to allow reconfiguration of any of the internal
    // components before initialization.
    seed.Configure();
    seed.SetAudioBlockSize(blockSize);
    seed.SetAudioSampleRate(sampleRate);
    seed.Init(true /* enable 480MHz boost */);

    potReader.init();
    buttonReader.init();

    // init leds
    daisy::I2CHandle i2c;
    i2c.Init({ daisy::I2CHandle::Config::Peripheral::I2C_1,
               { { DSY_GPIOB, 8 }, { DSY_GPIOB, 9 } },
               daisy::I2CHandle::Config::Speed::I2C_100KHZ });
    leds.Init(i2c,
              { 0 },
              ledDmaBufferA,
              ledDmaBufferB,
              { DSY_GPIOB, 6 });
}

void scanUiControls()
{
    potMonitor.Process();
    buttonMonitor.Process();
}

void audioCallback(const float* const* in, float** out, size_t size)
{
    cpuLoadMeter.OnBlockStart();

    scanUiControls(); // TODO: move to low prio timer?

    // peak meters
    peakMeterInL.readPeaks(in[0]);
    peakMeterInR.readPeaks(in[1]);

    // TODO: add processing here

    // peak meters
    peakMeterOutL.readPeaks(out[0]);
    peakMeterOutR.readPeaks(out[1]);

    // clip outputs
    // TODO

    cpuLoadMeter.OnBlockEnd();
}

int main(void)
{
    configureHardware();

    // init monitors
    potMonitor.Init(uiEventQueue,
                    potReader,
                    potIdleTimeoutMs);
    buttonMonitor.Init(uiEventQueue,
                       buttonReader,
                       buttonDebounceTimeoutMs,
                       buttonDoubleClickTimeoutMs,
                       retriggerTimeoutMs,
                       retriggerPeriodMs);
    // init UI
    initUi();
    initUiPages();

    // init peak meters
    peakMeterInL.init(0.5f);
    peakMeterInR.init(0.5f);
    peakMeterOutL.init(0.5f);
    peakMeterOutR.init(0.5f);

    // init CPU load meter
    cpuLoadMeter.Init(seed.AudioSampleRate(), seed.AudioBlockSize());

    // start audio
    seed.StartAudio(&audioCallback);

    // UI loop
    for (;;)
    {
        ui.Process();

        // Wait 5ms
        seed.system.Delay(5);
    }
}
