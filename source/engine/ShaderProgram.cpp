// Terrain Viewer
//
// Renderer, file: Renderer.cpp
//
// - Implementation of the Renderer class.
//
// Copyright (c) César González Segura 2015, ETSE-UV
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library.
//
#include "ShaderProgram.h"

ShaderProgram::ShaderProgram()
{
    _programId = glCreateProgram();
	_active = false;
	_built = false;
}

ShaderProgram::~ShaderProgram()
{
    if (glIsProgram(_programId) == GL_TRUE)
    {
        glDeleteProgram(_programId);
    }
}

void ShaderProgram::AttachShader(const Shader* shader)
{
    _shaders.push_back((Shader*) shader);
}

void ShaderProgram::Build()
{
    assert(glIsProgram(_programId) != GL_FALSE);

    for (unsigned int i = 0; i < _shaders.size(); i++)
    {
        glAttachShader(_programId, _shaders[i]->GetShaderId());
    }

    glLinkProgram(_programId);

    GLint compilationResult = GL_FALSE;
    int compilationLogLength = 0;

    glGetProgramiv(_programId, GL_LINK_STATUS, &compilationResult);
    glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &compilationLogLength);

    boost::scoped_array<char> log(new char[compilationLogLength]);
    glGetShaderInfoLog(_programId, compilationLogLength, NULL, log.get());

	/* Complain if the shader program didn't link successfully */
    if (compilationResult == GL_FALSE)
    {
		std::string err = "Shader program link failed:\n\n" + std::string(log.get());
        Debug::Log(err, __FILE__, __LINE__);
        throw std::runtime_error(err);
    }
    else
    {
        Debug::Log("Shader program link succeeded", __FILE__, __LINE__);
		_built = true;
    }

	RetrieveUniformLocations();
}

void ShaderProgram::SetActive(bool ena)
{
	if (!_built)
	{
		std::string msg = "The requested shader has not been compiled successfully";
		Debug::Log(msg, __FILE__, __LINE__);
		throw std::runtime_error(msg);
	}

	_active = ena;

	if (_active)
	{
		glUseProgram(_programId);
	}
	else
	{
		glUseProgram(0);
	}
}

std::map<std::string, GLuint>::iterator ShaderProgram::GetIteratorAt(const std::string & name)
{
	if (!_active)
	{
		std::string msg = "The requested shader is not active";
		Debug::Log(msg, __FILE__, __LINE__);
		throw std::runtime_error(msg);
	}

	std::map<std::string, GLuint>::iterator it;
	it = _uniformLocations.find(name);

	if (it == _uniformLocations.end())
	{
		std::string msg = name + " uniform variable not found in shader";
		Debug::Log(msg, __FILE__, __LINE__);
		throw std::runtime_error(msg);
	}
	else
	{
		return it;
	}
}

void ShaderProgram::SetUniformFloat(const std::string & name, float val)
{
	GLuint id = GetIteratorAt(name)->second;
	glUniform1f(id, val);
}

void ShaderProgram::SetUniformInteger(const std::string & name, int val)
{
	GLuint id = GetIteratorAt(name)->second;
	glUniform1i(id, val);
}

void ShaderProgram::SetUniformVector(const std::string & name, const glm::vec2 & vec)
{
	GLuint id = GetIteratorAt(name)->second;
	glUniform2f(id, vec.x, vec.y);
}
	
void ShaderProgram::SetUniformVector(const std::string & name, const glm::vec3 & vec)
{
	GLuint id = GetIteratorAt(name)->second;
	glUniform3f(id, vec.x, vec.y, vec.z);
}

void ShaderProgram::SetUniformVector(const std::string & name, const glm::vec4 & vec)
{
	GLuint id = GetIteratorAt(name)->second;
	glUniform4f(id, vec.x, vec.y, vec.z, vec.w);
}

void ShaderProgram::SetUniformTexture(const std::string & name, Texture* tex, int index)
{
	GLint maxUnits;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &maxUnits);

	if (GL_TEXTURE0 + index > maxUnits + GL_TEXTURE0)
	{
		std::string msg = "Exceeded maximum count of texture units: " +
			boost::lexical_cast<std::string>(index) + " / " +
			boost::lexical_cast<std::string>(maxUnits);

		Debug::Log(msg, __FILE__, __LINE__);
		throw std::runtime_error(msg);
	}

	GLuint id = GetIteratorAt(name)->second;
	
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, tex->GetTextureId());
	glUniform1i(id, index);
}

void ShaderProgram::SetUniformMatrix(const std::string & name, const glm::mat2 & mat, bool transpose)
{
	GLuint id = GetIteratorAt(name)->second;
	glUniformMatrix2fv(id, 1, transpose, &mat[0][0]);
}

void ShaderProgram::SetUniformMatrix(const std::string & name, const glm::mat3 & mat, bool transpose)
{
	GLuint id = GetIteratorAt(name)->second;
	glUniformMatrix3fv(id, 1, transpose, &mat[0][0]);
}

void ShaderProgram::SetUniformMatrix(const std::string & name, const glm::mat4 & mat, bool transpose)
{
	GLuint id = GetIteratorAt(name)->second;
	glUniformMatrix4fv(id, 1, transpose, &mat[0][0]);
}

void ShaderProgram::RetrieveUniformLocations()
{
	GLint uniformCount;

	glGetProgramiv(_programId, GL_ACTIVE_UNIFORMS, &uniformCount);
	GLchar name[128];

	for (GLint i = 0; i < uniformCount; i++)
	{
		GLint arraySize = 0;
		GLenum type = 0;
		GLsizei actualLength = 0;
		glGetActiveUniform(_programId, i, 128, &actualLength, &arraySize, &type, name);

		GLuint location = glGetUniformLocation(_programId, name);
		_uniformLocations.insert(std::pair<std::string, GLuint>(name, location));
	}
}

GLuint ShaderProgram::GetProgramId() const
{
    return _programId;
}
