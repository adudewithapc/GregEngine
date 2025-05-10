#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int pixelSize;

void main()
{
	gl_PointSize = pixelSize;
	gl_Position = projection * view * model * vec4(0, 0, 0, 1);
}