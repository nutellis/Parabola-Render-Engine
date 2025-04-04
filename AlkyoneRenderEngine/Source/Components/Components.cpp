#include <Components/RenderComponents/Components.h>

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

Vector3f PSceneComponent::GetScale() const
{
	return Parent->ObjectScale;
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//									POINT LIGHT COMPONENT
//----------------------------------------------------------------------------------------

PPointLightComponent::PPointLightComponent(PRenderActor* Parent)
{
	std::cout << "This is a PointLightComponent\n";
	Attributes = new PointLightAttributes();

	Attributes->Diffuse = Vector3f(1.0f, 1.0f, 1.0f);
	Attributes->Specular = Vector3f(1.0f, 1.0f, 1.0f);
	Attributes->Ambient = Vector3f(0.1f, 0.1f, 0.1f);

	Attributes->CutOff = 12.5f;
	Attributes->OuterCutOff = 17.5f;


	Attributes->Constant = 1.0f;
	Attributes->Linear = 0.09f;
	Attributes->Quadratic = 0.032f;

	this->Parent = Parent;
}

PPointLightComponent::PPointLightComponent(PSceneComponent * Default)
{
	std::cout << "This is a PointLightComponent\n";
	Attributes = new PointLightAttributes();

	Attributes->Diffuse = Vector3f(1.0f, 1.0f, 1.0f);
	Attributes->Specular = Vector3f(1.0f, 1.0f, 1.0f);
	Attributes->Ambient = Vector3f(0.1f, 0.1f, 0.1f);

	Attributes->CutOff = 12.5f;
	Attributes->OuterCutOff = 17.5f;
	
	
	Attributes->Constant = 1.0f;
	Attributes->Linear = 0.09f;
	Attributes->Quadratic = 0.032f;

}

PPointLightComponent::~PPointLightComponent()
{
	delete[] Attributes;
}

void PPointLightComponent::SetupShaderLight(Shader * ActiveShader) {
	
	//// be sure to activate shader when setting uniforms/drawing objects
	//ActiveShader->setVec3("light.position", Parent->ObjectPosition);
	////gShaderManager.GetShader("SimpleShader")->setVec3("light.position", Vector3f(lightX, 1.5f, lightZ));

	//ActiveShader->setFloat("light.cutOff", SMath::Cos(DegreesToRadians(Attributes->CutOff)));
	//ActiveShader->setFloat("light.outerCutOff", SMath::Cos(DegreesToRadians(Attributes->OuterCutOff)));
	//// light properties
	//ActiveShader->setVec3("light.ambient", Attributes->Ambient);
	//// we configure the diffuse intensity slightly higher; the right lighting conditions differ with each lighting method and environment.
	//// each environment and lighting type requires some tweaking to get the best out of your environment.
	//ActiveShader->setVec3("light.diffuse", Attributes->Diffuse);
	//ActiveShader->setVec3("light.specular", Attributes->Specular); // add to attributes
	//ActiveShader->setFloat("light.constant", Attributes->Constant);
	//ActiveShader->setFloat("light.linear", Attributes->Linear);
	//ActiveShader->setFloat("light.quadratic",Attributes->Quadratic);



	ActiveShader->setVec3("LightPosition", Parent->ObjectPosition);
	
	float environment_multiplier = 1.3f;
	ActiveShader->setFloat("environment_multiplier", environment_multiplier);

	/*labhelper::setUniformSlow(currentShaderProgram, "point_light_position", lightPosition);
	labhelper::setUniformSlow(currentShaderProgram, "point_light_color", point_light_color);
	labhelper::setUniformSlow(currentShaderProgram, "point_light_intensity_multiplier",
		point_light_intensity_multiplier);
	labhelper::setUniformSlow(currentShaderProgram, "viewSpaceLightPosition", vec3(viewSpaceLightPosition));
	labhelper::setUniformSlow(currentShaderProgram, "viewSpaceLightDir",
		normalize(vec3(viewMatrix * vec4(-lightPosition, 0.0f))));*/


}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//									CAMERA COMPONENT
//----------------------------------------------------------------------------------------
PCameraComponent::PCameraComponent(PRenderActor * Parent, Vector3f up, float yaw, float pitch) : PSceneComponent(Parent), Front(Vector3f(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{
	IsActiveCamera = true;

	Up = up;
	WorldUp = Up;

	Yaw = yaw;
	Pitch = pitch;

	MouseSensitivity = 0.2f;

	Zoom = 45.0f;

	CameraDirection = Normalize(Vector3f::ZERO - Parent->ObjectPosition);

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

void PCameraComponent::LookAt(const Vector3f& Eye, const Vector3f& Center, const Vector3f& Up)
{
	Vector3f F = Normalize(Center - Eye);
	Vector3f R = Normalize(F.Cross(Up));
	Vector3f U = (R.Cross(F));

	Matrix4f Result = Matrix4f::IDENTITY;

	Result[0][0] = R.X;
	Result[1][0] = R.Y;
	Result[2][0] = R.Z;

	Result[0][1] = U.X;
	Result[1][1] = U.Y;
	Result[2][1] = U.Z;

	Result[0][2] = -F.X;
	Result[1][2] = -F.Y;
	Result[2][2] = -F.Z;

	Result[3][0] = -R.Dot(Eye);
	Result[3][1] = -U.Dot(Eye);
	Result[3][2] = F.Dot(Eye);

	View = Result;
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

void PCameraComponent::Perspective(
	const float& FieldOfView,
	const float& AspectRatio,
	const float& ZNear,
	const float& ZFar
)
{
	Matrix4f Result = Matrix4f::ZERO;
	if (ZNear <= 0) {
		// Handle invalid ZNear value
	}
	else {
		float F = SMath::Tan(DegreesToRadians(FieldOfView) * 0.5f);

		Result[0][0] = 1.0f / (AspectRatio * F);
		Result[1][1] = 1.0f / F;
		Result[2][2] = -(ZFar + ZNear) / (ZFar - ZNear);
		Result[2][3] = -1.0f;
		Result[3][2] = -(2.0f * ZFar * ZNear) / (ZFar - ZNear);
	}
	Projection = Result;

	//// create Frustrum
	//Matrix4f VPM = Projection * View;

	//CameraFrustrum.LeftPlane = (Vector4f(VPM.GetVector(3))
	//	+ Vector4f(VPM.GetVector(0))
	//	).GetNormalized();  // Left Plane

	//CameraFrustrum.RightPlane = (Vector4f(VPM.GetVector(3))
	//	- Vector4f(VPM.GetVector(0))
	//	).GetNormalized();  // Right Plane

	//CameraFrustrum.BottomPlane = (Vector4f(VPM.GetVector(3))
	//	+ Vector4f(VPM.GetVector(1))
	//	).GetNormalized();  // Bottom Plane

	//CameraFrustrum.TopPlane = (Vector4f(VPM.GetVector(3))
	//	- Vector4f(VPM.GetVector(1))
	//	).GetNormalized();  // Top Plane

	//CameraFrustrum.NearPlane = (Vector4f(VPM.GetVector(3))
	//	+ Vector4f(VPM.GetVector(2))
	//	).GetNormalized();  // Near Plane

	//CameraFrustrum.FarPlane = (Vector4f(VPM.GetVector(3)) -
	//	Vector4f(VPM.GetVector(2))
	//	).GetNormalized(); // Far Plane

}

void PCameraComponent::Ortho(
	const float& Left, const float& Right, 
	const float& Top, const float& Bottom, 
	const float& ZNear, const float& ZFar
)
{
	Matrix4f Result = Matrix4f::IDENTITY;

	Result[0][0] = 2.0f / (Right - Left);
	Result[1][1] = 2.0f / (Top - Bottom);
	Result[2][2] = -2.0f / (ZFar - ZNear);

	Result[3][0] = -(Right + Left) / (Right - Left);
	Result[3][1] = -(Top + Bottom) / (Top - Bottom);
	Result[3][2] = -(ZFar + ZNear) / (ZFar - ZNear);

	Projection = Result;
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

void PCameraComponent::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	// update Front, Right and Up Vectors using the updated Euler angles
	UpdateCameraVectors();
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
	//view
	ActiveShader->SetMat4(ActiveShader->Uniforms.ViewLocation, false, View);
	//projection
	ActiveShader->SetMat4(ActiveShader->Uniforms.ProjectionLocation, false, Projection);
	
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