// Terrain Viewer
//
// Terrain, file: Terrain.cpp
//
// - Implementation of the Terrain class.
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
#include "Terrain.h"

Terrain::Terrain(const TerrainData & data, Texture* height, Texture* texture, ShaderProgram* shader, int subdivisionCount)
{
    _heightMap = height;
    _terrainTexture = texture;
    _terrainShader = shader;

    _heightMap->Grab();
    _terrainTexture->Grab();

    _data = data;
	_subdivisionCount = subdivisionCount;

	// Allocate vector to store control points coordinates
	std::vector<glm::vec4> v_ctrlPoints;

	// Step per control point
	float dx = 1.0f;
	float dz = 1.0f;

	float x = 0.0f;
	float z = 0.0f;

	for (int i = 0; i < subdivisionCount + 1; i++)
	{
		for (int j = 0; j < subdivisionCount + 1; j++)
		{
			glm::vec4 temp(x, 0.0f, z, 1.0f);
			v_ctrlPoints.push_back(temp);

			x += dx;
		}

		x = 0.0f;
		z += dz;
	}

	_pointsPerPatch = (int)v_ctrlPoints.size();

	// Allocate vector to store UV coordinates
	std::vector<glm::vec2> v_uvCoords;

	// Step per control point
	float s = 1.0f;
	float t = 1.0f;

	float ds = 1.0f / (float)subdivisionCount;
	float dt = 1.0f / (float)subdivisionCount;
	
	// UV coordinate calculation
	for (int i = 0; i < subdivisionCount + 1; i++)
	{
		for (int j = 0; j < subdivisionCount + 1; j++)
		{
			v_uvCoords.push_back(glm::vec2(s, t));
			s -= ds;
		}

		s = 1.0f;
		t -= dt;
	}

    glGenBuffers(1, &_ctrlPointsVbo);

	// Loading control points to the GPU
	{
		boost::scoped_array<GLfloat> vt_ctrlPoints(new GLfloat[v_ctrlPoints.size() * 4]);
		int counter = 0;

		for (int i = 0; i < v_ctrlPoints.size() * 4; i += 4)
		{
			vt_ctrlPoints[i] = v_ctrlPoints[counter].x;
			vt_ctrlPoints[i + 1] = v_ctrlPoints[counter].y;
			vt_ctrlPoints[i + 2] = v_ctrlPoints[counter].z;
			vt_ctrlPoints[i + 3] = v_ctrlPoints[counter].w;

			counter++;
		}

		glBindBuffer(GL_ARRAY_BUFFER, _ctrlPointsVbo);
		glBufferData(GL_ARRAY_BUFFER, v_ctrlPoints.size() * 4 * sizeof(GLfloat), vt_ctrlPoints.get(), GL_STATIC_DRAW);
	}

	glGenBuffers(1, &_boundingBoxVbo);

	// Loading bounding box to the GPU
	{
		std::vector<glm::vec2> v_boundingBox = height->GetBoundingBoxes();
		boost::scoped_array<GLfloat> vt_boundingBox(new GLfloat[v_boundingBox.size() * 2]);
		int counter = 0;

		for (int i = 0; i < v_boundingBox.size() * 2; i += 2)
		{
			vt_boundingBox[i] = v_boundingBox[counter].s;
			vt_boundingBox[i + 1] = v_boundingBox[counter].t;

			counter++;
		}

		glBindBuffer(GL_ARRAY_BUFFER, _boundingBoxVbo);
		glBufferData(GL_ARRAY_BUFFER, v_boundingBox.size() * 2 * sizeof(GLfloat), vt_boundingBox.get(), GL_STATIC_DRAW);
	}

	glGenBuffers(1, &_uvVbo);

	// Loading UV coordinates to the GPU
	{
		boost::scoped_array<GLfloat> vt_uvCoords(new GLfloat[v_uvCoords.size() * 2]);
		int counter = 0;

		for (int i = 0; i < (v_uvCoords.size() * 2) - 2; i += 2)
		{
			vt_uvCoords[i] = v_uvCoords[counter].s;
			vt_uvCoords[i + 1] = v_uvCoords[counter].t;

			counter++;
		}

		glBindBuffer(GL_ARRAY_BUFFER, _uvVbo);
		glBufferData(GL_ARRAY_BUFFER, v_uvCoords.size() * 2 * sizeof(GLfloat), vt_uvCoords.get(), GL_STATIC_DRAW);
	}

	glUseProgram(shader->GetProgramId());

	GLuint ctrlPointsId = glGetAttribLocation(shader->GetProgramId(), "aVertices");
	GLuint boundingBoxId = glGetAttribLocation(shader->GetProgramId(), "aBoundingBox");
	GLuint uvCoordsId = glGetAttribLocation(shader->GetProgramId(), "aUvCoordinates");

	glGenVertexArrays(1, &_vao);

	// Vertex attribute set-up
	glBindVertexArray(_vao);
		// Set-up control points
		if (ctrlPointsId != -1)
		{
			glBindBuffer(GL_ARRAY_BUFFER, _ctrlPointsVbo);
			glEnableVertexAttribArray(ctrlPointsId);
			glVertexAttribPointer(ctrlPointsId, 4, GL_FLOAT, GL_FALSE, 0, (void*) 0);
		}

		// Set-up bounding box
		if (boundingBoxId != -1)
		{
			glBindBuffer(GL_ARRAY_BUFFER, _boundingBoxVbo);
			glEnableVertexAttribArray(boundingBoxId);
			glVertexAttribPointer(boundingBoxId, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
		}

		// Set-up UV Coordinates
		if (uvCoordsId != -1)
		{
			glBindBuffer(GL_ARRAY_BUFFER, _uvVbo);
			glEnableVertexAttribArray(uvCoordsId);
			glVertexAttribPointer(uvCoordsId, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
		}

		glPatchParameteri(GL_PATCH_VERTICES, 1);
	glBindVertexArray(0);
	glUseProgram(0);
}

Terrain::~Terrain()
{
    _heightMap->Drop();
    _terrainTexture->Drop();

	glDeleteBuffers(1, &_ctrlPointsVbo);
	glDeleteBuffers(1, &_boundingBoxVbo);
	glDeleteBuffers(1, &_uvVbo);

	glDeleteVertexArrays(1, &_vao);
}

void Terrain::SetTerrainData(const TerrainData & data)
{
    _data = data;
}

Texture* Terrain::GetHeightmap() const
{
    return _heightMap;
}

Texture* Terrain::GetTerrainTexture() const
{
    return _terrainTexture;
}

GLuint Terrain::GetTerrainVAO() const
{
    return _vao;
}

ShaderProgram* Terrain::GetShaderProgram() const
{
    return _terrainShader;
}

const TerrainData & Terrain::GetTerrainData() const
{
    return _data;
}

int Terrain::GetSubdivisionCount() const
{
	return _subdivisionCount;
}

int Terrain::GetPointsPerPatch() const
{
	return _pointsPerPatch;
}