#pragma once


#include <ParabolaMath.h>

#include <Utilities/Containers/Array.h>

typedef Vector3f Point;


/** A 3D box aligned with the x/y/z axes.

This class represents a simple box which is aligned with the
axes. Internally it only stores 2 points as the extremeties of
the box, one which is the minima of all 3 axes, and the other
which is the maxima of all 3 axes. This class is typically used
for an axis-aligned bounding box (AABB) for collision and
visibility determination.
*/

typedef class AxisAlignedBoundingBox
{
public:
	AxisAlignedBoundingBox();
	~AxisAlignedBoundingBox();

private:

	Point Center;
	Point HalfWidth;


	Point Minimum;
	Point Maximum;

	/*
		   5-------4
		  /|      /|
		 / |     / |
		1-------2  |
		|  6----|--7
		| /     | /
		|/      |/
		0-------3
	*/
	enum CornerEnum {
		FAR_LEFT_BOTTOM = 6,
		FAR_LEFT_TOP = 5,
		FAR_RIGHT_TOP = 4,
		FAR_RIGHT_BOTTOM = 7,
		NEAR_RIGHT_BOTTOM = 3,
		NEAR_LEFT_BOTTOM = 0,
		NEAR_LEFT_TOP = 1,
		NEAR_RIGHT_TOP = 2
	};

	typedef TArray<Point> Corners;
	

public:

	Point GetCorner(CornerEnum cornerToGet) const;

	// Gets the centre of the box
	Point GetCenter(void) const;

	Corners GetAllCorners(void) const;

	Point GetSize() const;

	Point GetMinimum() const;

	Point GetMaximum() const;


	/** Sets both minimum and maximum extents at once.
*/
	void SetExtents(const Point & inMin, const Point & inMax);


	// Returns whether or not this box intersects another.
	uint8 Intersects(const AxisAlignedBoundingBox& b2) const;

	// Returns whether or not this box intersects with a point.
	uint8 Intersects(const Point& s) const;

	//uint8 Intersects(const Sphere& s) const;
	//uint8 Intersects(const Plane& s) const;



	// Tests whether another box contained by this box.
	uint8 Contains(const AxisAlignedBoundingBox& Other) const;

	// Tests whether the given point contained by this box.
	//uint8 Contains(const Point & v) const


	//OPERATORS



}AABB;
