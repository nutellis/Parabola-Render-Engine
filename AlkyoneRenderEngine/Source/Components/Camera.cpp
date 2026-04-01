#include <Components/CameraComponents/Camera.h>
#include <Components/RenderActor.h>
#include <Utilities/CameraUtiltities.h>

#include <Components/Colliders/BoundingBox.h>
#include <Components/Colliders/AxisAlignedBoundingBox.h>
#include <Components/Colliders/BoundingHelper.h>



PFrustrum::PFrustrum()
{
	FrustrumBox = new PBoundingBox();
	BoundingBox = new PAxisAlignedBoundingBox();
	NearPlane = 0.0f;
	FarPlane = 0.0f;
	Ratio = 0.0f;
	FieldOfView = 0.0f;

}

PFrustrum::~PFrustrum()
{
	delete FrustrumBox;
	delete BoundingBox;
}

void PFrustrum::CalculateFrustrumCorners(PCameraComponent* Camera, float Offset)
{
	float TanHalfVFOV = SMath::Tan(DegreesToRadians(FieldOfView) * 0.5f);

	float HNear = (2 * TanHalfVFOV * NearPlane) + Offset;
	float HFar = (2 * TanHalfVFOV * FarPlane) + Offset;

	float WNear = HNear * Ratio;
	float WFar = HFar * Ratio;

	Vector3f NearCenter = Camera->GetPosition() + (Camera->Front * NearPlane);
	Vector3f FarCenter = Camera->GetPosition() + (Camera->Front * FarPlane);

	Vector3f RightVector = Camera->Right * (WNear * 0.5f);
	Vector3f UpVector = Camera->Up * (HNear * 0.5f);

	TArray<Vector3f> FrustrumCorners = TArray<Vector3f>(8);
	// Near Plane
	FrustrumCorners.PushBack(NearCenter + UpVector - RightVector); //top left
	FrustrumCorners.PushBack(NearCenter + UpVector + RightVector); //top right
	FrustrumCorners.PushBack(NearCenter - UpVector - RightVector); //bottom left
	FrustrumCorners.PushBack(NearCenter - UpVector + RightVector); //bottom right

	RightVector = Camera->Right * (WFar * 0.5f);
	UpVector = Camera->Up * (HFar * 0.5f);

	// Far Plane
	FrustrumCorners.PushBack(FarCenter + UpVector - RightVector); //top left
	FrustrumCorners.PushBack(FarCenter + UpVector + RightVector); //top right
	FrustrumCorners.PushBack(FarCenter - UpVector - RightVector); //bottom left
	FrustrumCorners.PushBack(FarCenter - UpVector + RightVector); //bottom right

	FrustrumBox->CalculateFromCorners(FrustrumCorners);
	BoundingBox->CalculateFromCorners(FrustrumCorners);
}

