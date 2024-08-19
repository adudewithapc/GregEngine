#pragma once

template<typename T>
class UniquePointer
{
public:
    UniquePointer(T* pointer)
        : pointer(pointer)
    {}

    UniquePointer(UniquePointer&& other) noexcept
    {
        pointer = other.pointer;
        other.pointer = nullptr;
    }
    UniquePointer& operator =(UniquePointer&& other) noexcept
    {
        pointer = other.pointer;
        other.pointer = nullptr;
        return *this;
    }

    ~UniquePointer()
    {
        delete pointer;
    }

    T* operator ->()
    {
        return pointer;
    }

    T* Get()
    {
        return pointer;
    }

    UniquePointer(const UniquePointer&) = delete;
    UniquePointer operator =(const UniquePointer&) = delete;
private:
    T* pointer;
};
