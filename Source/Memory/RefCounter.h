#pragma once

struct RefCounter
{
    RefCounter()
    : sharedCounter(new size_t(0)),
      weakCounter(new size_t(0))
    {}


    void IncreaseSharedCounter()
    {
        *sharedCounter += 1;
    }

    void DecreaseSharedCounter()
    {
        *sharedCounter -= 1;
    }

    size_t& GetSharedCounter()
    {
        return *sharedCounter;
    }

    void IncreaseWeakCounter()
    {
        *weakCounter += 1;
    }

    void DecreaseWeakCounter()
    {
        *weakCounter -= 1;
        if(*weakCounter == 0)
            delete this;
    }

    size_t& GetWeakCounter()
    {
        return *weakCounter;
    }

    size_t* sharedCounter;
    size_t* weakCounter;
};
