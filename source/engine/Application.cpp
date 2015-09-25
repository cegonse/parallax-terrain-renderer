#include "Application.h"

Application* application;

Application::Application()
{
    application = this;

    if (!glfwInit())
    {
        throw std::exception();
    }

    _antiAliasLevel = 6;
    _majorVersion = 4;
    _minorVersion = 3;

    _screenHeight = 768;
    _screenWidth = 1024;
    _screenBpp = 32;
    _titleName = "Terrenos";
    _mainWindow = NULL;
    _mainProgram = NULL;
    _useTessellation = false;
    _useWireframe = false;

    _projectionMatrix = glm::ortho(0.0f, (float) _screenWidth,(float) _screenHeight,0.0f, 0.1f, 100.0f);
    _viewMatrix = glm::lookAt(glm::vec3(0,0,60), glm::vec3(0,0,0), glm::vec3(0,1,0));
    _modelMatrix = glm::translate(glm::vec3(((float)_screenWidth) / 2.0f, ((float)_screenHeight) / 2.0f, 0.0f)) *
                   glm::scale(glm::vec3(256.0f));

    _modelViewProjectionMatrix = _projectionMatrix * _viewMatrix * _modelMatrix;

	glewExperimental = true;

    if (glewInit() != GLEW_OK)
    {
        MessageBox(GetActiveWindow(), "Error iniciando GLEW", "Error", MB_OK | MB_ICONERROR);
        throw std::exception();
    }
}

Application::~Application()
{
    glDeleteBuffers(1, &_uvVertexBufferObject);
    glDeleteBuffers(1, &_vertexBufferObject);
    glDeleteVertexArrays(1, &_vertexArrayObject);

    if (_mainWindow != NULL)
    {
        glfwDestroyWindow(_mainWindow);
    }
}

void Application::Start(const std::string & settings)
{
    Start();
}

void Application::Start()
{
    glfwWindowHint(GLFW_SAMPLES, _antiAliasLevel);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _majorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _minorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Sólo iniciamos la aplicación una vez
    assert(_mainWindow == NULL);
    _mainWindow = glfwCreateWindow(_screenWidth, _screenHeight, _titleName.c_str(), NULL, NULL);

    if (_mainWindow == NULL)
    {
        glfwTerminate();
        Debug::Log("Raising fatal exception.", __FILE__, __LINE__);
        throw std::exception();
    }

    glfwMakeContextCurrent(_mainWindow);
    glewExperimental = true;

    if (glewInit() != GLEW_OK)
    {
        glfwDestroyWindow(_mainWindow);
        glfwTerminate();
        Debug::Log("Raising fatal exception.", __FILE__, __LINE__);
        throw std::exception();
    }

    glfwSetFramebufferSizeCallback(_mainWindow, applicationOnFramebufferResize);

    glGenVertexArrays(1, &_vertexArrayObject);
    glBindVertexArray(_vertexArrayObject);

    glfwSwapInterval(60);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glLineWidth(2.0f);

    Debug::Log("Application started successfully.", __FILE__, __LINE__);
}

bool Application::Run()
{
    assert(_mainWindow != NULL);
    return !glfwWindowShouldClose(_mainWindow);
}

void Application::InitGeometry()
{
    const GLfloat vertex_buffer_data[] = {
		-0.5f,  0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
	};

	const GLfloat uv_data[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f
	};

	glGenBuffers(1, &_vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &_uvVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, _uvVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uv_data), uv_data, GL_STATIC_DRAW);

	try
	{
	    _helpTexture.Load("textures\\help.dds");
	}
	catch (const std::exception &)
	{
	    std::cout << "Error loading help texture." << std::endl;
	}
}

