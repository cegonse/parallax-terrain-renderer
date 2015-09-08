#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <GL/glew.h>
#include <windows.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <boost/scoped_ptr.hpp>
#include <ctime>

#include "Texture.h"
#include "Color.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "Camera.h"
#include "Terrain.h"
#include "MemoryTexture.h"
#include "Project.h"
#include "Profiler.h"

enum VideoMemoryExtension
{
	VIDEO_EXT_NONE = 0,
	VIDEO_EXT_NV,
	VIDEO_EXT_AMD
};

class Renderer
{
    public:
        Renderer(int width, int height);
        ~Renderer();

        void SetViewportSize(int width, int height, bool overwrite = true);
		const glm::vec2 & GetViewportSize() const;
        void DoFrame(Color* clearColor);

        Texture* CreateTexture(const std::string & path, int subdiv = 0);
		Texture* CreateMipmapTexture(const std::string & path, int subdiv = 0);

        void Draw2DTexture(int x, int y, Texture* texture);
        void Draw3DLine(const glm::vec3 & startPosition, const glm::vec3 & endPosition, Color* color);
        void Draw3DBoundingBox(const glm::vec3 & center, const glm::vec3 & scale, const glm::vec3 & rotationAxis, float rotationAngle, Color* color);

        Camera* CreateCamera(const glm::vec3 & origin, const glm::vec3 & target, float fov = 45.0f, bool ortho = false);

        Terrain* CreateTerrain(const TerrainData & data, Texture* heightmap, Texture* texture, int subdivisionCount);
        void DrawTerrain(Terrain* terrain, const glm::vec2 & position = glm::vec2(0.0f), float metersPerPixel = 1.0f, bool manageLod = false, float maxDistance = 30.0f, int maxLod = 64);
		void DrawProject(Project* proj, bool manageLod = false, float maxDistance = 30.0f, int maxLod = 64);

        void SetViewportCamera(Camera* camera);
        Camera* GetViewportCamera() const;

        void SetWireframe(bool wire);

		MemoryTexture* GetFramebufferPixels(int height = 0, int width = 0);

		float GetRenderRadius() const;
		void SetRenderRadius(float radius);

		Profiler* GetProfiler() const;

		int GetAvailableVideoMemory() const;
		int GetTotalVideoMemory() const;
		int GetMaximumTessellationLevel() const;

		void ToggleParallax();

    private:
        void CleanTextures(bool force);

        std::vector<Texture*> _textures;
        std::vector<Camera*> _cameras;
        std::vector<Terrain*> _terrains;

        GLuint _2dTextureVbo;
        GLuint _vao;

		GLuint _renderTexture;
		GLuint _fbo;
		GLuint _renderBuffer;

        int _width;
        int _height;

        ShaderProgram* _2dTextureShader;
        ShaderProgram* _colorPtShader;
        ShaderProgram* _terrainShader;

        Camera* _viewportCamera;
		float _renderRadius;

		Profiler _profiler;
		bool _useParallax;
};

#endif // _RENDERER_H_
