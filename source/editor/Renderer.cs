using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Drawing.Imaging;
using OpenTK.Graphics.OpenGL;
using OpenTK;
using System.Windows.Forms;

namespace TerrenosHelper
{
    public class Renderer
    {
        private Color _workspaceColor;
        private int _width;
        private int _height;
        private ViewerForm _viewer;

        private Matrix4 _viewMatrix;
        private Matrix4 _projectionMatrix;

        private Vector2 _cameraPosition;
        private float _cameraZoom = 1f;

        private int _shaderProgram;
        private int _lineShaderProgram;
        private int _quadVbo;
        private int _quadUvVbo;
        private int _lineVbo;
        private int _vao;
        private int _cameraDecalTexture = -1;

        private const string _vertexShaderCode = 
            @"#version 330 core
                
              in vec3 aVertices;
              in vec2 aUvCoordinates;
              out vec2 vUvCoordinates;

              uniform mat4 uMvpMatrix;
              
              void main() {
                  gl_Position = uMvpMatrix * vec4(aVertices, 1.0f);
                  vUvCoordinates = aUvCoordinates;
            }";

        private const string _fragmentShaderCode =
            @"#version 330 core

              in vec2 vUvCoordinates;
              out vec4 fFragmentColor;

              uniform sampler2D uTexture;
              uniform sampler2D uHeightmap;
              uniform float uOpacity;

              void main() {
                 vec4 textureColor = texture2D(uTexture, vUvCoordinates);
                 vec4 heightmapColor = texture2D(uHeightmap, vUvCoordinates);

                 if (uOpacity == 0) {
                     fFragmentColor = heightmapColor;
                 }
                 else if (uOpacity == 1) {
                     fFragmentColor = textureColor;
                 }
                 else {
                     textureColor.r = textureColor.r * 0.5f;
                     textureColor.g = textureColor.g * 0.5f;
                     textureColor.b = textureColor.b * 0.5f;

                     heightmapColor.r = heightmapColor.r * 0.5f;
                     heightmapColor.g = heightmapColor.g * 0.5f;
                     heightmapColor.b = heightmapColor.b * 0.5f;

                     fFragmentColor = textureColor * vec4(1.0f, 1.0f, 1.0f, uOpacity) + heightmapColor * vec4(1.0f, 1.0f, 1.0f, 1.0f - uOpacity);
                 }
              }";

        private string _colorFragmentShaderCode =
            @"#version 330 core
            
              in vec2 vUvCoordinates;
              out vec4 fFragmentColor;

              void main() {
                  fFragmentColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
              }";

