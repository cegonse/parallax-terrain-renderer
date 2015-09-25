// Terrain Viewer
//
// Bezier curve, file: BezierCurve.cpp
//
// - Implementation of the BezierCurve class
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

#include "BezierCurve.h"

BezierCurve::BezierCurve()
{
	CubicCurve();

	_basisMatrix = glm::mat4(1, -3,  3, -1,
							 0,  3, -6,  3,
							 0,  0,  3, -3,
							 0,  0,  0,  1);
}