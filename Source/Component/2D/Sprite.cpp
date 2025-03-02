#include "Sprite.h"
#include "../../GameObject.h"
#include "../../2D/Camera2D.h"
#include "../../Rendering/Window.h"

const unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};

Sprite::Sprite(GameObject* owner, const std::string& textureLocation)
: Component(owner),
  primitive(textureLocation)
{
}

void Sprite::Draw(RenderTarget& target)
{
	primitive.Draw(GetOwner()->Position);
}


std::unique_ptr<Shader> Sprite::SpriteShader = nullptr;