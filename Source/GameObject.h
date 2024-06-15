#pragma once

#include <memory>
#include <vector>
#include "Component/Component.h"
#include "Math/Vector.h"
#include <iostream>

class GameObject
{
public:
	GameObject();

	void Update();
	void Draw();

	//Implemented in header due to templated arguments
	template<typename T, typename... Args>
	T* AddComponent(Args&&... args)
	{
		static_assert(std::is_base_of<Component, T>::value, "T has to inherit from Component.");
		//Forward ensures that each argument retains whether it's r-value or l-value
		std::unique_ptr<T> newComponent = std::make_unique<T>(this, std::forward<Args>(args)...);
		T* componentPointer = newComponent.get();
		components.push_back(std::move(newComponent));
		return componentPointer;
	}

	Vec2f Position;
private:
	std::vector<std::unique_ptr<Component>> components;
};