        public Renderer(Color wpColor, int width, int height, ViewerForm viewer)
        {
            _workspaceColor = wpColor;
            GL.ClearColor(_workspaceColor);
            _cameraPosition = new Vector2();
            _viewer = viewer;

            _width = width;
            _height = height;

            _vao = GL.GenVertexArray();
            GL.BindVertexArray(_vao);

            BuildShaders(_vertexShaderCode, _fragmentShaderCode, _colorFragmentShaderCode);
            CreateGeometry();
            SetCameraPosition(0f, 0f);
            SetCameraZoom(1f);

            OnContextChange(_width, _height);

            try
            {
                LoadCameraDecal("data\\camera.png");
            }
            catch (Exception)
            {
                LoadCameraDecal("data\\missing.bmp");
                MessageBox.Show("Internal application files coldn't be found. Please, check the installation integrity.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        public void DoFrame()
        {
            GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);
        }

        private void LoadCameraDecal(string path)
        {
            try
            {
                Bitmap decal = new Bitmap(path);

                if (_cameraDecalTexture == -1)
                {
                    _cameraDecalTexture = GL.GenTexture();
                    GL.BindTexture(TextureTarget.Texture2D, _cameraDecalTexture);

                    GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, (int)TextureMinFilter.Linear);
                    GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, (int)TextureMagFilter.Linear);

                    BitmapData decalData = decal.LockBits(new Rectangle(0, 0,
                        decal.Width, decal.Height), ImageLockMode.ReadOnly,
                        System.Drawing.Imaging.PixelFormat.Format32bppArgb);

                    GL.TexImage2D(TextureTarget.Texture2D, 0, PixelInternalFormat.Rgba, decalData.Width,
                        decalData.Height, 0, OpenTK.Graphics.OpenGL.PixelFormat.Bgra, PixelType.UnsignedByte,
                        decalData.Scan0);

                    decal.UnlockBits(decalData);
                    decal.Dispose();
                }
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        public void DrawCameraDecal(Camera camera)
        {
            camera.SetCameraZoom(_cameraZoom);

            GL.Enable(EnableCap.Blend);
            GL.BlendFunc(BlendingFactorSrc.SrcAlpha, BlendingFactorDest.OneMinusSrcAlpha);

            GL.UseProgram(_shaderProgram);

            int vertexDataId = GL.GetAttribLocation(_shaderProgram, "aVertices");
            int uvDataId = GL.GetAttribLocation(_shaderProgram, "aUvCoordinates");

            Matrix4 mvp = camera.GetModelMatrix() * _viewMatrix * _projectionMatrix;
            int matrixUniformId = GL.GetUniformLocation(_shaderProgram, "uMvpMatrix");
            GL.UniformMatrix4(matrixUniformId, false, ref mvp);

            int opacityUniformId = GL.GetUniformLocation(_shaderProgram, "uOpacity");
            GL.Uniform1(opacityUniformId, 1.0f);

            int textureSamplerId = GL.GetUniformLocation(_shaderProgram, "uTexture");
            GL.ActiveTexture(TextureUnit.Texture0);
            GL.BindTexture(TextureTarget.Texture2D, _cameraDecalTexture);
            GL.Uniform1(textureSamplerId, 0);

            GL.BindBuffer(BufferTarget.ArrayBuffer, _quadVbo);
            GL.EnableVertexAttribArray(vertexDataId);
            GL.VertexAttribPointer(vertexDataId, 3, VertexAttribPointerType.Float, false, 0, 0);

            GL.BindBuffer(BufferTarget.ArrayBuffer, _quadUvVbo);
            GL.EnableVertexAttribArray(uvDataId);
            GL.VertexAttribPointer(uvDataId, 2, VertexAttribPointerType.Float, false, 0, 0);

            GL.DrawArrays(PrimitiveType.TriangleStrip, 0, 4);

            GL.DisableVertexAttribArray(vertexDataId);
            GL.DisableVertexAttribArray(uvDataId);
        }

        public void DrawCameraConnectionLines(List<Camera> cameras)
        {
            GL.Disable(EnableCap.Blend);

            GL.BindBuffer(BufferTarget.ArrayBuffer, _lineVbo);
            Vector3[] lineVertices = null;

            if (cameras.Count < 4)
            {
                lineVertices = new Vector3[cameras.Count];

                for (int i = 0; i < cameras.Count; i++)
                {
                    Camera camera = cameras[i];
                    lineVertices[i].X = camera.GetPosition().X * 256f;
                    lineVertices[i].Y = -camera.GetPosition().Y * 256f;
                    lineVertices[i].Z = 0.5f;
                }
            }
            else
            {
                List<Vector3> bzLineVertices = new List<Vector3>();

                for (int i = 0; i < cameras.Count; i += 3)
                {
                    if (i + 3 < cameras.Count)
                    {
                        CubicCurve bc = new BezierCurve();
                        bc.AddControlPoint(new Vector2(cameras[i].GetPosition().X * 256f, -(cameras[i].GetPosition().Y * 256f)));
                        bc.AddControlPoint(new Vector2(cameras[i + 1].GetPosition().X * 256f, -(cameras[i + 1].GetPosition().Y * 256f)));
                        bc.AddControlPoint(new Vector2(cameras[i + 2].GetPosition().X * 256f, -(cameras[i + 2].GetPosition().Y * 256f)));
                        bc.AddControlPoint(new Vector2(cameras[i + 3].GetPosition().X * 256f, -(cameras[i + 3].GetPosition().Y * 256f)));

                        for (float t = 0f; t <= 1f; t += 0.01f)
                        {
                            Vector3 tempPoint = new Vector3();
                            Vector2 bzPoint = bc.Evaluate(t);

                            tempPoint.X = bzPoint.X;
                            tempPoint.Y = bzPoint.Y;
                            tempPoint.Z = 0.5f;

                            bzLineVertices.Add(tempPoint);
                        }
                    }
                }

                lineVertices = bzLineVertices.ToArray();
            }
            
            GL.BufferData<Vector3>(BufferTarget.ArrayBuffer,
                new IntPtr(lineVertices.Length * Vector3.SizeInBytes), lineVertices, BufferUsageHint.DynamicDraw);

            GL.UseProgram(_lineShaderProgram);

            int vertexDataId = GL.GetAttribLocation(_lineShaderProgram, "aVertices");
            int uvDataId = GL.GetAttribLocation(_lineShaderProgram, "aUvCoordinates");

            Matrix4 mvp = Matrix4.Identity * _viewMatrix* _projectionMatrix;
            int matrixUniformId = GL.GetUniformLocation(_lineShaderProgram, "uMvpMatrix");
            GL.UniformMatrix4(matrixUniformId, false, ref mvp);

            GL.BindBuffer(BufferTarget.ArrayBuffer, _lineVbo);
            GL.EnableVertexAttribArray(vertexDataId);
            GL.VertexAttribPointer(vertexDataId, 3, VertexAttribPointerType.Float, false, 0, 0);

            GL.BindBuffer(BufferTarget.ArrayBuffer, _quadUvVbo);
            GL.EnableVertexAttribArray(uvDataId);
            GL.VertexAttribPointer(uvDataId, 2, VertexAttribPointerType.Float, false, 0, 0);
            
            GL.DrawArrays(PrimitiveType.LineStrip, 0, lineVertices.Length);

            GL.DisableVertexAttribArray(vertexDataId);
            GL.DisableVertexAttribArray(uvDataId);
        }

        public void DrawTile(Tile tile, float opacity)
        {
            GL.Disable(EnableCap.Blend);

            int tileTextureId = tile.GetTextureGlId();
            int tileHeightmapId = tile.GetHeightmapGlId();

            if (tileTextureId == -1)
            {
                tileTextureId = GL.GenTexture();
                GL.BindTexture(TextureTarget.Texture2D, tileTextureId);

                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, (int)TextureMinFilter.Linear);
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, (int)TextureMagFilter.Linear);

                BitmapData tileBmpData = tile.GetTexturePreview().LockBits(new Rectangle(0, 0,
                    tile.GetTexturePreview().Width, tile.GetTexturePreview().Height), ImageLockMode.ReadOnly,
                    System.Drawing.Imaging.PixelFormat.Format32bppArgb);

                GL.TexImage2D(TextureTarget.Texture2D, 0, PixelInternalFormat.Rgba, tileBmpData.Width,
                    tileBmpData.Height, 0, OpenTK.Graphics.OpenGL.PixelFormat.Bgra, PixelType.UnsignedByte,
                    tileBmpData.Scan0);

                tile.GetTexturePreview().UnlockBits(tileBmpData);
                tile.SetTextureGlId(tileTextureId);
            }

            if (tileHeightmapId == -1)
            {
                tileHeightmapId = GL.GenTexture();
                GL.BindTexture(TextureTarget.Texture2D, tileHeightmapId);

                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, (int)TextureMinFilter.Linear);
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, (int)TextureMagFilter.Linear);

