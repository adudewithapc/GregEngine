#version 330 core

uniform vec3 lightColor;
uniform vec3 objectColor;

out vec4 fragColor;

void main()
{
	fragColor = vec4(lightColor * objectColor, 1);
}