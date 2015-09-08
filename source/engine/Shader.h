#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <boost/scoped_array.hpp>

#include "Debug.h"
#include "BuildSettings.hpp"

#ifdef WINDOWS_BUILD
#include <windows.h>
#endif

class Shader
{
    public:
        Shader(const std::string & path, GLenum shaderType);
        ~Shader();

        GLuint GetShaderId() const;

    private:
        std::ifstream _fileDescriptor;
        std::string _shaderSource;
        GLuint _shaderId;
        GLenum _shaderType;
};

#endif // SHADER_H
