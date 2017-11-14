using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OpenTK;

namespace TerrenosHelper
{
    public class Camera
    {
        private Vector2 _position;
        private float _rotation;
        private Matrix4 _modelMatrix;
        private float _zoomScale;

        public Camera()
        {
            _position = new Vector2(0f, 0f);
            _rotation = 0f;
            _zoomScale = 1f;

            SetModelMatrix();
        }

        public void SetCameraZoom(float zoom)
        {
            _zoomScale = zoom;
            SetModelMatrix();
        }

        public void SetPosition(Vector2 position)
        {
            _position = position;
            SetModelMatrix();
        }

        public void SetRotation(float rotation)
        {
            _rotation = rotation;
            SetModelMatrix();
        }

        public Vector2 GetPosition()
        {
            return _position;
        }

        public float GetRotation()
        {
            return _rotation;
        }

        private void SetModelMatrix()
        {
            _modelMatrix = Matrix4.CreateScale(64f * _zoomScale, 64f * _zoomScale, 1f) * Matrix4.CreateRotationZ(_rotation * 0.0174532925f) * Matrix4.CreateTranslation(_position.X * 256f, -(_position.Y * 256f), 0f);
        }

        public Matrix4 GetModelMatrix()
        {
            return _modelMatrix;
        }
    }
}
