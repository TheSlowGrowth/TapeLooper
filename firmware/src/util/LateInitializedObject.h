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
#include <stdint.h>
#include <cstddef>
#include <utility>

template <class... Types>
class LateInitializedObject
{
    template <typename T, typename... AllTypes>
    struct isOneOf : public std::false_type
    {
    };
    template <typename T, typename FirstType, typename... RestOfTypes>
    struct isOneOf<T, FirstType, RestOfTypes...> :
        public std::conditional<std::is_same<T, FirstType>::value,
                                std::true_type,
                                isOneOf<T, RestOfTypes...>>::type
    {
    };

    template <typename... Ts>
    using FirstTypeOf =
        typename std::tuple_element<0, std::tuple<Ts...>>::type;

public:
    LateInitializedObject() :
        object_(nullptr)
    {
    }

    template <class T = FirstTypeOf<Types...>,
              typename... Args,
              typename std::enable_if<isOneOf<T, Types...>::value>::type* = nullptr>
    T* create(Args&&... args)
    {
        object_ = new (::std::addressof(storage_)) T(std::forward<Args>(args)...);
        return reinterpret_cast<T*>(object_);
    }

    template <class T, typename std::enable_if<isOneOf<T, Types...>::value>::type* = nullptr>
    void destroy()
    {
        if (object_)
        {
            // manually call destructor - no delete since we used placement new()
            reinterpret_cast<T*>(object_)->~T();
            object_ = nullptr;
        }
    }

    template <class T, typename std::enable_if<isOneOf<T, Types...>::value>::type* = nullptr>
    T& as()
    {
        return *reinterpret_cast<T*>(object_);
    }

    template <class T, typename std::enable_if<isOneOf<T, Types...>::value>::type* = nullptr>
    const T& as() const
    {
        return *reinterpret_cast<T*>(object_);
    }

    /** operator->() for the first type in `Types` */
    FirstTypeOf<Types...>* operator->()
    {
        return reinterpret_cast<FirstTypeOf<Types...>*>(object_);
    }
    const FirstTypeOf<Types...>* operator->() const
    {
        return reinterpret_cast<FirstTypeOf<Types...>*>(object_);
    }

    /** conversion operator for the first type in `Types` */
    operator FirstTypeOf<Types...>&()
    {
        return *reinterpret_cast<FirstTypeOf<Types...>*>(object_);
    }
    operator const FirstTypeOf<Types...>&() const
    {
        return *reinterpret_cast<FirstTypeOf<Types...>*>(object_);
    }

private:
    typename std::aligned_union<0, Types...>::type storage_;
    void* object_;
};