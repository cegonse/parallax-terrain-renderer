// Terrain Viewer
//
// Screenshot Writter, file: ScreenshotWritter.cpp
//
// - Implementation of the Screenshot writter methods.
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
#include "ScreenshotWritter.h"

void SaveScreenshot(MemoryTexture* memtex, const std::string & path)
{
	/* Create the TIFF file for writing */
	TIFF* file = TIFFOpen(path.c_str(), "w");
	/* Grab the texture data */
	GLubyte* data = memtex->Get();

	if (file == NULL)
	{
		throw std::exception();
	}

	/* TIFF file header settings */
	TIFFSetField (file, TIFFTAG_IMAGEWIDTH, memtex->GetWidth());
	TIFFSetField(file, TIFFTAG_IMAGELENGTH, memtex->GetHeight());
	TIFFSetField(file, TIFFTAG_SAMPLESPERPIXEL, 3);
	TIFFSetField(file, TIFFTAG_BITSPERSAMPLE, 8);
	TIFFSetField(file, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
	TIFFSetField(file, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(file, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

	tsize_t bytesPerLine = 3 * memtex->GetWidth();
	unsigned char* lineBuffer = NULL;

	/* Allocate memory for the output TIFF file */
	if (TIFFScanlineSize(file) == bytesPerLine)
	{
		lineBuffer = (unsigned char*) _TIFFmalloc(bytesPerLine);
	}
	else
	{
		lineBuffer = (unsigned char*) _TIFFmalloc(TIFFScanlineSize(file));
	}

	TIFFSetField(file, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(file, 3 * memtex->GetWidth()));

	int height = memtex->GetHeight();

	/* Write all data */
	for (int y = 0; y < height; y++)
	{
		memcpy(lineBuffer, &data[(height - y - 1) * bytesPerLine], bytesPerLine);

		if (TIFFWriteScanline(file, lineBuffer, y, 0) < 0)
		{
			break;
		}
	}

	/* Close the stream */
	TIFFClose(file);

	if (lineBuffer)
	{
		_TIFFfree(lineBuffer);
	}
}