#pragma once
#include "SharedPointer.h"

template<typename T>
class WeakPointer
{
public:
    WeakPointer(const SharedPointer<T>& sharedPointer)
        : pointer(sharedPointer.pointer),
          referenceCounter(sharedPointer.referenceCounter)
    {}

    WeakPointer(const WeakPointer& other)
    {
        pointer = other.pointer;
        referenceCounter = other.referenceCounter;
        referenceCounter->IncreaseWeakCounter();
    }
    WeakPointer& operator =(const WeakPointer other)
    {
        pointer = other.pointer;
        referenceCounter = other.referenceCounter;
        referenceCounter->IncreaseSharedCounter();
        return *this;
    }

    WeakPointer(WeakPointer&& other) noexcept
    {
        pointer = other.pointer;
        referenceCounter = other.referenceCounter;

        other.pointer = nullptr;
        other.referenceCounter = nullptr;
    }
    WeakPointer& operator =(WeakPointer&& other) noexcept
    {
        pointer = other.pointer;
        referenceCounter = other.referenceCounter;

        other.pointer = nullptr;
        other.referenceCounter = nullptr;

        return *this;
    }

    ~WeakPointer()
    {
        referenceCounter->DecreaseWeakCounter();
        if(*referenceCounter->weakCounter < 0)
            referenceCounter->
    }
    
    bool IsValid() const
    {
        return *referenceCounter > 0;
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
    T* pointer;
    RefCounter* referenceCounter;
};
