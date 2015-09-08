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
    public partial class IberpixSettingsForm : Form
    {
        private IberpixDownloader _downloader;

        public IberpixSettingsForm()
        {
            InitializeComponent();
            _downloader = new IberpixDownloader();
        }

        private void buttonBrowse_Click(object sender, EventArgs e)
        {
            folderBrowserDialog.ShowDialog();
            _downloader.SetDestination(folderBrowserDialog.SelectedPath);
        }

        private void buttonDownload_Click(object sender, EventArgs e)
        {
            MessageBox.Show("El uso de estas debe cumplir las condiciones impuestas\n" +
            "por el Instituto Cartográfico Nacional. Para más información consultar la\n" +
            "página http://www.ign.es/ign/layoutIn/avisolegal.do .", "Aviso Legal", MessageBoxButtons.OK, MessageBoxIcon.Information);

            _downloader.SetCoordinates(int.Parse(textBoxStartI.Text), int.Parse(textBoxStartJ.Text),
                int.Parse(textBoxEndI.Text), int.Parse(textBoxEndJ.Text), int.Parse(textBoxUtm.Text),
                int.Parse(textBoxResolution.Text), (IberpixTileMode) comboBoxLayer.SelectedIndex);

            this.Hide();
            _downloader.StartDownload();
        }
    }
}
