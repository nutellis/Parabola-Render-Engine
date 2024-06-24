#include <Components/RenderComponents/Components.h>

#include <ParabolaMath.h>
#include <Components/RenderActor.h>
#include <Managers/ShaderManager.h>
#include <Components/Shader.h>
#include <Components/StaticMesh.h>
#include <Components/Material.h>

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

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//								STATIC MESH COMPONENT
//----------------------------------------------------------------------------------------
PStaticMeshComponent::PStaticMeshComponent()
{
}


PStaticMeshComponent::PStaticMeshComponent(const char* path)//const char* path,PSceneComponent *Default,bool isAbsolute)
{
	std::cout << "This is a StaticMeshComponent\n";
//	ComponentArchive->Load(path);
//	Deserialize(*ComponentArchive);

	Mesh = new PStaticMesh(ASSETLOADER.LoadAsset(path));
	Material = new PMaterial();

	// init the shader only one time
	if (SHADERMANAGER.GetShader("SimpleShader") == nullptr) {
		Material->ShaderID = SHADERMANAGER.CreateShader("SimpleShader", "H:/Users/Nutellis/Documents/Projects/OpenGLEngine/AlkyoneRenderEngine/Shaders/simpleVertex.vert", "H:/Users/Nutellis/Documents/Projects/OpenGLEngine/AlkyoneRenderEngine/Shaders/simpleFrag.frag", nullptr);
	}
	else {
		Material->ShaderID = SHADERMANAGER.GetShader("SimpleShader")->ID;
	}

	Material->Diffuse.Colour = Vector4f(Vector3f((float)(rand() % 100)/100, (float)(rand() % 100)/100, (float)(rand() % 100)/100), 1);
	Material->Specular.Colour = Vector4f(Vector3f(0.5f, 0.5f, 0.5f), 1);
	Material->Shinness = 32.0;

	Mesh->SetupBuffers();

}
void PStaticMeshComponent::SetShaderMaterial(Shader * ActiveShader) const
{
	ActiveShader->setFloat("material.shininess", Material->Shinness);
	ActiveShader->setVec3("material.diffuse", Vector3f(Material->Diffuse.Colour));
	ActiveShader->setVec3("material.specular", Vector3f(Material->Specular.Colour));

}

void PStaticMeshComponent::DrawComponent(Shader* ActiveShader) {
	
	SetShaderMaterial(ActiveShader);

	//ActiveShader->Uniforms.ModelLocation
	ActiveShader->SetMat4("model", true, Model);
	
	Mesh->VAO.Bind();
	glDrawElements(GL_TRIANGLES, Mesh->Indices.Size(), GL_UNSIGNED_INT, 0);
}

//void PStaticMeshComponent::Deserialize(UArchive & Ar)
//{
//	std::cout << "LOL" << std::endl;
//	Mesh = new PStaticMesh(Ar);
//	cMaterial = new PMaterial(Ar);
//	cMaterial->SetupMaterial();
//
//}
//void PStaticMeshComponent::BindTextures(Shader shader)
//{
//	cMaterial;
//	glActiveTexture(GL_TEXTURE0);
//	//glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
//	// and finally bind the texture
//	//glBindTexture(GL_TEXTURE_2D, textures[i].id);
//}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//									POINT LIGHT COMPONENT
//----------------------------------------------------------------------------------------

PPointLightComponent::PPointLightComponent()
{
	//glm::vec3(1.2f, 1.0f, 2.0f);
	std::cout << "This is a PointLightComponent\n";
	Attributes = new PointLightAttributes();
}

PPointLightComponent::PPointLightComponent(PSceneComponent * Default)
{
	std::cout << "This is a PointLightComponent\n";
	Attributes = new PointLightAttributes();

}

PPointLightComponent::~PPointLightComponent()
{
	delete[] Attributes;
}

