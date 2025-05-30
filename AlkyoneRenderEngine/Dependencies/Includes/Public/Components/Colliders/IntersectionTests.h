#pragma once
#ifndef INTERSECTION_TESTS_H
#define INTERSECTION_TESTS_H
#include <Core/RenderCore.h>

#include <ParabolaMath.h>

#include <Components/Colliders/AxisAlignedBoundingBox.h>


inline bool IntersectionTest(const AABB * ObjectBB, const AABB * FrustumBB)
{
	const Vector3f& FrustumMin = FrustumBB->Min;
	const Vector3f& FrustumMax = FrustumBB->Max;
	const Vector3f& ObjectMin = ObjectBB->Min;
	const Vector3f& ObjectMax = ObjectBB->Max;

	if (ObjectMin.X > FrustumMax.X || FrustumMin.X > ObjectMax.X) return false;
	if (ObjectMin.Y > FrustumMax.Y || FrustumMin.Y > ObjectMax.Y) return false;
	if (ObjectMin.Z > FrustumMax.Z || FrustumMin.Z > ObjectMax.Z) return false;

	return true;
}

inline bool SweepIntersectionTest(const AABB * ObjectBB, const AABB * FrustumBB, const Vector3f & SweepDir)
{
	const Vector3f FrustumMin = FrustumBB->Min;
	const Vector3f FrustumMax = FrustumBB->Max;
	const Vector3f ObjectMin = ObjectBB->Min;
	const Vector3f ObjectMax = ObjectBB->Max;

	// find centers and half sizes
	Vector3f FrustumCenter = FrustumBB->Center;
	Vector3f FrustumHalfSize = (FrustumMax - FrustumMin) * 0.5f;

	Vector3f ObjectCenter = ObjectBB->Center;
	Vector3f ObjectHalfSize = (ObjectMax - ObjectMin) * 0.5f;


	//project vectors to sweep direction
	float FrustumProj = Dot(FrustumCenter, SweepDir);
	float FrustrumHalfSizeProj = Dot(FrustumHalfSize, SMath::Abs(SweepDir));

	float FrustrumProjMin = FrustumProj - FrustrumHalfSizeProj;
	float FrustrumProjMax = FrustumProj + FrustrumHalfSizeProj;

	
	float ObjectProj = Dot(ObjectCenter, SweepDir);
	float ObjectHalfSizeProj = Dot(ObjectHalfSize, SMath::Abs(SweepDir));

	float ObjectProjMin = ObjectProj - ObjectHalfSizeProj;
	float ObjectProjMax = ObjectProj + ObjectHalfSizeProj;


	float MinDistance = FrustrumProjMin - ObjectProjMax;
	float MaxDistance = FrustrumProjMax - ObjectProjMin;
	if (MinDistance > MaxDistance) {
		Utilities::Swap(MinDistance, MaxDistance);
	}
	if (MaxDistance < 0.0f) {
		return false;
	}

	//SAT Algorith (Separating Axis Theorem)

	// X-Axis
	if (SweepDir.X == 0) {
		if (FrustumMin.X > ObjectMax.X || ObjectMin.X > FrustumMax.X) return false;
	}
	else {
		float MinDistX = (FrustumMin.X - ObjectMax.X) / SweepDir.X;
		float MaxDistX = (FrustumMax.X - ObjectMin.X) / SweepDir.X;
		if (MinDistX > MaxDistX) Utilities::Swap(MinDistX, MaxDistX);
		//TODO: check error here
		if (MinDistance > MaxDistX || MinDistX > MaxDistance) return false;
		// otherwise merge ranges
		MinDistance = SMath::Max(MinDistance, MinDistX);
		MaxDistance = SMath::Min(MaxDistance, MaxDistX);
	}

	// Y-Axis
	if (SweepDir.Y == 0) {
		if (FrustumMin.Y > ObjectMax.Y || ObjectMin.Y > FrustumMax.Y) return false;
	}
	else {
		float MinDistY = (FrustumMin.Y - ObjectMax.Y) / SweepDir.Y;
		float MaxDistY = (FrustumMax.Y - ObjectMin.Y) / SweepDir.Y;
		if (MinDistY > MaxDistY) Utilities::Swap(MinDistY, MaxDistY);

		if (MinDistance > MaxDistY || MinDistY > MaxDistance) return false;
		// otherwise merge ranges
		MinDistance = SMath::Max(MinDistance, MinDistY);
		MaxDistance = SMath::Min(MaxDistance, MaxDistY);
	}

	// Z-Axis
	if (SweepDir.Z == 0) {
		if (FrustumMin.Z > ObjectMax.Z || ObjectMin.Z > FrustumMax.Z) return false;
	}
	else {
		float MinDistZ = (FrustumMin.Z - ObjectMax.Z) / SweepDir.Z;
		float MaxDistZ = (FrustumMax.Z - ObjectMin.Z) / SweepDir.Z;
		if (MinDistZ > MaxDistZ) Utilities::Swap(MinDistZ, MaxDistZ);

		if (MinDistance > MaxDistZ || MinDistZ > MaxDistance) return false;

	}

	return true;
}





#endif