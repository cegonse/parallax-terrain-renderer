using System;
using System.Collections.Generic;
using System.Collections;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TerrenosHelper
{
    public enum CameraMode : byte
    {
        CameraModeConstantHeight = 0,
        CameraModeRelativeHeight
    };

    public class Project
    {
        private List<Camera> _cameras;
        private List<Tile> _tiles;

        private CameraMode _cameraMode;
        private float _relativeHeight;
        private float _constantHeight;
        private float _resolution;

        private string _name;
        private DateTime _creationTime;
        private string _author;

        public Project(string name)
        {
            _constantHeight = 0;
            _relativeHeight = 1;
            _resolution = 1f;
            _cameraMode = CameraMode.CameraModeRelativeHeight;

            _cameras = new List<Camera>();
            _tiles = new List<Tile>();

            _name = name;
            _creationTime = DateTime.Now;

            _author = "No author";
        }

        public Project Clone(bool cloneBitmaps = false)
        {
            Project proj = new Project(_name);
            proj._constantHeight = _constantHeight;
            proj._relativeHeight = _relativeHeight;
            proj._resolution = _resolution;
            proj._cameraMode = _cameraMode;
            proj._author = _author;
            proj._tiles = new List<Tile>();

            for (int i = 0; i < _tiles.Count; i++)
            {
                Tile temp = new Tile();
                temp.SetPosition(_tiles[i].GetPosition());
                
                if (!cloneBitmaps)
                {
                    temp.SetHeightmapPath(_tiles[i].GetHeightmapPath());
                    temp.SetTexturePath(_tiles[i].GetTexturePath());
                }

                proj._tiles.Add(temp);
            }

            for (int j = 0; j < _cameras.Count; j++)
            {
                Camera cam = new Camera();

                cam.SetPosition(_cameras[j].GetPosition());
                cam.SetRotation(_cameras[j].GetRotation());

                proj._cameras.Add(cam);
            }

            return proj;
        }

        public void SetProjectName(string name)
        {
            _name = name;
        }

        public void SetProjectAuthor(string author)
        {
            _author = author;
        }

        public void SetCameraMode(CameraMode mode)
        {
            _cameraMode = mode;
        }

        public void SetRelativeHeight(float height)
        {
            _relativeHeight = height;
        }

        public void SetConstantHeight(float height)
        {
            _constantHeight = height;
        }

        public CameraMode GetCameraMode()
        {
            return _cameraMode;
        }

        public float GetConstantHeight()
        {
            return _constantHeight;
        }

        public float GetRelativeHeight()
        {
            return _relativeHeight;
        }

        public void SetResolution(float res)
        {
            _resolution = res;
        }

        public float GetResolution()
        {
            return _resolution;
        }

        public List<Camera> GetCameraList()
        {
            return _cameras;
        }

        public List<Tile> GetTileList()
        {
            return _tiles;
        }

        public string GetProjectName()
        {
            return _name;
        }

        public string GetProjectAuthor()
        {
            return _author;
        }

        public DateTime GetCreationDate()
        {
            return _creationTime;
        }

        public void AddTile()
        {
            Tile t = new Tile();
            _tiles.Add(t);
        }

        public void AddCamera()
        {
            Camera c = new Camera();
            _cameras.Add(c);
        }

        public void OnDispose()
        {
            for (int i = 0; i < _tiles.Count; i++)
            {
                _tiles[i].OnDispose();
            }
        }
    }
}
