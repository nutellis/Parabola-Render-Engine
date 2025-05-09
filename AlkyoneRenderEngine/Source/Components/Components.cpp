#include <Components/Components.h>

#include <ParabolaMath.h>
#include <Components/RenderActor.h>
#include <Managers/ShaderManager.h>
#include <Components/Shader.h>

#include <Core/Utilities.h>

//-------------------------------------------------
/*ta components na uio8etoun to RelativeLocation tou root*/
//-------------------------------------------------

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

PSceneComponent::PSceneComponent()
{
}

void PSceneComponent::SetAbsolute(bool bNewAbsoluteLocation, bool bNewAbsoluteRotation, bool bNewAbsoluteScale)
{
	bAbsoluteLocation = bNewAbsoluteLocation;
	bAbsoluteRotation = bNewAbsoluteRotation;
	bAbsoluteScale = bNewAbsoluteScale;
}

Vector3f PSceneComponent::GetPosition() const
{
	return Parent->ObjectPosition;
}

Vector3f PSceneComponent::GetRotation() const
{
	return Parent->ObjectRotation;
}

Vector3f PSceneComponent::GetScale() const
{
	return Parent->ObjectScale;
}
