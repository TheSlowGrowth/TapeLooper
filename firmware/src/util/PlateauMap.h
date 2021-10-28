#include <stdint.h>
#include <initializer_list>
#include <memory.h>
#include <dsp/DspHelpers.h>

/**
 * Calculates the transition function parameters for the
 * plateauMap() function.
 * Can be evaluated at compile time.
 */
template <typename FloatType, size_t numPlateaus>
class PlateauMapParameters
{
public:
    constexpr PlateauMapParameters(const std::array<FloatType, numPlateaus>& plateauLevels) :
        plateauLevels_(simpleSort(plateauLevels)),
        transitionParameterSet_(calculateTransitionParameterSet(plateauLevels_))
    {
    }

    static constexpr size_t numPlateaus_ = numPlateaus;
    static constexpr size_t numTransitions_ = numPlateaus + 1;
    const std::array<FloatType, numPlateaus> plateauLevels_;

    struct TransitionParameters
    {
        FloatType a = FloatType(0);
        FloatType b = FloatType(0);
        FloatType c = FloatType(0);
        FloatType d = FloatType(0);
    };
    using TransitionParameterSet = std::array<TransitionParameters, numTransitions_>;
    const TransitionParameterSet transitionParameterSet_;

private:
    static constexpr std::array<FloatType, numPlateaus> simpleSort(std::array<FloatType, numPlateaus> array)
    {
        for (size_t i = 0; i < numPlateaus; i++)
        {
            for (size_t j = i; j < numPlateaus; j++)
            {
                if (array[j] < array[i])
                {
                    const auto tmp = array[i];
                    array[i] = array[j];
                    array[j] = tmp;
                }
            }
        }
        return array;
    }

    static constexpr TransitionParameterSet calculateTransitionParameterSet(
        const std::array<FloatType, numPlateaus>& plateauLevels)
    {
        TransitionParameterSet result {};

        for (size_t i = 0; i < numTransitions_; i++)
        {
            const auto isFirst = i == 0;
            const auto isLast = i == (numTransitions_ - 1);
            // 1st & 2nd boundary condition: match plateau levels at both ends
            const FloatType levelLeft = isFirst ? FloatType(0) : plateauLevels[i - 1];
            const FloatType levelRight = isLast ? FloatType(1) : plateauLevels[i];
            // 3rd & 4th boundary condition: match slope at both ends
            const FloatType slopeLeft = isFirst ? FloatType(1) : FloatType(0);
            const FloatType slopeRight = isLast ? FloatType(1) : FloatType(0);

            /* 
            Using this prototype for the transition function:
                f(x) = ax^3 + bx^2 + cx + d
            These are the boundary conditions:
               1: f(lL) = lL:
                    d = lL - a*lL^3 - b*lL^2 - c*lL
               2: f(lR) = lR:
                    d = lR - a*lR^3 - b*lR^2 - c*lR
               3: f'(lL) = sL
                    c = sL - 3*a*lL^2 - 2*b*lL
               4: f'(lR) = sR
                    c = sR - 3*a*lR^2 - 2*b*lR

            Solve the polynomial parameters with this python3 script:
            -----------------------------------------
            import sympy
            a, b, c, d, sL, sR, lL, lR = sympy.symbols("a b c d sL sR lL lR")
            result = sympy.solve_poly_system([lL - a*lL**3 - b*lL**2 - c*lL - d,
                                              lR - a*lR**3 - b*lR**2 - c*lR - d,
                                              sL - 3*a*lL**2 - 2*b*lL - c,
                                              sR - 3*a*lR**2 - 2*b*lR - c], a, b, c, d)
            print(result)
            -----------------------------------------
            Which outputs the following formulas:
            */
            const auto lL = levelLeft;
            const auto lR = levelRight;
            const auto sL = slopeLeft;
            const auto sR = slopeRight;
            const auto lLmlRSq = (lL - lR) * (lL - lR);

            result[i].a = (sL + sR - 2) / lLmlRSq;
            result[i].b = -(lL * sL + 2 * lL * sR - 3 * lL + 2 * lR * sL + lR * sR - 3 * lR) / lLmlRSq;
            result[i].c = (lL * lL * sR + 2 * lL * lR * sL + 2 * lL * lR * sR - 6 * lL * lR + lR * lR * sL) / lLmlRSq;
            result[i].d = -lL * lR * (lL * sR - lL + lR * sL - lR) / lLmlRSq;
        }

        return result;
    }
};

/**
 * Maps an input value of 0..1 to an output value of 0..1, creating
 * a number plateaus. This can be used to map a potentiometer value
 * so that it's easier to hit certain values.
 * 
 * At each plateau value, the slope of the mapping function will be
 * zero, so that the mapping function is level at the plateau.
 * It looks something like this:
 * 
 *    1 +                  /
 *      |                 /
 *      |           /----/
 *      |          /
 *      |   /-----/
 *      |  /
 *      | / 
 *      |/
 *    0 +------------------+-->
 *      0                  1
 * 
 * At each plateau, the output value is equal to the input value.
 * Between the plateaus, a qubic polynomial function creates a smooth
 * transition zone.
 * 
 * The PlateauMapParameters class is used to specify the level of the 
 * plateaus. It can be specified as constexpr so that all the 
 * preprocessing of the transition function parameters is done at compile 
 * time.
 * 
 *     // Create plateaus at 0.5 and 0.75
 *     // The values will be evaluated ad compile time.
 *     constexpr PlateauMapParameters<float, 2> parameters({ 0.5f, 0.75f });
 *     // this is how you map an input value (e.g. from a potentiometer)
 *     float result = plateauMap(inputValue, parameters);
 */
template <size_t numPlateaus, typename FloatType = float>
constexpr FloatType plateauMap(FloatType input, PlateauMapParameters<FloatType, numPlateaus> plateauParameters)
{
    if (input <= 0.0f)
        return 0.0f;
    if (input >= 1.0f)
        return 1.0f;

    size_t transitionIndex = 0;
    for (size_t i = 0; i < plateauParameters.numTransitions_; i++)
    {
        transitionIndex = i;
        if (input <= plateauParameters.plateauLevels_[i])
            break;
    }

    const auto& params = plateauParameters.transitionParameterSet_[transitionIndex];
    const auto x = input;
    const auto x2 = x * x;
    const auto x3 = x2 * x;
    const auto result = params.a * x3 + params.b * x2 + params.c * x + params.d;

    return result;
}
