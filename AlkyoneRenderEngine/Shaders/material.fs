#version 330 core
#define NR_POINT_LIGHTS 1

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
	vec3 Normal;  
}; 


uniform Material material;


out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;


struct Light {
    vec3 position;  
	vec3 direction;

	float cutOff;
	float outerCutOff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
}; 

uniform Light light;

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight; 

struct PointLight {    
    vec3 position;
    float intensity;
    float constant;
    float linear;
    float quadratic;  

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};  

uniform PointLight pointLights[NR_POINT_LIGHTS];

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
    // properties
    vec3 norm = normalize(material.Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    // phase 1: directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // phase 2: point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
    // phase 3: spot light
   // result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    
    FragColor = vec4(result, 1.0);
}



vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(texture_diffuse1, TexCoords)); //
    vec3 diffuse  =  diff * vec3(texture(texture_diffuse1, TexCoords)); //light.diffuse  *
    //vec3 specular =  spec * vec3(texture(material.specular, TexCoords)); //light.specular *
    return (ambient + diffuse); //+ specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	light.intensity = 1.0;
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance	= length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  *  vec3(texture(material.diffuse, TexCoords)); 
    vec3 diffuse  =  diff * vec3(texture(texture_diffuse1, TexCoords)); //light.diffuse  *
   // vec3 specular =  spec * vec3(texture(material.specular, TexCoords)); //light.specular *
    ambient  *= attenuation;
    diffuse  *= attenuation;
    //specular *= attenuation;
    return (ambient + diffuse)* light.intensity;
}			//+ specular) 