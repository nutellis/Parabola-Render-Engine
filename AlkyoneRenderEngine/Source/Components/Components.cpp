#include <Components/RenderComponents/Components.h>

#include <ParabolaMath.h>
#include <Components/RenderActor.h>
#include <Managers/ShaderManager.h>
#include <Components/Shader.h>
#include <Components/StaticMesh.h>
#include <Components/Material.h>

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
//								STATIC MESH COMPONENT
//----------------------------------------------------------------------------------------
PStaticMeshComponent::PStaticMeshComponent()
{
}


PStaticMeshComponent::PStaticMeshComponent(RenderActor* Parent, const char* path)//const char* path,PSceneComponent *Default,bool isAbsolute)
{
	std::cout << "This is a StaticMeshComponent\n";
//	ComponentArchive->Load(path);
//	Deserialize(*ComponentArchive);

	Asset* AssetToLoad = ASSETLOADER.LoadAsset(path);

	Mesh = new PStaticMesh(AssetToLoad);
	
	if (AssetToLoad->Materials.IsNotEmpty()) {
		for (uint32 i = 0; i < AssetToLoad->Materials.Size(); i++) {
			PMaterial * Material = new PMaterial();
			Material = &AssetToLoad->Materials[i];
			Materials.PushBack(Material);
		}
		Mesh->MaterialIndexMapping = AssetToLoad->MaterialIndexMapping;
	}
	else {
		PMaterial* Material = new PMaterial();
		if (AssetToLoad->Materials.Size() <= 1) {
			Material->Diffuse.Colour = Vector4f(Vector3f((float)(rand() % 100) / 100, (float)(rand() % 100) / 100, (float)(rand() % 100) / 100), 1);
			Material->Specular.Colour = Vector4f(Vector3f(0.5f, 0.5f, 0.5f), 1);
			Material->Shinness = 32.0;
		}
		Materials.PushBack(Material);
		
	}

	// init the shader only one time
	if (SHADERMANAGER.GetShader("SimpleShader") == nullptr) {
		Materials[0]->ShaderID = SHADERMANAGER.CreateShader("SimpleShader", "Q:/Users/Nutellis/Documents/Projects/OpenGLEngine/AlkyoneRenderEngine/Shaders/simpleVertex.vert", "Q:/Users/Nutellis/Documents/Projects/OpenGLEngine/AlkyoneRenderEngine/Shaders/simpleFrag.frag", nullptr);
	}
	else {
		Materials[0]->ShaderID = SHADERMANAGER.GetShader("SimpleShader")->ID;
	}
	
	Mesh->SetupBuffers();

	this->Parent = Parent;

}
void PStaticMeshComponent::SetShaderMaterial(Shader * ActiveShader, PMaterial *Material) const
{
	ActiveShader->setFloat("material.shininess", 32.0);
	ActiveShader->setVec3("material.diffuse", Vector3f(Material->Diffuse.Colour));
	ActiveShader->setVec3("material.specular", Vector3f(0.5f, 0.5f, 0.5f));


}

void PStaticMeshComponent::DrawComponent(Shader* ActiveShader) {
	
	Model = Matrix4f::IDENTITY;

	Model = Scale(GetScale(), Model);

	// fix angle parameter (StaticMesh->angle)
	// Model = Rotate(this->ObjectRotation, StaticMesh->angle, Model);

	Model = Translate(GetPosition(), Model);

	ActiveShader->SetMat4(ActiveShader->Uniforms.ModelLocation, true, Model);

	Mesh->VAO.Bind();

	// for each material
	for (uint32 i = 0; i < Materials.Size(); i++) {
		
			// bind material

			// setup buffers for each material
			// in detail:
			// 
			// we should create a map of -> int (material index) : vertex indexes 
			// for each material index render the indices and move to the next.
			// 
			// do everything as usual

		SetShaderMaterial(ActiveShader,Materials[i]);
		
		if (Mesh->MaterialIndexMapping.size() == 0) {
			glDrawElements(GL_TRIANGLES, Mesh->Indices.Size(), GL_UNSIGNED_INT, 0);
		}
		else {
			glVertexArrayElementBuffer(Mesh->VAO.GetID(), Mesh->EBOs[i]->GetID());
			//std::cout << "MaterialIndexMapping[" << i << "]:\n";
			//for (int j = 0; j < Mesh->MaterialIndexMapping[i].Size(); j++) {
			//	std::cout << Mesh->MaterialIndexMapping[i][j] << "\t";
			//}
			//std::cout << "\n";

			glDrawElements(GL_TRIANGLES, Mesh->MaterialIndexMapping[i].Size(), GL_UNSIGNED_INT, 0);
		}
	}
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

PPointLightComponent::PPointLightComponent(RenderActor* Parent)
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
	
	// be sure to activate shader when setting uniforms/drawing objects
	ActiveShader->setVec3("light.position", Parent->ObjectPosition);
	//SHADERMANAGER.GetShader("SimpleShader")->setVec3("light.position", Vector3f(lightX, 1.5f, lightZ));

	ActiveShader->setFloat("light.cutOff", SMath::Cos(DegreesToRadians(Attributes->CutOff)));
	ActiveShader->setFloat("light.outerCutOff", SMath::Cos(DegreesToRadians(Attributes->OuterCutOff)));
	// light properties
	ActiveShader->setVec3("light.ambient", Attributes->Ambient);
	// we configure the diffuse intensity slightly higher; the right lighting conditions differ with each lighting method and environment.
	// each environment and lighting type requires some tweaking to get the best out of your environment.
	ActiveShader->setVec3("light.diffuse", Attributes->Diffuse);
	ActiveShader->setVec3("light.specular", Attributes->Specular); // add to attributes
	ActiveShader->setFloat("light.constant", Attributes->Constant);
	ActiveShader->setFloat("light.linear", Attributes->Linear);
	ActiveShader->setFloat("light.quadratic",Attributes->Quadratic);
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//									CAMERA COMPONENT
//----------------------------------------------------------------------------------------
PCameraComponent::PCameraComponent(RenderActor * Parent, Vector3f up, float yaw, float pitch) : PSceneComponent(Parent), Front(Vector3f(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{
	IsActiveCamera = true;

	Up = up;
	WorldUp = Vector3f(Up);

	Yaw = yaw;
	Pitch = pitch;

	MouseSensitivity = 0.2f;

	Zoom = 60.0f;

	UpdateCameraVectors();

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
	Vector4f R = Normalize(F.Cross(Up));
	Vector4f U = R.Cross(F);

	Vector4f T = Vector4f(0.0f, 0.0f, 0.0f, 1.0f);

	Matrix4f Result = Matrix4f(R, U, -F, T);

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
	Right = Normalize((Front.Cross(WorldUp)));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
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
	if (direction == UP)
		Parent->ObjectPosition += (Up * velocity);
	if (direction == DOWN)
		Parent->ObjectPosition -= (Up * velocity);
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
	ActiveShader->SetMat4(ActiveShader->Uniforms.ViewLocation, true, View);
	//projection
	ActiveShader->SetMat4(ActiveShader->Uniforms.ProjectionLocation, false, Projection);

	ActiveShader->setVec3("viewPos", GetPosition());
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