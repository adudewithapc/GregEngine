#include "TriangleRenderer.h"

#include "../../GameObject.h"
#include "../../GregorianEngine.h"

namespace greg
{
TriangleRenderer::TriangleRenderer(GameObject* owner)
: Component(owner),
  primitive(GregorianEngine::Get().GetRenderTarget().GetRenderer().AddPrimitive<TrianglePrimitive>())
{
    
}

void TriangleRenderer::Update()
{
  primitive->Position = GetOwner()->Position;
}
}
