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
#include "Renderer.h"

Renderer::Renderer(int width, int height)
{
    _width = width;
    _height = height;
	_renderRadius = 800.0f;
	_useParallax = false;

	/* Resize render viewport */
    SetViewportSize(_width, _height);

	/* Vertex Array Object for 2D UI widgets */
    glGenVertexArrays(1, &_vao);

	/* Vertex positions and UV coordinates for 2D UI widgets */
    const GLfloat texture2dBufferData[] = {
		-0.5f,  0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.0f,
         0.0f,  1.0f,
         1.0f,  0.0f,
         1.0f,  1.0f
	};

	/* VBO for 2D UI widgets */
    glGenBuffers(1, &_2dTextureVbo);
	glBindBuffer(GL_ARRAY_BUFFER, _2dTextureVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texture2dBufferData), texture2dBufferData, GL_STATIC_DRAW);

	/* Shader loading */
	{
		boost::scoped_ptr<Shader> texture2dVertexShader, texture2dFragmentShader;

		/* Simple VS + FS for 2D UI widgets */
		try
		{
			texture2dVertexShader.reset(new Shader("shaders\\v_MatrixTransform.glsl", GL_VERTEX_SHADER));
			texture2dFragmentShader.reset(new Shader("shaders\\f_Texture.glsl", GL_FRAGMENT_SHADER));
		}
		catch (const std::exception & ex)
		{
			throw ex;
		}

		_2dTextureShader = new ShaderProgram();

		_2dTextureShader->AttachShader(texture2dVertexShader.get());
		_2dTextureShader->AttachShader(texture2dFragmentShader.get());

		_2dTextureShader->Build();

		boost::scoped_ptr<Shader> passthroughColor, color;

		/* Colored shader for lines */
		try
		{
			passthroughColor.reset(new Shader("shaders\\v_MatrixTransformColor.glsl", GL_VERTEX_SHADER));
			color.reset(new Shader("shaders\\f_Color.glsl", GL_FRAGMENT_SHADER));
		}
		catch (const std::exception & ex)
		{
			throw ex;
		}

		_colorPtShader = new ShaderProgram();
		_colorPtShader->AttachShader(passthroughColor.get());
		_colorPtShader->AttachShader(color.get());
		_colorPtShader->Build();

		boost::scoped_ptr<Shader> tessTerrainVertex, tessTerrainControl, tessTerrainEval,
			tessTerrainFrag, simpleTerrainVertex, simpleTerrainFrag;

		/* VS + TCS + TES + FS for terrain tessellation */
		try
		{
			tessTerrainVertex.reset(new Shader("shaders\\v_TerrainTess.glsl", GL_VERTEX_SHADER));
			tessTerrainControl.reset(new Shader("shaders\\tcs_PassThrough.glsl", GL_TESS_CONTROL_SHADER));
			tessTerrainEval.reset(new Shader("shaders\\tes_TerrainTess.glsl", GL_TESS_EVALUATION_SHADER));
			tessTerrainFrag.reset(new Shader("shaders\\f_TextureTessellation.glsl", GL_FRAGMENT_SHADER));

			simpleTerrainVertex.reset(new Shader("shaders\\v_TerrainSimple.glsl", GL_VERTEX_SHADER));
			simpleTerrainFrag.reset(new Shader("shaders\\f_TerrainSimple.glsl", GL_FRAGMENT_SHADER));
		}
		catch (const std::exception & ex)
		{
			throw ex;
		}

		_terrainShader = new ShaderProgram();

		_terrainShader->AttachShader(tessTerrainVertex.get());
		_terrainShader->AttachShader(tessTerrainControl.get());
		_terrainShader->AttachShader(tessTerrainEval.get());
		_terrainShader->AttachShader(tessTerrainFrag.get());

		try
		{
			_terrainShader->Build();
		}
		catch (const std::runtime_error & ex)
		{
			throw ex;
		}
	}

	/* Default viewport camera */
	Camera* cam = CreateCamera(glm::vec3(0.0f), glm::vec3(10.0f, 0.0f, 10.0f));
	SetViewportCamera(cam);

	/* Set-up of several OpenGL parameters */
	glLineWidth(2.5f);
	glPointSize(4.0f);

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

	/* Generation and set-up of the framebuffer object */
	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	
	glGenTextures(1, &_renderTexture);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, _renderTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _renderTexture, 0);

	glGenRenderbuffers(1, &_renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, _renderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _renderBuffer);

	GLenum bufs[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, bufs);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/* Clean-up */
