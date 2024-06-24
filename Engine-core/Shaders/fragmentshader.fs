#version 450 core

out vec4 FragColor;


struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 
  
struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
  
in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

uniform vec3 viewPos;
uniform PointLight pLight;
uniform Material material;
uniform vec3 objectColor;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


void main()
{
	vec3 lightColor = vec3(1.0,1.0,1.0);
	float specularStrength = 0.8;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(pLight.position - FragPos);  
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  

	float ambientStrength = 0.10;
    vec3 ambient = ambientStrength * lightColor;
	
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;  

    vec3 result = (diffuse + ambient + specular) * objectColor;
    FragColor = vec4(result, 1.0);

}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(0.3,0.5,0.4); //vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(0.3,0.5,0.4);// vec3(texture(material.diffuse, TexCoords));
  //  vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
   // specular *= attenuation;
    return (ambient + diffuse); // + specular);
}		