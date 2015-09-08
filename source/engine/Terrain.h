#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <boost/scoped_array.hpp>

#include "Texture.h"
#include "ShaderProgram.h"

enum TerrainUnits
{
    TERRAIN_UNITS_KILOMETER = 0,
    TERRAIN_UNITS_METER
};

enum TerrainFormat
{
    TERRAIN_FORMAT_RED_CHANNEL = 0,
    TERRAIN_FORMAT_RED_GREEN_CHANNEL = 1,
    TERRAIN_FORMAT_RED_GREEN_BLUE_CHANNEL = 2
};

struct TerrainData
{
    unsigned char units;
    float scale;
    float levelOfDetail;
    unsigned char format;

    float width;
    float length;
};

class Terrain
{
    public:
        Terrain(const TerrainData & data, Texture* height, Texture* texture, ShaderProgram* shader, int subdivisionCount = 1);
        ~Terrain();

        Texture* GetHeightmap() const;
        Texture* GetTerrainTexture() const;
        GLuint GetTerrainVAO() const;
		int GetSubdivisionCount() const;
		int GetPointsPerPatch() const;
        ShaderProgram* GetShaderProgram() const;

        const TerrainData & GetTerrainData() const;
        void SetTerrainData(const TerrainData & data);

    private:
        Texture* _heightMap;
        Texture* _terrainTexture;
        TerrainData _data;

        GLuint _ctrlPointsVbo;
		GLuint _boundingBoxVbo;
		GLuint _uvVbo;

		GLuint _vao;

		int _subdivisionCount;
		int _pointsPerPatch;
        ShaderProgram* _terrainShader;
};

#endif // _TERRAIN_H_
