// Terrain Viewer
//
// DDS image loader, file: DDSImageLoader.cpp
//
// - Implementation of the DDS image loader class.
//   DDS loading code based on code from http://www.opengl-tutorial.org/.
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

#include "DDSImageLoader.h"

DDSImageLoader::DDSImageLoader()
{
}

ImageData* DDSImageLoader::LoadImage(const std::string & path, bool generateMipmaps, Profiler* prof, int subdiv)
{
    ImageData* temp = new ImageData;

    unsigned char header[124];

#ifdef WINDOWS_BUILD
	uint64_t startTime = GetTickCount64();
#else
	time_t startTime = time(NULL);
#endif

	/* Open the DDS file */
	#ifdef WINDOWS_BUILD
    FILE *descriptor = NULL;
	fopen_s(&descriptor, path.c_str(), "rb");
	#else
	FILE *descriptor = fopen(path.c_str(), "rb");
	#endif

    if (descriptor == NULL)
    {
        Debug::Log("File \"" + path + "\" not found.", __FILE__, __LINE__);
        throw std::exception();
    }

    char filecode[4];
    fread(filecode, 1, 4, descriptor);

	/* Check the signature on the code. The texture must have
	   the string DDS to be a correct DDS file. */
    if (strncmp(filecode, "DDS ", 4) != 0)
    {
        Debug::Log("The texture file is not a correct DXT format file.", __FILE__, __LINE__);
        fclose(descriptor);
        throw std::exception();
    }

    /* Get the surface descriptor */
    fread(&header, 124, 1, descriptor);

    unsigned int height = *(unsigned int*)&(header[8 ]);
    unsigned int width = *(unsigned int*)&(header[12]);
    unsigned int linearSize = *(unsigned int*)&(header[16]);
    unsigned int mipMapCount = *(unsigned int*)&(header[24]);
    unsigned int fourCC = *(unsigned int*)&(header[80]);

    temp->height = height;
    temp->width = width;

    unsigned char* buffer;
    unsigned int bufsize;

    bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
    buffer = new unsigned char[bufsize * sizeof(unsigned char)];
    fread(buffer, 1, bufsize, descriptor);
    fclose(descriptor);

    unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4;
    unsigned int format;

	/* Check if the texture has a valid S3 DXT format */
    switch(fourCC)
    {
		case FOURCC_DXT1:
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			temp->format = IMAGE_FORMAT_DDS_DXT1;
			break;
		case FOURCC_DXT3:
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			temp->format = IMAGE_FORMAT_DDS_DXT3;
			break;
		case FOURCC_DXT5:
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			temp->format = IMAGE_FORMAT_DDS_DXT5;
			break;
		default:
			delete[] buffer;
			Debug::Log("The DXT format used by the texture is unsupported.", __FILE__, __LINE__);
			throw std::exception();
    }

	/* Create the OpenGL texture */
    glGenTextures(1, &temp->glId);
    glBindTexture(GL_TEXTURE_2D, temp->glId);

    unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    unsigned int offset = 0;

	/* Load the texture on the GPU and the included mipmaps if these exist */
    for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
    {
        unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize;
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
            0, size, buffer + offset);

        offset += size;
        width  /= 2;
        height /= 2;
    }

    delete[] buffer;

#ifdef WINDOWS_BUILD
	uint64_t uploadStart = GetTickCount64();
#else
	time_t uploadStart = time(NULL);
#endif

	if (prof != NULL)
	{
		uint64_t time = 0;

#ifdef WINDOWS_BUILD
		time = uploadStart - startTime;
#else
		time = difftime(uploadStart, startTime) * 1000.0f;
#endif

		prof->AddTextureLoadTime(time);
	}

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	/* Generate mipmaps if the DDS file doesn't contain them */
	if (generateMipmaps)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

#ifdef WINDOWS_BUILD
	uint64_t endTime = GetTickCount64();
#else
	time_t endTime = time(NULL);
#endif

	/* Commit texture loading time to the profiler */
	if (prof != NULL)
	{
		uint64_t time = 0;

#ifdef WINDOWS_BUILD
		time = endTime - uploadStart;
#else
		time = difftime(endTime, uploadStart) * 1000.0f;
#endif

		prof->AddTextureLoadTime(time);
	}

    return temp;
}

const std::vector<glm::vec2> & DDSImageLoader::GetBoundingBoxes() const
{
	return _boundingBoxes;
}