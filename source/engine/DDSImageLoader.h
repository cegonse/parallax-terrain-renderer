#ifndef _DDSIMAGELOADER_H_
#define _DDSIMAGELOADER_H_

#include "ImageLoader.h"
#include "Debug.h"
#include "BuildSettings.hpp"
#include "Profiler.h"

#include <cstdio>
#include <cstring>

#define FOURCC_DXT1 0x31545844
#define FOURCC_DXT3 0x33545844
#define FOURCC_DXT5 0x35545844

class DDSImageLoader : public ImageLoader
{
    public:
        DDSImageLoader();
        ImageData* LoadImage(const std::string & path, bool generateMipmaps = true, Profiler* prof = NULL, int subdiv = 0);
		const std::vector<glm::vec2> & GetBoundingBoxes() const;
};

#endif // _DDSIMAGELOADER_H_
