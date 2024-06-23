#pragma once

#include <memory>
#include <vector>
#include "Component/Component.h"
#include "Math/Vector.h"

class GameObject
{
public:
	GameObject(int index);

	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;

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

	void Destroy();
	const bool IsDestroyed() const;
	void SetIndex(int newIndex);

	Vec2f Position;
private:
	std::vector<std::unique_ptr<Component>> components;
	int index;
	bool isDestroyed = false;
};