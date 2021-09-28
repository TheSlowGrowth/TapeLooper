#pragma once

namespace dspdefs
{
    constexpr size_t numLoopers = 4;
    constexpr double loopLengthInS = 60.0;

    constexpr float speedMin = 0.25f;
    constexpr float speedMax = 2.0f;

    namespace paramIds
    {
        const auto chState = "chState";
        const auto chSpeed = "chSpeed";
        const auto chGain = "chGain";
    } // namespace paramIds
} // namespace dspdefs
