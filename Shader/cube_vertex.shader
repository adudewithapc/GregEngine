#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform vec3 lightPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragPos;
out vec3 normal;
out vec3 lightViewPos;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1);
	fragPos = vec3(view * model * vec4(aPos, 1));
	lightViewPos = vec3(view * vec4(lightPos, 1));
	//Recommended to do this on the CPU, since right now this will be done per vertex
	//We do this in order to ignore the translation matrix, which might affect the normal direction
	normal = mat3(transpose(inverse(view * model))) * aNormal;
}