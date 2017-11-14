﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using BitMiracle.LibTiff.Classic;

namespace TerrenosHelper
{
    public enum BitmapChannels : byte
    {
        ChannelRed = 0,
        ChannelBlue,
        ChannelGreen,
        ChannelAlpha
    }

    public class BitmapTerrain
    {
        public BitmapTerrain(int width, int height)
        {
            _bitmap = new Bitmap(width, height);
            _width = width;
            _height = height;
        }

        public BitmapTerrain(Bitmap source)
        {
            _bitmap = source;
            _width = source.Width;
            _height = source.Height;
        }

        public Bitmap GetBitmap()
        {
            return _bitmap;
        }

        public void ConvertFromSRTM(SRTMReader srtm, BitmapChannels channels)
        {
            int mean = 0, max = 0;

            if (channels == BitmapChannels.ChannelRed)
            {
                for (int i = 0; i < _height; i++)
                {
                    for (int j = 0; j < _width; j++)
                    {
                        mean += srtm.GetElevationAt(i, j);

                        if (max < srtm.GetElevationAt(i, j))
                            max = srtm.GetElevationAt(i, j);
                    }

                    mean /= _width;
                }
            }

            for (int i = 0; i < _height; i++)
            {
                for (int j = 0; j < _width; j++)
                {
                    Int16 height = srtm.GetElevationAt(i, j);
                    byte[] heightArray = BitConverter.GetBytes(height);

                    if (channels == BitmapChannels.ChannelRed)
                    {
                        int rHeight = height - max;
                        float pointHeight = (float)rHeight / (float)max;

                        _bitmap.SetPixel(j, i, Color.FromArgb(255, (byte)(pointHeight * 255), 0, 0));
                    }
                    else if (channels == (BitmapChannels.ChannelRed | BitmapChannels.ChannelGreen))
                    {
                        _bitmap.SetPixel(j, i, Color.FromArgb(255, heightArray[0], heightArray[1], 0));
                    }
                }
            }
        }

        public void WriteTIFF(string path)
        {
            TIFFWriter.ExportTIFF(_bitmap, path);
        }

        public void OnDispose()
        {
            if (_bitmap != null)
            {
                _bitmap.Dispose();
            }
        }

        private Bitmap _bitmap;
        private int _width;
        private int _height;
    }
}
