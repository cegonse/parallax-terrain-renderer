#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <boost/lexical_cast.hpp>

#include "Shader.h"
#include "BuildSettings.hpp"
#include "Texture.h"

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

		bool SetUniformVector(const std::string & name, const glm::vec2 & vec);
		bool SetUniformVector(const std::string & name, const glm::vec3 & vec);
		bool SetUniformVector(const std::string & name, const glm::vec4 & vec);

		bool SetUniformTexture(const std::string & name, Texture* tex, int index);

		bool SetUniformMatrix(const std::string & name, const glm::mat2 & mat, bool transpose = false);
		bool SetUniformMatrix(const std::string & name, const glm::mat3 & mat, bool transpose = false);
		bool SetUniformMatrix(const std::string & name, const glm::mat4 & mat, bool transpose = false);

		bool SetUniformFloat(const std::string & name, float val);
		bool SetUniformInteger(const std::string & name, int val);

		void SetActive(bool ena);

    private:
        GLuint _programId;
        std::vector<Shader*> _shaders;
		std::map<std::string, GLuint> _uniformLocations;
		bool _active;
		bool _built;

		void RetrieveUniformLocations();
		std::map<std::string, GLuint>::iterator ShaderProgram::GetIteratorAt(const std::string & name);
};

#endif // SHADERPROGRAM_H
