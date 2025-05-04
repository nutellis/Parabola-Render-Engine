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

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//									CAMERA COMPONENT
//----------------------------------------------------------------------------------------
PCameraComponent::PCameraComponent(PRenderActor * Parent, Vector3f up, float yaw, float pitch) : PSceneComponent(Parent), Front(Vector3f(0.0f, 0.0f, -1.0f)), Right(Vector3f(1.0f, 0.0f, 0.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{
	IsActiveCamera = true;

	Up = up;
	WorldUp = Up;

	Yaw = yaw;
	Pitch = pitch;

	MouseSensitivity = 0.2f;

	Zoom = 60.0f;

	CameraDirection = Front;

	//RotateCamera(Parent->ObjectRotation.X, Parent->ObjectRotation.Y);

	//UpdateCameraVectors();

}

PCameraComponent::PCameraComponent(float upX, float upY, float upZ, float yaw, float pitch)
	: Front(Vector3f(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{

	IsActiveCamera = true;

	Up = Vector3f(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	// UpdateCameraVectors();
}

void PCameraComponent::SetProjection(ProjectionType Type)
{
	if (Type == ORTHOGRAPHIC)
	{
		//Projection = Ortho();
	}
	else if (Type == PERSPECTIVE)
	{
		//Projection == Perspective();
	}
}


Matrix4f PCameraComponent::GetViewMatrix() const
{
	return View;
}

Matrix4f PCameraComponent::GetProjectionMatrix() const
{
	return Projection;
}

void PCameraComponent::UpdateCameraVectors()
{
	Front.X = SMath::Cos(DegreesToRadians(Yaw)) * SMath::Cos(DegreesToRadians(Pitch));
	Front.Y = SMath::Sin(DegreesToRadians(Pitch));
	Front.Z = SMath::Sin(DegreesToRadians(Yaw)) * SMath::Cos(DegreesToRadians(Pitch));
	Front = Normalize(Front);
	Right = Normalize((Front.Cross(WorldUp)));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = Normalize((Right.Cross(Front)));
}

void PCameraComponent::ProcessKeyboard(CameraMovement direction, float deltaTime)
{


	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		Parent->ObjectPosition += (CameraDirection * velocity);
	if (direction == BACKWARD)
		Parent->ObjectPosition -= (CameraDirection * velocity);
	if (direction == LEFT)
		Parent->ObjectPosition -= (CameraDirection.Cross(WorldUp) * velocity);
	if (direction == RIGHT)
		Parent->ObjectPosition += (CameraDirection.Cross(WorldUp) * velocity);
	if (direction == UP)
		Parent->ObjectPosition += (WorldUp * velocity);
	if (direction == DOWN)
		Parent->ObjectPosition -= (WorldUp * velocity);

	
}

void PCameraComponent::RotateCamera(float xoffset, float yoffset) {

	Matrix4f yaw = Rotate(WorldUp, xoffset, Matrix4f::IDENTITY);

	Matrix4f pitch = Rotate(Normalize(CameraDirection.Cross(WorldUp)), yoffset, Matrix4f::IDENTITY);

	CameraDirection = Vector3f((pitch * yaw) * Vector4f(CameraDirection, 0.0f));

}

void PCameraComponent::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	float rotationSpeed = 0.4f;
	xoffset = rotationSpeed * 0.3f * -xoffset;
	yoffset = rotationSpeed * 0.3f * -yoffset;

	//Parent->AddRotation(Vector3f(yoffset, xoffset, 0.0f));

	//RotateCamera(xoffset,yoffset);
}

void PCameraComponent::ProcessMouseScroll(float yoffset)
{
	Zoom -= (float)yoffset;
	if (Zoom < 1.0f)
		Zoom = 1.0f;
	if (Zoom > 45.0f)
		Zoom = 45.0f;
}

void PCameraComponent::SetupShaderCamera(Shader* ActiveShader) {

}

void PCameraComponent::SetDefaults()
{
	const float YAW = 90.0f;
	const float PITCH = 0.0f;
	const float SPEED = 2.5f;
	const float SENSITIVTY = 0.2f;
	const float FOV = 60.0f;
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------