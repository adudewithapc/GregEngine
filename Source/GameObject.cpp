#include "GameObject.h"
#include "GregorianEngine.h"

GameObject::GameObject(int index) : index(index), isDestroyed(false)
{
}

void GameObject::Update()
{
	for(int i = components.size() - 1; i >= 0 && !isDestroyed; i--)
	{
		components[i]->Update();
	}
}

void GameObject::Draw(RenderTarget& target)
{
	for (int i = components.size() - 1; i >= 0; i--)
	{
		components[i]->Draw(target);
	}
}

void GameObject::CollisionEntered(GameObject* other)
{
	for (int i = components.size() - 1; i >= 0 && !isDestroyed; i--)
	{
		components[i]->CollisionEntered(other);
	}
}

void GameObject::CollisionExited(GameObject* other)
{
	for (int i = components.size() - 1; i >= 0 && !isDestroyed; i--)
	{
		components[i]->CollisionExited(other);
	}
}

void GameObject::Destroy()
{
	GregorianEngine::Get().GetCurrentLevel()->DestroyObject(index);
	isDestroyed = true;
}

const bool GameObject::IsDestroyed() const
{
	return isDestroyed;
}

void GameObject::SetIndex(int newIndex)
{
	index = newIndex;
}
