using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Drawing.Imaging;
using BitMiracle.LibTiff.Classic;

namespace TerrenosHelper
{
    public class TIFFWriter
    {
        public static void ExportTIFF(Bitmap bitmap, string path)
        {
            Tiff tif = Tiff.Open(path, "w");
            
            byte[] raster = getImageRasterBytes(bitmap, PixelFormat.Format32bppArgb);
            tif.SetField(TiffTag.IMAGEWIDTH, bitmap.Width);
            tif.SetField(TiffTag.IMAGELENGTH, bitmap.Height);
            tif.SetField(TiffTag.COMPRESSION, Compression.LZW);
            tif.SetField(TiffTag.PHOTOMETRIC, Photometric.RGB);

            tif.SetField(TiffTag.ROWSPERSTRIP, bitmap.Height);

            tif.SetField(TiffTag.XRESOLUTION, bitmap.HorizontalResolution);
            tif.SetField(TiffTag.YRESOLUTION, bitmap.VerticalResolution);

            tif.SetField(TiffTag.BITSPERSAMPLE, 8);
            tif.SetField(TiffTag.SAMPLESPERPIXEL, 4);

            tif.SetField(TiffTag.PLANARCONFIG, PlanarConfig.CONTIG);

            int stride = raster.Length / bitmap.Height;
            convertSamples(raster, bitmap.Width, bitmap.Height);

            for (int i = 0, offset = 0; i < bitmap.Height; i++)
            {
                tif.WriteScanline(raster, offset, i, 0);
                offset += stride;
            }

            tif.Close();
        }

        public static void ExportTIFF(UInt16[,] heights, UInt16[,] normals, int width, int height, string path)
        {
            Tiff tif = Tiff.Open(path, "w");

            int channelsPerPixel = 4;
            int bytesPerChannel = 2;

            // w * h * 16 bytes * samples per pixel
            byte[] raster = new byte[width * height * bytesPerChannel * channelsPerPixel];

            tif.SetField(TiffTag.IMAGEWIDTH, width);
            tif.SetField(TiffTag.IMAGELENGTH, height);
            tif.SetField(TiffTag.COMPRESSION, Compression.LZW);
            tif.SetField(TiffTag.PHOTOMETRIC, Photometric.RGB);

            tif.SetField(TiffTag.ROWSPERSTRIP, height);

            tif.SetField(TiffTag.XRESOLUTION, 72);
            tif.SetField(TiffTag.YRESOLUTION, 72);
            tif.SetField(TiffTag.PLANARCONFIG, PlanarConfig.CONTIG);

            tif.SetField(TiffTag.BITSPERSAMPLE, 8 * bytesPerChannel);
            tif.SetField(TiffTag.SAMPLESPERPIXEL, channelsPerPixel);
            tif.SetField(TiffTag.FILLORDER, FillOrder.MSB2LSB);

            // Convert the height and normal data matrix into a raster vector
            for (int x = 0; x < width; x++)
            {
                for (int y = 0; y < height; y++)
                {
                    // Fetch the height and normal for this pixel
                    UInt16 pxHeight = heights[x,y];
                    UInt16 pxNormal = normals[x,y];

                    byte[] tempHeight = new byte[2];
                    byte[] tempNormal = new byte[2];

                    // Convert them into a 2 byte sequence
                    if (!BitConverter.IsLittleEndian)
                    {
                        tempHeight[0] = (byte)(pxHeight & 0x00FF);
                        tempHeight[1] = (byte)((pxHeight & 0xFF00) >> 8);

                        tempNormal[0] = (byte)(pxNormal & 0x00FF);
                        tempNormal[1] = (byte)((pxNormal & 0xFF00) >> 8);
                    }
                    else
                    {
                        tempHeight[0] = (byte)((pxHeight & 0xFF00) >> 8);
                        tempHeight[1] = (byte)(pxHeight & 0x00FF);

                        tempNormal[0] = (byte)((pxNormal & 0xFF00) >> 8);
                        tempNormal[1] = (byte)(pxNormal & 0x00FF);
                    }

                    // Fill the raster for this pixel
                    //
                    // R channel (height)
                    raster[(x * channelsPerPixel * bytesPerChannel) + (y * height * channelsPerPixel * bytesPerChannel)] = tempHeight[0];
                    raster[(x * channelsPerPixel * bytesPerChannel) + 1 + (y * height * channelsPerPixel * bytesPerChannel)] = tempHeight[1];
                    // G channel (normal)
                    raster[(x * channelsPerPixel * bytesPerChannel) + 2 + (y * height * channelsPerPixel * bytesPerChannel)] = tempNormal[0];
                    raster[(x * channelsPerPixel * bytesPerChannel) + 3 + (y * height * channelsPerPixel * bytesPerChannel)] = tempNormal[1];
                    // Channels B and A are set to zeroes
                    raster[(x * channelsPerPixel * bytesPerChannel) + 4 + (y * height * channelsPerPixel * bytesPerChannel)] = 0;
                    raster[(x * channelsPerPixel * bytesPerChannel) + 5 + (y * height * channelsPerPixel * bytesPerChannel)] = 0;

                    raster[(x * channelsPerPixel * bytesPerChannel) + 6 + (y * height * channelsPerPixel * bytesPerChannel)] = 0;
                    raster[(x * channelsPerPixel * bytesPerChannel) + 7 + (y * height * channelsPerPixel * bytesPerChannel)] = 0;
                }
            }

            // Write out the raster
            int stride = width * bytesPerChannel * channelsPerPixel;

            for (int y = 0; y < height; y++)
            {
                byte[] buffer = new byte[stride];
                Buffer.BlockCopy(raster, stride * y, buffer, 0, buffer.Length);
                tif.WriteScanline(buffer, y);
            }

            tif.Close();
        }

        private static byte[] getImageRasterBytes(Bitmap bitmap, PixelFormat format)
        {
            Rectangle rect = new Rectangle(0, 0, bitmap.Width, bitmap.Height);
            byte[] bits = null;

            try
            {
                // Lock the managed memory
                BitmapData bitmapdata = bitmap.LockBits(rect, ImageLockMode.ReadWrite, format);

                // Declare an array to hold the bytes of the bitmap.
                bits = new byte[bitmapdata.Stride * bitmapdata.Height];

                // Copy the values into the array.
                System.Runtime.InteropServices.Marshal.Copy(bitmapdata.Scan0, bits, 0, bits.Length);

                // Release managed memory
                bitmap.UnlockBits(bitmapdata);
            }
            catch
            {
                return null;
            }

            return bits;
        }

        private static void convertSamples(byte[] data, int width, int height)
        {
            int stride = data.Length / height;
            const int samplesPerPixel = 4;

            for (int y = 0; y < height; y++)
            {
                int offset = stride * y;
                int strideEnd = offset + width * samplesPerPixel;

                for (int i = offset; i < strideEnd; i += samplesPerPixel)
                {
                    byte temp = data[i + 2];
                    data[i + 2] = data[i];
                    data[i] = temp;
                }
            }
        }
    }
}
