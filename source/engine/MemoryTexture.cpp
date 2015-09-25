// Terrain Viewer
//
// Memory texture, file: MemoryTexture.cpp
//
// - Implementation of the Memory texture loader class.
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


#include "MemoryTexture.h"

MemoryTexture::MemoryTexture()
{
	_buffer = NULL;
	_width = 0;
	_height = 0;
}

MemoryTexture::MemoryTexture(int width, int height)
{
	_width = width;
	_height = height;

	Allocate();
}

MemoryTexture::~MemoryTexture()
{
	if (_buffer != NULL)
	{
		delete _buffer;
	}
}

int MemoryTexture::GetWidth() const
{
	return _width;
}

int MemoryTexture::GetHeight() const
{
	return _height;
}

void MemoryTexture::Set(GLubyte* data)
{
	if (_buffer == NULL)
	{
		Allocate();
	}

	memcpy(_buffer, data, 3 * _width * _height);
}

GLubyte* MemoryTexture::Get() const
{
	return _buffer;
}

void MemoryTexture::Allocate()
{
	_buffer = new GLubyte[3 * _width * _height];
}