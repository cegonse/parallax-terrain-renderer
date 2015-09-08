// Terrain Viewer
//
// Uniform B-Spline, file: UniformBSpline.cpp
//
// - Implementation of the Uniform B-Spline class.
//
// Copyright (c) C�sar Gonz�lez Segura 2015, ETSE-UV
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
#include "UniformBSpline.h"

UniformBSpline::UniformBSpline()
{
	CubicCurve();

	_basisMatrix = glm::mat4(1, -3,  3, -1,
							 4,  0, -6,  3,
							 1,  3,  3, -3,
							 0,  0,  0,  1);
}