                BitmapData tileBmpData = tile.GetHeightmapPreview().LockBits(new Rectangle(0, 0,
                    tile.GetHeightmapPreview().Width, tile.GetHeightmapPreview().Height), ImageLockMode.ReadOnly,
                    System.Drawing.Imaging.PixelFormat.Format32bppArgb);

                GL.TexImage2D(TextureTarget.Texture2D, 0, PixelInternalFormat.Rgba, tileBmpData.Width,
                    tileBmpData.Height, 0, OpenTK.Graphics.OpenGL.PixelFormat.Bgra, PixelType.UnsignedByte,
                    tileBmpData.Scan0);

                tile.GetHeightmapPreview().UnlockBits(tileBmpData);
                tile.SetHeightmapGlId(tileHeightmapId);
            }

            GL.UseProgram(_shaderProgram);

            int vertexDataId = GL.GetAttribLocation(_shaderProgram, "aVertices");
            int uvDataId = GL.GetAttribLocation(_shaderProgram, "aUvCoordinates");

            Matrix4 mvp = tile.GetModelMatrix() * _viewMatrix * _projectionMatrix;
            int matrixUniformId = GL.GetUniformLocation(_shaderProgram, "uMvpMatrix");
            GL.UniformMatrix4(matrixUniformId, false, ref mvp);

