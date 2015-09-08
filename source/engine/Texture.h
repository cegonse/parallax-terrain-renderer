#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>
#include <vector>

#include "Debug.h"
#include "ImageLoaderBase.h"
#include "Profiler.h"

class Texture
{
    public:
        Texture(const std::string & path, int subdiv = 0);
        Texture(GLuint id, int width, int height);
		Texture();
        ~Texture();

        GLuint GetTextureId() const;
        void Load(const std::string & path, bool generateMipmaps = true, Profiler* prof = NULL, int subdiv = 0);

        int GetHeight() const;
        int GetWidth() const;

        void Drop();
        void Grab();
        int GetUseCount() const;

		const std::vector<glm::vec2> & GetBoundingBoxes() const;

    private:
        GLuint _textureId;
        int _height;
        int _width;

        int _useCount;
		bool _releaseAtDeletion;
		
		int _subdivisionCount;
		std::vector<glm::vec2> _boundingBoxes;
};

#endif // TEXTURE_H
