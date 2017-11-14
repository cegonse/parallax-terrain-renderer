// Terrain Viewer
//
// Cubic curve, file: CubicCurve.cpp
//
// - Implementation of the CubicCurve class.
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

#include "CubicCurve.h"

CubicCurve::CubicCurve()
{
	/* Set the basis matrix as an identity matrix for
	   the base class. */
	_basisMatrix = glm::mat4(1.0f);
}

void CubicCurve::AddControlPoint(const glm::vec2 & cp)
{
	if (_controlPoints.size() == 4)
	{
		throw std::exception();
	}
	else
	{
		_controlPoints.push_back(cp);
	}
}

void CubicCurve::ResetControlPoints()
{
	_controlPoints.clear();
}

glm::vec2 CubicCurve::Evaluate(float t) const
{
	/* The curve can't be evaluated unless it has at least
	   four control points. */
	if (_controlPoints.size() != 4)
    {
		throw std::exception();
    }

    if (t == 0.0f)
    {
        return _controlPoints[0];
    }
    else if (t == 1.0f)
    {
        return _controlPoints[3];
    }

    glm::vec2 result;

    float P0x = _controlPoints[0].x;
    float P1x = _controlPoints[1].x;
    float P2x = _controlPoints[2].x;
    float P3x = _controlPoints[3].x;

    float P0y = _controlPoints[0].y;
    float P1y = _controlPoints[1].y;
    float P2y = _controlPoints[2].y;
    float P3y = _controlPoints[3].y;

	float Bm11 = _basisMatrix[0].x;
	float Bm12 = _basisMatrix[0].y;
	float Bm13 = _basisMatrix[0].z;
	float Bm14 = _basisMatrix[0].w;

	float Bm21 = _basisMatrix[1].x;
	float Bm22 = _basisMatrix[1].y;
	float Bm23 = _basisMatrix[1].z;
	float Bm24 = _basisMatrix[1].w;

	float Bm31 = _basisMatrix[2].x;
	float Bm32 = _basisMatrix[2].y;
	float Bm33 = _basisMatrix[2].z;
	float Bm34 = _basisMatrix[2].w;

	float Bm41 = _basisMatrix[3].x;
	float Bm42 = _basisMatrix[3].y;
	float Bm43 = _basisMatrix[3].z;
	float Bm44 = _basisMatrix[3].w;

    /* This could be made way faster using matrix operations */
	result.x = (Bm14 * P0x + Bm24 * P1x + Bm34 * P2x + Bm44 * P3x) * (t * t * t) +
                       (Bm13 * P0x + Bm23 * P1x + Bm33 * P2x + Bm43 * P3x) * (t * t) +
                       (Bm12 * P0x + Bm22 * P1x + Bm32 * P2x + Bm42 * P3x) * t +
                        Bm11 * P0x + Bm21 * P1x + Bm31 * P2x + Bm41 * P3x;

	result.y = (Bm14 * P0y + Bm24 * P1y + Bm34 * P2y + Bm44 * P3y) * (t * t * t) +
                       (Bm13 * P0y + Bm23 * P1y + Bm33 * P2y + Bm43 * P3y) * (t * t) + 
                       (Bm12 * P0y + Bm22 * P1y + Bm32 * P2y + Bm42 * P3y) * t + 
                       Bm11 * P0y + Bm21 * P1y + Bm31 * P2y + Bm41 * P3y;

    return result;
}

