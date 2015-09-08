using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.ComponentModel;
using System.Windows.Forms;

namespace TerrenosHelper
{
    public enum IberpixTileMode
    {
        IberpixSpot5 = 0,
        IberpixHeightmap
    }

    public class IberpixDownloader
    {
        public IberpixDownloader()
        {
            _web = new WebClient();

            _web.DownloadFileCompleted += new AsyncCompletedEventHandler(Completed);
            _web.DownloadProgressChanged += new DownloadProgressChangedEventHandler(ProgressChanged);

            _form = new IberpixDownloadProgressForm();
            _totalProgress = 0;
            _actualI = 0;
            _actualJ = 0;
            _totalFiles = 0;
        }

        public void SetCoordinates(int startI, int startJ, int endI, int endJ, int z, int r, IberpixTileMode n)
        {
            _startI = startI;
            _startJ = startJ;
            _endI = endI;
            _endJ = endJ;
            _z = z;
            _r = r;
            _n = n;
        }

        public void SetDestination(string destination)
        {
            _destination = destination;
        }

        public int GetStartI()
        {
            return _startI;
        }

        public int GetStartJ()
        {
            return _startJ;
        }

        public int GetEndI()
        {
            return _endI;
        }

        public int GetEndJ()
        {
            return _endJ;
        }

        public int GetZ()
        {
            return _z;
        }

        public int GetR()
        {
            return _r;
        }
        
        public IberpixTileMode GetN()
        {
            return _n;
        }

        public void StartDownload()
        {
            _form.SetTotalFiles((_endI - _startI)*(_endJ - _startJ));
            _form.SetDownloadedFiles(0);

            _actualI = _startI;
            _actualJ = _startJ;

            _completeUri = _uriBase;
            
            if (_n == IberpixTileMode.IberpixHeightmap)
            {
                _completeUri += "n=relieve;";
            }
            else if (_n == IberpixTileMode.IberpixSpot5)
            {
                _completeUri += "n=spot5;";
            }

            _completeUri += "z=" + _z + ";";
            _completeUri += "r=" + _r * 1000 + ";";
            _completeUri += "i=" + _actualI + ";";
            _completeUri += "j=" + _actualJ + ".jpg";

            WebRequest sizeRequest = HttpWebRequest.Create(_completeUri);
            sizeRequest.Method = "HEAD";

            using (WebResponse response = sizeRequest.GetResponse())
            {
                _form.SetFileSize((int) response.ContentLength);
                _lastFileSize = (int) response.ContentLength;
            }

            _web.DownloadFileAsync(new Uri(_completeUri), _destination + "\\" + _actualI + "_" + _actualJ + ".jpg");
            _form.Show();
        }

        private void ProgressChanged(object sender, DownloadProgressChangedEventArgs e)
        {
            _form.SetDownloadProgress((int)e.BytesReceived / 1024);
        }

        private void Completed(object sender, AsyncCompletedEventArgs e)
        {
            _totalProgress++;
            _form.SetDownloadedFiles(_totalProgress);
            _form.SetDownloadProgress(_lastFileSize);

            {
                int iCoord = 0, jCoord = 0;

                if (_endI >= _startI)
                {
                    iCoord = _endI - _startI;
                }
                else
                {
                    iCoord = _startI - _endI;
                }

                if (_endJ >= _startJ)
                {
                    jCoord = _endJ - _startJ;
                }
                else
                {
                    jCoord = _startJ - _endJ;
                }

                _totalFiles = iCoord * jCoord;
            }

            if (_totalProgress > _totalFiles)
            {
                _form.SetDownloadedFiles(_totalFiles);
                MessageBox.Show("Download completed.", "End", MessageBoxButtons.OK, MessageBoxIcon.Information);
                _form.Hide();
            }
            else
            {
                _actualI++;

                if (_actualI >= _endI)
                {
                    _actualI = _startI;
                    _actualJ++;
                }

                _completeUri = _uriBase;

                if (_n == IberpixTileMode.IberpixHeightmap)
                {
                    _completeUri += "n=relieve;";
                }
                else if (_n == IberpixTileMode.IberpixSpot5)
                {
                    _completeUri += "n=spot5;";
                }

                _completeUri += "z=" + _z + ";";
                _completeUri += "r=" + _r * 1000 + ";";
                _completeUri += "i=" + _actualI + ";";
                _completeUri += "j=" + _actualJ + ".jpg";

                WebRequest sizeRequest = HttpWebRequest.Create(_completeUri);
                sizeRequest.Method = "HEAD";

                using (WebResponse response = sizeRequest.GetResponse())
                {
                    _form.SetFileSize((int)response.ContentLength);
                    _lastFileSize = (int)response.ContentLength;
                }

                _web.DownloadFileAsync(new Uri(_completeUri), _destination + "\\" + _actualI + "_" + _actualJ + ".jpg");
            }
        }

        private int _totalProgress;
        private int _actualI;
        private int _actualJ;
        private string _completeUri;
        private int _lastFileSize;
        private int _totalFiles;

        private WebClient _web;
        private IberpixDownloadProgressForm _form;

        private const string _uriBase = "http://www2.ign.es/iberpix/tileserver/";
        private string _destination;
        private int _startI;
        private int _startJ;
        private int _endI;
        private int _endJ;
        private int _z;
        private int _r;
        private IberpixTileMode _n;
    }
}