Renderer::~Renderer()
{
	/* Delete all active textures */
    CleanTextures(true);

	/* Delete shader programs */
    delete _2dTextureShader;
    delete _colorPtShader;
	delete _terrainShader;

	/* Delete FBO and related objects */
	glDeleteTextures(1, &_renderTexture);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteRenderbuffers(1, &_renderBuffer);
	glDeleteFramebuffers(1, &_fbo);

    glDeleteBuffers(1, &_2dTextureVbo);
    glDeleteVertexArrays(1, &_vao);

	/* Delete all cameras */
    for (unsigned int i = 0; i < _cameras.size(); i++)
    {
        delete _cameras[i];
    }

	/* Delete all terrain tiles */
    for (unsigned int i = 0; i < _terrains.size(); i++)
    {
        delete _terrains[i];
    }
}

void Renderer::SetViewportSize(int width, int height, bool overwrite)
{
	if (overwrite)
	{
		_width = width;
		_height = height;
	}

    glViewport(0, 0, _width, _height);

    for (unsigned int i = 0; i < _cameras.size(); i++)
    {
         _cameras[i]->SetViewportSize(width, height);
    }
}

const glm::vec2 & Renderer::GetViewportSize() const
{
	return glm::vec2(_width, _height);
}

void Renderer::DoFrame(Color* clearColor)
{
    CleanTextures(false);

    GLfloat* cc = clearColor->Get();

    glClearColor(cc[0], cc[1], cc[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Texture* Renderer::CreateMipmapTexture(const std::string & path, int subdiv)
{
	Texture* tex = NULL;

	/* Try to load the texture and save it into the stack
	   if successful */
	try
	{
		tex = new Texture;
		tex->Load(path, true, &_profiler, subdiv);
		_textures.push_back(tex);
	}
	catch (const std::exception & ex)
	{
		throw ex;
	}

    return tex;
}

Texture* Renderer::CreateTexture(const std::string & path, int subdiv)
{
	Texture* tex = NULL;

	/* Try to load the texture and save it into the stack
	   if successful */
	try
	{
		tex = new Texture;
		tex->Load(path, false, &_profiler, subdiv);
		_textures.push_back(tex);
	}
	catch (const std::exception & ex)
	{
		throw ex;
	}

    return tex;
}

void Renderer::Draw2DTexture(int x, int y, Texture* texture)
{
    GLuint tex = texture->GetTextureId();
    GLuint shader = _2dTextureShader->GetProgramId();

    int h = texture->GetHeight();
    int w = texture->GetWidth();

    glUseProgram(shader);

	/* Set up of model-view-projection matrix */
    glm::mat4 model = glm::translate(glm::vec3(x, y, 0)) * glm::scale(glm::vec3(w, h, 1.0f));
    glm::mat4 view = glm::lookAt(glm::vec3(0,0,60), glm::vec3(0,0,0), glm::vec3(0,1,0));
    glm::mat4 projection = glm::ortho(0.0f, (float) _width,(float) _height,0.0f, 0.1f, 100.0f);

    glm::mat4 mvp = projection * view * model;

    GLuint matrixUniformId = glGetUniformLocation(shader, "uMvpMatrix");
    glUniformMatrix4fv(matrixUniformId, 1, GL_FALSE, &mvp[0][0]);

    GLuint samplerId = glGetUniformLocation(shader, "uTextureSampler");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    glUniform1i(samplerId, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _2dTextureVbo);

    GLuint vertexDataId = glGetAttribLocation(shader, "aVertices");
    GLuint uvDataId = glGetAttribLocation(shader, "aUvCoordinates");

	glBindVertexArray(_vao);

    glEnableVertexAttribArray(vertexDataId);
    glVertexAttribPointer(vertexDataId, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(uvDataId);
    glVertexAttribPointer(uvDataId, 2, GL_FLOAT, GL_FALSE, 0, (void*)(3*4*sizeof(GLfloat)));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(vertexDataId);
    glDisableVertexAttribArray(uvDataId);

	glBindVertexArray(0);
}

void Renderer::Draw3DLine(const glm::vec3 & startPosition, const glm::vec3 & endPosition, Color* color)
{
	/* VBO for the line */
    GLfloat lineBuffer[12];
    GLfloat* c = color->Get();

    lineBuffer[0] = startPosition.x;
    lineBuffer[1] = startPosition.y;
    lineBuffer[2] = startPosition.z;

    lineBuffer[3] = endPosition.x;
    lineBuffer[4] = endPosition.y;
    lineBuffer[5] = endPosition.z;

    lineBuffer[6] = c[0];
    lineBuffer[7] = c[1];
    lineBuffer[8] = c[2];

    lineBuffer[9] = c[0];
    lineBuffer[10] = c[1];
    lineBuffer[11] = c[2];

    GLuint lineVbo;

    glGenBuffers(1, &lineVbo);
	glBindBuffer(GL_ARRAY_BUFFER, lineVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lineBuffer), lineBuffer, GL_STATIC_DRAW);

    GLuint shader = _colorPtShader->GetProgramId();
    glUseProgram(shader);

    glm::mat4 mvp = _viewportCamera->GetProjectionMatrix() * _viewportCamera->GetViewMatrix() * glm::scale(glm::vec3(1.0f));

    GLuint matrixUniformId = glGetUniformLocation(shader, "uMvpMatrix");
    glUniformMatrix4fv(matrixUniformId, 1, GL_FALSE, &mvp[0][0]);

    glBindBuffer(GL_ARRAY_BUFFER, lineVbo);

    GLuint vertexDataId = glGetAttribLocation(shader, "aVertices");
    GLuint colorDataId = glGetAttribLocation(shader, "aColor");

	glBindVertexArray(_vao);

    glEnableVertexAttribArray(vertexDataId);
    glVertexAttribPointer(vertexDataId, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(colorDataId);
    glVertexAttribPointer(colorDataId, 3, GL_FLOAT, GL_FALSE, 0, (void*)(3*2*sizeof(GLfloat)));

    glDrawArrays(GL_LINES, 0, 2);

    glDisableVertexAttribArray(vertexDataId);
    glDisableVertexAttribArray(colorDataId);

	glBindVertexArray(0);

	glDeleteBuffers(1, &lineVbo);
}

void Renderer::Draw3DBoundingBox(const glm::vec3 & center, const glm::vec3 & scale, const glm::vec3 & rotationAxis, float rotationAngle, Color* color)
{
    GLfloat* c = color->Get();

    GLfloat dataBuffer[144] = {
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
         c[0],  c[1],  c[2],
         c[0],  c[1],  c[2],
         c[0],  c[1],  c[2],
         c[0],  c[1],  c[2],
         c[0],  c[1],  c[2],
         c[0],  c[1],  c[2],
         c[0],  c[1],  c[2],
         c[0],  c[1],  c[2],
         c[0],  c[1],  c[2],
         c[0],  c[1],  c[2],
         c[0],  c[1],  c[2],
         c[0],  c[1],  c[2],
         c[0],  c[1],  c[2],
         c[0],  c[1],  c[2],
         c[0],  c[1],  c[2],
         c[0],  c[1],  c[2],
         c[0],  c[1],  c[2],
         c[0],  c[1],  c[2],
         c[0],  c[1],  c[2],
         c[0],  c[1],  c[2],
         c[0],  c[1],  c[2],
         c[0],  c[1],  c[2],
         c[0],  c[1],  c[2],
         c[0],  c[1],  c[2]
    };

    GLuint boxVbo;

    glGenBuffers(1, &boxVbo);
	glBindBuffer(GL_ARRAY_BUFFER, boxVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(dataBuffer), dataBuffer, GL_STATIC_DRAW);

    GLuint shader = _colorPtShader->GetProgramId();
    glUseProgram(shader);

    glm::mat4 modelMatrix(0.0f);

    modelMatrix = glm::translate(modelMatrix, center);

    if (rotationAngle > 0.0f || rotationAngle < 0.0f)
    {
        modelMatrix = glm::rotate(modelMatrix, rotationAngle, rotationAxis);
    }

    modelMatrix = glm::scale(modelMatrix, scale);

    glm::mat4 mvp = _viewportCamera->GetProjectionMatrix() * _viewportCamera->GetViewMatrix() * modelMatrix;

    GLuint matrixUniformId = glGetUniformLocation(shader, "uMvpMatrix");
    glUniformMatrix4fv(matrixUniformId, 1, GL_FALSE, &mvp[0][0]);

    glBindBuffer(GL_ARRAY_BUFFER, boxVbo);

    GLuint vertexDataId = glGetAttribLocation(shader, "aVertices");
    GLuint colorDataId = glGetAttribLocation(shader, "aColor");

	glBindVertexArray(_vao);

    glEnableVertexAttribArray(vertexDataId);
    glVertexAttribPointer(vertexDataId, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(colorDataId);
    glVertexAttribPointer(colorDataId, 3, GL_FLOAT, GL_FALSE, 0, (void*)(3*24*sizeof(GLfloat)));

    glDrawArrays(GL_LINES, 0, 24);

    glDisableVertexAttribArray(vertexDataId);
    glDisableVertexAttribArray(colorDataId);

	glBindVertexArray(0);

	glDeleteBuffers(1, &boxVbo);
}

void Renderer::DrawTerrain(Terrain* terrain, const glm::vec2 & position, float metersPerPixel, bool manageLod, float maxDistance, int maxLod)
{
    TerrainData data = terrain->GetTerrainData();
	terrain->GetShaderProgram()->SetActive(true);

	float heightmapSize = (float)terrain->GetHeightmap()->GetWidth();

    glm::mat4 scale = glm::mat4(1.0f);
	glm::mat4 rotation = glm::mat4(1.0f);
	glm::mat4 translation = glm::translate(glm::vec3(position.x, 0.0f, position.y));

    glm::mat4 modelMatrix = translation * rotation * scale;
    glm::mat4 mvp = _viewportCamera->GetProjectionMatrix() * _viewportCamera->GetViewMatrix() * modelMatrix;

	try
	{
		terrain->GetShaderProgram()->SetUniformFloat("uHeightScale", data.scale);
		terrain->GetShaderProgram()->SetUniformFloat("uLevelOfDetail", maxLod);

		terrain->GetShaderProgram()->SetUniformVector("uEyeWorldPosition", _viewportCamera->GetOrigin());

		terrain->GetShaderProgram()->SetUniformInteger("uFrustrumCulling", 1);
		terrain->GetShaderProgram()->SetUniformFloat("uMaxLodDistance", maxDistance);
		terrain->GetShaderProgram()->SetUniformInteger("uManageLod", manageLod ? 1 : 0);
		terrain->GetShaderProgram()->SetUniformInteger("uParallax", _useParallax ? 1 : 0);

		terrain->GetShaderProgram()->SetUniformInteger("uSubdivisionCount", terrain->GetSubdivisionCount());

		terrain->GetShaderProgram()->SetUniformMatrix("uMvpMatrix", mvp);
		terrain->GetShaderProgram()->SetUniformMatrix("uModelMatrix", modelMatrix);

		terrain->GetShaderProgram()->SetUniformTexture("uTextureSampler", terrain->GetTerrainTexture(), 0);
		terrain->GetShaderProgram()->SetUniformTexture("uHeightmap", terrain->GetHeightmap(), 1);
	}
	catch (const std::runtime_error & ex)
	{
	}

	int L = terrain->GetPointsPerPatch();

    glBindVertexArray(terrain->GetTerrainVAO());
		glDrawArrays(GL_PATCHES, 0, L);
	glBindVertexArray(0);

	terrain->GetShaderProgram()->SetActive(false);
}

void Renderer::DrawProject(Project* project, bool manageLod, float maxDistance, int maxLod)
{
#ifdef WINDOWS_BUILD
	uint64_t startTime = GetTickCount64();
#else
	time_t startTime = time(NULL);
#endif

	std::vector<ProjectTile*> tiles = project->GetTileList();

	/* First pass: draw into FBO */
	/*glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	SetViewportSize(512, 512, false);

	for (unsigned int i = 0; i < tiles.size(); i++)
	{
		if (tiles[i] != NULL)
		{
			float x = tiles[i]->x;
			float y = tiles[i]->y;
			Terrain* terrain = tiles[i]->terrain;

			if (glm::distance(glm::vec3(x, 0, y), _viewportCamera->GetOrigin()) < _renderRadius)
			{
				DrawTerrain(terrain, glm::vec2(-x, y), 1.0f, manageLod, maxDistance, maxLod);
			}
		}
	}*/

	/* Second pass: draw to screen framebuffer */
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	SetViewportSize(_width, _height);

	for (unsigned int i = 0; i < tiles.size(); i++)
	{
		if (tiles[i] != NULL)
		{
			float x = tiles[i]->x;
			float y = tiles[i]->y;
			Terrain* terrain = tiles[i]->terrain;

			if (glm::distance(glm::vec3(x, 0, y), _viewportCamera->GetOrigin()) < _renderRadius)
			{
				DrawTerrain(terrain, glm::vec2(-x, y), 1.0f, manageLod, maxDistance, maxLod);
			}
		}
	}
	/*
	Texture temp(_renderTexture, 512, 512);
	Draw2DTexture(0, 0, &temp);*/
	
#ifdef WINDOWS_BUILD
	uint64_t endTime = GetTickCount64();
	_profiler.AddRenderTime(endTime - startTime);
#else
	time_t endTime = time(NULL);
	_profiler.AddRenderTime(difftime(endTime, startTime) / 1000);
#endif
}

int Renderer::GetAvailableVideoMemory() const
{
	short extType = VIDEO_EXT_NONE;
	int availableMem = -1;

	if (glewIsSupported("GL_NVX_gpu_memory_info"))
	{
		extType = VIDEO_EXT_NV;
	}
	else if (glewIsSupported("GL_ATI_meminfo"))
	{
		extType = VIDEO_EXT_AMD;
	}
	
	if (extType == VIDEO_EXT_NV)
	{
		int memKb = 0;
		glGetIntegerv(0x9049, &memKb);

		availableMem = memKb / 1000;
	}
	else if (extType == VIDEO_EXT_AMD)
	{
		GLint texMemData[4];
		glGetIntegerv(0x87FC, (GLint*) &texMemData);

		availableMem = texMemData[0] / 1000;
	}

	return availableMem;
}

int Renderer::GetTotalVideoMemory() const
{
	short extType = VIDEO_EXT_NONE;
	int totalMem = -1;

	if (glewIsSupported("GL_NVX_gpu_memory_info"))
	{
		extType = VIDEO_EXT_NV;
	}
	else if (glewIsSupported("GL_ATI_meminfo"))
	{
		extType = VIDEO_EXT_AMD;
	}
	
	if (extType == VIDEO_EXT_NV)
	{
		int memKb = 0;
		glGetIntegerv(0x9047, &memKb);

		totalMem = memKb / 1000;
	}
	else if (extType == VIDEO_EXT_AMD)
	{
		// I couldn't find a method to get total
		// video memory on AMD cards
	}

	return totalMem;
}

Camera* Renderer::CreateCamera(const glm::vec3 & origin, const glm::vec3 & target, float fov, bool ortho)
{
    Camera* cam = new Camera(origin, target, fov, ortho, _width, _height);
    _cameras.push_back(cam);

    return cam;
}

void Renderer::SetViewportCamera(Camera* camera)
{
    _viewportCamera = camera;
}

Camera* Renderer::GetViewportCamera() const
{
    return _viewportCamera;
}

void Renderer::CleanTextures(bool force)
{
    for (unsigned int i = 0; i < _textures.size(); i++)
    {
		/* Delete all textures independently of
		   their state. */
        if (force)
        {
            delete _textures[i];
        }
		/* Don't delete textures that have been "Grab"ed by
		   other objects in the application. */
        else
        {
            if (_textures[i]->GetUseCount() <= 0)
            {
                delete _textures[i];
				_textures.erase(_textures.begin() + i);
            }
        }
    }
}

Terrain* Renderer::CreateTerrain(const TerrainData & data, Texture* heightmap, Texture* texture, int subdivisionCount)
{
    Terrain* temp = new Terrain(data, heightmap, texture, _terrainShader, subdivisionCount);
    _terrains.push_back(temp);
    return temp;
}

void Renderer::SetWireframe(bool wire)
{
    wire ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

MemoryTexture* Renderer::GetFramebufferPixels(int width, int height)
{
	MemoryTexture* memtex = NULL;

	if (width == 0 && height == 0)
	{
		memtex = new MemoryTexture(_width, _height);
		GLubyte* pixels = memtex->Get();

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(0, 0, _width, _height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}

	return memtex;
}

void Renderer::SetRenderRadius(float radius)
{
	_renderRadius = radius;
}

float Renderer::GetRenderRadius() const
{
	return _renderRadius;
}

Profiler* Renderer::GetProfiler() const
{
	return (Profiler*) &_profiler;
}

int Renderer::GetMaximumTessellationLevel() const
{
	GLint tessLevels = 0;
	glGetIntegerv(GL_MAX_TESS_GEN_LEVEL, &tessLevels);
	return (int) tessLevels;
}

void Renderer::ToggleParallax()
{
	_useParallax = !_useParallax;
}