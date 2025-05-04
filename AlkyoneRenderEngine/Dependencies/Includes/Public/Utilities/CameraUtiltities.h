#pragma once
#ifndef CAMERA_UTILITIES_H
#define CAMERA_UTILITIES_H

#include <ParabolaMath.h>


inline Matrix4f LookAt(const Vector3f& Eye, const Vector3f& Center, const Vector3f& Up = Vector3f(0.0f, 1.0f, 0.0f))
{
	Vector3f F = Normalize(Center - Eye);
	Vector3f R = Normalize(Cross(F, Up));
	Vector3f U = (Cross(R,F));

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

	Result[3][0] = -Dot(R, Eye);
	Result[3][1] = -Dot(U, Eye);
	Result[3][2] = Dot(F, Eye);

	return Result;
}

inline Matrix4f Ortho(
	const float& Left, const float& Right,
	const float& Top, const float& Bottom,
	const float& ZNear = 0.1f, const float& ZFar = 100.0f
)
{
	Matrix4f Result = Matrix4f::IDENTITY;

	Result[0][0] = 2.0f / (Right - Left);
	Result[1][1] = 2.0f / (Top - Bottom);
	Result[2][2] = -2.0f / (ZFar - ZNear);

	Result[3][0] = -(Right + Left) / (Right - Left);
	Result[3][1] = -(Top + Bottom) / (Top - Bottom);
	Result[3][2] = - (ZFar + ZNear) / (ZFar - ZNear);

	return Result;
}

inline Matrix4f Perspective(
	const float& FieldOfView,
	const float& AspectRatio,
	const float& ZNear = 0.1f, 
	const float& ZFar = 100.0f
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
	return Result;

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

#endif //CAMERA_UTILITIES_H