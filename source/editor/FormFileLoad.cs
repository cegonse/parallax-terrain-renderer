using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TerrenosHelper
{
    public partial class FormFileLoad : Form
    {
        private MainForm _main;
        private string _path;
        private int _fileSize;
        private long _readSize;

        public FormFileLoad(MainForm main)
        {
            _main = main;
            InitializeComponent();
        }

        private void FormFileLoad_Load(object sender, EventArgs e)
        {

        }

        public void SetFilePath(string path)
        {
            _path = path;

            if (path.Length > 50)
            {
                path = _path.Substring(0, 50) + "...";
            }

            pathLabel.Text = "Archivo:\n\n" + path;
        }

        public void SetFileSize(int size)
        {
            _fileSize = size;
            sizeReadLabel.Text = _readSize / 1000 + " KB / " + _fileSize + " KB";
        }

        public void SetReadSize(long size)
        {
            _readSize = size;
            sizeReadLabel.Text = _readSize / 1000 + " KB / " + _fileSize / 1000 + " KB";

            if (_readSize < _fileSize)
            {
                progressBar1.Value = (int)(((float)_readSize) / ((float)_fileSize) * 100f);
            }
            else
            {
                progressBar1.Value = 100;
            }
        }

        delegate void Safe_SetReadSizeCallback(long size);

        public void Safe_SetReadSize(long size)
        {
            if (this.sizeReadLabel.InvokeRequired)
            {
                Safe_SetReadSizeCallback callback = new Safe_SetReadSizeCallback(SetReadSize);
                this.Invoke(callback, new object[] { size });
            }
            else
            {
                SetReadSize(size);
            }
        }
    }
}
