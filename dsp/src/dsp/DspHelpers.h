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

#pragma once

#ifndef MANUAL_INLINE
#    define MANUAL_INLINE inline __attribute__((always_inline))
#endif

#include <sprout/math/exp.hpp>

template <typename IntType, int size>
class WrappedInt
{
public:
    template <typename OtherIntType>
    MANUAL_INLINE WrappedInt(OtherIntType initialValue = OtherIntType(0))
    {
        value_ = getWrapped(initialValue);
    }

    MANUAL_INLINE WrappedInt(const WrappedInt& other)
    {
        value_ = other.value_;
    }

    template <typename OtherIntType>
    MANUAL_INLINE operator OtherIntType() const
    {
        return OtherIntType(value_);
    }

    MANUAL_INLINE const WrappedInt& operator=(const WrappedInt& other)
    {
        value_ = other.value_;
        return *this;
    }

    template <typename OtherIntType>
    MANUAL_INLINE const WrappedInt& operator=(const OtherIntType rhs)
    {
        value_ = getWrapped(IntType(rhs));
        return *this;
    }

    template <typename OtherIntType>
    MANUAL_INLINE const WrappedInt& operator+=(const OtherIntType rhs)
    {
        value_ = getWrapped(IntType(OtherIntType(value_) + rhs));
        return *this;
    }

    template <typename OtherIntType>
    MANUAL_INLINE const WrappedInt& operator-=(const OtherIntType rhs)
    {
        value_ = getWrapped(OtherIntType(value_) - rhs);
        return *this;
    }

    MANUAL_INLINE const WrappedInt& operator++(int)
    {
        value_++;
        if (value_ >= size)
            value_ = 0;
        return *this;
    }

private:
    template <typename OtherIntType>
    MANUAL_INLINE IntType getWrapped(OtherIntType in)
    {
        while (in < 0)
            in += size;
        while (in >= size)
            in -= size;
        return in;
    }
    IntType value_;
};

template <typename IntType, int size, typename OtherIntType>
MANUAL_INLINE WrappedInt<IntType, size> operator+(OtherIntType lhs, const WrappedInt<IntType, size>& rhs)
{
    return WrappedInt<IntType, size>(lhs + OtherIntType(rhs));
}

template <typename IntType, int size, typename OtherIntType>
MANUAL_INLINE WrappedInt<IntType, size> operator+(const WrappedInt<IntType, size>& lhs, OtherIntType rhs)
{
    return WrappedInt<IntType, size>(OtherIntType(lhs) + rhs);
}

template <typename IntType, int size, typename OtherIntType>
MANUAL_INLINE WrappedInt<IntType, size> operator-(OtherIntType lhs, const WrappedInt<IntType, size>& rhs)
{
    return WrappedInt<IntType, size>(lhs - OtherIntType(rhs));
}

template <typename IntType, int size, typename OtherIntType>
MANUAL_INLINE WrappedInt<IntType, size> operator-(const WrappedInt<IntType, size>& lhs, OtherIntType rhs)
{
    return WrappedInt<IntType, size>(OtherIntType(lhs) - rhs);
}

template <typename IntType>
IntType getFlooredPositiveInt(float input)
{
    // TODO: make this fast
    return (IntType) input;
}

class ExponentialSmoother
{
public:
    class TimeConstant
    {
    public:
        constexpr TimeConstant(float smoothingTimeInS, float sampleRate, int blockSize) :
            value_(1.0f - sprout::math::exp(-2.2f * float(blockSize) / sampleRate / smoothingTimeInS))
        {
        }

        static constexpr TimeConstant fromRawValue(float value)
        {
            return TimeConstant(value);
        }

        static constexpr TimeConstant instantaneous()
        {
            return TimeConstant(1.0f);
        }

        float value_;

    private:
        constexpr TimeConstant(float rawValue) :
            value_(rawValue)
        {
        }
    };

    ExponentialSmoother() :
        lastVal_(0.0f)
    {
    }

    MANUAL_INLINE float smooth(float targetVal, const TimeConstant& timeConstant)
    {
        lastVal_ = (targetVal - lastVal_) * timeConstant.value_ + lastVal_;
        return lastVal_;
    }

    void reset(float valueToResetTo = 0.0f)
    {
        lastVal_ = valueToResetTo;
    }

private:
    float lastVal_;
};

template <typename Type>
MANUAL_INLINE constexpr Type linMap(Type relativeValue, Type min, Type max)
{
    return min + relativeValue * (max - min);
}

template <typename Type>
MANUAL_INLINE constexpr Type limit(Type value, Type min, Type max)
{
    return value < min ? min : (max < value ? max : value);
}
