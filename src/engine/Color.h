// Terrain Viewer
//
// Color class header, file: Color.h
//
// - This file is the header for the Color class.
//	 Color is a helper class containing four values
//	 for each color channel.
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


#ifndef COLOR_H
#define COLOR_H

#include <GL/glew.h>
#include <boost/shared_array.hpp>

class Color
{
    public:
		/*
		  Color (constructor)

		  Input:
		   - r (float): intensity of the red component.
		   - g (float): intensity of the green component.
		   - b (float): intensity of the blue component.
		   - a (float): intensity of the alpha component.
		*/
        Color(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);

		/*
		  Get,
		    get a pointer to the color as a plataform independent OpenGL array.
		  
		  Output:
		   - (GLfloat[]): array to the color data.
		*/
        GLfloat* Get() const;

		/*
		  GetNative,
		    get a pointer tot he color as a plataform native array.
		  
		  Output:
		   - (float[]): array to the color data.
		*/
        float* GetNative() const;

        ~Color();

    private:
        float _red;
        float _green;
        float _blue;
        float _alpha;

        GLfloat* _glArraySingleton;
        float* _nativeArraySingleton;
};

#endif // COLOR_H
