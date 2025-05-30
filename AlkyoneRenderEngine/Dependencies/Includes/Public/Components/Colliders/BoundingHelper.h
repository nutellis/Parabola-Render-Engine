#pragma once

#include <Components/Colliders/AxisAlignedBoundingBox.h>
#include <ParabolaMath.h>

namespace BoundingHelper {

	inline PAxisAlignedBoundingBox* TransformAABB(const PBoundingBox* Box, const Matrix4f Transform) {

		Vector3f Min(FLT_MAX, FLT_MAX, FLT_MAX);
		Vector3f Max(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		for (Vector3f Corner : Box->Corners) {
			Vector4f Transformed = Transform * Vector4f(Corner, 1.0f);
			Vector3f P = Transformed / Transformed.W; // it results to Transformed for non projection changes

			Min.X = SMath::Min(Min.X, P.X);
			Max.X = SMath::Max(Max.X, P.X);
			Min.Y = SMath::Min(Min.Y, P.Y);
			Max.Y = SMath::Max(Max.Y, P.Y);
			Min.Z = SMath::Min(Min.Z, P.Z);
			Max.Z = SMath::Max(Max.Z, P.Z);
		}

		AABB * newAABB = new AABB(Min, Max);

		return newAABB;
	}

	inline PAxisAlignedBoundingBox* CalculateAABBFromCorners(TArray<Vector3f> Corners)
	{
		PAxisAlignedBoundingBox* NewAABB = new AABB();
		NewAABB->Min = Vector3f(std::numeric_limits<float>::infinity());
		NewAABB->Max = Vector3f(-std::numeric_limits<float>::infinity());
		for (int i = 0; i < Corners.Size(); i++) {

			NewAABB->Min.X = SMath::Min(NewAABB->Min.X, Corners[i].X);
			NewAABB->Max.X = SMath::Max(NewAABB->Max.X, Corners[i].X);
			NewAABB->Min.Y = SMath::Min(NewAABB->Min.Y, Corners[i].Y);
			NewAABB->Max.Y = SMath::Max(NewAABB->Max.Y, Corners[i].Y);
			NewAABB->Min.Z = SMath::Min(NewAABB->Min.Z, Corners[i].Z);
			NewAABB->Max.Z = SMath::Max(NewAABB->Max.Z, Corners[i].Z);
		}
		NewAABB->Corners = Utilities::Move(NewAABB->GetCornersFromMinMax());
		NewAABB->Center = NewAABB->Max.MidPoint(NewAABB->Min);

		return NewAABB;
	}

	inline PBoundingBox* Transform(const PBoundingBox* Box, const Matrix4f Transform) {

		TArray<Vector3f> Corners = TArray<Vector3f>(8);
		for (Vector3f Corner : Box->Corners) {
			Vector4f Transformed = Transform * Vector4f(Corner, 1.0f);
			Vector3f P = Transformed / Transformed.W; // it results to Transformed for non projection changes
			Corners.PushBack(P);
		}

		PBoundingBox* NewBox = new PBoundingBox(Corners);

		return NewBox;
	}

	inline PAxisAlignedBoundingBox* Transform(const PAxisAlignedBoundingBox* Box, const Matrix4f Transform) {

		TArray<Vector3f> Corners = TArray<Vector3f>(8);
		for (Vector3f Corner : Box->Corners) {
			Vector4f Transformed = Transform * Vector4f(Corner, 1.0f);
			Vector3f P = Transformed / Transformed.W; // it results to Transformed for non projection changes
			Corners.PushBack(P);
		}

		PAxisAlignedBoundingBox* NewBox = Utilities::Move(CalculateAABBFromCorners(Corners));

		return NewBox;
	}

	inline AABB * UnionAABB(const TArray<AABB *> Objects) {
		Vector3f Min = Objects[0]->Min;
		Vector3f Max = Objects[0]->Max;
		for (size_t i = 1; i < Objects.Size(); ++i) {
			Min.X = SMath::Min(Min.X, Objects[i]->Min.X);
			Max.X = SMath::Max(Max.X, Objects[i]->Max.X);
			Min.Y = SMath::Min(Min.Y, Objects[i]->Min.Y);
			Max.Y = SMath::Max(Max.Y, Objects[i]->Max.Y);
			Min.Z = SMath::Min(Min.Z, Objects[i]->Min.Z);
			Max.Z = SMath::Max(Max.Z, Objects[i]->Max.Z);
		}
		AABB* NewAABB = new AABB(Min, Max);
		return NewAABB;
	}
}

