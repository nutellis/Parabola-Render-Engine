

#include <Components/RenderTarget.h>



RenderTarget::RenderTarget()
{}

RenderTarget::RenderTarget(std::string Name, uint32 inWidth, uint32 inHeight) : Name(Name), Width(inWidth), Height(inHeight)
{
}



RenderTarget::~RenderTarget()
{}