void PPointLightComponent::SetupShaderLight() {
	glUseProgram(SHADERMANAGER.GetShader("SimpleShader")->ID);
	//
	// be sure to activate shader when setting uniforms/drawing objects
	SHADERMANAGER.GetShader("SimpleShader")->setVec3("light.position", Vector3f(0, 1.5f, 2));
	//SHADERMANAGER.GetShader("SimpleShader")->setVec3("light.position", Vector3f(lightX, 1.5f, lightZ));

	SHADERMANAGER.GetShader("SimpleShader")->setFloat("light.cutOff", SMath::Cos(DegreesToRadians(12.5f)));
	SHADERMANAGER.GetShader("SimpleShader")->setFloat("light.outerCutOff", SMath::Cos(DegreesToRadians(17.5f)));
	// light properties
	SHADERMANAGER.GetShader("SimpleShader")->setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
	// we configure the diffuse intensity slightly higher; the right lighting conditions differ with each lighting method and environment.
	// each environment and lighting type requires some tweaking to get the best out of your environment.
	SHADERMANAGER.GetShader("SimpleShader")->setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
	SHADERMANAGER.GetShader("SimpleShader")->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	SHADERMANAGER.GetShader("SimpleShader")->setFloat("light.constant", 1.0f);
	SHADERMANAGER.GetShader("SimpleShader")->setFloat("light.linear", 0.09f);
	SHADERMANAGER.GetShader("SimpleShader")->setFloat("light.quadratic", 0.032f);
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//									CAMERA COMPONENT
//----------------------------------------------------------------------------------------
PCameraComponent::PCameraComponent(RenderActor * Parent, Vector3f up, float yaw, float pitch) : PSceneComponent(Parent), Front(Vector3f(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{
	IsActiveCamera = true;

	Up = up;
	Yaw = yaw;
	Pitch = pitch;

	MouseSensitivity = 0.2f;

	Zoom = 60.0f;

	UpdateCameraVectors();

	ProcessMouseMovement(0, 0);
}

PCameraComponent::PCameraComponent(float upX, float upY, float upZ, float yaw, float pitch)
	: Front(Vector3f(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{

	IsActiveCamera = true;

	Up = Vector3f(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	UpdateCameraVectors();
}

void PCameraComponent::LookAt(const Vector4f& Eye, const Vector4f& At, const Vector4f& Up)
{
	Vector4f F = Normalize(At - Eye);
	Vector4f S = Normalize(F.Cross(Up));
	Vector4f U = S.Cross(F);

	Vector4f T = Vector4f(0.0f, 0.0f, 0.0f, 1.0f);

	Matrix4f Result = Matrix4f(S, U, -F, T);

	Matrix4f Translation;
	Translation = Matrix4f::IDENTITY;

	Translation = Translate(-Eye, Matrix4f::IDENTITY);

	//Translation[0][3] = - Eye.X;
	//Translation[1][3] = - Eye.Y;
	//Translation[2][3] = - Eye.Z;

	Result = Result * Translation;

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

	}
	else {

		float F = SMath::Cotan(DegreesToRadians(FieldOfView) * 0.5f);

		Result[0][0] = F / AspectRatio;
		Result[1][1] = F;
		Result[2][2] = (ZFar + ZNear) / (ZNear - ZFar);
		Result[2][3] = -1;
		Result[3][2] = (2 * ZFar * ZNear) / (ZNear - ZFar);

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
	Right = Normalize((Front.Cross(Up)));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = Normalize((Right.Cross(Front)));
}

void PCameraComponent::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		Parent->ObjectPosition += (Front * velocity);
	if (direction == BACKWARD)
		Parent->ObjectPosition -= (Front * velocity);
	if (direction == LEFT)
		Parent->ObjectPosition -= (Right * velocity);
	if (direction == RIGHT)
		Parent->ObjectPosition += (Right * velocity);
}

void PCameraComponent::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	//xoffset *= MouseSensitivity;
	//yoffset *= MouseSensitivity;

	//Yaw += xoffset;
	//Pitch += yoffset;

	//// Make sure that when pitch is out of bounds, screen doesn't get flipped
	//if (constrainPitch)
	//{
	//	if (Pitch > 89.0f)
	//		Pitch = 89.0f;
	//	if (Pitch < -89.0f)
	//		Pitch = -89.0f;
	//}

	// Update Front, Right and Up Vectors using the updated Eular angles
	//UpdateCameraVectors();

	//xoffset *= MouseSensitivity;
	//yoffset *= MouseSensitivity;

	//Yaw += xoffset;
	//Pitch += yoffset;
	//float camX;// = SMath::Sin(xoffset);

	//float radius = 5;
	//camX = SMath::Cos(DegreesToRadians(Yaw)) * SMath::Cos(DegreesToRadians(Pitch));

	//float camZ;

	//camZ = SMath::Sin(DegreesToRadians(Yaw)) * SMath::Cos(DegreesToRadians(Pitch));

	//if (Pitch > 89.0f)
	//{
	//	Pitch = 89.0f;
	//}
	//if (Pitch < -89.0f)
	//{
	//	Pitch = -89.0f;
	//}
	//Parent->ObjectPosition = Vector3f(camX, SMath::Sin(DegreesToRadians(Pitch)), camZ) * radius;
}

void PCameraComponent::ProcessMouseScroll(float yoffset)
{
	Zoom -= (float)yoffset;
	if (Zoom < 1.0f)
		Zoom = 1.0f;
	if (Zoom > 45.0f)
		Zoom = 45.0f;
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