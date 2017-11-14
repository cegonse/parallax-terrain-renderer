// Terrain Viewer
//
// Project, file: Project.cpp
//
// - Implementation of the Project class.
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
#include "Project.h"
#include "Renderer.h"

Project::Project(const std::string & name)
{
    _name = name;
    _author = "";
    _date = "";

    _relativeHeight = 0.0f;
    _constantHeight = 0.0f;
    _cameraMode = CameraModeConstantHeight;
    _resolution = 1.0f;
}

Project::~Project()
{
	/*for (unsigned int i = 0; i < _cameras.size(); i++)
	{
		delete _cameras[i];
	}

	for (unsigned int i = 0; i < _tiles.size(); i++)
	{
		delete _tiles[i]->terrain;
		delete _tiles[i];
	}*/
}

void Project::SetProjectName(const std::string & name)
{
    _name = name;
}

const std::string & Project::GetProjectName() const
{
    return _name;
}

void Project::SetProjectAuthor(const std::string & author)
{
    _author = author;
}

const std::string & Project::GetProjectAuthor() const
{
    return _author;
}

void Project::SetProjectCreationDate(const std::string & date)
{
    _date = date;
}

const std::string & Project::GetProjectCreationDate() const
{
    return _date;
}

void Project::SetProjectResolution(float resolution)
{
    _resolution = resolution;
}

float Project::GetProjectResolution() const
{
    return _resolution;
}

void Project::SetRelativeHeight(float height)
{
    _relativeHeight = height;
}

float Project::GetRelativeHeight() const
{
    return _relativeHeight;
}

void Project::SetConstantHeight(float height)
{
    _constantHeight = height;
}

float Project::GetConstantHeight() const
{
    return _constantHeight;
}

void Project::SetCameraMode(CameraMode mode)
{
    _cameraMode = mode;
}

CameraMode Project::GetCameraMode() const
{
    return _cameraMode;
}

void Project::AddTile(ProjectTile* tile)
{
    _tiles.push_back(tile);
}

const std::vector<ProjectTile*> & Project::GetTileList() const
{
    return _tiles;
}

void Project::AddCamera(Camera* camera)
{
    _cameras.push_back(camera);
}

const std::vector<Camera*> & Project::GetCameraList() const
{
    return _cameras;
}

