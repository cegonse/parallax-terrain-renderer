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
    public partial class ProjectSettingsForm : DockContent
    {
        private MainForm _main;
        private bool _projectCreated = false;

        public ProjectSettingsForm(MainForm main)
        {
            InitializeComponent();
            _main = main;
        }

        public void OnProjectCreate()
        {
            _projectCreated = true;
        }

        public void OnProjectDestroy()
        {
            _projectCreated = false;
        }

        public void SetProjectName(string name)
        {
            textBoxName.Text = name;
        }

        public void SetProjectAuthor(string author)
        {
            textBoxAuthor.Text = author;
        }

        public void SetResolution(float resolution)
        {
            textBoxResolution.Text = resolution.ToString();
        }

        public void SetCameraMode(CameraMode mode)
        {
            if (mode == CameraMode.CameraModeConstantHeight)
            {
                radioButtonConstantHeight.Checked = true;
                radioButtonRelativeHeight.Checked = false;
            }
            else if (mode == CameraMode.CameraModeRelativeHeight)
            {
                radioButtonConstantHeight.Checked = false;
                radioButtonRelativeHeight.Checked = true;
            }
        }

        public void SetCameraHeight(float height)
        {
            textBoxHeight.Text = height.ToString();
        }

        private void textBoxName_TextChanged(object sender, EventArgs e)
        {
            if (_projectCreated)
                _main.OnProjectNameChanged(textBoxName.Text);
            else
                textBoxName.Text = "";
        }

        private void textBoxAuthor_TextChanged(object sender, EventArgs e)
        {
            if (_projectCreated)
                _main.OnProjectAuthorChanged(textBoxAuthor.Text);
            else
                textBoxAuthor.Text = "";
        }

        private void radioButtonConstantHeight_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButtonConstantHeight.Checked)
            {
                if (_projectCreated)
                {
                    _main.OnProjectCameraModeChanged(CameraMode.CameraModeConstantHeight);
                }
            }
        }

        private void radioButtonRelativeHeight_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButtonRelativeHeight.Checked)
            {
                if (_projectCreated)
                {
                    _main.OnProjectCameraModeChanged(CameraMode.CameraModeRelativeHeight);
                }
            }
        }

        private void textBoxHeight_TextChanged(object sender, EventArgs e)
        {
            if (_projectCreated)
            {
                try
                {
                    _main.OnProjectCameraHeightChanged(float.Parse(textBoxHeight.Text));
                }
                catch (Exception)
                {
                    textBoxHeight.Text = _main.GetProject().GetRelativeHeight().ToString();
                }
            }
            else
            {
                textBoxHeight.Text = "";
            }
        }

        private void textBoxResolution_TextChanged(object sender, EventArgs e)
        {
            if (_projectCreated)
            {
                try
                {
                    _main.OnProjectResolutionChanged(float.Parse(textBoxResolution.Text));
                }
                catch (Exception)
                {
                    textBoxResolution.Text = _main.GetProject().GetResolution().ToString();
                }
            }
            else
            {
                textBoxResolution.Text = "";
            }
        }
    }
}
