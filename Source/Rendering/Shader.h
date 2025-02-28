#pragma once

#include <glad.h>
#include <string>
#include "../Math/Vector.h"

class Shader
{
public:

	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();
	
	void Use();
	
	void SetBool(const std::string& name, const bool value) const;
	void SetInt(const std::string& name, const int value) const;
	void SetFloat(const std::string& name, const float value) const;
	void SetVec3(const std::string& name, const Vec3f& value) const;
	void SetVec3(const std::string& name, const float x, const float y, const float z) const;

	unsigned int programID;
};

