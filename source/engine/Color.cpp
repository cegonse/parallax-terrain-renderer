// Terrain Viewer
//
// Color implementation, file: Color.cpp
//
// - This file is the implementation file of the Color class.
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


#include "Color.h"

Color::Color(float r, float g, float b, float a)
{
    _red = r;
    _green = g;
    _blue = b;
    _alpha = a;

    _glArraySingleton = new GLfloat[4];
    _glArraySingleton[0] = _red;
    _glArraySingleton[1] = _green;
    _glArraySingleton[2] = _blue;
    _glArraySingleton[3] = _alpha;



    _nativeArraySingleton = new float[4];
    _nativeArraySingleton[0] = _red;
    _nativeArraySingleton[1] = _green;
    _nativeArraySingleton[2] = _blue;
    _nativeArraySingleton[3] = _alpha;
}

Color::~Color()
{
    if (_glArraySingleton != NULL)
    {
        delete[] _glArraySingleton;
    }

    if (_nativeArraySingleton != NULL)
    {
        delete[] _nativeArraySingleton;
    }
}

GLfloat* Color::Get() const
{
    return _glArraySingleton;
}

float* Color::GetNative() const
{
    return _nativeArraySingleton;
}