glm::vec2 CubicCurve::Derive(float t) const
{
	/* The curve can't be evaluated unless it has at least
	   four control points. */
	if (_controlPoints.size() != 4)
    {
		throw std::exception();
    }

    glm::vec2 result;

    float P0x = _controlPoints[0].x;
    float P1x = _controlPoints[1].x;
    float P2x = _controlPoints[2].x;
    float P3x = _controlPoints[3].x;

    float P0y = _controlPoints[0].y;
    float P1y = _controlPoints[1].y;
    float P2y = _controlPoints[2].y;
    float P3y = _controlPoints[3].y;

	float Bm11 = _basisMatrix[0].x;
	float Bm12 = _basisMatrix[0].y;
	float Bm13 = _basisMatrix[0].z;
	float Bm14 = _basisMatrix[0].w;

	float Bm21 = _basisMatrix[1].x;
	float Bm22 = _basisMatrix[1].y;
	float Bm23 = _basisMatrix[1].z;
	float Bm24 = _basisMatrix[1].w;

	float Bm31 = _basisMatrix[2].x;
	float Bm32 = _basisMatrix[2].y;
	float Bm33 = _basisMatrix[2].z;
	float Bm34 = _basisMatrix[2].w;

	float Bm41 = _basisMatrix[3].x;
	float Bm42 = _basisMatrix[3].y;
	float Bm43 = _basisMatrix[3].z;
	float Bm44 = _basisMatrix[3].w;

	/* This could be made way faster using matrix operations */
    result.x = (3 * t * t) * (Bm14 * P0x + Bm24 * P1x + Bm34 * P2x + Bm44 * P3x) + 
               (Bm12 * P0x + Bm22 * P1x + Bm32 * P2x + Bm42 * P3x) + 
               (2 * t) * (Bm13 * P0x + Bm23 * P1x + Bm33 * P2x + Bm43 * P3x);

    result.y = (3 * t * t) * (Bm14 * P0y + Bm24 * P1y + Bm34 * P2y + Bm44 * P3y) + 
               Bm12 * P0y + Bm22 * P1y + Bm32 * P2y + Bm42 * P3y + 
               (2 * t) * (Bm13 * P0y + Bm23 * P1y + Bm33 * P2y + Bm43 * P3y);

    return result;
}

float CubicCurve::Integrate(float t) const
{
	/* The curve can't be evaluated unless it has at least
	   four control points. */
	if (_controlPoints.size() != 4)
    {
		throw std::exception();
    }

    glm::vec2 temp;

    float P0x = _controlPoints[0].x;
    float P1x = _controlPoints[1].x;
    float P2x = _controlPoints[2].x;
    float P3x = _controlPoints[3].x;

    float P0y = _controlPoints[0].y;
    float P1y = _controlPoints[1].y;
    float P2y = _controlPoints[2].y;
    float P3y = _controlPoints[3].y;

	float Bm11 = _basisMatrix[0].x;
	float Bm12 = _basisMatrix[0].y;
	float Bm13 = _basisMatrix[0].z;
	float Bm14 = _basisMatrix[0].w;

	float Bm21 = _basisMatrix[1].x;
	float Bm22 = _basisMatrix[1].y;
	float Bm23 = _basisMatrix[1].z;
	float Bm24 = _basisMatrix[1].w;

	float Bm31 = _basisMatrix[2].x;
	float Bm32 = _basisMatrix[2].y;
	float Bm33 = _basisMatrix[2].z;
	float Bm34 = _basisMatrix[2].w;

	float Bm41 = _basisMatrix[3].x;
	float Bm42 = _basisMatrix[3].y;
	float Bm43 = _basisMatrix[3].z;
	float Bm44 = _basisMatrix[3].w;

	/* This could be made way faster using matrix operations */
    temp.x = ((Bm14 * P0x) / 4 + (Bm24 * P1x) / 4 + (Bm34 * P2x) / 4 + 
             (Bm44 * P3x) / 4) * (t * t * t * t) + ((Bm13 * P0x) / 3 + 
             (Bm23 * P1x) / 3 + (Bm33 * P2x) / 3 + (Bm43 * P3x) / 3) * (t * t * t) + 
             ((Bm12 * P0x) / 2 + (Bm22 * P1x) / 2 + (Bm32 * P2x) / 2 + 
             (Bm42 * P3x) / 2) * (t * t) + 
             (Bm11 * P0x + Bm21 * P1x + Bm31 * P2x + Bm41 * P3x) * t;

    temp.y = ((Bm14 * P0y) / 4 + (Bm24 * P1y) / 4 + (Bm34 * P2y) / 4 + 
                (Bm44 * P3y) / 4) * (t * t * t * t) + ((Bm13 * P0y) / 3 + 
                (Bm23 * P1y) / 3 + (Bm33 * P2y) / 3 + (Bm43 * P3y) / 3) * (t * t * t) + 
                ((Bm12 * P0y) / 2 + (Bm22 * P1y) / 2 + (Bm32 * P2y) / 2 + 
                (Bm42 * P3y) / 2) * (t * t) + 
                (Bm11 * P0y + Bm21 * P1y + Bm31 * P2y + Bm41 * P3y) * t;

	return glm::length(temp);
}
