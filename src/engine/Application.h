#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <windows.h>
#include <string>
#include <assert.h>
#include <boost/shared_array.hpp>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "Debug.h"
#include "Color.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Texture.h"

void applicationOnFramebufferResize(GLFWwindow* window, int width, int height);

class Application
{
    public:
        Application();
        ~Application();

        void Start(const std::string & settings);
        void Start();
        bool Run();
        void InitGeometry();
        void DoFrame(const Color & backColor);
        void EndFrame() const;
        void UseProgram(const ShaderProgram & program);
        void SetDefaultProgram(const ShaderProgram & program);
        int GetMaximumTessellationControlPoints() const;
        void UseTexture(Texture* tex);

        bool GetKeyDown(int key) const;
        void ToggleTessellation(bool mode);
        void ToggleWireframe(bool mode);

        void SetCamera(const glm::vec3 & position, const glm::vec3 & target);
        void SetPerspectiveProjection(const float fov);
        void SetOrthographicProjection();

        void ResizeFramebuffer(int width, int height);

    private:
        void DrawHelpBox();

        int _antiAliasLevel;
        int _majorVersion;
        int _minorVersion;

        int _screenHeight;
        int _screenWidth;
        int _screenBpp;
        std::string _titleName;

        GLFWwindow *_mainWindow;
        GLuint _vertexArrayObject;
        ShaderProgram* _mainProgram;
        ShaderProgram* _defaultProgram;

        GLuint _vertexBufferObject;
        GLuint _uvVertexBufferObject;

        glm::mat4 _projectionMatrix;
        glm::mat4 _viewMatrix;
        glm::mat4 _modelMatrix;
        glm::mat4 _modelViewProjectionMatrix;

        bool _useTessellation;
        bool _useWireframe;

        Texture* _mainTexture;
        Texture _helpTexture;
};

#endif // APPLICATION_H
