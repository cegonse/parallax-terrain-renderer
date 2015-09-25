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
    public partial class ExplorerForm : DockContent
    {
        private MainForm _main;
        private Tile _selectedTile;
        private Camera _selectedCamera;

        public ExplorerForm(MainForm main)
        {
            InitializeComponent();
            _main = main;
            _selectedTile = null;
            _selectedCamera = null;
        }

        private void ExplorerForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (!_main.IsClosing())
            {
                e.Cancel = true;
            }
        }

        public void UpdateTiles()
        {
            if (_main.GetProject() != null)
            {
                listBoxTiles.Items.Clear();

                for (int i = 0; i < _main.GetProject().GetTileList().Count; i++)
                {
                    listBoxTiles.Items.Add("Cuadrante " + (i + 1).ToString());
                }

                listBoxTiles.SelectedIndex = -1;
                listBoxTiles_SelectedIndexChanged(this, null);
            }
        }

        public void UpdateCameras()
        {
            if (_main.GetProject() != null)
            {
                listBoxCameras.Items.Clear();

                for (int i = 0; i < _main.GetProject().GetCameraList().Count; i++)
                {
                    listBoxCameras.Items.Add("Cámara " + (i + 1).ToString());
                }

                listBoxCameras.SelectedIndex = -1;
                listBoxCameras_SelectedIndexChanged(this, null);
            }
        }

        private void buttonAddTile_Click(object sender, EventArgs e)
        {
            if (_main.GetProject() != null)
            {
                _main.GetProject().AddTile();
                UpdateTiles();
                SelectLastTile();
            }
        }

        public void SelectLastTile()
        {
            listBoxTiles.SelectedIndex = listBoxTiles.Items.Count - 1;
        }

        public void SelectLastCamera()
        {
            listBoxCameras.SelectedIndex = listBoxCameras.Items.Count - 1;
        }

        private void listBoxTiles_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (_main.GetProject() != null && listBoxTiles.SelectedIndex != -1)
            {
                _selectedTile = _main.GetProject().GetTileList()[listBoxTiles.SelectedIndex];

                OpenTK.Vector2 tilePosition = _selectedTile.GetPosition();

                textBoxLatitude.Text = tilePosition.X.ToString();
                textBoxLongitude.Text = tilePosition.Y.ToString();
            }
            else
            {
                _selectedTile = null;
            }
        }

        private void textBoxLatitude_TextChanged(object sender, EventArgs e)
        {
            try
            {
                float newLatitude = float.Parse(textBoxLatitude.Text);
                
                if (_selectedTile != null)
                {
                    OpenTK.Vector2 newTilePosition = _selectedTile.GetPosition();
                    newTilePosition.X = newLatitude;
                    _selectedTile.SetPosition(newTilePosition);
                }
                else
                {
                    textBoxLatitude.Text = "";
                }
            }
            catch (Exception)
            {
                if (_selectedTile == null)
                   textBoxLatitude.Text = "";
            }
        }

        private void textBoxLongitude_TextChanged(object sender, EventArgs e)
        {
            try
            {
                float newLongitude = float.Parse(textBoxLongitude.Text);

                if (_selectedTile != null)
                {
                    OpenTK.Vector2 newTilePosition = _selectedTile.GetPosition();
                    newTilePosition.Y = newLongitude;
                    _selectedTile.SetPosition(newTilePosition);
                }
                else
                {
                    textBoxLongitude.Text = "";
                }
            }
            catch (Exception)
            {
                if (_selectedTile == null)
                    textBoxLatitude.Text = "";
            }
        }

        private void buttonHeightmap_Click(object sender, EventArgs e)
        {
            try
            {
                if (_selectedTile != null)
                {
                    openHeightmapDialog.ShowDialog();
                    string path = openHeightmapDialog.FileName;
                    _selectedTile.LoadHeightmapData(path);
                }
            }
            catch (Exception)
            {

            }
        }

        private void buttonTexture_Click(object sender, EventArgs e)
        {
            try
            {
                if (_selectedTile != null)
                {
                    openTextureDialog.ShowDialog();
                    string path = openTextureDialog.FileName;
                    _selectedTile.LoadTextureData(path);
                }
            }
            catch (Exception)
            {

            }
        }

        private void buttonDeleteTile_Click(object sender, EventArgs e)
        {
            if (_main.GetProject() != null)
            {
                int index = listBoxTiles.SelectedIndex;

                if (index >= 0)
                {
                    _main.GetProject().GetTileList().RemoveAt(index);
                    UpdateTiles();
                }
            }
        }

        private void listBoxCameras_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (_main.GetProject() != null && listBoxCameras.SelectedIndex != -1)
            {
                _selectedCamera = _main.GetProject().GetCameraList()[listBoxCameras.SelectedIndex];

                OpenTK.Vector2 cameraPosition = _selectedCamera.GetPosition();
                float yaw = _selectedCamera.GetRotation();

                textBoxCameraPositionX.Text = cameraPosition.X.ToString();
                textBoxCameraPositionZ.Text = cameraPosition.Y.ToString();
                textBoxCameraRotationX.Text = yaw.ToString();
            }
            else
            {
                _selectedTile = null;
            }
        }

        private void buttonAddCamera_Click(object sender, EventArgs e)
        {
            if (_main.GetProject() != null)
            {
                _main.GetProject().AddCamera();
                UpdateCameras();
                SelectLastCamera();
            }
        }

        private void textBoxCameraPositionX_TextChanged(object sender, EventArgs e)
        {
            try
            {
                float newX = float.Parse(textBoxCameraPositionX.Text);

                if (_selectedCamera != null)
                {
                    OpenTK.Vector2 newCameraPosition = _selectedCamera.GetPosition();
                    newCameraPosition.X = newX;
                    _selectedCamera.SetPosition(newCameraPosition);
                }
                else
                {
                    textBoxCameraPositionX.Text = "";
                }
            }
            catch (Exception)
            {
                if (_selectedCamera == null)
                    textBoxCameraPositionX.Text = "";
            }
        }

        private void textBoxCameraPositionZ_TextChanged(object sender, EventArgs e)
        {
            try
            {
                float newZ = float.Parse(textBoxCameraPositionZ.Text);

                if (_selectedCamera != null)
                {
                    OpenTK.Vector2 newCameraPosition = _selectedCamera.GetPosition();
                    newCameraPosition.Y = newZ;
                    _selectedCamera.SetPosition(newCameraPosition);
                }
                else
                {
                    textBoxCameraPositionZ.Text = "";
                }
            }
            catch (Exception)
            {
                if (_selectedCamera == null)
                    textBoxCameraPositionZ.Text = "";
            }
        }

        private void textBoxCameraRotationX_TextChanged(object sender, EventArgs e)
        {
            try
            {
                float newYaw = float.Parse(textBoxCameraRotationX.Text);

                if (_selectedCamera != null)
                {
                    _selectedCamera.SetRotation(newYaw);
                }
                else
                {
                    textBoxCameraRotationX.Text = "";
                }
            }
            catch (Exception)
            {
                if (_selectedCamera == null)
                    textBoxCameraRotationX.Text = "";
            }
        }

        public void TranslateSelectedCamera(float dx, float dy)
        {
            if (_selectedCamera != null)
            {
                OpenTK.Vector2 newPosition = _selectedCamera.GetPosition();
                newPosition.X += dx;
                newPosition.Y += dy;

                _selectedCamera.SetPosition(newPosition);
            }
        }

        public void RotateSelectedCamera(float dy)
        {
            if (_selectedCamera != null)
            {
                float newRotation = _selectedCamera.GetRotation() + dy;
                _selectedCamera.SetRotation(newRotation);
            }
        }

        private void buttonDeleteCamera_Click(object sender, EventArgs e)
        {
            if (_main.GetProject() != null)
            {
                int index = listBoxCameras.SelectedIndex;

                if (index >= 0)
                {
                    _main.GetProject().GetCameraList().RemoveAt(index);
                    UpdateCameras();
                }
            }
        }

        private void listBoxCameras_KeyDown(object sender, KeyEventArgs e)
        {
            if (_main.GetProject() != null)
            {
                int index = listBoxCameras.SelectedIndex;

                if (index >= 0)
                {
                    if (e.KeyCode == Keys.Delete)
                    {
                        _main.GetProject().GetCameraList().RemoveAt(index);
                        UpdateCameras();
                    }
                }
            }
        }

        private void listBoxTiles_KeyDown(object sender, KeyEventArgs e)
        {
            if (_main.GetProject() != null)
            {
                int index = listBoxTiles.SelectedIndex;

                if (index >= 0)
                {
                    if (e.KeyCode == Keys.Delete)
                    {
                        _main.GetProject().GetTileList().RemoveAt(index);
                        UpdateTiles();
                    }
                }
            }
        }

        private void buttonCameraUp_Click(object sender, EventArgs e)
        {
            if (_main.GetProject() != null)
            {
                int index = listBoxCameras.SelectedIndex;

                if (index > 0)
                {
                    List<Camera> cameras = _main.GetProject().GetCameraList();
                    Camera temp = cameras[index - 1];
                    cameras[index - 1] = cameras[index];
                    cameras[index] = temp;

                    UpdateCameras();
                }
            }
        }

        private void buttonCameraDown_Click(object sender, EventArgs e)
        {
            if (_main.GetProject() != null)
            {
                int index = listBoxCameras.SelectedIndex;

                if (index < _main.GetProject().GetCameraList().Count - 1)
                {
                    List<Camera> cameras = _main.GetProject().GetCameraList();
                    Camera temp = cameras[index + 1];
                    cameras[index + 1] = cameras[index];
                    cameras[index] = temp;

                    UpdateCameras();
                }
            }
        }

        public Tile GetSelectedTile()
        {
            return _selectedTile;
        }
    }
}
