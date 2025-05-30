#pragma once
#ifndef AXISALIGNEDBOUNDINGBOX_H
#define AXISALIGNEDBOUNDINGBOX_H

#include <Components/Colliders/BoundingBox.h>

class PStaticMeshComponent;


/** A 3D box aligned with the x/y/z axes.

This class represents a simple box which is aligned with the
axes. Internally it only stores 2 points as the extremeties of
the box, one which is the minima of all 3 axes, and the other
which is the maxima of all 3 axes. This class is typically used
for an axis-aligned bounding box (AABB) for collision and
visibility determination.
*/

typedef class PAxisAlignedBoundingBox: public PBoundingBox
{
public:
	PAxisAlignedBoundingBox();
	PAxisAlignedBoundingBox(Vector3f Min, Vector3f Max);
	~PAxisAlignedBoundingBox();

public:

	Vector3f GetCorner(CornerEnum cornerToGet) const;

	void CalculateFromCorners(TArray<Vector3f> Corners) override;

	// Gets the centre of the box
	Vector3f GetCenter(void) const;

	TArray<Vector3f> GetAllCorners(void) const;

	Vector3f GetSize() const;

	Vector3f GetMinimum() const;

	Vector3f GetMaximum() const;

	TArray<Vector3f> GetCornersFromMinMax();

	/** Sets both minimum and maximum extents at once.
*/
	void SetExtents(const Vector3f & inMin, const Vector3f & inMax);


	// Returns whether or not this box intersects another.
	uint8 Intersects(const PAxisAlignedBoundingBox& b2) const;

	// Returns whether or not this box intersects with a point.
	uint8 Intersects(const Vector3f& s) const;

	//uint8 Intersects(const Sphere& s) const;
	//uint8 Intersects(const Plane& s) const;



	// Tests whether another box contained by this box.
	uint8 Contains(const PAxisAlignedBoundingBox& Other) const;


	// Tests whether the given point contained by this box.
	//uint8 Contains(const Point & v) const


	//OPERATORS


}AABB;
#endif