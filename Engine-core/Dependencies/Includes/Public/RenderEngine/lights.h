#pragma once
#ifndef LIGHTS_H
#define LIGHTS_H

#include <GL\glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <RenderEngine/shader.h>
#include <Object.h>


/*
enum Light_Caster {
	LIGHT_POINT,
	LIGHT_SPOT,
	DIRECTIONAL
}; */

/*
class Light : public Object
{

public:
	glm::vec3 color;
	
	Shader SetShader(Shader shader, int index) = 0;
	Light();
	Light(float inten, glm::vec3 col = glm::vec3(1.0f));
	float intensity;
	void Draw(Shader shader);

};

class DirectionalLight : public Light
{

public:
	glm::vec3 direction;

	
};
*/
class PointLightAttributes
{
public:

	glm::vec3 color;
	float intensity;

	float constant;
	float linear;
	float quadratic;

	bool emmitsDiffuse;
	bool emmitsSpecular;

	//visual aspect

	PointLightAttributes();
	PointLightAttributes(glm::vec3 pos, float cons, float lin, float quad,float inten = 1.0f);
	void SetShader(Shader shader,glm::vec3 position, int index = 0);
};

#endif // !LIGHTS_H




