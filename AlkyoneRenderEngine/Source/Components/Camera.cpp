#include <Components/CameraComponents/Camera.h>
#include <Components/RenderActor.h>
#include <Utilities/CameraUtiltities.h>
#include <Components/RenderComponents/StaticMeshComponent.h>
#include <Managers/ShaderManager.h>
#include <Components/Shader.h>


PFrustrum::PFrustrum()
{
	Corners = TArray<Vector3f>(8);
	Center = Vector3f::ZERO;
	Min = Vector3f::ZERO;
	Max = Vector3f::ZERO;
	Extents = Vector3f::ZERO;
	DiagonalLength = 0.0f;
	NearPlane = 0.0f;
	FarPlane = 0.0f;
	Ratio = 0.0f;
	FieldOfView = 0.0f;

	DebugFrustrumMesh = new PStaticMeshComponent(nullptr);
	if (DebugFrustrumMesh != nullptr) {
		DebugFrustrumMesh->Vertices = TArray<VertexFormat>(24);
	}

}

PFrustrum::~PFrustrum()
{
	if (DebugFrustrumMesh != nullptr) {
		delete DebugFrustrumMesh;
		DebugFrustrumMesh = nullptr;
	}
	Corners.Destroy();
}

void PFrustrum::CalculateFrustrumCorners(PCameraComponent* Camera)
{
	float TanHalfVFOV = SMath::Tan(DegreesToRadians(FieldOfView) * 0.5f);

	float HNear = 2 * TanHalfVFOV * NearPlane;
	float HFar = 2 * TanHalfVFOV * FarPlane;

	float WNear = HNear * Ratio;
	float WFar = HFar * Ratio;

	Vector3f NearCenter = Camera->GetPosition() + (Camera->Front * NearPlane);
	Vector3f FarCenter = Camera->GetPosition() + (Camera->Front * FarPlane);

	Vector3f RightVector = Camera->Right * (WNear * 0.5f);
	Vector3f UpVector = Camera->Up * (HNear * 0.5f);

	// Near Plane
	Corners.Clear();
	Corners.PushBack(NearCenter + UpVector - RightVector); //top left
	Corners.PushBack(NearCenter + UpVector + RightVector); //top right
	Corners.PushBack(NearCenter - UpVector - RightVector); //bottom left
	Corners.PushBack(NearCenter - UpVector + RightVector); //bottom right

	RightVector = Camera->Right * (WFar * 0.5f);
	UpVector = Camera->Up * (HFar * 0.5f);

	// Far Plane
	Corners.PushBack(FarCenter + UpVector - RightVector); //top left
	Corners.PushBack(FarCenter + UpVector + RightVector); //top right
	Corners.PushBack(FarCenter - UpVector - RightVector); //bottom left
	Corners.PushBack(FarCenter - UpVector + RightVector); //bottom right

	Min = Corners[0];
	Max = Corners[0];
	Center = Corners[0];
	for (int i = 1; i < Corners.Size(); ++i) {
		Min = SMath::Min(Min, Corners[i]);
		Max = SMath::Max(Max, Corners[i]);
		Center += Corners[i];
	}
	Center = Center / Corners.Size();

	DiagonalLength = (Max - Min).Length();

}

void PFrustrum::SetupDebugFrustrumEdges(Vector3f LightPosition)
{
	DebugFrustrumMesh->Vertices.Clear();
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[0]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[1]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[1]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[3]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[3]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[2]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[2]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[0]));

	// Far plane edges			 							
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[4]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[5]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[5]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[7]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[7]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[6]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[6]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[4]));
	// Connect near and far planes					
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[0]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[4]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[1]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[5]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[2]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[6]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[3]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[7]));

	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Center));
	//DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Center + LightPosition * 10));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Center + LightPosition * (FarPlane - NearPlane)));


	DebugFrustrumMesh->SetupBuffers(GL_DYNAMIC_DRAW);
}

