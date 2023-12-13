#version 330 core

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light
{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

in vec2 texCoords;
in vec3 fragPos;
in vec3 normal;
in vec3 lightViewPos;

uniform Light light;
uniform Material material;

out vec4 fragColor;

void main()
{
	//Ambient lighting
	//vec3 ambient = light.ambient * material.ambient;
	vec3 ambient = light.ambient * texture(material.diffuse, texCoords).rgb;

	//Diffuse lighting
	vec3 norm = normalize(normal);
	vec3 lightDirection = normalize(lightViewPos - fragPos);
	float diffuseAmount = max(dot(norm, lightDirection), 0);
	//vec3 diffuse = light.diffuse * diffuseAmount * material.diffuse;
	vec3 diffuse = light.diffuse * diffuseAmount * texture(material.diffuse, texCoords).rgb;

	//Specular lighting
	vec3 viewDirection = normalize(-fragPos);
	vec3 reflectionDirection = reflect(-lightDirection, norm);

	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0), material.shininess);
	vec3 specular = light.specular * specAmount * texture(material.specular, texCoords).rgb;

	//Attenuation
	float distance = length(light.position - fragPos);
	//float distance = 2;
	float attentuation = 1 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	ambient *= attentuation;
	diffuse *= attentuation;
	specular *= attentuation;

	//Result
	vec3 result = ambient + diffuse + specular;

	//Bonus result with emission map
	//vec3 result = ambient + diffuse + specular + texture(material.emission, texCoords).rgb;

	fragColor = vec4(result, 1);
}