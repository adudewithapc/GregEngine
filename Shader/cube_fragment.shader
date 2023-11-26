#version 330 core

in vec3 fragPos;
in vec3 normal;
in vec3 lightViewPos;

uniform vec3 lightColor;
uniform vec3 objectColor;

out vec4 fragColor;

void main()
{
	//Ambient lighting
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	//Diffuse lighting
	vec3 norm = normalize(normal);
	vec3 lightDirection = normalize(lightViewPos - fragPos);
	float diffuseAmount = max(dot(norm, lightDirection), 0);
	vec3 diffuse = diffuseAmount * lightColor;

	//Specular lighting
	float specularStrength = 0.5;
	vec3 viewDirection = normalize(-fragPos);
	vec3 reflectionDirection = reflect(-lightDirection, norm);

	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0), 32);
	vec3 specular = specularStrength * specAmount * lightColor;

	//Result
	vec3 result = (ambient + diffuse + specular) * objectColor;

	fragColor = vec4(result, 1);
}