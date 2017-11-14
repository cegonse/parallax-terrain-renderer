// Terrain Viewer
//
// Shader, file: Shader.cpp
//
// - Implementation of the Shader class.
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
#include "Shader.h"

Shader::Shader(const std::string & path, GLenum shaderType)
{
    _shaderType = shaderType;

    _fileDescriptor.open(path.c_str(), std::ios::in);

    if (_fileDescriptor.fail())
    {
        throw std::exception();
    }

    std::string line = "";

    while(getline(_fileDescriptor, line))
    {
        _shaderSource += "\n" + line;
    }

    _fileDescriptor.close();
    _shaderId = glCreateShader(_shaderType);

    const char* srcPtr = _shaderSource.c_str();
    glShaderSource(_shaderId, 1, &srcPtr, NULL);
    glCompileShader(_shaderId);

    GLint compilationResult = GL_FALSE;
    int compilationLogLength = 0;

    glGetShaderiv(_shaderId, GL_COMPILE_STATUS, &compilationResult);
    glGetShaderiv(_shaderId, GL_INFO_LOG_LENGTH, &compilationLogLength);

    boost::scoped_array<char> log(new char[compilationLogLength]);
    glGetShaderInfoLog(_shaderId, compilationLogLength, NULL, log.get());

	/* Complain if the shader didn't compile successfully */
    if (compilationResult == GL_FALSE)
    {
#ifdef WINDOWS_BUILD
		std::string caption = "Archivo " + path + ":\n\n" + log.get();
		MessageBox(GetActiveWindow(), caption.c_str(), "Error compilando shaders", MB_OK | MB_ICONERROR);
#endif

        Debug::Log("Shader compilation failed:\n\n" + std::string(log.get()), __FILE__, __LINE__);
        throw std::exception();
    }
    else
    {
        Debug::Log("Shader compilation succeeded.", __FILE__, __LINE__);
    }
}

Shader::~Shader()
{
    if (glIsShader(_shaderId) == GL_TRUE)
    {
        glDeleteShader(_shaderId);
    }
}

GLuint Shader::GetShaderId() const
{
    return _shaderId;
}
