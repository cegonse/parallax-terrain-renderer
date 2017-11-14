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
#include <fstream>
using namespace std;

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
    uint16 bitsPerSample = 0, config;
	size_t pixelCount = 0;
    uint32 * data = NULL;
	uint64 * multiByteData = NULL;
	uint16 compr, nsamp;
//	string nomtxt = path + ".txt";
//	ifstream ftxt;
//static char numfich = '0';
//ofstream fout;

	/* Get the image width and height from the header */
    TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &width);
	TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &height);
	TIFFGetField(tiff, TIFFTAG_BITSPERSAMPLE, &bitsPerSample);
	TIFFGetField(tiff, TIFFTAG_PLANARCONFIG, &config);
	TIFFGetField(tiff, TIFFTAG_COMPRESSION, &compr);
	TIFFGetField(tiff, TIFFTAG_SAMPLESPERPIXEL, &nsamp);

	bool multiByte = (bitsPerSample == 16) ? true : false;

	temp->width = width;
	temp->height = height;
	temp->format = IMAGE_FORMAT_TIFF;

	pixelCount = width * height;

	if (multiByte)
	{
		multiByteData = (uint64 *) _TIFFmalloc((tsize_t)pixelCount * sizeof(uint64));
//		ftxt.open(nomtxt.c_str());
/*
string s = "alturas";
s+= numfich;
s+= ".txt";
numfich ++;
fout.open(s.c_str());
fout << "IMAGEWIDTH " << width << endl;
fout << "IMAGELENGTH " << height << endl;
fout << "BITSPERSAMPLE " << bitsPerSample << endl;
fout << "SAMPLESPERPIXEL " << nsamp << endl;
*/
/*
		if (!ftxt.is_open())
		{
			delete temp;
			char msg[128];
			sprintf_s(msg, 128, "Error al abrir %s", nomtxt.c_str());
			MessageBox(0, msg, "", MB_OK);
			throw std::exception();
		}
		else
		{
			// Lee la cabecera
			string auxs;
			int auxi;
			bool error = false;
			ftxt >> auxs >> auxi;
			if ( auxs != "IMAGEWIDTH" || auxi != width )
				error = true;
			ftxt >> auxs >> auxi;
			if ( auxs != "IMAGELENGTH" || auxi != height )
				error = true;
			ftxt >> auxs >> auxi;
			if ( auxs != "BITSPERSAMPLE" || auxi != bitsPerSample )
				error = true;
			ftxt >> auxs >> auxi;
			if ( auxs != "SAMPLESPERPIXEL" || auxi != nsamp )
				error = true;
			if ( error )
			{
				delete temp;
				char msg[128];
				sprintf_s(msg, 128, "Error al leer la cabecera de %s", nomtxt.c_str());
				MessageBox(0, msg, "", MB_OK);
				throw std::exception();
			}
		}
*/
	}
	else
	{
		data = (uint32 *) _TIFFmalloc((tsize_t)pixelCount * sizeof(uint32));
	}

	uint64 uploadStart;

    if ((!multiByte && data != NULL) || (multiByte && multiByteData != NULL))
    {
#ifdef WINDOWS_BUILD
		uploadStart = GetTickCount64();
#else
		time_t uploadStart = time(NULL);
#endif

		if (prof != NULL)
		{
			uint64 time = 0;
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
			uint64 * tempBuffer = new uint64[stride];
			//int tamFila = width * nsamp; 
			//uint16 * bufTxt = new uint16[tamFila];
			//uint16 aux;

			for (int y = 0, offset = width * (height - 1); y < height; y++)
			{
				TIFFReadScanline(tiff, (void *)tempBuffer, y);
/*
				//for (int x = 0; x < tamFila; x ++ )
				for (int x = 0; x < width; x ++ )
				{
					//ftxt >> bufTxt[x];
//if ( x % 4 == 0 )
//fout << bufTxt[x] << " ";
fout << uint16(tempBuffer[x]&0xffff) << " ";
				}
fout << endl;
*/
				memcpy(multiByteData + offset, (void *)tempBuffer, stride);
				//memcpy(multiByteData + offset, bufTxt, tamFila * 2);
				offset -= width;
			}
			delete []tempBuffer;
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
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT, multiByteData);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

		if (generateMipmaps)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
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
			int dx = width / (subdiv + 1);
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
		else
		{
			int dx = width / (subdiv + 1);
			float maxValue = 0.0f, minValue = 1.0f;

			for (int tileX = 0; tileX < (subdiv + 1); tileX++)
			{
				for (int tileY = 0; tileY < (subdiv + 1); tileY++)
				{
					for (int x = dx * tileX; x < dx * (tileX + 1); x++)
					{
						for (int y = dx * tileY; y < dx * (tileY + 1); y++)
						{
							float currentValue = (float)data[y * width + x] / 255.0f;

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
	//ftxt.close();
//fout.close();
    return temp;
}

const std::vector<glm::vec2> & TIFFImageLoader::GetBoundingBoxes() const
{
	return _boundingBoxes;
}