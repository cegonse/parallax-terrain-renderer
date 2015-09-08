#ifndef _IMAGELOADER_H_
#define _IMAGELOADER_H_

#include <GL/glew.h>
#include <string>
#include <ctime>
#include <glm/glm.hpp>
#include <vector>

#include "BuildSettings.hpp"
#include "Profiler.h"

#ifdef WINDOWS_BUILD
#include <Windows.h>
#endif

enum ImageFormat
{
    IMAGE_FORMAT_UNKNOWN = 0,
    IMAGE_FORMAT_JPEG,
    IMAGE_FORMAT_PNG,
    IMAGE_FORMAT_TIFF,
    IMAGE_FORMAT_DDS_DXT1,
    IMAGE_FORMAT_DDS_DXT3,
    IMAGE_FORMAT_DDS_DXT5
};

struct ImageData
{
    GLuint glId;
    int height;
    int width;
    unsigned char format;
};

class ImageLoader
{
	protected:
		std::vector<glm::vec2> _boundingBoxes;
    public:
		ImageLoader();
        virtual ImageData* LoadImage(const std::string & path, bool generateMipmaps = true, Profiler* prof = NULL, int subdiv = 0) = 0;
		virtual const std::vector<glm::vec2> & GetBoundingBoxes() const = 0;
};

#endif // _IMAGELOADER_H_
