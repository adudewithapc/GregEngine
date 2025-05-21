#include "Sprite.h"
#include "../../GameObject.h"
#include "../../GregorianEngine.h"
#include "../../Rendering/Window.h"

Sprite::Sprite(GameObject* owner, const std::string& textureLocation)
: Component(owner),
  primitive(GregorianEngine::Get().GetRenderTarget().GetRenderer().AddPrimitive<SpritePrimitive>(textureLocation))
{
}

void Sprite::Update()
{
  primitive->Position = GetOwner()->Position;
}

