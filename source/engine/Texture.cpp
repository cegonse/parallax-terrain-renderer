// Terrain Viewer
//
// Texture, file: Texture.cpp
//
// - Implementation of the Texture class.
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
#include "Texture.h"

Texture::Texture(const std::string & path, int subdiv)
{
	_subdivisionCount = subdiv;

    Load(path);
    
	_useCount = 0;
	_releaseAtDeletion = true;
}

Texture::Texture(GLuint id, int width, int height)
{
	_textureId = id;
	_width = width;
	_height = height;

	_useCount = 0;
	_releaseAtDeletion = false;
	_subdivisionCount = 0;
}

Texture::Texture()
{
    _useCount = 0;
	_subdivisionCount = 0;
	_releaseAtDeletion = true;
}

int Texture::GetHeight() const
{
    return _height;
}

int Texture::GetWidth() const
{
    return _width;
}

void Texture::Drop()
{
    _useCount--;
}

void Texture::Grab()
{
    _useCount++;
}

int Texture::GetUseCount() const
{
    return _useCount;
}

void Texture::Load(const std::string & path, bool generateMipmaps, Profiler* prof, int subdiv)
{
    ImageData* temp;
    ImageLoader* loader;

	if (subdiv != 0)
	{
		_subdivisionCount = subdiv;
	}

	/* Find the texture type by its path (only TIFF and S3TC supported) */
    if (path.find(".dds") != std::string::npos || path.find(".dxt") != std::string::npos)
    {
        loader = new DDSImageLoader;
    }
    else if (path.find(".tif") != std::string::npos || path.find(".tiff") != std::string::npos)
    {
        loader = new TIFFImageLoader;
    }

	try
	{
		temp = loader->LoadImage(path, generateMipmaps, prof, _subdivisionCount);
		_boundingBoxes = loader->GetBoundingBoxes();
	}
	catch (const std::exception & ex)
	{
		delete loader;
		throw ex;
	}

	delete loader;

    _textureId = temp->glId;
    _height = temp->height;
    _width = temp->width;
	
    std::cout << "Texture loaded: " << path << std::endl;
    std::cout << "- Dimensions: " << _width << "x" << _height << std::endl;
    std::cout << "- Format: ";

    switch (temp->format)
    {
        case IMAGE_FORMAT_DDS_DXT1:
            std::cout << "DXT1 (Hardware decompression)" << std::endl;
            break;

        case IMAGE_FORMAT_DDS_DXT3:
            std::cout << "DXT3 (Hardware decompression)" << std::endl;
            break;

        case IMAGE_FORMAT_DDS_DXT5:
            std::cout << "DXT5 (Hardware decompression)" << std::endl;
            break;

        case IMAGE_FORMAT_TIFF:
            std::cout << "TIFF (Software decompression)" << std::endl;
            break;

        case IMAGE_FORMAT_PNG:
            std::cout << "PNG (Software decompression)" << std::endl;
            break;

        case IMAGE_FORMAT_JPEG:
            std::cout << "JPEG (Software decompression)" << std::endl;
            break;
    }
}

Texture::~Texture()
{
	if (_releaseAtDeletion)
	    glDeleteTextures(1, &_textureId);
}

GLuint Texture::GetTextureId() const
{
    return _textureId;
}

const std::vector<glm::vec2> & Texture::GetBoundingBoxes() const
{
	return _boundingBoxes;
}