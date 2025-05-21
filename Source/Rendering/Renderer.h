#pragma once
#include <vector>

#include "Primitive.h"
#include "../Math/Vector.h"
#include "Windows.h"

struct Color;

class Renderer
{
public:
    Renderer(const HDC& hdc);

    Renderer(const Renderer& other) = delete;
    Renderer& operator=(const Renderer& other) = delete;
    
    Renderer(Renderer&& other) noexcept = default;
    Renderer& operator=(Renderer&& other) noexcept = default;

    virtual ~Renderer() = default;

    template<typename T, typename... Args>
    std::shared_ptr<T> AddPrimitive(Args&&... args)
    {
        static_assert(std::is_base_of<Primitive, T>::value, "T has to inherit from Primitive.");
        std::shared_ptr<T> newPrimitive = std::make_shared<T>(std::forward<Args>(args)...);
        primitives.push_back(newPrimitive);
        SetupPrimitive(newPrimitive);
        return newPrimitive;
    }

    virtual void Render(const Color& clearColor) = 0;

protected:
    virtual void SetupPrimitive(std::shared_ptr<Primitive> primitive) = 0;

    //The renderer doesn't know or care when primitives are deleted.
    //It can check if a primitive is valid and remove it if it's not.
    //Hence this is a vector of weak pointers
    std::vector<std::weak_ptr<Primitive>> primitives; 
    HDC deviceContext;
};
