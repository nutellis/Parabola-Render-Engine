#pragma once
#ifndef LIGHTS_H
#define LIGHTS_H

#include <Core/RenderCore.h>

#include <ParabolaMath.h>

enum Light_Caster {
	LIGHT_POINT,
	LIGHT_SPOT,
	LIGHT_DIRECTIONAL
}; 


//Phong Model
class Light 
{

public:
	Vector3f color;
	
	//Shader SetShader(Shader shader, int index) = 0;
	Light();
	//Light(float inten, Vector3f col = Vector3f(1.0f));
	float intensity;
	

};

class DirectionalLight : public Light
{

public:
	Vector3f direction;	
};

class PointLightAttributes
{
public:

	Vector3f color;
	float intensity;

	float constant;
	float linear;
	float quadratic;

	bool emmitsDiffuse;
	bool emmitsSpecular;

	//visual aspect

	PointLightAttributes();
	PointLightAttributes(Vector3f pos, float cons, float lin, float quad,float inten = 1.0f);
	//void SetShader(Shader shader,Vector3f position, int index = 0);
};

#endif // !LIGHTS_H




