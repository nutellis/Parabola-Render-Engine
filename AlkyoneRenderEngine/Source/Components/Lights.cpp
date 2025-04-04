#include "Components/Lights.h"
/*
Shader Light::SetShader(Shader shader, int index)
{
	return shader;
}

Light::Light() : Object()
{
	//StaticMesh = ModelMatrix();
	color = glm::vec3(1.0f);
	intensity = 1.0f;
}

Light::Light(float inten, glm::vec3 col) : Object()
{
	//StaticMesh = ModelMatrix();
	color = col;
	intensity = inten;
}

void Light::Draw(Shader shader)
{
	//StaticMesh.Draw(shader);
}


*/
PointLightAttributes::PointLightAttributes()
{
	Diffuse = Vector3f(1.0f);
	Intensity = 1.0f;

	Constant = 1.0f;
	Linear = 0.09f;
	Quadratic = 0.032f;

	EmmitsDiffuse = true;
	EmmitsSpecular = true;
}

PointLightAttributes::PointLightAttributes(Vector3f pos, float cons, float lin, float quad,float inten)
{
	Constant = cons;
	Linear = lin;
	Quadratic = quad;

	EmmitsDiffuse = true;
	EmmitsSpecular = true;

}

//void PointLightAttributes::SetShader(Shader shader,glm::vec3 position,int index)
//{
//
//	//shader.setVec3("pLight.position", position);
//	shader.setFloat("pLight.constant", this->constant);
//	shader.setFloat("pLight.linear", this->linear);
//	shader.setFloat("pLight.quadratic", this->quadratic);
//	shader.setFloat("material.shininess", 32.0f);
//	
//	//pointLights[" + std::to_string(index) + "].
//
//	//return shader;
//}