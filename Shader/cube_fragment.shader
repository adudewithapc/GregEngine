#version 330 core

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 fragPos;
in vec3 normal;
in vec3 lightViewPos;

uniform Light light;
uniform Material material;

uniform vec3 objectColor;

out vec4 fragColor;

void main()
{
	//Ambient lighting
	vec3 ambient = light.ambient * material.ambient;

	//Diffuse lighting
	vec3 norm = normalize(normal);
	vec3 lightDirection = normalize(lightViewPos - fragPos);
	float diffuseAmount = max(dot(norm, lightDirection), 0);
	vec3 diffuse = light.diffuse * diffuseAmount * material.diffuse;

	//Specular lighting
	vec3 viewDirection = normalize(-fragPos);
	vec3 reflectionDirection = reflect(-lightDirection, norm);

	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0), material.shininess);
	vec3 specular = light.specular * specAmount * material.specular;

	//Result
	vec3 result = (ambient + diffuse + specular) * objectColor;

	fragColor = vec4(result, 1);
}