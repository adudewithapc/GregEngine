#pragma once

#include <glad/glad.h>
#include <string>

class Shader
{
public:

	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();
	void Use();
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;

	unsigned int programID;
};

