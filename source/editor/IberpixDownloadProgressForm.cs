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
    public partial class IberpixDownloadProgressForm : Form
    {
        private int _totalFiles;
        private int _fileSize;

        public IberpixDownloadProgressForm()
        {
            InitializeComponent();
        }

        public void SetDownloadProgress(int progress)
        {
            progressBarDownload.Value = (int)((float)((float) progress / (float) _fileSize) * 100f);
            labelProgress.Text = progress / 1024 + " KB / " + _fileSize / 1024 + " KB";
        }

        public void SetFileSize(int size)
        {
            _fileSize = size;
        }

        public void SetTotalFiles(int total)
        {
            _totalFiles = total;
        }

        public void SetDownloadedFiles(int files)
        {
            progressBarTotal.Value = (int)((float)((float)files / (float)_totalFiles) * 100f);
            labelTotalProgress.Text = files + " / " + _totalFiles;
        }
    }
}
