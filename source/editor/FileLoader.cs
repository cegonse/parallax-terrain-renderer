using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace TerrenosHelper
{
    public class FileLoader
    {
        private FormFileLoad _form;
        private MainForm _main;
        private string _path;

        public void SetLoadedBytes(long bytes)
        {
            _form.Safe_SetReadSize(bytes);
        }

        public void Run()
        {
            try
            {
                FileStream file = File.Open(_path, FileMode.Open);
                BinaryReader reader = new BinaryReader(file);

                SRTMFormat format = SRTMFormat.Unknown;

                if (file.Length == 3601 * 3601 * sizeof(Int16))
                {
                    format = SRTMFormat.SRTM1;
                }
                else if (file.Length == 1201 * 1201 * sizeof(Int16))
                {
                    format = SRTMFormat.SRTM3;
                }

                SRTMReader srtm = new SRTMReader(reader, this, format);

                srtm.ReadAllData();

                file.Close();
                _main.Safe_OnSRTMFileLoadComplete(srtm);
            }
            catch (Exception ex)
            {
                throw ex;
            }
        }

        public FileLoader(string path, FormFileLoad form, MainForm main)
        {
            _main = main;
            _form = form;
            _path = path;
        }
    }
}
