#ifndef _PROJECT_H_
#define _PROJECT_H_

#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>

#include "Terrain.h"
#include "Camera.h"
#include "irrXML.h"

class Renderer;

enum
{
    CameraModeConstantHeight = 0,
    CameraModeRelativeHeight
};

enum
{
    ProjectElementNone = 0,
    ProjectElementName,
    ProjectElementAuthor,
    ProjectElementDate,
    ProjectElementCameraMode,
    ProjectElementConstantHeight,
    ProjectElementRelativeHeight,
    ProjectElementResolution,
    ProjectElementTile,
    ProjectElementTileLatitude,
    ProjectElementTileLongitude,
    ProjectElementCamera,
    ProjectElementCameraX,
    ProjectElementCameraY,
    ProjectElementCameraYaw
};

typedef int CameraMode;
typedef int ProjectElement;

struct ProjectTile
{
    float x;
    float y;
    Terrain* terrain;
};

class Project
{
    public:
        Project(const std::string & name);
        ~Project();

        void SetProjectName(const std::string & name);
        const std::string & GetProjectName() const;

        void SetProjectAuthor(const std::string & author);
        const std::string & GetProjectAuthor() const;

        void SetProjectCreationDate(const std::string & date);
        const std::string & GetProjectCreationDate() const;

        void SetProjectResolution(float resolution);
        float GetProjectResolution() const;

        void SetRelativeHeight(float height);
        float GetRelativeHeight() const;

        void SetConstantHeight(float height);
        float GetConstantHeight() const;

        void SetCameraMode(CameraMode mode);
        CameraMode GetCameraMode() const;

        void AddTile(ProjectTile* tile);
        const std::vector<ProjectTile*> & GetTileList() const;

        void AddCamera(Camera* camera);
        const std::vector<Camera*> & GetCameraList() const;

    private:
        CameraMode _cameraMode;
        float _constantHeight;
        float _relativeHeight;
        float _resolution;

        std::vector<Camera*> _cameras;
        std::vector<ProjectTile*> _tiles;

        std::string _name;
        std::string _author;
        std::string _date;
};

Project* MarshallProject(const std::string & path, Renderer* renderer);
void SerializeProject(const std::string & path, Project* proj);

#endif // _PROJECT_H_
