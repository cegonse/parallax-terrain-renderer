using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using OpenTK;

namespace TerrenosHelper
{
    class ProjectSerializer
    {
        public static void SerializeProject(Project src, string path)
        {
            using (XmlWriter writer = XmlWriter.Create(path))
            {
                writer.WriteStartDocument();
                writer.WriteStartElement("project");

                writer.WriteElementString("name", src.GetProjectName());
                writer.WriteElementString("author", src.GetProjectAuthor());
                writer.WriteElementString("date", src.GetCreationDate().ToLongDateString());
                writer.WriteElementString("resolution", src.GetResolution().ToString());

                if (src.GetCameraMode() == CameraMode.CameraModeConstantHeight)
                {
                    writer.WriteElementString("camera-mode", "constant-height");
                    writer.WriteElementString("height", src.GetConstantHeight().ToString());
                }
                else if (src.GetCameraMode() == CameraMode.CameraModeRelativeHeight)
                {
                    writer.WriteElementString("camera-mode", "relative-height");
                    writer.WriteElementString("height", src.GetRelativeHeight().ToString());
                }

                List<Tile> tiles = src.GetTileList();

                if (tiles.Count != 0)
                {
                    writer.WriteStartElement("tiles");
                }

                for (int i = 0; i < tiles.Count; i++)
                {
                    writer.WriteStartElement("tile");

                    writer.WriteElementString("latitude", tiles[i].GetPosition().X.ToString());
                    writer.WriteElementString("longitude", tiles[i].GetPosition().Y.ToString());
                    writer.WriteElementString("texture", tiles[i].GetTexturePath());
                    writer.WriteElementString("heightmap", tiles[i].GetHeightmapPath());

                    writer.WriteEndElement();
                }

                if (tiles.Count != 0)
                {
                    writer.WriteEndElement();
                }

                List<Camera> cameras = src.GetCameraList();

                if (cameras.Count != 0)
                {
                    writer.WriteStartElement("cameras");
                }

                for (int i = 0; i < cameras.Count; i++)
                {
                    writer.WriteStartElement("camera");
                    
                    writer.WriteElementString("x", cameras[i].GetPosition().X.ToString());
                    writer.WriteElementString("y", cameras[i].GetPosition().Y.ToString());
                    writer.WriteElementString("yaw", cameras[i].GetRotation().ToString());

                    writer.WriteEndElement();
                }

                if (cameras.Count != 0)
                {
                    writer.WriteEndElement();
                }

                writer.WriteEndElement();
                writer.WriteEndDocument();
            }
        }

        public static Project MarshallProject(string path, bool completePaths = false)
        {
            Project proj = new Project("");

            using (XmlReader reader = XmlReader.Create(path))
            {
                while (reader.Read())
                {
                    if (reader.IsStartElement())
                    {
                        switch (reader.Name)
                        {
                            case "name":
                                reader.Read();
                                proj.SetProjectName(reader.ReadContentAsString());
                                break;

                            case "author":
                                reader.Read();
                                proj.SetProjectAuthor(reader.ReadContentAsString());
                                break;

                            case "resolution":
                                reader.Read();
                                proj.SetResolution(reader.ReadContentAsFloat());
                                break;

                            case "camera-mode":
                                reader.Read();
                                string mode = reader.ReadContentAsString();

                                if (mode == "constant-height")
                                {
                                    proj.SetCameraMode(CameraMode.CameraModeConstantHeight);
                                }
                                else if (mode == "relative-height")
                                {
                                    proj.SetCameraMode(CameraMode.CameraModeRelativeHeight);
                                }
                                break;

                            case "height":
                                reader.Read();
                                float height = reader.ReadContentAsFloat();

                                proj.SetConstantHeight(height);
                                proj.SetRelativeHeight(height);
                                break;

                            case "tile":
                                Tile t = new Tile();

                                reader.Read();
                                
                                float x = reader.ReadElementContentAsFloat();
                                float y = reader.ReadElementContentAsFloat();

                                t.SetPosition(new Vector2(x, y));

                                string texturePath = reader.ReadElementContentAsString();
                                string heightmapPath = reader.ReadElementContentAsString();

                                if (completePaths)
                                {
                                    int index = path.LastIndexOf('\\');
                                    string folderPath = path.Substring(0, index);

                                    texturePath = texturePath.Insert(0, folderPath + System.IO.Path.DirectorySeparatorChar + "textures" + System.IO.Path.DirectorySeparatorChar);
                                    heightmapPath = heightmapPath.Insert(0, folderPath + System.IO.Path.DirectorySeparatorChar + "heightmaps" + System.IO.Path.DirectorySeparatorChar);
                                }

                                t.LoadTextureData(texturePath);
                                t.LoadHeightmapData(heightmapPath);

                                proj.GetTileList().Add(t);
                                break;

                            case "camera":
                                Camera c = new Camera();

                                reader.Read();

                                string scx = reader.ReadElementContentAsString();
                                string scy = reader.ReadElementContentAsString();
                                string scyaw = reader.ReadElementContentAsString();

                                float cx = float.Parse(scx);
                                float cy = float.Parse(scy);
                                float cyaw = float.Parse(scyaw);

                                //System.Windows.Forms.MessageBox.Show(scx + "," + scy);

                                c.SetPosition(new Vector2(cx, cy));
                                c.SetRotation(cyaw);

                                proj.GetCameraList().Add(c);
                                break;
                        }
                    }
                }
            }

            return proj;
        }
    }
}
