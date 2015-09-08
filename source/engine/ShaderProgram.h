#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/glew.h>
#include <vector>

#include "Shader.h"
#include "BuildSettings.hpp"

#ifdef WINDOWS_BUILD
#include <windows.h>
#endif

class ShaderProgram
{
    public:
        ShaderProgram();
        ~ShaderProgram();

        void AttachShader(const Shader* shader);
        void Build();
        GLuint GetProgramId() const;

    private:
        GLuint _programId;
        std::vector<Shader*> _shaders;
};

#endif // SHADERPROGRAM_H
