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
    public partial class ExportProgressForm : Form
    {
        private int _fileCount;

        public ExportProgressForm()
        {
            InitializeComponent();
        }

        public void SetActualFileName(string name)
        {
            labelActualFile.Text = "Escribiendo archivo" + name + "...";
        }

        public void SetFileCount(int count)
        {
            _fileCount = count;
            labelCount.Text = "0 / " + _fileCount.ToString();
        }

        public void SetFileProgress(int progress)
        {
            labelCount.Text = progress.ToString() + " / " + _fileCount.ToString();

            float max = (float)_fileCount;
            float now = (float)progress;
            float perc = (now / max) * 100f;

            progressBar.Value = (int)perc;
        }
    }
}
