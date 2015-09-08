using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI.Docking;

namespace TerrenosHelper
{
    public enum ViewerCameraMode
    {
        ViewMove,
        ViewZoom,
        CameraMove,
        CameraRotate,
        None
    }

    public partial class ViewerForm : DockContent
    {
        private MainForm _main;
        private bool _glLoaded = false;
        private Renderer _renderer;
        private ViewerCameraMode _cameraMode = ViewerCameraMode.None;
        private int _mouseStartPositionX, _mouseStartPositionY;
        private bool _mouseDown = false;

        public ViewerForm(MainForm main)
        {
            InitializeComponent();
            _main = main;
        }

        public void OnViewerSetOrigin()
        {
            _renderer.SetCameraPosition(0f, 0f);
            _renderer.SetCameraZoom(1f);
        }

        public void OnViewerCameraModeChange(ViewerCameraMode newMode)
        {
            _cameraMode = newMode;

            if (_cameraMode == ViewerCameraMode.CameraMove || _cameraMode == ViewerCameraMode.ViewMove)
            {
                glControl.Cursor = Cursors.NoMove2D;
            }
            else if (_cameraMode == ViewerCameraMode.ViewZoom || _cameraMode == ViewerCameraMode.CameraRotate)
            {
                glControl.Cursor = Cursors.SizeNS;
            }
            else
            {
                glControl.Cursor = Cursors.Default;
            }
        }

        private void ViewerForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (!_main.IsClosing())
            {
                e.Cancel = true;
            }
            else
            {
                _renderer.OnDispose();
            }
        }

        private void glControl_Load(object sender, EventArgs e)
        {
            _glLoaded = true;
            _renderer = new Renderer(_main.GetApplicationSettings().workspaceColor, glControl.Width, glControl.Height, this);
            Application.Idle += Application_Idle;
        }

        private void Application_Idle(object sender, EventArgs e)
        {
            if (_renderer != null)
            {
                _renderer.DoFrame();

                if (_main.GetProject() != null)
                {
                    List<Tile> _tiles = _main.GetProject().GetTileList();
                    List<Camera> _cameras = _main.GetProject().GetCameraList();

                    for (int i = 0; i < _tiles.Count; i++)
                    {
                        _renderer.DrawTile(_tiles[i], _main.GetTileOpacity());
                    }

                    _renderer.DrawCameraConnectionLines(_main.GetProject().GetCameraList());

                    for (int i = 0; i < _cameras.Count; i++)
                    {
                        _renderer.DrawCameraDecal(_cameras[i]);
                    }
                }
                
                glControl.Context.SwapBuffers();
            }
        }

        private void glControl_Paint(object sender, PaintEventArgs e)
        {
            if (_glLoaded)
            {
                _renderer.DoFrame();
                glControl.SwapBuffers();
            }
        }

        private void glControl_Resize(object sender, EventArgs e)
        {
            if (_glLoaded)
            {
                _renderer.OnContextChange(glControl.Width, glControl.Height);
                glControl.Invalidate();
            }
        }

        private void glControl_MouseDown(object sender, MouseEventArgs e)
        {
            _mouseDown = true;
            _mouseStartPositionX = e.X;
            _mouseStartPositionY = e.Y;
        }

        private void glControl_MouseMove(object sender, MouseEventArgs e)
        {
            if (_mouseDown && e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                float deltaX = (e.X - _mouseStartPositionX);
                float deltaY = -(e.Y - _mouseStartPositionY);

                switch (_cameraMode)
                {
                    case ViewerCameraMode.ViewMove:
                        _renderer.TranslateCamera(-deltaX * _renderer.GetCameraZoom(), -deltaY * _renderer.GetCameraZoom());
                        break;

                    case ViewerCameraMode.ViewZoom:
                        _renderer.TranslateCameraZoom(deltaY * 0.01f);
                        break;

                    case ViewerCameraMode.CameraMove:
                        _main.GetExplorerForm().TranslateSelectedCamera(deltaX * _renderer.GetCameraZoom() * 0.001f, deltaY * _renderer.GetCameraZoom() * 0.001f);
                        _main.GetExplorerForm().UpdateCameras();
                        break;

                    case ViewerCameraMode.CameraRotate:
                        _main.GetExplorerForm().RotateSelectedCamera(deltaY * _renderer.GetCameraZoom());
                        _main.GetExplorerForm().UpdateCameras();
                        break;
                }

                _mouseStartPositionX = e.X;
                _mouseStartPositionY = e.Y;
            }
        }

        private void glControl_MouseUp(object sender, MouseEventArgs e)
        {
            _mouseDown = false;
        }

        public Renderer GetRenderEngine()
        {
            return _renderer;
        }
    }
}
