#include <Components/Colliders/BoundingBox.h>
#include <Core/RenderCore.h>
#include <Components/RenderComponents/StaticMeshComponent.h>
#include <Managers/ShaderManager.h>
#include <Components/Shader.h>



PBoundingBox::PBoundingBox() : Min(0), Max(0), Center(0)
{
	Corners = TArray<Vector3f>(8);
	DebugFrustrumMesh = nullptr;
}

PBoundingBox::PBoundingBox(const PBoundingBox& Other) :Min(Other.Min), Max(Other.Max), Center(Other.Center)
{
	Corners = Other.Corners;
	if (Other.DebugFrustrumMesh)
		DebugFrustrumMesh = new PStaticMeshComponent(*Other.DebugFrustrumMesh);  // deep copy
	else
		DebugFrustrumMesh = nullptr;
}

PBoundingBox::PBoundingBox(PBoundingBox&& Other) noexcept : 
	Min(Other.Min),
	Max(Other.Max),
	Center(Other.Center),
	Corners(Utilities::Move(Other.Corners)),
	DebugFrustrumMesh(Other.DebugFrustrumMesh)
{
	Other.DebugFrustrumMesh = nullptr;
}

PBoundingBox::PBoundingBox(TArray<Vector3f> InCorners)
{
	CalculateFromCorners(InCorners);
	DebugFrustrumMesh = nullptr;
	//DebugFrustrumMesh = new PStaticMeshComponent(nullptr);
	//if (DebugFrustrumMesh != nullptr) {
	//	DebugFrustrumMesh->Vertices = TArray<VertexFormat>(24);
	//}
}

PBoundingBox::PBoundingBox(Vector3f Min, Vector3f Max) : Min(Min), Max(Max)
{
	Corners = TArray<Vector3f>(8);
	DebugFrustrumMesh = nullptr;
	//DebugFrustrumMesh = new PStaticMeshComponent(nullptr);
	//if (DebugFrustrumMesh != nullptr) {
	//	DebugFrustrumMesh->Vertices = TArray<VertexFormat>(24);
	//}
}

PBoundingBox::~PBoundingBox()
{
	Corners.Clear();
	if (DebugFrustrumMesh != nullptr) {
		delete DebugFrustrumMesh;
		DebugFrustrumMesh = nullptr;
	}
}

PBoundingBox& PBoundingBox::operator=(PBoundingBox&& Other) noexcept {
	if (this != &Other) {

		Corners.Clear();
		//if (DebugFrustrumMesh) {
		//	delete DebugFrustrumMesh;
		//	DebugFrustrumMesh = nullptr;
		//}

		Min = Other.Min;
		Max = Other.Max;
		Center = Other.Center;
		Corners = Utilities::Move(Other.Corners);

		//DebugFrustrumMesh = Other.DebugFrustrumMesh;
		//Other.DebugFrustrumMesh = nullptr;
	}
	return *this;
}

// just copies the corners and calculates min/max & center.
void PBoundingBox::CalculateFromCorners(TArray<Vector3f> Corners)
{
	if (this->Corners == Corners) {
		return; // no need to recalculate
	}
	this->Corners = Corners;
	this->Min = Vector3f(std::numeric_limits<float>::infinity());
	this->Max = Vector3f(-std::numeric_limits<float>::infinity());
	for (int i = 0; i < Corners.Size(); i++) {

		this->Min.X = SMath::Min(this->Min.X, Corners[i].X);
		this->Max.X = SMath::Max(this->Max.X, Corners[i].X);
		this->Min.Y = SMath::Min(this->Min.Y, Corners[i].Y);
		this->Max.Y = SMath::Max(this->Max.Y, Corners[i].Y);
		this->Min.Z = SMath::Min(this->Min.Z, Corners[i].Z);
		this->Max.Z = SMath::Max(this->Max.Z, Corners[i].Z);
		this->Center += Corners[i];
	}
	this->Center = this->Center / Corners.Size();
}



void PBoundingBox::SetupDebugFrustrumEdges()
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

	DebugFrustrumMesh->SetupBuffers(GL_DYNAMIC_DRAW);
}

void PBoundingBox::SetupDebugFrustrumPlanes()
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

void PBoundingBox::RenderDebugBoundingBox(int RenderOption, Vector4f Colour, Matrix4f Projection, Matrix4f View, Matrix4f Model)
{
	if (DebugFrustrumMesh == nullptr) {
		DebugFrustrumMesh = new PStaticMeshComponent(nullptr);
		if (DebugFrustrumMesh != nullptr) {
			DebugFrustrumMesh->Vertices = TArray<VertexFormat>(24);
		}
	}
	Shader* LineShader = gShaderManager.GetShader("SimpleShader");
	glUseProgram(LineShader->ID);

	//Get model matrix from parent
	LineShader->SetMat4("modelViewProjectionMatrix", false, Projection * View * Model);

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
		SetupDebugFrustrumEdges();
		DebugFrustrumMesh->BindVAO();
		LineShader->SetVec4("uColour", Vector4f(Vector3f(Colour), 1.0));
		glDrawArrays(GL_LINES, 0, 24);
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

		SetupDebugFrustrumEdges();
		DebugFrustrumMesh->BindVAO();
		LineShader->SetVec4("uColour", Vector4f(Vector3f(Colour), 1.0));
		glDrawArrays(GL_LINES, 0, 24);
		CHECK_GL_ERROR();
		break;
	default:
		break;
	}
}