            int opacityUniformId = GL.GetUniformLocation(_shaderProgram, "uOpacity");
            GL.Uniform1(opacityUniformId, opacity);

            int textureSamplerId = GL.GetUniformLocation(_shaderProgram, "uTexture");
            GL.ActiveTexture(TextureUnit.Texture0);
            GL.BindTexture(TextureTarget.Texture2D, tileTextureId);
            GL.Uniform1(textureSamplerId, 0);

            int heightmapSamplerId = GL.GetUniformLocation(_shaderProgram, "uHeightmap");
            GL.ActiveTexture(TextureUnit.Texture1);
            GL.BindTexture(TextureTarget.Texture2D, tileHeightmapId);
            GL.Uniform1(heightmapSamplerId, 1);

            GL.BindBuffer(BufferTarget.ArrayBuffer, _quadVbo);
            GL.EnableVertexAttribArray(vertexDataId);
            GL.VertexAttribPointer(vertexDataId, 3, VertexAttribPointerType.Float, false, 0, 0);

            GL.BindBuffer(BufferTarget.ArrayBuffer, _quadUvVbo);
            GL.EnableVertexAttribArray(uvDataId);
            GL.VertexAttribPointer(uvDataId, 2, VertexAttribPointerType.Float, false, 0, 0);

            GL.DrawArrays(PrimitiveType.TriangleStrip, 0, 4);

