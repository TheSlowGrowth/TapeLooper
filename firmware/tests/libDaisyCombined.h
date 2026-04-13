#pragma once

#include <cstdint>

// substitutes for the stuff provided by the daisy library
#define FBIPMAX 0.999985f /**< close to 1.0f-LSB at 16 bit */
#define FBIPMIN (-FBIPMAX) /**< - (1 - LSB) */
#define F2S32_SCALE 2147483647.f /**< (2 ** 31) - 1 */
#define S322F_SCALE 4.6566129e-10f /**< 1 / (2** 31) */
#define S242F_SCALE 1.192092896e-07f /**< 1 / (2 ** 23) */
#define S24SIGN 0x800000 /**< 2 ** 23 */
#define S162F_SCALE 3.0517578125e-05f /**< 1 / (2** 15) */

inline int32_t f2s32(float x)
{
    x = x <= FBIPMIN ? FBIPMIN : x;
    x = x >= FBIPMAX ? FBIPMAX : x;
    return (int32_t) (x * F2S32_SCALE);
}
inline float s322f(int32_t x)
{
    return (float) x * S322F_SCALE;
}
inline float s242f(int32_t x)
{
    x = (x ^ S24SIGN) - S24SIGN; // sign extend aka ((x<<8)>>8)
    return (float) x * S242F_SCALE;
}
inline float s162f(int16_t x)
{
    return (float) x * S162F_SCALE;
}