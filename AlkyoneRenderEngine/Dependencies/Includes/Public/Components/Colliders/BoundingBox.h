#pragma once
#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <ParabolaMath.h>
#include <Utilities/Containers/Array.h>

class PStaticMeshComponent;

enum CornerEnum {
	FAR_LEFT_BOTTOM = 6,
	FAR_LEFT_TOP = 4,
	FAR_RIGHT_TOP = 5,
	FAR_RIGHT_BOTTOM = 7,
	NEAR_RIGHT_BOTTOM = 3,
	NEAR_LEFT_BOTTOM = 2,
	NEAR_LEFT_TOP = 0,
	NEAR_RIGHT_TOP = 1
};

// A simple box class 
class PBoundingBox
{
public:
	PBoundingBox();
	PBoundingBox(const PBoundingBox& Other);
	PBoundingBox(PBoundingBox&& Other) noexcept;
	PBoundingBox(TArray<Vector3f> InCorners);
	PBoundingBox(Vector3f Min, Vector3f Max);
	~PBoundingBox();

	PBoundingBox& operator=(PBoundingBox&& Other) noexcept;

public:

	Vector3f Center;
	Vector3f HalfWidth;


	Vector3f Min;
	Vector3f Max;

	TArray<Vector3f> Corners;
	/*
		   4-------5
		  /|      /|
		 / |     / |
		0-------1  |
		|  6----|--7
		| /     | /
		|/      |/
		2-------3
	*/
	

	Vector3f DebugColour = Vector3f(0.0, 0.0, 1.0);
//private:
	PStaticMeshComponent* DebugFrustrumMesh;

public:

	virtual void CalculateFromCorners(TArray<Vector3f> Corners);

	void SetupDebugFrustrumEdges();

	void SetupDebugFrustrumPlanes();

	void RenderDebugBoundingBox(int RenderOption, Vector4f Colour, Matrix4f Projection, Matrix4f View, Matrix4f Model = Matrix4f::IDENTITY);

};
#endif