#pragma once

template<typename T>
class SharedPointer
{
public:
    SharedPointer(T* pointer)
        : referenceCounter(new int),
          pointer(pointer)
    {
        *referenceCounter = 1;
        PrintCount();
    }

    SharedPointer(const SharedPointer& other)
    {
        pointer = other.pointer;
        referenceCounter = other.referenceCounter;
        *referenceCounter += 1;
        PrintCount();
    }
    SharedPointer& operator =(const SharedPointer& other)
    {
        if(this != &other )
        {
            pointer = other.pointer;
            referenceCounter = other.referenceCounter;
            *referenceCounter += 1;
        }
        PrintCount();
        return *this;
    }

    SharedPointer(SharedPointer&& other) noexcept
    {
        pointer = other.pointer;
        referenceCounter = other.referenceCounter;
        other.pointer = nullptr;
        other.referenceCounter = nullptr;
        PrintCount();
        return *this;
    }

    SharedPointer& operator =(SharedPointer&& other) noexcept
    {
        pointer = other.pointer;
        referenceCounter = other.referenceCounter;
        other.pointer = nullptr;
        other.referenceCounter = nullptr;
        PrintCount();
        return *this;
    }

    ~SharedPointer()
    {
        int& references = *referenceCounter;
        references--;
        PrintCount();
        if(references <= 0)
        {
            delete pointer;
            delete referenceCounter;
        }
    }

    T* operator ->()
    {
        return pointer;
    }

    void PrintCount()
    {
        std::cout << "References: " << *referenceCounter << std::endl;
    }
private:
    int* referenceCounter;
    T* pointer;
};
