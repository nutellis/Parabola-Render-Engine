#include <Components/Camera.h>
#include <Components/RenderActor.h>
#include <Utilities/CameraUtiltities.h>


PFrustrum::PFrustrum()
{
}

PFrustrum::~PFrustrum()
{
}

void PFrustrum::CalculateFrustrumCorners(PCameraComponent* Camera)
{
	float TanHalfVFOV = SMath::Tan(DegreesToRadians(FieldOfView) * 0.5f);

	float HNear = 2 * TanHalfVFOV * NearPlane;
	float HFar = 2 * TanHalfVFOV * FarPlane;

	float WNear = HNear * Ratio;
	float WFar = HFar * Ratio;

	Vector3f CenterNear = Camera->GetPosition() + (Camera->CameraDirection * NearPlane);
	Vector3f CenterFar = Camera->GetPosition() + (Camera->CameraDirection * FarPlane);

	Vector3f RightVector = Camera->Right * (WNear * 0.5f);
	Vector3f UpVector = Camera->Up * (HNear * 0.5f);

	// Near Plane
	Corners[0] = CenterNear + UpVector - RightVector;
	Corners[1] = CenterNear + UpVector + RightVector;
	Corners[2] = CenterNear - UpVector - RightVector;
	Corners[3] = CenterNear - UpVector + RightVector;

	RightVector = Camera->Right * (WFar * 0.5f);
	UpVector = Camera->Up * (HFar * 0.5f);

	// Far Plane
	Corners[4] = CenterFar + UpVector - RightVector;
	Corners[5] = CenterFar + UpVector + RightVector;
	Corners[6] = CenterFar - UpVector - RightVector;
	Corners[7] = CenterFar - UpVector + RightVector;


	Center = (CenterFar + CenterNear) * 0.5;

	DiagonalLength = (Corners[0] - Corners[7]).Length();

}


//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//									CAMERA COMPONENT
//----------------------------------------------------------------------------------------
PCameraComponent::PCameraComponent(PRenderActor* Parent, Vector3f up, float yaw, float pitch) : PSceneComponent(Parent), Front(Vector3f(0.0f, 0.0f, -1.0f)), Right(Vector3f(1.0f, 0.0f, 0.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{
	IsActiveCamera = true;

	Up = up;
	WorldUp = Up;

	Yaw = yaw;
	Pitch = pitch;

	MouseSensitivity = 0.2f;

	Zoom = 60.0f;

	CameraDirection = Front;

	Frustrum.FieldOfView = 45.0f;
	Frustrum.NearPlane = 0.5f;
	Frustrum.FarPlane = 1000.0f;
	Frustrum.Ratio = 16.0f / 9.0f;

	//RotateCamera(Parent->ObjectRotation.X, Parent->ObjectRotation.Y);

	//UpdateCameraVectors();

	Frustrum.CalculateFrustrumCorners(this);

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

void PCameraComponent::ControlCamera(uint32 Width, uint32 Height) {


	//TODO: do some checking ?
	Frustrum.Ratio = (float)Width / (float)Height;

	Projection = Perspective(Frustrum.FieldOfView, Frustrum.Ratio, Frustrum.NearPlane, Frustrum.FarPlane);

	View = LookAt(Parent->ObjectPosition, Parent->ObjectPosition + CameraDirection, WorldUp);
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------