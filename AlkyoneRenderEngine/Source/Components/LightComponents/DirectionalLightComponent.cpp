#include "Components/LightComponents/DirectionalLightComponent.h"
#include <Components/RenderActor.h>

#include <Components/Shader.h>

PDirectionalLightComponent::PDirectionalLightComponent(PRenderActor* Parent)
{
	std::cout << "This is a DirectionalLightComponent\n";

	this->Parent = Parent;

	Intensity = 1.0f;

	LightColour = Vector3f(1.0, 1.0, 1.0);

	IndirectContribution = 1.0;

	SetDirection();
	//LightPosition = Rotate(Azimuth, Vector3f(0, 1, 0)) * Rotate(Zenith, Vector3f(0, 0, 1));
}

PDirectionalLightComponent::PDirectionalLightComponent(PSceneComponent* Default)
{
}

PDirectionalLightComponent::~PDirectionalLightComponent()
{
}

void PDirectionalLightComponent::SetupShaderLight(Shader* ActiveShader) {

	//// be sure to activate shader when setting uniforms/drawing objects
	//ActiveShader->setVec3("light.position", Parent->ObjectPosition);
	////gShaderManager.GetShader("SimpleShader")->setVec3("light.position", Vector3f(lightX, 1.5f, lightZ));

	//ActiveShader->setFloat("light.cutOff", SMath::Cos(DegreesToRadians(Attributes->CutOff)));
	//ActiveShader->setFloat("light.outerCutOff", SMath::Cos(DegreesToRadians(Attributes->OuterCutOff)));
	//// light properties
	//ActiveShader->setVec3("light.ambient", Attributes->Ambient);
	//// we configure the diffuse intensity slightly higher; the right lighting conditions differ with each lighting method and environment.
	//// each environment and lighting type requires some tweaking to get the best out of your environment.
	//ActiveShader->setVec3("light.diffuse", Attributes->Diffuse);
	//ActiveShader->setVec3("light.specular", Attributes->Specular); // add to attributes
	//ActiveShader->setFloat("light.constant", Attributes->Constant);
	//ActiveShader->setFloat("light.linear", Attributes->Linear);
	//ActiveShader->setFloat("light.quadratic",Attributes->Quadratic);



	
	ActiveShader->SetVec3("LightPosition", Parent->ObjectPosition);
	ActiveShader->SetVec3("LightDirection", Normalize(LightDirection));

	float environment_multiplier = 1.3f;
	ActiveShader->SetFloat("environment_multiplier", environment_multiplier);

	/*labhelper::setUniformSlow(currentShaderProgram, "point_light_position", lightPosition);
	labhelper::setUniformSlow(currentShaderProgram, "point_light_color", point_light_color);
	labhelper::setUniformSlow(currentShaderProgram, "point_light_intensity_multiplier",
		point_light_intensity_multiplier);
	labhelper::setUniformSlow(currentShaderProgram, "viewSpaceLightPosition", vec3(viewSpaceLightPosition));
	labhelper::setUniformSlow(currentShaderProgram, "viewSpaceLightDir",
		normalize(vec3(viewMatrix * vec4(-lightPosition, 0.0f))));*/


}

void PDirectionalLightComponent::SetDirection()
{
	float AzimuthRad = DegreesToRadians(Azimuth);
	float ZenithRad = DegreesToRadians(Zenith);

	float X = sin(ZenithRad) * cos(AzimuthRad);
	float Y = cos(ZenithRad);
	float Z = sin(ZenithRad) * sin(AzimuthRad);

	LightDirection = Normalize(Vector3f(X, Y, Z));
}
