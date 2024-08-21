#pragma once
#include "RefCounter.h"

template<typename T>
class SharedPointer
{
public:
    SharedPointer(T* pointer)
        : referenceCounter(new RefCounter),
          pointer(pointer)
    {
    }

    SharedPointer(const SharedPointer& other)
    {
        pointer = other.pointer;
        referenceCounter = other.referenceCounter;
        referenceCounter->IncreaseSharedCounter();
    }
    SharedPointer& operator =(const SharedPointer& other)
    {
        if(this != &other )
        {
            pointer = other.pointer;
            referenceCounter = other.referenceCounter;
            referenceCounter->IncreaseSharedCounter();
        }
        return *this;
    }

    SharedPointer(SharedPointer&& other) noexcept
    {
        pointer = other.pointer;
        referenceCounter = other.referenceCounter;
        other.pointer = nullptr;
        other.referenceCounter = nullptr;
        return *this;
    }

    SharedPointer& operator =(SharedPointer&& other) noexcept
    {
        pointer = other.pointer;
        referenceCounter = other.referenceCounter;
        other.pointer = nullptr;
        other.referenceCounter = nullptr;
        return *this;
    }

    ~SharedPointer()
    {
        referenceCounter->DecreaseWeakCounter();
        if(referenceCounter->GetWeakCounter() == 0)
        {
            delete referenceCounter;
            delete pointer; 
        }
    }

    size_t GetReferenceCount() const
    {
        return referenceCounter->GetSharedCounter();
    }

    T* operator ->()
    {
        return pointer;
    }

    T* Get() const
    {
        return pointer;
    }

private:
    RefCounter* referenceCounter;
    T* pointer;
};
