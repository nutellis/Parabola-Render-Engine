#include <Components/BoundingBox.h>

AxisAlignedBoundingBox::AxisAlignedBoundingBox()
{

	Minimum = Vector3f(0,0,0);
	Maximum = Vector3f(100, 100, 100);
}

AxisAlignedBoundingBox::~AxisAlignedBoundingBox()
{}

Point AxisAlignedBoundingBox::GetCorner(CornerEnum CornerToGet) const
{
	Point Result;

	switch (CornerToGet)
	{
	case FAR_LEFT_BOTTOM:
		Result = Minimum;
	case FAR_LEFT_TOP:
		Result = Point(Minimum.X, Maximum.Y, Minimum.Z);
	case FAR_RIGHT_TOP:
		Result = Point(Maximum.X, Maximum.Y, Minimum.Z);
	case FAR_RIGHT_BOTTOM:
		Result = Point(Maximum.X, Minimum.Y, Minimum.Z);
	case NEAR_RIGHT_BOTTOM:
		Result = Point(Maximum.X, Minimum.Y, Maximum.Z);
	case NEAR_LEFT_BOTTOM:
		Result = Point(Minimum.X, Minimum.Y, Maximum.Z);
	case NEAR_LEFT_TOP:
		Result = Point(Minimum.X, Maximum.Y, Maximum.Z);
	case NEAR_RIGHT_TOP:
		Result = Maximum;
	default:
		Result = Point();
	}

	return Result;
}

Point AxisAlignedBoundingBox::GetCenter(void) const
{
	return Maximum.MidPoint(Minimum);
}

AxisAlignedBoundingBox::Corners AxisAlignedBoundingBox::GetAllCorners() const
{
	Corners Result = Corners(8);

	Result.PushBack(GetCorner(NEAR_LEFT_BOTTOM));
	Result.PushBack(GetCorner(NEAR_LEFT_TOP));
	Result.PushBack(GetCorner(NEAR_RIGHT_TOP));
	Result.PushBack(GetCorner(NEAR_RIGHT_BOTTOM));

	Result.PushBack(GetCorner(FAR_RIGHT_TOP));
	Result.PushBack(GetCorner(FAR_LEFT_TOP));
	Result.PushBack(GetCorner(FAR_LEFT_BOTTOM));
	Result.PushBack(GetCorner(FAR_RIGHT_BOTTOM));

	return Result;
}

Point AxisAlignedBoundingBox::GetSize() const
{
	return (Maximum - Minimum);
}

Point AxisAlignedBoundingBox::GetMinimum() const
{
	return Minimum;
}

Point AxisAlignedBoundingBox::GetMaximum() const
{
	return Maximum;
}

void AxisAlignedBoundingBox::SetExtents(const Point & inMin, const Point & inMax)
{
	//assert((min.x <= max.x && min.y <= max.y && min.z <= max.z) &&
	//	"The minimum corner of the box must be less than or equal to maximum corner");

	//mExtent = EXTENT_FINITE;
	Minimum = inMin;
	Maximum = inMax;
}

uint8 AxisAlignedBoundingBox::Contains(const AxisAlignedBoundingBox & Other) const
{
	return Minimum.X <= Other.Minimum.X && Maximum.X >= Other.Maximum.X &&
		Minimum.Y <= Other.Minimum.Y && Maximum.Y >= Other.Maximum.Y &&
		Minimum.Z <= Other.Minimum.Z && Maximum.Z >= Other.Maximum.Z;
}