void PFrustrum::SetupDebugFrustrumPlanes()
{
	DebugFrustrumMesh->Vertices.Clear();

	// === Near Plane ===
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[0]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[2]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[1]));

	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[2]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[3]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[1]));

	// === Far Plane ===
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[4]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[5]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[6]));

	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[6]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[5]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[7]));

	// === Left Plane ===
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[0]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[4]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[2]));

	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[2]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[4]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[6]));

	// === Right Plane ===
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[1]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[3]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[5]));

	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[3]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[7]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[5]));

	// === Top Plane ===
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[0]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[1]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[4]));

	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[1]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[5]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[4]));

	// === Bottom Plane ===
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[2]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[6]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[3]));

	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[3]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[6]));
	DebugFrustrumMesh->Vertices.PushBack(VertexFormat(Corners[7]));

	DebugFrustrumMesh->SetupBuffers(GL_DYNAMIC_DRAW);
}

void PFrustrum::RenderDebugFrustrum(int RenderOption, Vector4f Colour, Matrix4f Projection, Matrix4f View, Vector3f LightPosition)
{
	if (DebugFrustrumMesh != nullptr)
	{
		Shader* LineShader = gShaderManager.GetShader("SimpleShader");
		glUseProgram(LineShader->ID);

		LineShader->SetMat4("modelViewProjectionMatrix", false, Projection * View * Matrix4f::IDENTITY);
		
		switch (RenderOption)
		{
		case 1:
			SetupDebugFrustrumPlanes();
			DebugFrustrumMesh->BindVAO();
			LineShader->SetVec4("uColour", Colour);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDisable(GL_CULL_FACE);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glEnable(GL_CULL_FACE);
			glDisable(GL_BLEND);

			CHECK_GL_ERROR();
			break;
		case 2:
			SetupDebugFrustrumEdges(LightPosition);
			DebugFrustrumMesh->BindVAO();
			LineShader->SetVec4("uColour", Vector4f(Vector3f(Colour),1.0));
			glDrawArrays(GL_LINES, 0, 26);
			CHECK_GL_ERROR();
			break;
		case 3:
			SetupDebugFrustrumPlanes();
			DebugFrustrumMesh->BindVAO();
			LineShader->SetVec4("uColour", Colour);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDisable(GL_CULL_FACE);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glEnable(GL_CULL_FACE);
			glDisable(GL_BLEND);

			SetupDebugFrustrumEdges(LightPosition);
			DebugFrustrumMesh->BindVAO();
			LineShader->SetVec4("uColour", Vector4f(Vector3f(Colour), 1.0));
			glDrawArrays(GL_LINES, 0, 26); 
			CHECK_GL_ERROR();
			break;
		default:
			break;
		}
	}
}


