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
#ifdef WINDOWS_BUILD
		std::string caption = "Shader program link failed:\n\n" + std::string(log.get());
		MessageBox(GetActiveWindow(), caption.c_str(), "Error compilando shaders", MB_OK | MB_ICONERROR);
#endif

        Debug::Log("Shader program link failed:\n\n" + std::string(log.get()), __FILE__, __LINE__);
        throw std::exception();
    }
    else
    {
        Debug::Log("Shader program link succeeded.", __FILE__, __LINE__);
    }
}

GLuint ShaderProgram::GetProgramId() const
{
    return _programId;
}
