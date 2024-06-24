#version 450 core
out vec4 FragColor;


struct Material {
  //  sampler2D diffuse;
  vec3 diffuse;
  vec3      specular;
  float     shininess;
}; 

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

//in vec3 Color;
in vec3 FragmentPosition;
in vec3 Normal;
in vec2 Texture;

uniform Light light;  
uniform vec3 viewPos; 
uniform Material material;



void main()
{

	vec3 lightColor = vec3(1.0f,1.0f,1.0f);

    // ambient
   
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragmentPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    
    
    // specular
    vec3 viewDir = normalize(viewPos - FragmentPosition);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
         
	//// spotlight (soft edges)
   // float theta = dot(lightDir, normalize(-light.direction)); 
  //  float epsilon = (light.cutOff - light.outerCutOff);
  //  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
   // diffuse  *= intensity;
   // specular *= intensity;
    
    // attenuation
    float distance    = length(light.position - FragmentPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
	vec3 ambient = light.ambient * material.diffuse;
	vec3 diffuse = light.diffuse * diff * material.diffuse;
	vec3 specular = light.specular * spec * lightColor;  
	
	
	ambient  *= attenuation; 
    diffuse   *= attenuation;
    specular *= attenuation;   

    vec3 result = (ambient + diffuse + specular) ;
    FragColor = vec4(result, 1.0);

	//FragColor = vec4(vec3(gl_FragCoord.z), 1.0);

} 