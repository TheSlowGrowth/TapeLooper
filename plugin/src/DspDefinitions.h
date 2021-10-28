#pragma once

namespace dspdefs
{
    constexpr size_t numLoopers = 4;
    constexpr double loopLengthInS = 60.0;

    constexpr float speedMin = 0.25f;
    constexpr float speedMax = 4.0f;

    namespace paramIds
    {
        const auto chState = "chState";
        const auto chSpeed = "chSpeed";
        const auto chDrive = "chDrive";
        const auto chGrainAmt = "chGrainAmt";
        const auto chWowAndFlutterAmt = "chWowAndFlutterAmt";
        const auto chPostGain = "chPostGain";
    } // namespace paramIds
} // namespace dspdefs
