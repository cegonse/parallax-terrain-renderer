using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Windows.Forms;
using System.IO;
using WeifenLuo.WinFormsUI.Docking;
using BitMiracle.LibTiff.Classic;

namespace TerrenosHelper
{
    public struct ApplicationSettings
    {
        public string language;
        public Color workspaceColor;
    }

    public partial class MainForm : Form
    {
        private FormFileLoad _loadForm;
        private ViewerForm _viewer;
        private ExplorerForm _explorer;
        private IberpixSettingsForm _iberpix;
        private ProjectSettingsForm _settings;
        private bool _closing;
        private bool _projectSaved = false;
        private ApplicationSettings _appSettings;
        private float _opacity = 100f;
        private ViewerCameraMode _cameraMode = ViewerCameraMode.None;
        private Project _project;

        public MainForm()
        {
            InitializeComponent();

            _viewer = new ViewerForm(this);
            _viewer.Show(dockPanel1, DockState.Document);

            _explorer = new ExplorerForm(this);
            _explorer.Show(dockPanel1, DockState.DockLeft);

            _iberpix = new IberpixSettingsForm();

            _settings = new ProjectSettingsForm(this);
            _settings.Show(dockPanel1, DockState.DockRight);

            _closing = false;

            _appSettings = new ApplicationSettings();
            _appSettings.language = "es_es";
            _appSettings.workspaceColor = SystemColors.AppWorkspace;
        }

        public ApplicationSettings GetApplicationSettings()
        {
            return _appSettings;
        }

        public bool IsClosing()
        {
            return _closing;
        }

        public ViewerCameraMode GetCameraMode()
        {
            return _cameraMode;
        }

        private void salirToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MainForm_Closing(this, new FormClosingEventArgs(CloseReason.UserClosing, false));
        }

        private bool QuitApplication()
        {
            DialogResult _cancelled = System.Windows.Forms.DialogResult.No;
            bool _quit = false;

            if (_project != null)
            {
                if (!_projectSaved)
                {
                    _cancelled = PromtSaveProject();
                }
            }

            if (_cancelled != System.Windows.Forms.DialogResult.Cancel)
            {
                _quit = true;
            }

            return _quit;
        }

        public float GetTileOpacity()
        {
            return _opacity / 100f;
        }

        private void archivoDeElevaciónToolStripMenuItem_Click(object sender, EventArgs e)
        {
            openFileDialogSrtm.ShowDialog();
            string path = openFileDialogSrtm.FileName;

            FileInfo fInfo = new FileInfo(path);

            _loadForm = new FormFileLoad(this);
            _loadForm.SetFilePath(path);
            _loadForm.SetFileSize((int) fInfo.Length);

            _loadForm.Show();

            FileLoader loader = new FileLoader(path, _loadForm, this);
            
            Thread loaderThread = new Thread(new ThreadStart(loader.Run));
            loaderThread.Priority = ThreadPriority.Highest;
            loaderThread.Start();
        }

        public void OnSRTMFileLoadComplete(SRTMReader srtm)
        {
            _loadForm.Hide();

            BitmapTerrain tempBitmap = new BitmapTerrain(srtm.GetTileSize(), srtm.GetTileSize());
            tempBitmap.ConvertFromSRTM(srtm, BitmapChannels.ChannelRed | BitmapChannels.ChannelGreen);

            try
            {
                saveFileDialogTiff.ShowDialog();
                string tiffPath = saveFileDialogTiff.FileName;

                TIFFWriter.ExportTIFF(tempBitmap.GetBitmap(), tiffPath);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "File conversion failed", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        delegate void OnSRTMFileLoadCompleteCallback(SRTMReader srtm);

        public void Safe_OnSRTMFileLoadComplete(SRTMReader srtm)
        {
            if (_loadForm.InvokeRequired)
            {
                OnSRTMFileLoadCompleteCallback callback = new OnSRTMFileLoadCompleteCallback(OnSRTMFileLoadComplete);
                this.Invoke(callback, new object[] { srtm });
            }
            else
            {
                OnSRTMFileLoadComplete(srtm);
            }
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            
        }

        private void mapaDeAlturaToolStripMenuItem_Click(object sender, EventArgs e)
        {
            saveFileDialogTiff.ShowDialog();

            string path = saveFileDialogTiff.FileName;
            //_bitmap.WriteTIFF(path);
        }

        private void MainForm_Resize(object sender, EventArgs e)
        {
        }

        private void MainForm_Closing(object sender, FormClosingEventArgs e)
        {
            _closing = true;

            if (!QuitApplication())
            {
                e.Cancel = true;
            }
            else
            {
                if (_project != null)
                {
                    _project.OnDispose();
                }

                Application.Exit();
            }
        }

        private void iberpixDownloadToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (_iberpix.IsDisposed)
                _iberpix = new IberpixSettingsForm();

            _iberpix.Show();
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                openFileDialogProject.ShowDialog();
                OpenProject(openFileDialogProject.FileName);
            }
            catch (Exception)
            {
            }
        }

