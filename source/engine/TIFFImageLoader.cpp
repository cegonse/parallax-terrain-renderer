// Terrain Viewer
//
// TIFF Image Loader, file: Texture.cpp
//
// - Implementation of the TIFF Image loader class.
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
#include "TIFFImageLoader.h"

/* Error handling callback for libTIFF errors */
void ErrorHandler(const char* module, const char* fmt, va_list ap)
{
	std::cout << "LibTIFF error: ";
	printf(fmt, ap);
	std::cout << std::endl;
}

TIFFImageLoader::TIFFImageLoader()
{
}

ImageData* TIFFImageLoader::LoadImage(const std::string & path, bool generateMipmaps, Profiler* prof, int subdiv)
{
    ImageData* temp = new ImageData;

#ifdef WINDOWS_BUILD
	uint64_t startTime = GetTickCount64();
#else
	time_t startTime = time(NULL);
#endif

    TIFF* tiff = TIFFOpen(path.c_str(), "r");

    if (!tiff)
    {
        delete temp;
        Debug::Log("Error opening TIFF file for reading.", "TIFFImageLoader.cpp", __LINE__);
        throw std::exception();
    }

	/* Set the callback for error handling */
	TIFFSetErrorHandler(&ErrorHandler);

    uint32 width = 0, height = 0;
    uint32 bitsPerSample = 0;
	size_t pixelCount = 0;
    uint32_t* data = NULL;
	uint64_t* multiByteData = NULL;

	/* Get the image width and height from the header */
    TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &width);
	TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &height);
	TIFFGetField(tiff, TIFFTAG_BITSPERSAMPLE, &bitsPerSample);

	bool multiByte = (bitsPerSample == 16) ? true : false;

	temp->width = width;
	temp->height = height;
	temp->format = IMAGE_FORMAT_TIFF;

	pixelCount = width * height;

	if (multiByte)
	{
		multiByteData = (uint64_t*) _TIFFmalloc((tsize_t)pixelCount * sizeof(uint64_t));
	}
	else
	{
		data = (uint32_t*) _TIFFmalloc((tsize_t)pixelCount * sizeof(uint32_t));
	}

	uint64_t uploadStart;

    if ((!multiByte && data != NULL) || (multiByte && multiByteData != NULL))
    {
#ifdef WINDOWS_BUILD
		uploadStart = GetTickCount64();
#else
		time_t uploadStart = time(NULL);
#endif

		if (prof != NULL)
		{
			uint64_t time = 0;
#ifdef WINDOWS_BUILD
			time = uploadStart - startTime;
#else
			time = difftime(uploadStart, startTime) / 1000.0f;
#endif

			prof->AddTextureLoadTime(time);
		}

		if (multiByte)
		{
			int stride = TIFFScanlineSize(tiff);
			boost::scoped_array<uint64_t> tempBuffer(new uint64_t[stride]);

			for (int y = 0, offset = width * (height - 1); y < height; y++)
			{
				TIFFReadScanline(tiff, tempBuffer.get(), y, stride);
				memcpy(multiByteData + offset, tempBuffer.get(), stride);
				offset -= width;
			}
		}
		else
		{
			if (!TIFFReadRGBAImage(tiff, width, height, data, 0))
			{
				delete temp;

				if (multiByte)
				{
					_TIFFfree(multiByteData);
				}
				else
				{
					_TIFFfree(data);
				}

				TIFFClose(tiff);

				Debug::Log("Error reading TIFF image contents.", "TIFFImageLoader.cpp", __LINE__);
				throw std::exception();
			}
		}

		/* Upload the texture to the graphics card */
        glGenTextures(1, &temp->glId);
        glBindTexture(GL_TEXTURE_2D, temp->glId);

		/*
		  16 bit / channel textures are uploaded using only the red and green channels,
		  since those are used for the height / normal values.

		  8 bit / channel textures as assumed to be the color data and are uploaded using
		  all channels.
		*/
		if (multiByte)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT, multiByteData);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

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

		/* Build the bounding box from the heights on
		   the texture, asume it is square. */
		if (multiByte)
		{
			int dx = width / floor((double)(subdiv + 1));

			float maxValue = 0.0f, minValue = 1.0f;

			for (int tileX = 0; tileX < (subdiv + 1); tileX++)
			{
				for (int tileY = 0; tileY < (subdiv + 1); tileY++)
				{
					for (int x = dx * tileX; x < dx * (tileX + 1); x++)
					{
						for (int y = dx * tileY; y < dx * (tileY + 1); y++)
						{
							float currentValue = (multiByteData[y * width + x] & 0xFFFF) / 65535.0f;

							if (currentValue < minValue)
							{
								minValue = currentValue;
							}

							if (currentValue > maxValue)
							{
								maxValue = currentValue;
							}
						}
					}

					_boundingBoxes.push_back(glm::vec2(minValue, maxValue));

					minValue = 1.0f;
					maxValue = 0.0f;
				}
			}
		}

#ifdef WINDOWS_BUILD
		uint64_t endTime = GetTickCount64();
#else
		time_t endTime = time(NULL);
#endif

		if (prof != NULL)
		{
			uint64_t time = 0;

#ifdef WINDOWS_BUILD
			time = endTime - uploadStart;
#else
			time = difftime(endTime, uploadStart) / 1000.0f;
#endif

			prof->AddTextureUploadTime(time);
		}

		if (multiByte)
		{
			_TIFFfree(multiByteData);
		}
		else
		{
			_TIFFfree(data);
		}
	}
	else
    {
        delete temp;
        TIFFClose(tiff);

        Debug::Log("Error allocating memory for loading TIFF image.", "TIFFImageLoader.cpp", __LINE__);
        throw std::exception();
    }

    TIFFClose(tiff);

    return temp;
}

const std::vector<glm::vec2> & TIFFImageLoader::GetBoundingBoxes() const
{
	return _boundingBoxes;
}