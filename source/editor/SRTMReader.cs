using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace TerrenosHelper
{
    public enum SRTMFormat
    {
        SRTM3,
        SRTM1,
        Unknown
    }

    public class SRTMReader
    {
        public SRTMReader(BinaryReader reader, FileLoader loader, SRTMFormat format)
        {
            if (reader == null)
            {
                throw new NullReferenceException("BinaryReader musn't be NULL.");
            }

            _binaryReader = reader;
            _format = format;

            if (_format == SRTMFormat.SRTM1)
            {
                _tileSize = 3601;
            }
            else if (_format == SRTMFormat.SRTM3)
            {
                _tileSize = 1201;
            }
            else if (_format == SRTMFormat.Unknown)
            {
                throw new FormatException("Unknown HGT file format.");
            }

            _heightData = new Int16[_tileSize, _tileSize];
            _loader = loader;
        }

        public void ReadAllData(bool buffer = true)
        {
            bool littleEndian = BitConverter.IsLittleEndian;
            long bytes = 0;
            Int16[] data = null;
            int bufferIndex = 0;

            if (buffer)
            {
                data = new Int16[_tileSize * _tileSize];
                byte[] bBuffer = _binaryReader.ReadBytes(_tileSize * _tileSize * 2);

                for (int i = 0; i < bBuffer.Length; i += 2)
                {
                    data[bufferIndex] = BitConverter.ToInt16(bBuffer, i);
                    bufferIndex++;
                }
            }

            bufferIndex = 0;

            for (int i = 0; i < _tileSize; i++)
            {
                for (int j = 0; j < _tileSize; j++)
                {
                    Int16 temp = 0;

                    if (buffer)
                    {
                        temp = data[bufferIndex];
                        bufferIndex++;
                    }
                    else
                    {
                        temp = _binaryReader.ReadInt16();
                    }

                    if (temp == sizeof(Int16) * 4)
                        temp = 0;

                    if (_loader != null)
                        _loader.SetLoadedBytes(bytes);
                    
                    if (littleEndian)
                    {
                        byte[] tempByteArray = BitConverter.GetBytes(temp);
                        tempByteArray.Reverse();
                        temp = BitConverter.ToInt16(tempByteArray, 0);
                    }

                    _heightData[i, j] = temp;
                    bytes += 2;
                }
            }
        }

        public Int16[,] GetElevationDataMatrix()
        {
            return _heightData;
        }

        public Int16 GetElevationAt(int x, int y)
        {
            return _heightData[x, y];
        }

        public int GetTileSize()
        {
            return _tileSize;
        }

        private BinaryReader _binaryReader;
        private Int16[,] _heightData;
        private FileLoader _loader;
        private SRTMFormat _format;
        private int _tileSize;
    }
}
