#include "GameObject.h"

void GameObject::Update()
{
	for(int i = components.size() - 1; i >= 0; i--)
	{
		components[i]->Update();
	}
}

void GameObject::Draw()
{
	for (int i = components.size() - 1; i >= 0; i--)
	{
		components[i]->Draw();
	}
}