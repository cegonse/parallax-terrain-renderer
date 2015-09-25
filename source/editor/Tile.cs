using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.IO;
using OpenTK;

namespace TerrenosHelper
{
    public class Tile
    {

        private BitmapTerrain _heightmapPreview;
        private Bitmap _texturePreview;

        private int _heightmapGlId = -1;
        private int _textureGlId = -1;

        private float _width = 128f, _height = 128f;

        private Vector2 _position;
        private Matrix4 _modelMatrix;

        string _texturePath;
        string _heightmapPath;

        public Tile()
        {
            _position = new Vector2();
            SetModelMatrix();

            _heightmapPreview = new BitmapTerrain(new Bitmap("data\\missing.bmp"));
            _texturePreview = new Bitmap("data\\missing.bmp");

            _texturePath = "";
            _heightmapPath = "";

            _position = new Vector2(0f, 0f);
        }

        public void LoadTextureData(string texture)
        {
            try
            {
                if (_texturePreview != null)
                {
                    _texturePreview.Dispose();
                }

                Bitmap highResolutionTexture = (Bitmap)Image.FromFile(texture);
                _texturePreview = new Bitmap(highResolutionTexture, new Size(128, 128));
                highResolutionTexture.Dispose();

                _texturePath = texture;
                _width = _texturePreview.Width;
                _height = _texturePreview.Height;

                SetModelMatrix();

                if (_textureGlId != -1)
                {
                    OpenTK.Graphics.GL.DeleteTexture(_textureGlId);
                    _textureGlId = -1;
                }
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        public void LoadHeightmapData(string heightmap)
        {
            try
            {
                if (_heightmapPreview != null)
                {
                    _heightmapPreview.GetBitmap().Dispose();
                }

                Bitmap highResolutionBitmap = (Bitmap)Image.FromFile(heightmap);
                _heightmapPreview = new BitmapTerrain(new Bitmap(highResolutionBitmap, new Size(128, 128)));
                highResolutionBitmap.Dispose();

                _heightmapPath = heightmap;

                SetModelMatrix();

                if (_heightmapGlId != -1)
                {
                    OpenTK.Graphics.GL.DeleteTexture(_heightmapGlId);
                    _heightmapGlId = -1;
                }
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        public string GetHeightmapPath()
        {
            return _heightmapPath;
        }

        public string GetTexturePath()
        {
            return _texturePath;
        }

        public void SetPosition(Vector2 newPosition)
        {
            _position = newPosition;
            SetModelMatrix();
        }

        private void SetModelMatrix()
        {
            _modelMatrix = Matrix4.CreateScale(256f, 256f, 1f) * Matrix4.CreateTranslation(_position.X * 256f, -(_position.Y * 256f), 0f);
        }

        public Matrix4 GetModelMatrix()
        {
            return _modelMatrix;
        }

        public Bitmap GetHeightmapPreview()
        {
            return _heightmapPreview.GetBitmap();
        }

        public Bitmap GetTexturePreview()
        {
            return _texturePreview;
        }

        public void SetHeightmapGlId(int id)
        {
            _heightmapGlId = id;
        }

        public void SetTextureGlId(int id)
        {
            _textureGlId = id;
        }

        public int GetHeightmapGlId()
        {
            return _heightmapGlId;
        }

        public int GetTextureGlId()
        {
            return _textureGlId;
        }

        public void OnDispose()
        {
            if (_texturePreview != null)
                _texturePreview.Dispose();

            if (_heightmapPreview != null)
                _heightmapPreview.GetBitmap().Dispose();

            if (_textureGlId != -1)
                OpenTK.Graphics.GL.DeleteTexture(_textureGlId);

            if (_heightmapGlId != -1)
                OpenTK.Graphics.GL.DeleteTexture(_heightmapGlId);
        }

        public Vector2 GetPosition()
        {
            return _position;
        }

        public void SetTexturePath(string path)
        {
            _texturePath = path;
        }

        public void SetHeightmapPath(string path)
        {
            _heightmapPath = path;
        }
    }
}