Project* MarshallProject(const std::string & path, Renderer* renderer)
{
    Project* proj = new Project("");
	/* Load the project's XML file */
    irr::io::IrrXMLReader* reader = irr::io::createIrrXMLReader(path.c_str());
    ProjectElement nextElement = ProjectElementNone;

	Profiler* prof = renderer->GetProfiler();
	uint64_t startTime = GetTickCount64();

	int heightmapSize = -1;
	int textureSize = -1;

    float tileLatitude = 0.0f;
    float tileLongitude = 0.0f;

    float cameraX = 0.0f;
    float cameraY = 0.0f;
    float cameraYaw = 0.0f;

    while (reader && reader->read())
    {
        switch (reader->getNodeType())
        {
            case irr::io::EXN_ELEMENT:
                {
                    std::string elementName = reader->getNodeName();

                    if (elementName == "name")
                    {
                        nextElement = ProjectElementName;
                    }
                    else if (elementName == "author")
                    {
                        nextElement = ProjectElementAuthor;
                    }
                    else if (elementName == "resolution")
                    {
                        nextElement = ProjectElementResolution;
                    }
                    else if (elementName == "date")
                    {
                        nextElement = ProjectElementDate;
                    }
                    else if (elementName == "camera-mode")
                    {
                        nextElement = ProjectElementCameraMode;
                    }
                    else if (elementName == "height")
                    {
                        nextElement = ProjectElementRelativeHeight;
                    }
                    else if (elementName == "tile")
                    {
                        nextElement = ProjectElementTile;
                    }
                    else if (elementName == "latitude")
                    {
                        nextElement = ProjectElementTileLatitude;
                    }
                    else if (elementName == "longitude")
                    {
                        nextElement = ProjectElementTileLongitude;
                    }
                    else if (elementName == "camera")
                    {
                        nextElement = ProjectElementCamera;
                    }
                    else if (elementName == "x")
                    {
                        nextElement = ProjectElementCameraX;
                    }
                    else if (elementName == "y")
                    {
                        nextElement = ProjectElementCameraY;
                    }
                    else if (elementName == "yaw")
                    {
                        nextElement = ProjectElementCameraYaw;
                    }
                    else
                    {
                        nextElement = ProjectElementNone;
                    }
                }

                break;

            case irr::io::EXN_TEXT:
                {
                    switch (nextElement)
                    {
                        case ProjectElementName:
                            proj->SetProjectName(reader->getNodeData());
							prof->SetProjectName(proj->GetProjectName());
                            break;

                        case ProjectElementAuthor:
                            proj->SetProjectAuthor(reader->getNodeData());
                            break;

                        case ProjectElementDate:
                            proj->SetProjectCreationDate(reader->getNodeData());
                            break;

                        case ProjectElementCameraMode:
                            {
                                std::string cmode = reader->getNodeData();

                                if (cmode == "constant-height")
                                {
                                    proj->SetCameraMode(CameraModeConstantHeight);
                                }
                                else if (cmode == "relative-height")
                                {
                                    proj->SetCameraMode(CameraModeRelativeHeight);
                                }
                            }
                            break;

                        case ProjectElementRelativeHeight:
                        case ProjectElementConstantHeight:
							{
								std::string hStr = reader->getNodeData();
								std::cout << hStr << std::endl;

								try
								{
									float height = boost::lexical_cast<float>(hStr);
									proj->SetConstantHeight(height);
									proj->SetRelativeHeight(height);
								}
								catch (const boost::bad_lexical_cast & ex)
								{
									throw ex;
								}
							}
                            break;

                        case ProjectElementResolution:
                            try
                            {
                                float resolution = boost::lexical_cast<float>(reader->getNodeData());
                                proj->SetProjectResolution(resolution);
                            }
                            catch (const boost::bad_lexical_cast & ex)
                            {
                                throw ex;
                            }
                            break;

                        case ProjectElementTileLatitude:
                            try
                            {
                                tileLatitude = boost::lexical_cast<float>(reader->getNodeData());
                            }
                            catch (const boost::bad_lexical_cast & ex)
                            {
                                throw ex;
                            }
                            break;

                        case ProjectElementTileLongitude:
                            {
                                try
                                {
                                    tileLongitude = boost::lexical_cast<float>(reader->getNodeData());
                                }
                                catch (const boost::bad_lexical_cast & ex)
                                {
                                    throw ex;
                                }

                                /* Extract the project path from the file name */
                                std::string pathBase = "";

                                for (size_t i = path.length(); i >= 0; i--)
                                {
                                    if (path[i] == '\\' || path[i] == '/')
                                    {
                                        pathBase = path.substr(0, i);
                                        break;
                                    }
                                }

                                /* Create the path of the splat texture and heightmap */
                                std::string texture = pathBase;
                                texture.append("\\textures\\");
                                texture.append(boost::lexical_cast<std::string>(tileLatitude));
                                texture.append("_");
                                texture.append(boost::lexical_cast<std::string>(tileLongitude));
                                texture.append(".tiff");

                                std::string heightmap = pathBase;
                                heightmap.append("\\heightmaps\\");
                                heightmap.append(boost::lexical_cast<std::string>(tileLatitude));
                                heightmap.append("_");
                                heightmap.append(boost::lexical_cast<std::string>(tileLongitude));
                                heightmap.append(".tiff");

                                /* Add the tile to the list */
                                Texture* textureData = renderer->CreateMipmapTexture(texture);
// BEGIN_MARIANO: Con mipmapping en el mapa de alturas
                                Texture* heightmapData = renderer->CreateMipmapTexture(heightmap, 16);
// END_MARIANO: Con mipmapping
// BEGIN_MARIANO: Sin mipmapping en el mapa de alturas
                                //Texture* heightmapData = renderer->CreateTexture(heightmap, 9);
// END_MARIANO: Sin mipmapping

								if (textureData == NULL || heightmapData == NULL)
								{
									throw std::exception();
								}

								if (textureSize == -1 && heightmapSize == -1)
								{
									textureSize = textureData->GetHeight() * textureData->GetWidth() * 4;
									heightmapSize = heightmapData->GetHeight() * heightmapData->GetWidth() * 4;
								}

                                TerrainData tData;
                                tData.format = TERRAIN_FORMAT_RED_CHANNEL;
                                tData.length = (float)textureData->GetHeight();
                                tData.width = (float)textureData->GetWidth();
// BEGIN_MARIANO: Comentar
//                              tData.scale = 0.32f;
// END_MARIANO: Comentar
// BEGIN_MARIANO: Sustituir por
                                tData.scale = 1.0f;
// END_MARIANO: Comentar

								GLint tessLevels = 0;
								glGetIntegerv(GL_MAX_TESS_GEN_LEVEL, &tessLevels);

                                tData.levelOfDetail = (float)tessLevels;
                                tData.units = TERRAIN_UNITS_METER;

                                Terrain* terrainTemp = renderer->CreateTerrain(tData, heightmapData, textureData, 16);

                                ProjectTile* pTile = new ProjectTile;

                                pTile->terrain = terrainTemp;
                                pTile->x = -tileLatitude;
                                pTile->y = tileLongitude;

                                proj->AddTile(pTile);
                            }
                            break;

                        case ProjectElementCameraX:
                            try
                            {
                                std::string scx = reader->getNodeData();

                                for (unsigned int i = 0; i < scx.length(); i++)
                                {
                                    if (scx[i] == ',')
                                    {
                                        scx[i] = '.';
                                    }
                                }

                                cameraX = boost::lexical_cast<float>(scx);
                            }
                            catch (const boost::bad_lexical_cast & ex)
                            {
                                throw ex;
                            }
                            break;

                        case ProjectElementCameraY:
                            try
                            {
                                std::string scy = reader->getNodeData();

                                for (unsigned int i = 0; i < scy.length(); i++)
                                {
                                    if (scy[i] == ',')
                                    {
                                        scy[i] = '.';
                                    }
                                }

                                cameraY = boost::lexical_cast<float>(scy);
                            }
                            catch (const boost::bad_lexical_cast & ex)
                            {
                                throw ex;
                            }
                            break;

                        case ProjectElementCameraYaw:
                            {
                                try
                                {
                                    std::string scyaw = reader->getNodeData();

                                    for (unsigned int i = 0; i < scyaw.length(); i++)
                                    {
                                        if (scyaw[i] == ',')
                                        {
                                            scyaw[i] = '.';
                                        }
                                    }

                                    cameraYaw = boost::lexical_cast<float>(scyaw);
                                }
                                catch (const boost::bad_lexical_cast & ex)
                                {
                                    throw ex;
                                }

                                float height = (proj->GetCameraMode() == CameraModeConstantHeight) ? proj->GetConstantHeight() : proj->GetRelativeHeight();

								float terrainScale = proj->GetTileList()[0]->terrain->GetTerrainData().scale;

								glm::vec3 origin(cameraX * terrainScale, height, cameraY * terrainScale);
                                glm::vec3 target(cameraX * terrainScale + 1.1, height - 0.1, cameraY * terrainScale + 1.1);

                                Camera* cam = renderer->CreateCamera(origin, target, 45.0f, false);
                                proj->AddCamera(cam);
                            }
                            break;

                        case ProjectElementNone:
                            reader->getNodeData();
                            break;
                    }
                }
                break;

            default:
                break;
        }
    }

    delete reader;

	uint64_t endTime = GetTickCount64();
	prof->SetProjectLoadingTime((endTime - startTime) / 1000.0f);
	prof->SetGpuMemoryUsed(renderer->GetTotalVideoMemory() - renderer->GetAvailableVideoMemory());
	prof->SetEstimatedMemoryUsed((proj->GetTileList().size() * (heightmapSize + textureSize)) / 1000000);

    return proj;
}

void SerializeProject(const std::string & path, Project* proj)
{

}
