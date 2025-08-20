#pragma once
#ifndef DIRECTIONAL_LIGHT_COMPONENT_H
#define DIRECTIONAL_LIGHT_COMPONENT_H

#include <Components/Components.h>
#include <Utilities/PrimitiveShapes.h>


class PDirectionalLightComponent : public PSceneComponent
{
public:
	float Intensity;
	Vector3f LightColour;

	Vector3f Angle;

	float Azimuth = 0.f;
	float Zenith = 45.f;

	//float Distance = 55.0f;

	float IndirectContribution;

	Vector4f LightDirection = Vector4f(1.0, 0.0, 0.0, 0.0);

	float LightDistance = 50.0f;

	//ArrowShape DirectionArrow;

	Matrix4f LightProjectionMatrix;
	Matrix4f LightViewMatrix;
	Matrix4f LightViewProjectionMatrix;

	float FieldOfView = 90.0f;
	float AspectRation = 1.7779;
	float ZNear = 0.5f;
	float ZFar = 50.0f;

public:
	PDirectionalLightComponent(RRenderActor* Parent);
	PDirectionalLightComponent(PSceneComponent* Default);
	~PDirectionalLightComponent();
	void SetupShaderLight(Shader* ActiveShader, Matrix4f ViewMatrix);
	void SetDirection();

	void UpdateLight(Vector3f Center);

};

#endif // !DIRECTIONAL_LIGHT_COMPONENT_H