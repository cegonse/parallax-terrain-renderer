// Terrain Viewer
//
// Bezier curve, file: BezierCurve.h
//
// - BezierCurve is a derived class from the CubicCurve
//	 class implementing a Bezier cubic curve.
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
#ifndef _BEZIERCURVE_H_
#define _BEZIERCURVE_H_

#include "CubicCurve.h"

class BezierCurve : public CubicCurve
{
public:
	BezierCurve();
};

#endif