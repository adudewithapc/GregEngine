#pragma once

#include "Shader.h"

class TriangleRenderer
{
public:
	TriangleRenderer(const float vertices[32], const unsigned int indices[6]);
	~TriangleRenderer();

	void Draw();
private:

	Shader* shader;

	float vertices[32];
	unsigned int indices[6];

	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	unsigned int texture1;
	unsigned int texture2;

	float mixAmount = 0.2f;
};