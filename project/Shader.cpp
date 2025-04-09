#include "Shader.h"

#include <glad/glad.h>

#include <iostream>

#include "glm/ext.hpp"

namespace {
	bool CompileShader(int shader)
	{
		glCompileShader(shader);
		int success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
			return false;
		}
		return true;
	}
}  // namespace

Shader::Shader(const std::string_view vertexShaderName, const std::string_view fragmentShaderName)
{

	// Vertex Shader erstellen und kompilieren
	vertexID = glCreateShader(GL_VERTEX_SHADER);
	const char* vShaderCode = vertexShaderName.data();
	glShaderSource(vertexID, 1, &vShaderCode, nullptr);
	glCompileShader(vertexID);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexID, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
	}
	

	// Fragment Shader erstellen und kompilieren
	fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fShaderCode = fragmentShaderName.data();
	glShaderSource(fragmentID, 1, &fShaderCode, nullptr);
	glCompileShader(fragmentID);
	glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentID, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << "\n";
	}

	// Shaderprogramm erstellen, Shader anhängen und linken
	ID = glCreateProgram();
	glAttachShader(ID, vertexID);
	glAttachShader(ID, fragmentID);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
	}
	
}

Shader::~Shader()
{
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
	glDeleteProgram(ID);
}

void Shader::use()
{
	// bind program
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	// set uniform bool
	glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	// set uniform float
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
	// set uniform vec3
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setMat4x4(const std::string& name, const glm::mat4x4& value) const
{
	// set uniform mat4x4
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

unsigned int Shader::GetProgramId() const
{
	return ID;
}

unsigned int Shader::GetVertexShaderId() const
{
	return vertexID;
}

unsigned int Shader::GetFragmentShaderId() const
{
	return fragmentID;
}
