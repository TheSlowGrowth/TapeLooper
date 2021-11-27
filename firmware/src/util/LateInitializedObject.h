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

template <typename T>
class LateInitializedObject
{
public:
    LateInitializedObject() :
        object_(nullptr)
    {
    }

    template <typename... Args>
    void create(Args&&... args)
    {
        object_ = new (::std::addressof(storage_)) T(std::forward<Args>(args)...);
    }

    operator T*() { return object_; }
    operator const T*() const { return object_; }
    T* operator->() { return object_; }
    const T* operator->() const { return object_; }

private:
    typename std::aligned_storage<sizeof(T)>::type storage_;
    T* object_;
};