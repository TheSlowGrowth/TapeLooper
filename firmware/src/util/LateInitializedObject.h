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