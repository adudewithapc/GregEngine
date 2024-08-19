
#include <iostream>

#include "../../Source/Memory/UniquePointer.h"
#include <memory>

#include "../../Source/Memory/SharedPointer.h"

class Entity
{
public:
    Entity()
    {
        std::cout << "Constructed" << "\n";
    }

    ~Entity()
    {
        std::cout << "Destroyed" << "\n";
    }

    void Print()
    {
        std::cout << "Woah " << x << "\n";
    }

    float x, y, z;
};

int main(int argc, char* argv[])
{
    SharedPointer<Entity> gameObject(new Entity);
    {
        gameObject->x = 3;
        gameObject->Print();
        SharedPointer<Entity> gameObject2 = gameObject;
    }
    return 0;
}