            GL.DisableVertexAttribArray(vertexDataId);
            GL.DisableVertexAttribArray(uvDataId);
        }

        public void SetWorkspaceColor(Color wpColor)
        {
            _workspaceColor = wpColor;
            GL.ClearColor(_workspaceColor);
        }

        public void OnContextChange(int width, int height)
        {
            _width = width;
            _height = height;

            GL.Viewport(0, 0, width, height);

            SetViewMatrix();
            SetProjectionMatrix();
        }

        public void OnDispose()
        {
            if (GL.IsTexture(_cameraDecalTexture))
                GL.DeleteTexture(_cameraDecalTexture);

            if (GL.IsProgram(_shaderProgram))
                GL.DeleteProgram(_shaderProgram);

            if (GL.IsProgram(_lineShaderProgram))
                GL.DeleteProgram(_lineShaderProgram);

            if (GL.IsBuffer(_quadVbo))
                GL.DeleteBuffer(_quadVbo);

            if (GL.IsBuffer(_quadUvVbo))
                GL.DeleteBuffer(_quadUvVbo);

            if (GL.IsBuffer(_lineVbo))
                GL.DeleteBuffer(_lineVbo);

            if (GL.IsVertexArray(_vao))
                GL.DeleteVertexArray(_vao);
        }

        public void SetCameraPosition(float x, float y)
        {
            _cameraPosition.X = x;
            _cameraPosition.Y = y;

            SetViewMatrix();
            SetProjectionMatrix();
        }

        public void TranslateCamera(float dx, float dy)
        {
            _cameraPosition.X += dx;
            _cameraPosition.Y += dy;

            SetViewMatrix();
            SetProjectionMatrix();
        }

        public void SetCameraZoom(float z)
        {
            if (_cameraZoom > 0.1f)
            {
                _cameraZoom = z;

                SetViewMatrix();
                SetProjectionMatrix();
            }
        }

        public void TranslateCameraZoom(float z)
        {
            _cameraZoom += z;

            if (_cameraZoom < 0.1f)
                _cameraZoom = 0.1f;

            SetViewMatrix();
            SetProjectionMatrix();
        }

        public float GetCameraZoom()
        {
            return _cameraZoom;
        }

        private void SetProjectionMatrix()
        {
            _projectionMatrix = Matrix4.CreateOrthographicOffCenter(
                -0.5f * (_width + _cameraPosition.X) * _cameraZoom,
                0.5f * (_width - _cameraPosition.X) * _cameraZoom,
                0.5f * (_height + _cameraPosition.Y) * _cameraZoom,
                -0.5f * (_height - _cameraPosition.Y) * _cameraZoom,
                0.01f, 100f);
        }

        private void SetViewMatrix()
        {
            Vector3 eye = new Vector3(_cameraPosition.X, _cameraPosition.Y, 1f);
            Vector3 target = new Vector3(_cameraPosition.X, _cameraPosition.Y, 0f);
            Vector3 up = new Vector3(0f, 1f, 0f);

            _viewMatrix = Matrix4.LookAt(eye, target, up);
        }

        private void BuildShaders(string vertex, string fragment, string color)
        {
            int vertexHandle, fragmentHandle, colorHandle;

            vertexHandle = GL.CreateShader(ShaderType.VertexShader);
            fragmentHandle = GL.CreateShader(ShaderType.FragmentShader);
            colorHandle = GL.CreateShader(ShaderType.FragmentShader);

            GL.ShaderSource(vertexHandle, vertex);
            GL.ShaderSource(fragmentHandle, fragment);
            GL.ShaderSource(colorHandle, color);

            GL.CompileShader(vertexHandle);
            GL.CompileShader(fragmentHandle);
            GL.CompileShader(colorHandle);

            string log;

            log = GL.GetShaderInfoLog(vertexHandle);

            if (log != "")
                MessageBox.Show("GLSL compile warning (Vertex Shader): \n" + log, "Advertencia", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

            log = GL.GetShaderInfoLog(fragmentHandle);

            if (log != "")
                MessageBox.Show("GLSL compile warning (Fragment Shader): \n" + log, "Advertencia", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

            log = GL.GetShaderInfoLog(colorHandle);

            if (log != "")
                MessageBox.Show("GLSL compile warning (Fragment Shader): \n" + log, "Advertencia", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

            _shaderProgram = GL.CreateProgram();
            _lineShaderProgram = GL.CreateProgram();

            GL.AttachShader(_shaderProgram, vertexHandle);
            GL.AttachShader(_shaderProgram, fragmentHandle);

            GL.LinkProgram(_shaderProgram);

            log = GL.GetProgramInfoLog(_shaderProgram);

            if (log != "")
                MessageBox.Show("GLSL link warning: \n" + log, "Advertencia", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

            GL.AttachShader(_lineShaderProgram, vertexHandle);
            GL.AttachShader(_lineShaderProgram, colorHandle);

            GL.LinkProgram(_lineShaderProgram);

            log = GL.GetProgramInfoLog(_lineShaderProgram);

            if (log != "")
                MessageBox.Show("GLSL link warning: \n" + log, "Advertencia", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

            GL.DeleteShader(vertexHandle);
            GL.DeleteShader(fragmentHandle);
            GL.DeleteShader(colorHandle);
        }

        private void CreateGeometry()
        {
            _quadVbo = GL.GenBuffer();
            _quadUvVbo = GL.GenBuffer();
            _lineVbo = GL.GenBuffer();

            GL.BindBuffer(BufferTarget.ArrayBuffer, _quadVbo);

            Vector3[] vertices = new Vector3[4];

            vertices[0] = new Vector3(-0.5f, 0.5f, 0.0f);
            vertices[1] = new Vector3(-0.5f, -0.5f, 0.0f);
            vertices[2] = new Vector3(0.5f, 0.5f, 0.0f);
            vertices[3] = new Vector3(0.5f, -0.5f, 0.0f);

            GL.BufferData<Vector3>(BufferTarget.ArrayBuffer, 
                new IntPtr(vertices.Length * Vector3.SizeInBytes), vertices, BufferUsageHint.StaticDraw);

            Vector2[] uv = new Vector2[4];

            uv[0] = new Vector2(0.0f, 1.0f);
            uv[1] = new Vector2(0.0f, 0.0f);
            uv[2] = new Vector2(1.0f, 1.0f);
            uv[3] = new Vector2(1.0f, 0.0f);

            GL.BindBuffer(BufferTarget.ArrayBuffer, _quadUvVbo);
            GL.BufferData<Vector2>(BufferTarget.ArrayBuffer,
                new IntPtr(uv.Length * Vector2.SizeInBytes), uv, BufferUsageHint.StaticDraw);
        }
    }
}
