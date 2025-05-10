#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_PointSize = 1.0;
	gl_Position = projection * view * model * vec4(0, 0, 0, 1);
}