//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//									CAMERA COMPONENT
//----------------------------------------------------------------------------------------
PCameraComponent::PCameraComponent(PRenderActor* Parent, Vector3f up, float yaw, float pitch) : PSceneComponent(Parent), Front(Vector3f(0.0f, 0.0f, -1.0f)), Right(Vector3f(1.0f, 0.0f, 0.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{
	Up = up;
	WorldUp = Up;

	Yaw = yaw;
	Pitch = pitch;

	MouseSensitivity = 0.5f;
	MovementSpeed = 15.0f;
	Zoom = 60.0f;

	Frustrum.FieldOfView = 60.0f;
	Frustrum.NearPlane = 0.5f;
	Frustrum.FarPlane = 1000.0f;
	Frustrum.Ratio = 16.0f / 9.0f;

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

PCameraActor::PCameraActor(std::string NodeName) : PRenderActor(NodeName)
{
	ActorType = EntityType::CAMERA;
	Camera = new PCameraComponent(this);

	//TODO: move this
	ControlCamera(1920, 1080);
}

void PCameraActor::ControlCamera(uint32 Width, uint32 Height) {
	//TODO: do some checking ?
	Camera->Frustrum.Ratio = (float)Width / (float)Height;

	Camera->UpdateCameraVectors();

	Camera->Frustrum.CalculateFrustrumCorners(Camera);

	Camera->Projection = Perspective(Camera->Frustrum.FieldOfView, Camera->Frustrum.Ratio, Camera->Frustrum.NearPlane, Camera->Frustrum.FarPlane);

	Camera->View = LookAt(ObjectPosition, ObjectPosition + Camera->Front, Camera->WorldUp);

	//SetupDebugFrustrum();
}

void PCameraActor::SetupDebugFrustrum()
{
	this->StaticMesh = new PStaticMeshComponent(this);
	this->StaticMesh->Vertices = TArray<VertexFormat>(24);
	TArray<Vector3f> Corners = this->Camera->Frustrum.Corners;

	this->StaticMesh->Vertices.PushBack(VertexFormat(Corners[0]));
	this->StaticMesh->Vertices.PushBack(VertexFormat(Corners[1]));
	this->StaticMesh->Vertices.PushBack(VertexFormat(Corners[1])); 
	this->StaticMesh->Vertices.PushBack(VertexFormat(Corners[3]));
	this->StaticMesh->Vertices.PushBack(VertexFormat(Corners[3]));
	this->StaticMesh->Vertices.PushBack(VertexFormat(Corners[2]));
	this->StaticMesh->Vertices.PushBack(VertexFormat(Corners[2])); 
	this->StaticMesh->Vertices.PushBack(VertexFormat(Corners[0]));
								 							
	// Far plane edges			 							
	this->StaticMesh->Vertices.PushBack(VertexFormat(Corners[4]));
	this->StaticMesh->Vertices.PushBack(VertexFormat(Corners[5]));
	this->StaticMesh->Vertices.PushBack(VertexFormat(Corners[5]));
	this->StaticMesh->Vertices.PushBack(VertexFormat(Corners[7]));
	this->StaticMesh->Vertices.PushBack(VertexFormat(Corners[7]));
	this->StaticMesh->Vertices.PushBack(VertexFormat(Corners[6]));
	this->StaticMesh->Vertices.PushBack(VertexFormat(Corners[6]));
	this->StaticMesh->Vertices.PushBack(VertexFormat(Corners[4]));				
	// Connect near and far planes					
	this->StaticMesh->Vertices.PushBack(VertexFormat(Corners[0]));
	this->StaticMesh->Vertices.PushBack(VertexFormat(Corners[4]));
	this->StaticMesh->Vertices.PushBack(VertexFormat(Corners[1]));
	this->StaticMesh->Vertices.PushBack(VertexFormat(Corners[5]));
	this->StaticMesh->Vertices.PushBack(VertexFormat(Corners[7]));
	this->StaticMesh->Vertices.PushBack(VertexFormat(Corners[2])); 
	this->StaticMesh->Vertices.PushBack(VertexFormat(Corners[6]));
	this->StaticMesh->Vertices.PushBack(VertexFormat(Corners[3]));

	/*Matrix4f InverseProjectionView = Inverse(Projection * View);
	for (int i = 0; i < 8; ++i) {
		Vector4f WordSpacePosition = InverseProjectionView * Vector4f(this->StaticMesh->Vertices[i].Position, 1.0);
		this->StaticMesh->Vertices[i].Position = Vector3f(WordSpacePosition) / WordSpacePosition.W;
	}*/

	this->StaticMesh->SetupBuffers();
}

void PCameraActor::RenderDebugFrustrum(Matrix4f Projection, Matrix4f View)
{
	if (this->StaticMesh != nullptr)
	{
		Shader* LineShader = gShaderManager.GetShader("SimpleShader");
		glUseProgram(LineShader->ID);

		LineShader->SetMat4("modelViewProjectionMatrix", false, Projection * View * Matrix4f::IDENTITY);
		LineShader->SetVec3("uColour", Vector3f(0.0, 1.0, 1.0));

		this->StaticMesh->BindVAO();
	//	glDisable(GL_DEPTH_TEST);
		glDrawArrays(GL_LINES, 0, 24); // 12 lines × 2 vertices
		//glEnable(GL_DEPTH_TEST);
		CHECK_GL_ERROR();
	}
}
