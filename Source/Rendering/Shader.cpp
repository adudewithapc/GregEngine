#include "Shader.h"

#include <format>
#include <fstream>
#include <sstream>
#include <glad.h>

#include "../Debugging/Log.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	//Retrieve shader code from path
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch(std::ifstream::failure e)
	{
		greg::log::Fatal("Shader", std::format("Failed to read shader file! Error: {}", e.what()));
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);

	int success;
	char infoLog[512];
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		greg::log::Fatal("Shader", std::format("Failed to compile vertex shader! Error: {}", infoLog));
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
		greg::log::Fatal("Shader", std::format("Failed to compile fragment shader! Error: {}", infoLog));
	}

	programID = glCreateProgram();
	glAttachShader(programID, vertex);
	glAttachShader(programID, fragment);
	glLinkProgram(programID);

	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(programID, 512, nullptr, infoLog);
		greg::log::Fatal("Shader", std::format("Failed to link shader program! Error: {}", infoLog));
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader()
{
	glDeleteProgram(programID);
}

void Shader::Use()
{
	glUseProgram(programID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::SetVec3(const std::string& name, const Vec3f& value) const
{
	float vec3[] = { value.x, value.y, value.z };
	glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, &vec3[0]);
}

void Shader::SetVec3(const std::string& name, const float x, const float y, const float z) const
{
	float vec3[] = { x, y, z };
	glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, &vec3[0]);
}
