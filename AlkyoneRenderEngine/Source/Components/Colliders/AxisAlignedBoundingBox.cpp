#include <Components/Colliders/AxisAlignedBoundingBox.h>
#include <Core/RenderCore.h>
#include <Components/RenderComponents/StaticMeshComponent.h>
#include <Managers/ShaderManager.h>
#include <Components/Shader.h>

PAxisAlignedBoundingBox::PAxisAlignedBoundingBox() : PBoundingBox()
{

	Min = Vector3f(std::numeric_limits<float>::infinity());
	Max = Vector3f(-std::numeric_limits<float>::infinity());

}

PAxisAlignedBoundingBox::PAxisAlignedBoundingBox(const PAxisAlignedBoundingBox& Other) : PBoundingBox(Other)
{}

PAxisAlignedBoundingBox::PAxisAlignedBoundingBox(PAxisAlignedBoundingBox&& Other) noexcept
	: PBoundingBox(Utilities::Move(Other)) {}


PAxisAlignedBoundingBox::PAxisAlignedBoundingBox(Vector3f Min, Vector3f Max) : PBoundingBox(Min, Max)
{
	Corners = {
		Vector3f(Min.X, Max.Y, Min.Z), // 0: Left Near Top
		Vector3f(Max.X, Max.Y, Min.Z), // 1: Right Near Top
		Vector3f(Min.X, Min.Y, Min.Z), // 2: Left Near Bottom
		Vector3f(Max.X, Min.Y, Min.Z), // 3: Right Near Bottom

		Vector3f(Min.X, Max.Y, Max.Z), // 4: Left Far Top
		Vector3f(Max.X, Max.Y, Max.Z), // 5: Right Far Top
		Vector3f(Min.X, Min.Y, Max.Z), // 6: Left Far Bottom
		Vector3f(Max.X, Min.Y, Max.Z)  // 7: Right Far Bottom
	};
	Center = Max.MidPoint(Min);
}

PAxisAlignedBoundingBox::~PAxisAlignedBoundingBox()
{}


PAxisAlignedBoundingBox& PAxisAlignedBoundingBox::operator=(PAxisAlignedBoundingBox&& Other) noexcept {
	if (this != &Other) {
		PBoundingBox::operator=(Utilities::Move(Other));
	}
	return *this;
}

Vector3f PAxisAlignedBoundingBox::GetCorner(CornerEnum CornerToGet) const
{
	switch (CornerToGet)
	{
	case FAR_LEFT_BOTTOM:
		return Min;
	case FAR_LEFT_TOP:
		return Vector3f(Min.X, Max.Y, Min.Z);
	case FAR_RIGHT_TOP:
		return Vector3f(Max.X, Max.Y, Min.Z);
	case FAR_RIGHT_BOTTOM:
		return Vector3f(Max.X, Min.Y, Min.Z);
	case NEAR_RIGHT_BOTTOM:
		return Vector3f(Max.X, Min.Y, Max.Z);
	case NEAR_LEFT_BOTTOM:
		return Vector3f(Min.X, Min.Y, Max.Z);
	case NEAR_LEFT_TOP:
		return Vector3f(Min.X, Max.Y, Max.Z);
	case NEAR_RIGHT_TOP:
		return Max;
	default:
		return Vector3f();
	}
}

void PAxisAlignedBoundingBox::CalculateFromCorners(TArray<Vector3f> Corners)
{
	this->Min = Vector3f(std::numeric_limits<float>::infinity());
	this->Max = Vector3f(-std::numeric_limits<float>::infinity());
	for (int i = 0; i < Corners.Size(); i++) {

		this->Min.X = SMath::Min(this->Min.X, Corners[i].X);
		this->Max.X = SMath::Max(this->Max.X, Corners[i].X);
		this->Min.Y = SMath::Min(this->Min.Y, Corners[i].Y);
		this->Max.Y = SMath::Max(this->Max.Y, Corners[i].Y);
		this->Min.Z = SMath::Min(this->Min.Z, Corners[i].Z);
		this->Max.Z = SMath::Max(this->Max.Z, Corners[i].Z);
	}
	GetCornersFromMinMax(); 
	this->Center = this->Max.MidPoint(this->Min);
}


Vector3f PAxisAlignedBoundingBox::GetCenter() const
{
	return Max.MidPoint(Min);
}

TArray<Vector3f> PAxisAlignedBoundingBox::GetAllCorners() const
{
	return Corners;
}

Vector3f PAxisAlignedBoundingBox::GetSize() const
{
	return (Max - Min);
}

Vector3f PAxisAlignedBoundingBox::GetMinimum() const
{
	return Min;
}

Vector3f PAxisAlignedBoundingBox::GetMaximum() const
{
	return Max;
}

void PAxisAlignedBoundingBox::SetExtents(const Vector3f & inMin, const Vector3f & inMax)
{
	//assert((min.x <= max.x && min.y <= max.y && min.z <= max.z) &&
	//	"The minimum corner of the box must be less than or equal to maximum corner");

	//mExtent = EXTENT_FINITE;
	Min = inMin;
	Max = inMax;
}

uint8 PAxisAlignedBoundingBox::Contains(const PAxisAlignedBoundingBox & Other) const
{
	return Min.X <= Other.Min.X && Max.X >= Other.Max.X &&
		Min.Y <= Other.Min.Y && Max.Y >= Other.Max.Y &&
		Min.Z <= Other.Min.Z && Max.Z >= Other.Max.Z;
}

void PAxisAlignedBoundingBox::GetCornersFromMinMax()
{
	Corners = {
		Vector3f(Min.X, Max.Y, Min.Z), // 0: Left Near Top
			Vector3f(Max.X, Max.Y, Min.Z), // 1: Right Near Top
			Vector3f(Min.X, Min.Y, Min.Z), // 2: Left Near Bottom
			Vector3f(Max.X, Min.Y, Min.Z), // 3: Right Near Bottom

			Vector3f(Min.X, Max.Y, Max.Z), // 4: Left Far Top
			Vector3f(Max.X, Max.Y, Max.Z), // 5: Right Far Top
			Vector3f(Min.X, Min.Y, Max.Z), // 6: Left Far Bottom
			Vector3f(Max.X, Min.Y, Max.Z)  // 7: Right Far Bottom}
	};
}


