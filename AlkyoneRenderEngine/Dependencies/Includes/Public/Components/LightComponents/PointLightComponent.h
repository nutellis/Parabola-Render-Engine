#pragma once

#ifndef POINT_LIGHT_COMPONENT_H
#define POINT_LIGHT_COMPONENT_H

#include <Components/Components.h>

class PPointLightComponent : public PSceneComponent
{
public:
	PPointLightComponent(RRenderActor* Parent);

	PPointLightComponent(PSceneComponent* Default);

	~PPointLightComponent();

	void SetupShaderLight(Shader* ActiveShader);

	PointLightAttributes* Attributes;																			//EEEEEEEEEEEEEE OPOINTLIGHTATTRIBUTES
};



#endif // !POINT_LIGHT_COMPONENT_H