void Application::DoFrame(const Color & backColor)
{
    glfwSetTime(0.0);

    GLfloat* _backColorPtr = backColor.Get();
    glClearColor(_backColorPtr[0], _backColorPtr[1], _backColorPtr[2], _backColorPtr[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLuint _matrixUniformId = glGetUniformLocation(_mainProgram->GetProgramId(), "uMvpMatrix");
    glUniformMatrix4fv(_matrixUniformId, 1, GL_FALSE, &_modelViewProjectionMatrix[0][0]);

    GLuint samplerId = glGetUniformLocation(_mainProgram->GetProgramId(), "uTextureSampler");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _mainTexture->GetTextureId());

    glUniform1i(samplerId, 0);

    GLuint vertexDataId = glGetAttribLocation(_mainProgram->GetProgramId(), "aVertices");

    glEnableVertexAttribArray(vertexDataId);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
    glVertexAttribPointer(vertexDataId, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    GLuint uvDataId = glGetAttribLocation(_mainProgram->GetProgramId(), "aUvCoordinates");

    glEnableVertexAttribArray(uvDataId);
    glBindBuffer(GL_ARRAY_BUFFER, _uvVertexBufferObject);
    glVertexAttribPointer(uvDataId, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    if (_useWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (!_useTessellation)
    {
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    else
    {
        glDrawArrays(GL_PATCHES, 0, 4);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void Application::UseTexture(Texture* tex)
{
    _mainTexture = tex;
}

void Application::EndFrame() const
{
    glfwSwapBuffers(_mainWindow);
    glfwPollEvents();

    double frameTime = glfwGetTime();
    std::string newTitle = _titleName;
    newTitle += "   [FPS: " + boost::lexical_cast<std::string>((int) (1.0 / frameTime));
    newTitle += "]";

    glfwSetWindowTitle(_mainWindow, newTitle.c_str());
}

void Application::UseProgram(const ShaderProgram & program)
{
    _mainProgram = (ShaderProgram*) &program;
    glUseProgram(_mainProgram->GetProgramId());
}

void Application::SetCamera(const glm::vec3 & position, const glm::vec3 & target)
{
    _viewMatrix = glm::lookAt(position, target, glm::vec3(0,1,0));
    _modelViewProjectionMatrix = _projectionMatrix * _viewMatrix * _modelMatrix;
}

void Application::SetOrthographicProjection()
{
    _projectionMatrix = glm::ortho(0.0f, (float) _screenWidth,(float) _screenHeight,0.0f, 0.1f, 100.0f);
    _modelViewProjectionMatrix = _projectionMatrix * _viewMatrix * _modelMatrix;
}

void Application::SetPerspectiveProjection(const float fov)
{
    _projectionMatrix = glm::perspective(fov, (float)_screenWidth / (float)_screenHeight, 0.1f, 100.0f);
    _modelViewProjectionMatrix = _projectionMatrix * _viewMatrix * _modelMatrix;
}

int Application::GetMaximumTessellationControlPoints() const
{
    GLint maxPatchVertices = 0;
    glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxPatchVertices);
    return maxPatchVertices;
}

void Application::ToggleTessellation(bool mode)
{
    _useTessellation = mode;
}

void Application::ToggleWireframe(bool mode)
{
    _useWireframe = mode;
}

bool Application::GetKeyDown(int key) const
{
    return glfwGetKey(_mainWindow, key) == GLFW_PRESS;
}

void Application::ResizeFramebuffer(int width, int height)
{
    _screenWidth = width;
    _screenHeight = height;

    glViewport(0, 0, width, height);

    _projectionMatrix = glm::ortho(0.0f, (float) _screenWidth,(float) _screenHeight,0.0f, 0.1f, 100.0f);
    _modelMatrix = glm::translate(glm::vec3(((float)_screenWidth) / 2.0f, ((float)_screenHeight) / 2.0f, 0.0f)) *
                   glm::scale(glm::vec3(256.0f));

    _modelViewProjectionMatrix = _projectionMatrix * _viewMatrix * _modelMatrix;
}

void Application::SetDefaultProgram(const ShaderProgram & program)
{
    _defaultProgram = (ShaderProgram*) &program;
}

void applicationOnFramebufferResize(GLFWwindow* window, int width, int height)
{
    application->ResizeFramebuffer(width, height);
}
