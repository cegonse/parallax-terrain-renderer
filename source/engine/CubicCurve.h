// Terrain Viewer
//
// Cubic curve, file: BezierCurve.h
//
// - The Cubic curve class offers an implementation of
//   a cubic curve that can be evaluated, differentiated
//	 or integrated on any point of its interval.
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
#ifndef _CUBICCURVE_H_
#define _CUBICCURVE_H_

#include <glm\glm.hpp>
#include <vector>

class CubicCurve
{
public:
	/*
	  CubicCurve (Constructor),
	    this constructor creates a cubic curve with an identity matrix
		as its basis matrix. To use a different basis, use the BezierCurve
		or UniformBSpline classes.
	*/
	CubicCurve();

	/*
	  AddControlPoint,
	    adds a control point to the cubic curve.

	  Input:
	   - cp (glm::vec2): coordinates of the control point.
	*/
	void AddControlPoint(const glm::vec2 & cp);

	/*
	  ResetControlPoints,
	    resets all control points to its initial state.
	*/
	void ResetControlPoints();

	/*
	  Evaluate,
	    evaluates the curve in the time instant t.

	  Input:
	   - t (float): time instant to evaluate.
	  
	  Output:
	   - (glm::vec2): coordinates of the cubic curve on
	                  time t.
	*/
	const glm::vec2 & Evaluate(float t) const;

	/*
	*/
	const glm::vec2 & Derive(float t) const;

	/*
	*/
	float Integrate(float t) const;

private:
	std::vector<glm::vec2> _controlPoints;

protected:
	glm::mat4 _basisMatrix;
};

#endif