        public void OpenProject(string path)
        {
            try
            {
                _project = ProjectSerializer.MarshallProject(openFileDialogProject.FileName);
                this.Text = "Terrain Designer - (" + _project.GetProjectName() + ")*";

                _projectSaved = false;

                _settings.OnProjectCreate();
                _settings.SetProjectName(_project.GetProjectName());
                _settings.SetCameraHeight(_project.GetRelativeHeight());
                _settings.SetCameraMode(_project.GetCameraMode());
                _settings.SetProjectAuthor(_project.GetProjectAuthor());
                _settings.SetResolution(_project.GetResolution());

                _explorer.UpdateTiles();
                _explorer.UpdateCameras();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Project loading failed", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        public void OnProjectNameChanged(string name)
        {
            if (_project != null)
            {
                _project.SetProjectName(name);
                this.Text = "Terrain Designer - (" + name + ")*";
            }
        }

        public void OnProjectAuthorChanged(string author)
        {
            if (_project != null)
                _project.SetProjectAuthor(author);
        }

        public void OnProjectCameraModeChanged(CameraMode mode)
        {
            if (_project != null)
            {
                _project.SetCameraMode(mode);
            }
        }

        public void OnProjectCameraHeightChanged(float height)
        {
            if (_project != null)
            {
                _project.SetConstantHeight(height);
                _project.SetRelativeHeight(height);
            }
        }

        public void OnProjectResolutionChanged(float resolution)
        {
            if (_project != null)
            {
                _project.SetResolution(resolution);
            }
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            bool _created = true;

            if (_project == null)
            {
                _project = new Project("Untitled");
                this.Text = "Terrain Designer - (Untitled)*";
            }
            else
            {
                if (!_projectSaved)
                {
                    DialogResult res = PromtSaveProject();

                    if (res == System.Windows.Forms.DialogResult.Cancel)
                    {
                        _created = false;
                    }
                }
            }

            if (_created)
            {
                _project = new Project("Untitled");
                _project.SetRelativeHeight(0f);
                _project.SetConstantHeight(0f);
                _project.SetCameraMode(CameraMode.CameraModeConstantHeight);
                _project.SetProjectAuthor("No Author");
                _projectSaved = false;
                this.Text = "Terrain Designer - (Untitled)*";

                _settings.OnProjectCreate();
                _settings.SetProjectName(_project.GetProjectName());
                _settings.SetCameraHeight(_project.GetRelativeHeight());
                _settings.SetCameraMode(_project.GetCameraMode());
                _settings.SetProjectAuthor(_project.GetProjectAuthor());
            }
        }

        private DialogResult PromtSaveProject()
        {
            DialogResult res = MessageBox.Show("Save the current project? Unsaved changes will be lost.", "Save", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);

            if (res == System.Windows.Forms.DialogResult.Yes)
            {
                SaveProject();
            }

            return res;
        }

        private void SaveProject()
        {
            if (_project != null)
            {
                try
                {
                    saveFileDialogProject.ShowDialog();
                    string path = saveFileDialogProject.FileName;
                    ProjectSerializer.SerializeProject(_project, path);

                    this.Text = "Terrain Designer - (" + _project.GetProjectName() + ")";
                    _projectSaved = true;
                }
                catch (Exception)
                {
                    MessageBox.Show("Project saving failed.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (_project != null)
                SaveProject();
        }

        private void toolStripTextBoxOpacity_TextChanged(object sender, EventArgs e)
        {
            try
            {
                _opacity = float.Parse(toolStripTextBoxOpacity.Text);
            }
            catch (Exception)
            {

            }
        }

        private void toolStripButtonMoveView_Click(object sender, EventArgs e)
        {
            toolStripButtonMoveView.Checked = true;
            toolStripButtonZoomView.Checked = false;
            toolStripButtonCameraMove.Checked = false;
            toolStripButtonCameraRotate.Checked = false;

            _cameraMode = ViewerCameraMode.ViewMove;
            _viewer.OnViewerCameraModeChange(_cameraMode);
        }

        private void toolStripButtonZoomView_Click(object sender, EventArgs e)
        {
            toolStripButtonMoveView.Checked = false;
            toolStripButtonZoomView.Checked = true;
            toolStripButtonCameraMove.Checked = false;
            toolStripButtonCameraRotate.Checked = false;

            _cameraMode = ViewerCameraMode.ViewZoom;
            _viewer.OnViewerCameraModeChange(_cameraMode);
        }

        private void toolStripButtonCameraMove_Click(object sender, EventArgs e)
        {
            toolStripButtonMoveView.Checked = false;
            toolStripButtonZoomView.Checked = false;
            toolStripButtonCameraMove.Checked = true;
            toolStripButtonCameraRotate.Checked = false;

            _cameraMode = ViewerCameraMode.CameraMove;
            _viewer.OnViewerCameraModeChange(_cameraMode);
        }

        private void toolStripButtonCameraRotate_Click(object sender, EventArgs e)
        {
            toolStripButtonMoveView.Checked = false;
            toolStripButtonZoomView.Checked = false;
            toolStripButtonCameraMove.Checked = false;
            toolStripButtonCameraRotate.Checked = true;

            _cameraMode = ViewerCameraMode.CameraRotate;
            _viewer.OnViewerCameraModeChange(_cameraMode);
        }

        private void toolStripButtonHome_Click(object sender, EventArgs e)
        {
            _viewer.OnViewerSetOrigin();
        }

        public void AddTile()
        {
            _project.AddTile();
        }

        public Project GetProject()
        {
            return _project;
        }

        private void toolStripButtonAddCamera_Click(object sender, EventArgs e)
        {
            if (_project != null)
            {
                _project.AddCamera();
                _explorer.UpdateCameras();
                _explorer.SelectLastCamera();
            }
        }

        public ExplorerForm GetExplorerForm()
        {
            return _explorer;
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AboutForm about = new AboutForm();
            about.Show();
        }

        private void exportToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            try
            {
                ExportProject();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Project build failed", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void toolStripButtonExport_Click(object sender, EventArgs e)
        {
            ExportProject();
            try
            {
               
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Project build failed", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void ExportProject()
        {
            if (_project != null)
            {
                folderBrowserDialogExport.ShowDialog();
                string path = folderBrowserDialogExport.SelectedPath;

                ExportProgressForm exportForm = new ExportProgressForm();
                exportForm.SetFileCount((_project.GetTileList().Count * 2) + 1);
                exportForm.Show();

                int fileCount = 0;

                // Guardo el proyecto en un XML con los nuevos nombres de los archivos
                Project tempProject = _project.Clone();

                for (int i = 0; i < tempProject.GetTileList().Count; i++)
                {
                    OpenTK.Vector2 tilePosition = tempProject.GetTileList()[i].GetPosition();
                    string newName = tilePosition.X.ToString() + "_" + tilePosition.Y.ToString() + ".tiff";

                    tempProject.GetTileList()[i].SetHeightmapPath(newName);
                    tempProject.GetTileList()[i].SetTexturePath(newName);
                }

                ProjectSerializer.SerializeProject(tempProject, System.IO.Path.Combine(path, "project.xml"));
                fileCount++;

                // Guardo todas las imagenes TIFF de las texturas y de los mapas de altura
                if (!System.IO.Directory.Exists(System.IO.Path.Combine(path, "textures")))
                {
                    System.IO.Directory.CreateDirectory(System.IO.Path.Combine(path, "textures"));
                }

                if (!System.IO.Directory.Exists(System.IO.Path.Combine(path, "heightmaps")))
                {
                    System.IO.Directory.CreateDirectory(System.IO.Path.Combine(path, "heightmaps"));
                }
                
                for (int i = 0; i < _project.GetTileList().Count; i++)
                {
                    Tile temp = _project.GetTileList()[i];

                    string texturePath = System.IO.Path.Combine(path + "\\textures", temp.GetPosition().X.ToString() + "_" + temp.GetPosition().Y.ToString() + ".tiff");
                    string heightmapPath = System.IO.Path.Combine(path + "\\heightmaps", temp.GetPosition().X.ToString() + "_" + temp.GetPosition().Y.ToString() + ".tiff");

                    // Open the high resolution texture and convert it to TIFF
                    Bitmap highResolutionTexture = (Bitmap)Image.FromFile(temp.GetTexturePath());

                    TIFFWriter.ExportTIFF(highResolutionTexture, texturePath);
                    highResolutionTexture.Dispose();

                    fileCount++;
                    exportForm.SetFileProgress(fileCount);
                    exportForm.Refresh();
                    exportForm.Invalidate();

                    // If the high resolution heightmap has TIFF format, load it using libtiff.
                    // If it has another format, load it using System.Image
                    // Load the high resolution heightmap
                    bool isTiff = temp.GetHeightmapPath().Contains(".tiff") || temp.GetHeightmapPath().Contains(".tif");
                    int hgWidth = 0;
                    int hgHeight = 0;
                    UInt16[,] heightData = null;

                    if (false/*isTiff*/)
                    {
                        // Open the TIFF image
                        using (Tiff rgHeightmap = Tiff.Open(temp.GetHeightmapPath(), "r"))
                        {
                            if (rgHeightmap == null)
                            {
                                throw new Exception();
                            }

                            FieldValue[] val = rgHeightmap.GetField(TiffTag.IMAGEWIDTH);
                            hgWidth = val[0].ToInt();

                            val = rgHeightmap.GetField(TiffTag.IMAGELENGTH);
                            hgHeight = val[0].ToInt();

                            // Allocate a temporary vector to store the heightmap data
                            int[] raster = new int[hgWidth * hgHeight];

                            // Allocate the height data matrix
                            heightData = new UInt16[hgWidth, hgHeight];

                            // Unpack the raster temporary data into the array
                            if (rgHeightmap.ReadRGBAImage(hgWidth, hgHeight, raster))
                            {
                                for (int x = 0; x < hgWidth; x++)
                                {
                                    for (int y = 0; y < hgHeight; y++)
                                    {
                                        int offset = (hgHeight - y - 1) * hgWidth + x;
                                        heightData[x, y] = (UInt16)Tiff.GetR(raster[offset]);
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        // Load the native heightmap
                        Bitmap rgHeightmap = (Bitmap)Image.FromFile(temp.GetHeightmapPath());
                        hgWidth = rgHeightmap.Width;
                        hgHeight = rgHeightmap.Height;

                        // Allocate the height data matrix
                        heightData = new UInt16[hgWidth, hgHeight];

                        // And store the height value (R channel) in the matrix
                        for (int x = 0; x < hgWidth; x++)
                        {
                            for (int y = 0; y < hgHeight; y++)
                            {
                                heightData[x, y] = rgHeightmap.GetPixel(x, y).R;
                            }
                        }

                        // Dispose the original Bitmap data
                        rgHeightmap.Dispose();
                    }

                    // Create a matrix to store the terrain normal vectors
                    UInt16[,] normals = new UInt16[hgWidth, hgWidth];
                    List<OpenTK.Vector3> tempNormals = new List<OpenTK.Vector3>();

                    // For each height, obtain all the normal vectors with its neighbors. Then
                    // obtain the mean normal vector
                    for (int x = 0; x < hgWidth; x++)
                    {
                        for (int y = 0; y < hgHeight; y++)
                        {
                            if (x == 0 && y == 0)
                            {
                                OpenTK.Vector3 currentVx = new OpenTK.Vector3(x, heightData[x, y], y);
                                OpenTK.Vector3 rightVx = new OpenTK.Vector3(x + 1, heightData[x+1, y], y);
                                OpenTK.Vector3 downVx = new OpenTK.Vector3(x, heightData[x, y+1], y + 1);

                                OpenTK.Vector3 vecCurrRight = rightVx - currentVx;
                                OpenTK.Vector3 vecCurrDown = downVx - currentVx;

                                tempNormals.Add(OpenTK.Vector3.Cross(vecCurrDown, vecCurrRight).Normalized());
                            }
                            else if (x == 0 && y == (hgHeight - 1))
                            {
                                OpenTK.Vector3 currentVx = new OpenTK.Vector3(x, heightData[x, y], y);
                                OpenTK.Vector3 rightVx = new OpenTK.Vector3(x + 1, heightData[x+1, y], y);
                                OpenTK.Vector3 upVx = new OpenTK.Vector3(x, heightData[x, y-1], y - 1);

                                OpenTK.Vector3 vecCurrRight = rightVx - currentVx;
                                OpenTK.Vector3 vecCurrUp = upVx - currentVx;

                                tempNormals.Add(OpenTK.Vector3.Cross(vecCurrRight, vecCurrUp).Normalized());
                            }
                            else if (x == (hgWidth - 1) && y == 0)
                            {
                                OpenTK.Vector3 currentVx = new OpenTK.Vector3(x, heightData[x, y], y);
                                OpenTK.Vector3 leftVx = new OpenTK.Vector3(x - 1, heightData[x-1, y], y);
                                OpenTK.Vector3 downVx = new OpenTK.Vector3(x, heightData[x, y+1], y + 1);

                                OpenTK.Vector3 vecCurrLeft = leftVx - currentVx;
                                OpenTK.Vector3 vecCurrDown = downVx - currentVx;

                                tempNormals.Add(OpenTK.Vector3.Cross(vecCurrLeft, vecCurrDown).Normalized());
                            }
                            else if (x == (hgWidth - 1) && y == (hgHeight - 1))
                            {
                                OpenTK.Vector3 currentVx = new OpenTK.Vector3(x, heightData[x, y], y);
                                OpenTK.Vector3 leftVx = new OpenTK.Vector3(x - 1, heightData[x-1, y], y);
                                OpenTK.Vector3 upVx = new OpenTK.Vector3(x, heightData[x, y-1], y - 1);

                                OpenTK.Vector3 vecCurrLeft = leftVx - currentVx;
                                OpenTK.Vector3 vecCurrUp = upVx - currentVx;

                                tempNormals.Add(OpenTK.Vector3.Cross(vecCurrUp, vecCurrLeft).Normalized());
                            }
                            else if (x == 0 && y != 0 && y != (hgHeight - 1))
                            {
                                OpenTK.Vector3 currentVx = new OpenTK.Vector3(x, heightData[x, y], y);
                                OpenTK.Vector3 rightVx = new OpenTK.Vector3(x + 1, heightData[x+1, y], y);
                                OpenTK.Vector3 upVx = new OpenTK.Vector3(x, heightData[x, y-1], y - 1);
                                OpenTK.Vector3 downVx = new OpenTK.Vector3(x, heightData[x, y+1], y + 1);

                                OpenTK.Vector3 vecCurrRight = rightVx - currentVx;
                                OpenTK.Vector3 vecCurrUp = upVx - currentVx;
                                OpenTK.Vector3 vecCurrDown = downVx - currentVx;

                                tempNormals.Add(OpenTK.Vector3.Cross(vecCurrDown, vecCurrRight).Normalized());
                                tempNormals.Add(OpenTK.Vector3.Cross(vecCurrRight, vecCurrUp).Normalized());
                            }
                            else if (x != 0 && x != (hgWidth - 1) && y == 0)
                            {
                                OpenTK.Vector3 currentVx = new OpenTK.Vector3(x, heightData[x, y], y);
                                OpenTK.Vector3 leftVx = new OpenTK.Vector3(x - 1, heightData[x-1, y], y);
                                OpenTK.Vector3 rightVx = new OpenTK.Vector3(x + 1, heightData[x+1, y], y);
                                OpenTK.Vector3 downVx = new OpenTK.Vector3(x, heightData[x, y+1], y + 1);

                                OpenTK.Vector3 vecCurrLeft = leftVx - currentVx;
                                OpenTK.Vector3 vecCurrRight = rightVx - currentVx;
                                OpenTK.Vector3 vecCurrDown = downVx - currentVx;

                                tempNormals.Add(OpenTK.Vector3.Cross(vecCurrLeft, vecCurrDown).Normalized());
                                tempNormals.Add(OpenTK.Vector3.Cross(vecCurrDown, vecCurrRight).Normalized());
                            }
                            else if (x == (hgWidth - 1) && y != 0 && y != (hgWidth - 1))
                            {
                                OpenTK.Vector3 currentVx = new OpenTK.Vector3(x, heightData[x, y], y);
                                OpenTK.Vector3 leftVx = new OpenTK.Vector3(x - 1, heightData[x-1, y], y);
                                OpenTK.Vector3 upVx = new OpenTK.Vector3(x, heightData[x, y-1], y - 1);
                                OpenTK.Vector3 downVx = new OpenTK.Vector3(x, heightData[x, y+1], y + 1);

                                OpenTK.Vector3 vecCurrLeft = leftVx - currentVx;
                                OpenTK.Vector3 vecCurrUp = upVx - currentVx;
                                OpenTK.Vector3 vecCurrDown = downVx - currentVx;

                                tempNormals.Add(OpenTK.Vector3.Cross(vecCurrLeft, vecCurrDown).Normalized());
                                tempNormals.Add(OpenTK.Vector3.Cross(vecCurrUp, vecCurrLeft).Normalized());
                            }
                            else if (x != 0 && x != (hgWidth - 1) && y == 0)
                            {
                                OpenTK.Vector3 currentVx = new OpenTK.Vector3(x, heightData[x, y], y);
                                OpenTK.Vector3 leftVx = new OpenTK.Vector3(x - 1, heightData[x-1, y], y);
                                OpenTK.Vector3 rightVx = new OpenTK.Vector3(x + 1, heightData[x+1, y], y);
                                OpenTK.Vector3 upVx = new OpenTK.Vector3(x, heightData[x, y-1], y - 1);

                                OpenTK.Vector3 vecCurrLeft = leftVx - currentVx;
                                OpenTK.Vector3 vecCurrRight = rightVx - currentVx;
                                OpenTK.Vector3 vecCurrUp = upVx - currentVx;

                                tempNormals.Add(OpenTK.Vector3.Cross(vecCurrRight, vecCurrUp).Normalized());
                                tempNormals.Add(OpenTK.Vector3.Cross(vecCurrUp, vecCurrLeft).Normalized());
                            }
                            else if (x != 0 && x != (hgWidth - 1) && y == (hgHeight - 1))
                            {
                                // Get current vertex and adjacent vertices
                                OpenTK.Vector3 currentVx = new OpenTK.Vector3(x, heightData[x, y], y);
                                OpenTK.Vector3 leftVx = new OpenTK.Vector3(x - 1, heightData[x - 1, y], y);
                                OpenTK.Vector3 rightVx = new OpenTK.Vector3(x + 1, heightData[x + 1, y], y);
                                OpenTK.Vector3 upVx = new OpenTK.Vector3(x, heightData[x, y - 1], y - 1);

                                // Obtain the vector from the current vertex and its adjacents
                                OpenTK.Vector3 vecCurrLeft = leftVx - currentVx;
                                OpenTK.Vector3 vecCurrRight = rightVx - currentVx;
                                OpenTK.Vector3 vecCurrUp = upVx - currentVx;

                                // Calculate all normals between them
                                OpenTK.Vector3 rightUpNormal = OpenTK.Vector3.Cross(vecCurrRight, vecCurrUp);
                                OpenTK.Vector3 upLeftNormal = OpenTK.Vector3.Cross(vecCurrUp, vecCurrLeft);

                                tempNormals.Add(rightUpNormal.Normalized());
                                tempNormals.Add(upLeftNormal.Normalized());
                            }
                            // General case
                            else
                            {
                                // Get current vertex and adjacent vertices
                                OpenTK.Vector3 currentVx = new OpenTK.Vector3(x, heightData[x, y], y);
                                OpenTK.Vector3 leftVx = new OpenTK.Vector3(x - 1, heightData[x-1, y], y);
                                OpenTK.Vector3 rightVx = new OpenTK.Vector3(x + 1, heightData[x+1, y], y);
                                OpenTK.Vector3 upVx = new OpenTK.Vector3(x, heightData[x, y-1], y - 1);
                                OpenTK.Vector3 downVx = new OpenTK.Vector3(x, heightData[x, y+1], y + 1);

                                // Obtain the vector from the current vertex and its adjacents
                                OpenTK.Vector3 vecCurrLeft = leftVx - currentVx;
                                OpenTK.Vector3 vecCurrRight = rightVx - currentVx;
                                OpenTK.Vector3 vecCurrUp = upVx - currentVx;
                                OpenTK.Vector3 vecCurrDown = downVx - currentVx;

                                // Calculate all normals between them
                                OpenTK.Vector3 leftDownNormal = OpenTK.Vector3.Cross(vecCurrLeft, vecCurrDown);
                                OpenTK.Vector3 downRightNormal = OpenTK.Vector3.Cross(vecCurrDown, vecCurrRight);
                                OpenTK.Vector3 rightUpNormal = OpenTK.Vector3.Cross(vecCurrRight, vecCurrUp);
                                OpenTK.Vector3 upLeftNormal = OpenTK.Vector3.Cross(vecCurrUp, vecCurrLeft);

                                tempNormals.Add(leftDownNormal.Normalized());
                                tempNormals.Add(downRightNormal.Normalized());
                                tempNormals.Add(rightUpNormal.Normalized());
                                tempNormals.Add(upLeftNormal.Normalized());
                            }

                            int normCount = tempNormals.Count;
                            OpenTK.Vector3 meanNormal = tempNormals[0];

                            // Get the mean from all normals for this vertex
                            for (int n = 1; n < normCount; n++)
                            {
                                // Only store Y component
                                meanNormal += tempNormals[n].Normalized();
                            }

                            meanNormal /= normCount;

                            normals[x, y] = (UInt16)(meanNormal.Y * 255f);
                            tempNormals.Clear();
                        }
                    }

                    // Export the height and normal data
                    TIFFWriter.ExportTIFF(heightData, normals, hgWidth, hgHeight, heightmapPath);
                    fileCount++;

                    exportForm.SetFileProgress(fileCount);
                    exportForm.Refresh();
                    exportForm.Invalidate();
                }

                exportForm.SetFileProgress(fileCount);
                exportForm.Refresh();
                exportForm.Invalidate();

                exportForm.Close();
            }
        }

        private void toolStripButtonSave_Click(object sender, EventArgs e)
        {
            SaveProject();
        }

        private void convertirSRTMATIFFToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                openFileDialogSrtm.ShowDialog();
                string path = openFileDialogSrtm.FileName;

                FileInfo fInfo = new FileInfo(path);

                _loadForm = new FormFileLoad(this);
                _loadForm.SetFilePath(path);
                _loadForm.SetFileSize((int)fInfo.Length);

                _loadForm.Show();

                FileLoader loader = new FileLoader(path, _loadForm, this);

                Thread loaderThread = new Thread(new ThreadStart(loader.Run));
                loaderThread.Start();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "File conversion failed", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void toolStripButtonShowAll_Click(object sender, EventArgs e)
        {
            if (_project != null)
            {
                Tile selectedTile = _explorer.GetSelectedTile();

                if (selectedTile != null)
                {
                    _viewer.GetRenderEngine().SetCameraPosition(selectedTile.GetPosition().X * 256f * _viewer.GetRenderEngine().GetCameraZoom(),
                        selectedTile.GetPosition().Y * 256f  * _viewer.GetRenderEngine().GetCameraZoom());
                }
            }
        }

        private void ImportProject(string path)
        {
            try
            {
                _project = ProjectSerializer.MarshallProject(importFileDialogProject.FileName, true);
                this.Text = "Terrain Designer - (" + _project.GetProjectName() + ")*";

                _projectSaved = false;

                _settings.OnProjectCreate();
                _settings.SetProjectName(_project.GetProjectName());
                _settings.SetCameraHeight(_project.GetRelativeHeight());
                _settings.SetCameraMode(_project.GetCameraMode());
                _settings.SetProjectAuthor(_project.GetProjectAuthor());
                _settings.SetResolution(_project.GetResolution());

                _explorer.UpdateTiles();
                _explorer.UpdateCameras();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Project loading failed", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void importarToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                importFileDialogProject.ShowDialog();
                ImportProject(importFileDialogProject.FileName);
            }
            catch (Exception)
            {
            }
        }
    }
}
