project(tapeLooperDsp)
cmake_minimum_required(VERSION 3.19)

set(TARGET tapeLooperDsp)

set(HEADER_FILES
    src/dsp/AudioBuffer.h
    src/dsp/DspHelpers.h
    src/dsp/PeakMeter.h
    src/dsp/Player.h
    src/dsp/Recorder.h
    src/dsp/TapeLooper.h
    src/dsp/TapeProcessor.h
)

add_subdirectory(lib/sprout)

add_library(${TARGET} INTERFACE)
target_include_directories(${TARGET} INTERFACE 
    src/
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/sprout/
) 