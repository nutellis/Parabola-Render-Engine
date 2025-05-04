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

	Vector3f LightDirection;

	//ArrowShape DirectionArrow;

public:
	PDirectionalLightComponent(PRenderActor* Parent);
	PDirectionalLightComponent(PSceneComponent* Default);
	~PDirectionalLightComponent();
	void SetupShaderLight(Shader* ActiveShader);
	void SetDirection();
};

#endif // !DIRECTIONAL_LIGHT_COMPONENT_H