void PFrustrum::CalculateFrustrumPlanes(PCameraComponent* Camera)
{
	auto MakePlane = [](const Vector3f& A, const Vector3f& B, const Vector3f& C)
	{
		PPlane NewPlane;
		Vector3f Normal = Normalize(Cross(B - A, C - A));
		NewPlane.Normal = Normal;
		NewPlane.D = -Dot(Normal, A);

		return NewPlane;
	};
	Planes.Clear();
	// 1. Left Plane
	Planes.PushBack(MakePlane(FrustrumBox->GetCorner(NEAR_LEFT_BOTTOM), FrustrumBox->GetCorner(FAR_LEFT_TOP), FrustrumBox->GetCorner(NEAR_LEFT_TOP)));
	// 2. Right Plane
	Planes.PushBack(MakePlane(FrustrumBox->GetCorner(NEAR_RIGHT_TOP), FrustrumBox->GetCorner(FAR_RIGHT_BOTTOM), FrustrumBox->GetCorner(NEAR_RIGHT_BOTTOM)));
	// 3. Top Plane
	Planes.PushBack(MakePlane(FrustrumBox->GetCorner(NEAR_LEFT_TOP), FrustrumBox->GetCorner(FAR_RIGHT_TOP), FrustrumBox->GetCorner(NEAR_RIGHT_TOP)));
	// 4. Bottom Plane
	Planes.PushBack(MakePlane(FrustrumBox->GetCorner(NEAR_RIGHT_BOTTOM), FrustrumBox->GetCorner(FAR_LEFT_BOTTOM), FrustrumBox->GetCorner(NEAR_LEFT_BOTTOM)));
	// 5. Near Plane 
	Planes.PushBack(MakePlane(FrustrumBox->GetCorner(NEAR_LEFT_BOTTOM), FrustrumBox->GetCorner(NEAR_RIGHT_TOP), FrustrumBox->GetCorner(NEAR_RIGHT_BOTTOM)));
	// 6. Far Plane
	Planes.PushBack(MakePlane(FrustrumBox->GetCorner(FAR_RIGHT_BOTTOM), FrustrumBox->GetCorner(FAR_LEFT_TOP), FrustrumBox->GetCorner(FAR_LEFT_BOTTOM)));
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//									CAMERA COMPONENT
//----------------------------------------------------------------------------------------
PCameraComponent::PCameraComponent(RRenderActor* Parent, Vector3f up, float yaw, float pitch) : PSceneComponent(Parent), Front(Vector3f(0.0f, 0.0f, -1.0f)), Right(Vector3f(1.0f, 0.0f, 0.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{
	Up = up;
	WorldUp = Up;

	Yaw = yaw;
	Pitch = pitch;

	MouseSensitivity = 0.5f;
	MovementSpeed = 1.0f;
	Zoom = 60.0f;

	Frustrum = new PFrustrum();
	Frustrum->FieldOfView = 60.0f;
	Frustrum->NearPlane = 0.5f;
	Frustrum->FarPlane = 1000.0f;
	Frustrum->Ratio = 16.0f / 9.0f;

	//RotateCamera(Parent->ObjectRotation.X, Parent->ObjectRotation.Y);

	//UpdateCameraVectors();

	//Frustrum.CalculateFrustrumCorners(this);

}

PCameraComponent::PCameraComponent(float upX, float upY, float upZ, float yaw, float pitch)
	: Front(Vector3f(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{

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
	float Yaw = DegreesToRadians(Parent->ObjectRotation.Y - 90.0f);
	float Pitch = DegreesToRadians(Parent->ObjectRotation.X);

	Front.X = SMath::Cos(Pitch) * SMath::Cos(Yaw);
	Front.Y = SMath::Sin(Pitch);
	Front.Z = SMath::Sin(Yaw) * SMath::Cos(Pitch);

	Front = Normalize(Front);
	Right = Normalize(Cross(Front,WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = Normalize(Cross(Right,Front));
}

void PCameraComponent::ProcessKeyboard(CameraMovement direction, float deltaTime)
{

	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		Parent->ObjectPosition += (Front * velocity);
	if (direction == BACKWARD)
		Parent->ObjectPosition -= (Front * velocity);
	if (direction == LEFT)
		Parent->ObjectPosition -= (Front.Cross(WorldUp) * velocity);
	if (direction == RIGHT)
		Parent->ObjectPosition += (Front.Cross(WorldUp) * velocity);
	if (direction == UP)
		Parent->ObjectPosition += (WorldUp * velocity);
	if (direction == DOWN)
		Parent->ObjectPosition -= (WorldUp * velocity);


}

void PCameraComponent::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	float rotationSpeed = 0.4f;
	xoffset = rotationSpeed * 0.3f * xoffset;
	yoffset = rotationSpeed * 0.3f * -yoffset;

	Parent->ObjectRotation.Y += xoffset;
	Parent->ObjectRotation.X += yoffset;

	const float pitchLimit = 89.0f;
	if (constrainPitch) {
		if (Parent->ObjectRotation.X > pitchLimit) Parent->ObjectRotation.X = pitchLimit;
		if (Parent->ObjectRotation.X < -pitchLimit) Parent->ObjectRotation.X = -pitchLimit;
	}
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

void PCameraComponent::AdjustPlanesBasedOnObjects(TArray<PAxisAlignedBoundingBox> Objects)
{
	if (Objects.IsEmpty()) {
		return;
	}

	// get the union of all objects
	PAxisAlignedBoundingBox Union = BoundingHelper::UnionAABB(Objects);

//	Union->RenderDebugBoundingBox(1, Vector4f(1.0, 0.0f, 0.0f, 0.1f), RCamera->Projection, RCamera->View);

	float NewNearPlane = FLT_MAX;
	float NewFarPlane = -FLT_MAX;

	for (int i = 0; i < 8; ++i) {

		Vector3f PointToCam = Union.Corners[i] - GetPosition();
		float Z = Dot(PointToCam, this->Front);

		// find boundary values
		if (Z > NewFarPlane) NewFarPlane = Z;
		if (Z < NewNearPlane) NewNearPlane = Z;
	}

	NewNearPlane = SMath::Max(NewNearPlane, ZNearMin);
	NewFarPlane = SMath::Max(NewFarPlane, ZNearMin + 1.0f);

	Frustrum->NearPlane = NewNearPlane;
	Frustrum->FarPlane = NewFarPlane;

	UpdateCamera();
}

void PCameraComponent::UpdateCamera(uint32 Width, uint32 Height) {

	if (Width != 0 && Height != 0) {
		Frustrum->Ratio = (float)Width / (float)Height;
	}
	
	UpdateCameraVectors();

	Frustrum->CalculateFrustrumCorners(this);
	Frustrum->CalculateFrustrumPlanes(this);

	Projection = Perspective(Frustrum->FieldOfView, Frustrum->Ratio, Frustrum->NearPlane, Frustrum->FarPlane);

	View = LookAt(Parent->ObjectPosition, Parent->ObjectPosition + Front, WorldUp);

}

void PCameraComponent::SetDefaults()
{
	const float YAW = 0.0f;
	const float PITCH = 0.0f;
	const float SPEED = 2.5f;
	const float SENSITIVTY = 0.2f;
	const float FOV = 60.0f;
}



//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

PCameraActor::PCameraActor(std::string NodeName) : RRenderActor(NodeName)
{
	ActorType = EntityType::CAMERA;
	Camera = new PCameraComponent(this);

}

void PCameraActor::InitCamera()
{
	ControlCamera(1920, 1080);
}

void PCameraActor::ControlCamera(uint32 Width, uint32 Height) {
	Camera->UpdateCamera(Width, Height);
}