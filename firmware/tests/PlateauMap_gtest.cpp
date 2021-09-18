#include <gtest/gtest.h>
#include "util/PlateauMap.h"

TEST(PlateauMapTest, a_matchesPlateauLevelsAndEndStops)
{
    constexpr std::array<float, 3> plateaus = { 0.23f, 0.45f, 0.89f };
    constexpr PlateauMapParameters<float, 3> parameters(plateaus);
    EXPECT_NEAR(plateauMap(0.0f, parameters), 0.0f, 0.000001f);
    EXPECT_NEAR(plateauMap(plateaus[0], parameters), plateaus[0], 0.000001f);
    EXPECT_NEAR(plateauMap(plateaus[1], parameters), plateaus[1], 0.000001f);
    EXPECT_NEAR(plateauMap(plateaus[2], parameters), plateaus[2], 0.000001f);
    EXPECT_NEAR(plateauMap(1.0f, parameters), 1.0f, 0.000001f);
}

TEST(PlateauMapTest, b_monotonicRising)
{
    constexpr PlateauMapParameters<float, 3> parameters({ 0.123f, 0.504f, 0.99f });
    auto lastOutputValue = 0.0f;
    const auto stepSize = 0.01f;
    auto value = stepSize; // we can start with the first step; input = 0 will result in output = 0
    while (value < 1.0f)
    {
        const auto outputValue = plateauMap(value, parameters);
        EXPECT_GT(outputValue, lastOutputValue);
        lastOutputValue = outputValue;
        value += stepSize;
    }
}


TEST(PlateauMapTest, c_plateauSlopeIsZero)
{
    constexpr PlateauMapParameters<float, 1> parameters({ 0.5f });
    const auto delta = 0.00001f;
    const auto left = plateauMap(0.5f - delta, parameters);
    const auto right = plateauMap(0.5f + delta, parameters);
    const auto slope = (right - left) / (2 * delta);
    // the slope at the plateaus is (almost) zero
    EXPECT_LT(std::abs(slope), 0.001f);
    // the slope at the plateau is rising, never falling
    EXPECT_GE(slope, 0.0f);
}

TEST(PlateauMapTest, d_endStopSlopeIsOne)
{
    constexpr PlateauMapParameters<float, 1> parameters({ 0.5f });
    const auto delta = 0.00001f;
    // left endstop
    {
        const auto left = plateauMap(0.0f, parameters);
        const auto right = plateauMap(0.0f + delta, parameters);
        const auto slope = (right - left) / delta;
        // the slope at the end stop is (almost) one
        EXPECT_LT(std::abs(slope - 1.0), 0.01f);
    }
    // right endstop
    {
        const auto left = plateauMap(1.0f - delta, parameters);
        const auto right = plateauMap(1.0f, parameters);
        const auto slope = (right - left) / delta;
        // the slope at the end stop is (almost) one
        EXPECT_LT(std::abs(slope - 1.0), 0.01f);
    }
}

TEST(PlateauMapTest, e_sortsPlateauLevels)
{
    // check that plateau levels are sorted
    constexpr std::array<float, 4> plateaus = { 0.5f, 0.1f, 0.6f, 0.2f }; // intensionally in the wrong order
    constexpr PlateauMapParameters<float, 4> parameters(plateaus);
    constexpr std::array<float, 4> sortedPlateauLevels = { 0.1f, 0.2f, 0.5f, 0.6f };
    EXPECT_EQ(parameters.plateauLevels_, sortedPlateauLevels);
}

TEST(PlateauMapTest, f_limitsTo0to1Range)
{
    constexpr PlateauMapParameters<float, 3> parameters({ 0.3342f, 0.43335f, 0.69f });

    EXPECT_EQ(plateauMap(-0.3f, parameters), 0.0f);
    EXPECT_EQ(plateauMap(1.3f, parameters), 1.0f);
}