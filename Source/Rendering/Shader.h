#pragma once

#include <string>
#include "../Math/Vector.h"
#include "../Math/Matrix/Mat4x4.h"

struct Color;

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

	void SetVec4(const std::string& name, const Vec4f& value) const;
	void SetColor(const std::string& name, const Color& value) const;
	
	void SetMatrix4x4(const std::string& name, const Mat4x4f& matrix) const;

	unsigned int programID;
};

