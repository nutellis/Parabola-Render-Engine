#include "Components/Lights.h"
/*
Shader Light::SetShader(Shader shader, int index)
{
	return shader;
}

Light::Light() : Object()
{
	//StaticMesh = Model();
	color = glm::vec3(1.0f);
	intensity = 1.0f;
}

Light::Light(float inten, glm::vec3 col) : Object()
{
	//StaticMesh = Model();
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
	color = Vector3f(1.0f);
	intensity = 1.0f;

	constant = 1.0f;
	linear = 0.09f;
	quadratic = 0.032f;

	emmitsDiffuse = true;
	emmitsSpecular = true;
}

PointLightAttributes::PointLightAttributes(Vector3f pos, float cons, float lin, float quad,float inten)
{
	constant = cons;
	linear = lin;
	quadratic = quad;

	emmitsDiffuse = true;
	emmitsSpecular = true;

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