#ifndef _TIFFIMAGELOADER_H_
#define _TIFFIMAGELOADER_H_

#include "ImageLoader.h"

#include <string>
#include <tiffio.h>
#include <cstdint>
#include <boost/scoped_array.hpp>
#include <Windows.h>

#include "Debug.h"
#include "Profiler.h"

void ErrorHandler(const char* module, const char* fmt, va_list ap);

class TIFFImageLoader : public ImageLoader
{
    public:
        TIFFImageLoader();
        ImageData* LoadImage(const std::string & path, bool generateMipmaps = true, Profiler* prof = NULL, int subdiv = 0);
		const std::vector<glm::vec2> & GetBoundingBoxes() const;
};

#endif // _TIFFIMAGELOADER_H_
