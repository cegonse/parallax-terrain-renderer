namespace TerrenosHelper
{
    partial class ExplorerForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ExplorerForm));
            this.tabControl = new System.Windows.Forms.TabControl();
            this.tabPageTiles = new System.Windows.Forms.TabPage();
            this.groupBoxTile = new System.Windows.Forms.GroupBox();
            this.buttonTexture = new System.Windows.Forms.Button();
            this.labelTexture = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.buttonHeightmap = new System.Windows.Forms.Button();
            this.labelHeightmap = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.textBoxLongitude = new System.Windows.Forms.TextBox();
            this.labelLongitude = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.textBoxLatitude = new System.Windows.Forms.TextBox();
            this.labelLatitude = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.buttonDeleteTile = new System.Windows.Forms.Button();
            this.buttonAddTile = new System.Windows.Forms.Button();
            this.listBoxTiles = new System.Windows.Forms.ListBox();
            this.tabPageCameras = new System.Windows.Forms.TabPage();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.textBoxCameraRotationX = new System.Windows.Forms.TextBox();
            this.labelCameraRotation = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.textBoxCameraPositionX = new System.Windows.Forms.TextBox();
            this.textBoxCameraPositionZ = new System.Windows.Forms.TextBox();
            this.labelCameraPosition = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.buttonAddCamera = new System.Windows.Forms.Button();
            this.buttonDeleteCamera = new System.Windows.Forms.Button();
            this.buttonCameraDown = new System.Windows.Forms.Button();
            this.buttonCameraUp = new System.Windows.Forms.Button();
            this.listBoxCameras = new System.Windows.Forms.ListBox();
            this.openTextureDialog = new System.Windows.Forms.OpenFileDialog();
            this.openHeightmapDialog = new System.Windows.Forms.OpenFileDialog();
            this.tabControl.SuspendLayout();
            this.tabPageTiles.SuspendLayout();
            this.groupBoxTile.SuspendLayout();
            this.tabPageCameras.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl
            // 
            this.tabControl.Controls.Add(this.tabPageTiles);
            this.tabControl.Controls.Add(this.tabPageCameras);
            this.tabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl.Location = new System.Drawing.Point(0, 0);
            this.tabControl.Name = "tabControl";
            this.tabControl.SelectedIndex = 0;
            this.tabControl.Size = new System.Drawing.Size(279, 741);
            this.tabControl.TabIndex = 0;
            // 
            // tabPageTiles
            // 
            this.tabPageTiles.Controls.Add(this.groupBoxTile);
            this.tabPageTiles.Controls.Add(this.buttonDeleteTile);
            this.tabPageTiles.Controls.Add(this.buttonAddTile);
            this.tabPageTiles.Controls.Add(this.listBoxTiles);
            this.tabPageTiles.Location = new System.Drawing.Point(4, 25);
            this.tabPageTiles.Name = "tabPageTiles";
            this.tabPageTiles.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageTiles.Size = new System.Drawing.Size(271, 712);
            this.tabPageTiles.TabIndex = 0;
            this.tabPageTiles.Text = "Tiles";
            this.tabPageTiles.UseVisualStyleBackColor = true;
            // 
            // groupBoxTile
            // 
            this.groupBoxTile.Controls.Add(this.buttonTexture);
            this.groupBoxTile.Controls.Add(this.labelTexture);
            this.groupBoxTile.Controls.Add(this.label4);
            this.groupBoxTile.Controls.Add(this.buttonHeightmap);
            this.groupBoxTile.Controls.Add(this.labelHeightmap);
            this.groupBoxTile.Controls.Add(this.label3);
            this.groupBoxTile.Controls.Add(this.textBoxLongitude);
            this.groupBoxTile.Controls.Add(this.labelLongitude);
            this.groupBoxTile.Controls.Add(this.label2);
            this.groupBoxTile.Controls.Add(this.textBoxLatitude);
            this.groupBoxTile.Controls.Add(this.labelLatitude);
            this.groupBoxTile.Controls.Add(this.label1);
            this.groupBoxTile.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBoxTile.Location = new System.Drawing.Point(3, 239);
            this.groupBoxTile.Name = "groupBoxTile";
            this.groupBoxTile.Size = new System.Drawing.Size(265, 470);
            this.groupBoxTile.TabIndex = 3;
            this.groupBoxTile.TabStop = false;
            this.groupBoxTile.Text = "Tile";
            // 
            // buttonTexture
            // 
            this.buttonTexture.Dock = System.Windows.Forms.DockStyle.Top;
            this.buttonTexture.Location = new System.Drawing.Point(3, 226);
            this.buttonTexture.Name = "buttonTexture";
            this.buttonTexture.Size = new System.Drawing.Size(259, 28);
            this.buttonTexture.TabIndex = 11;
            this.buttonTexture.Text = "Browse...";
            this.buttonTexture.UseVisualStyleBackColor = true;
            this.buttonTexture.Click += new System.EventHandler(this.buttonTexture_Click);
            // 
            // labelTexture
            // 
            this.labelTexture.AutoSize = true;
            this.labelTexture.Dock = System.Windows.Forms.DockStyle.Top;
            this.labelTexture.Location = new System.Drawing.Point(3, 209);
            this.labelTexture.Name = "labelTexture";
            this.labelTexture.Size = new System.Drawing.Size(60, 17);
            this.labelTexture.TabIndex = 10;
            this.labelTexture.Text = "Texture:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Dock = System.Windows.Forms.DockStyle.Top;
            this.label4.Location = new System.Drawing.Point(3, 192);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(12, 17);
            this.label4.TabIndex = 9;
            this.label4.Text = " ";
            // 
            // buttonHeightmap
            // 
            this.buttonHeightmap.Dock = System.Windows.Forms.DockStyle.Top;
            this.buttonHeightmap.Location = new System.Drawing.Point(3, 164);
            this.buttonHeightmap.Name = "buttonHeightmap";
            this.buttonHeightmap.Size = new System.Drawing.Size(259, 28);
            this.buttonHeightmap.TabIndex = 8;
            this.buttonHeightmap.Text = "Browse...";
            this.buttonHeightmap.UseVisualStyleBackColor = true;
            this.buttonHeightmap.Click += new System.EventHandler(this.buttonHeightmap_Click);
            // 
            // labelHeightmap
            // 
            this.labelHeightmap.AutoSize = true;
            this.labelHeightmap.Dock = System.Windows.Forms.DockStyle.Top;
            this.labelHeightmap.Location = new System.Drawing.Point(3, 147);
            this.labelHeightmap.Name = "labelHeightmap";
            this.labelHeightmap.Size = new System.Drawing.Size(80, 17);
            this.labelHeightmap.TabIndex = 7;
            this.labelHeightmap.Text = "Heightmap:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Dock = System.Windows.Forms.DockStyle.Top;
            this.label3.Location = new System.Drawing.Point(3, 130);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(12, 17);
            this.label3.TabIndex = 6;
            this.label3.Text = " ";
            // 
            // textBoxLongitude
            // 
            this.textBoxLongitude.Dock = System.Windows.Forms.DockStyle.Top;
            this.textBoxLongitude.Location = new System.Drawing.Point(3, 108);
            this.textBoxLongitude.Name = "textBoxLongitude";
            this.textBoxLongitude.Size = new System.Drawing.Size(259, 22);
            this.textBoxLongitude.TabIndex = 5;
            this.textBoxLongitude.TextChanged += new System.EventHandler(this.textBoxLongitude_TextChanged);
            // 
            // labelLongitude
            // 
            this.labelLongitude.AutoSize = true;
            this.labelLongitude.Dock = System.Windows.Forms.DockStyle.Top;
            this.labelLongitude.Location = new System.Drawing.Point(3, 91);
            this.labelLongitude.Name = "labelLongitude";
            this.labelLongitude.Size = new System.Drawing.Size(75, 17);
            this.labelLongitude.TabIndex = 4;
            this.labelLongitude.Text = "Longitude:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Dock = System.Windows.Forms.DockStyle.Top;
            this.label2.Location = new System.Drawing.Point(3, 74);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(12, 17);
            this.label2.TabIndex = 3;
            this.label2.Text = " ";
            // 
            // textBoxLatitude
            // 
            this.textBoxLatitude.Dock = System.Windows.Forms.DockStyle.Top;
            this.textBoxLatitude.Location = new System.Drawing.Point(3, 52);
            this.textBoxLatitude.Name = "textBoxLatitude";
            this.textBoxLatitude.Size = new System.Drawing.Size(259, 22);
            this.textBoxLatitude.TabIndex = 2;
            this.textBoxLatitude.TextChanged += new System.EventHandler(this.textBoxLatitude_TextChanged);
            // 
            // labelLatitude
            // 
            this.labelLatitude.AutoSize = true;
            this.labelLatitude.Dock = System.Windows.Forms.DockStyle.Top;
            this.labelLatitude.Location = new System.Drawing.Point(3, 35);
            this.labelLatitude.Name = "labelLatitude";
            this.labelLatitude.Size = new System.Drawing.Size(63, 17);
            this.labelLatitude.TabIndex = 1;
            this.labelLatitude.Text = "Latitude:";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Dock = System.Windows.Forms.DockStyle.Top;
            this.label1.Location = new System.Drawing.Point(3, 18);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(12, 17);
            this.label1.TabIndex = 0;
            this.label1.Text = " ";
            // 
            // buttonDeleteTile
            // 
            this.buttonDeleteTile.Dock = System.Windows.Forms.DockStyle.Top;
            this.buttonDeleteTile.Location = new System.Drawing.Point(3, 211);
            this.buttonDeleteTile.Name = "buttonDeleteTile";
            this.buttonDeleteTile.Size = new System.Drawing.Size(265, 28);
            this.buttonDeleteTile.TabIndex = 2;
            this.buttonDeleteTile.Text = "Remove";
            this.buttonDeleteTile.UseVisualStyleBackColor = true;
            this.buttonDeleteTile.Click += new System.EventHandler(this.buttonDeleteTile_Click);
            // 
            // buttonAddTile
            // 
            this.buttonAddTile.Dock = System.Windows.Forms.DockStyle.Top;
            this.buttonAddTile.Location = new System.Drawing.Point(3, 183);
            this.buttonAddTile.Name = "buttonAddTile";
            this.buttonAddTile.Size = new System.Drawing.Size(265, 28);
            this.buttonAddTile.TabIndex = 1;
            this.buttonAddTile.Text = "Add";
            this.buttonAddTile.UseVisualStyleBackColor = true;
            this.buttonAddTile.Click += new System.EventHandler(this.buttonAddTile_Click);
            // 
            // listBoxTiles
            // 
            this.listBoxTiles.Dock = System.Windows.Forms.DockStyle.Top;
            this.listBoxTiles.FormattingEnabled = true;
            this.listBoxTiles.ItemHeight = 16;
            this.listBoxTiles.Location = new System.Drawing.Point(3, 3);
            this.listBoxTiles.Name = "listBoxTiles";
            this.listBoxTiles.Size = new System.Drawing.Size(265, 180);
            this.listBoxTiles.TabIndex = 0;
            this.listBoxTiles.SelectedIndexChanged += new System.EventHandler(this.listBoxTiles_SelectedIndexChanged);
            this.listBoxTiles.KeyDown += new System.Windows.Forms.KeyEventHandler(this.listBoxTiles_KeyDown);
            // 
            // tabPageCameras
            // 
            this.tabPageCameras.Controls.Add(this.tableLayoutPanel2);
            this.tabPageCameras.Controls.Add(this.labelCameraRotation);
            this.tabPageCameras.Controls.Add(this.label6);
            this.tabPageCameras.Controls.Add(this.tableLayoutPanel1);
            this.tabPageCameras.Controls.Add(this.labelCameraPosition);
            this.tabPageCameras.Controls.Add(this.label5);
            this.tabPageCameras.Controls.Add(this.buttonAddCamera);
            this.tabPageCameras.Controls.Add(this.buttonDeleteCamera);
            this.tabPageCameras.Controls.Add(this.buttonCameraDown);
            this.tabPageCameras.Controls.Add(this.buttonCameraUp);
            this.tabPageCameras.Controls.Add(this.listBoxCameras);
            this.tabPageCameras.Location = new System.Drawing.Point(4, 25);
            this.tabPageCameras.Name = "tabPageCameras";
            this.tabPageCameras.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageCameras.Size = new System.Drawing.Size(271, 712);
            this.tabPageCameras.TabIndex = 1;
            this.tabPageCameras.Text = "Cameras";
            this.tabPageCameras.UseVisualStyleBackColor = true;
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 3;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 34F));
            this.tableLayoutPanel2.Controls.Add(this.textBoxCameraRotationX, 0, 0);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Top;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(3, 396);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 1;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(265, 33);
            this.tableLayoutPanel2.TabIndex = 10;
            // 
            // textBoxCameraRotationX
            // 
            this.textBoxCameraRotationX.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxCameraRotationX.Enabled = false;
            this.textBoxCameraRotationX.Location = new System.Drawing.Point(3, 3);
            this.textBoxCameraRotationX.Name = "textBoxCameraRotationX";
            this.textBoxCameraRotationX.Size = new System.Drawing.Size(81, 22);
            this.textBoxCameraRotationX.TabIndex = 0;
            this.textBoxCameraRotationX.Visible = false;
            this.textBoxCameraRotationX.TextChanged += new System.EventHandler(this.textBoxCameraRotationX_TextChanged);
            // 
            // labelCameraRotation
            // 
            this.labelCameraRotation.AutoSize = true;
            this.labelCameraRotation.Dock = System.Windows.Forms.DockStyle.Top;
            this.labelCameraRotation.Location = new System.Drawing.Point(3, 379);
            this.labelCameraRotation.Name = "labelCameraRotation";
            this.labelCameraRotation.Size = new System.Drawing.Size(106, 17);
            this.labelCameraRotation.TabIndex = 9;
            this.labelCameraRotation.Text = "Rotación (yaw):";
            this.labelCameraRotation.Visible = false;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Dock = System.Windows.Forms.DockStyle.Top;
            this.label6.Location = new System.Drawing.Point(3, 362);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(12, 17);
            this.label6.TabIndex = 8;
            this.label6.Text = " ";
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 3;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 34F));
            this.tableLayoutPanel1.Controls.Add(this.textBoxCameraPositionX, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.textBoxCameraPositionZ, 2, 0);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(3, 329);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 1;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(265, 33);
            this.tableLayoutPanel1.TabIndex = 7;
            // 
            // textBoxCameraPositionX
            // 
            this.textBoxCameraPositionX.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxCameraPositionX.Location = new System.Drawing.Point(3, 3);
            this.textBoxCameraPositionX.Name = "textBoxCameraPositionX";
            this.textBoxCameraPositionX.Size = new System.Drawing.Size(81, 22);
            this.textBoxCameraPositionX.TabIndex = 0;
            this.textBoxCameraPositionX.TextChanged += new System.EventHandler(this.textBoxCameraPositionX_TextChanged);
            // 
            // textBoxCameraPositionZ
            // 
            this.textBoxCameraPositionZ.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBoxCameraPositionZ.Location = new System.Drawing.Point(177, 3);
            this.textBoxCameraPositionZ.Name = "textBoxCameraPositionZ";
            this.textBoxCameraPositionZ.Size = new System.Drawing.Size(85, 22);
            this.textBoxCameraPositionZ.TabIndex = 2;
            this.textBoxCameraPositionZ.TextChanged += new System.EventHandler(this.textBoxCameraPositionZ_TextChanged);
            // 
            // labelCameraPosition
            // 
            this.labelCameraPosition.AutoSize = true;
            this.labelCameraPosition.Dock = System.Windows.Forms.DockStyle.Top;
            this.labelCameraPosition.Location = new System.Drawing.Point(3, 312);
            this.labelCameraPosition.Name = "labelCameraPosition";
            this.labelCameraPosition.Size = new System.Drawing.Size(62, 17);
            this.labelCameraPosition.TabIndex = 6;
            this.labelCameraPosition.Text = "Position:";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Dock = System.Windows.Forms.DockStyle.Top;
            this.label5.Location = new System.Drawing.Point(3, 295);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(12, 17);
            this.label5.TabIndex = 5;
            this.label5.Text = " ";
            // 
            // buttonAddCamera
            // 
            this.buttonAddCamera.Dock = System.Windows.Forms.DockStyle.Top;
            this.buttonAddCamera.Location = new System.Drawing.Point(3, 267);
            this.buttonAddCamera.Name = "buttonAddCamera";
            this.buttonAddCamera.Size = new System.Drawing.Size(265, 28);
            this.buttonAddCamera.TabIndex = 4;
            this.buttonAddCamera.Text = "Add";
            this.buttonAddCamera.UseVisualStyleBackColor = true;
            this.buttonAddCamera.Click += new System.EventHandler(this.buttonAddCamera_Click);
            // 
            // buttonDeleteCamera
            // 
            this.buttonDeleteCamera.Dock = System.Windows.Forms.DockStyle.Top;
            this.buttonDeleteCamera.Location = new System.Drawing.Point(3, 239);
            this.buttonDeleteCamera.Name = "buttonDeleteCamera";
            this.buttonDeleteCamera.Size = new System.Drawing.Size(265, 28);
            this.buttonDeleteCamera.TabIndex = 3;
            this.buttonDeleteCamera.Text = "Remove";
            this.buttonDeleteCamera.UseVisualStyleBackColor = true;
            this.buttonDeleteCamera.Click += new System.EventHandler(this.buttonDeleteCamera_Click);
            // 
            // buttonCameraDown
            // 
            this.buttonCameraDown.Dock = System.Windows.Forms.DockStyle.Top;
            this.buttonCameraDown.Location = new System.Drawing.Point(3, 211);
            this.buttonCameraDown.Name = "buttonCameraDown";
            this.buttonCameraDown.Size = new System.Drawing.Size(265, 28);
            this.buttonCameraDown.TabIndex = 2;
            this.buttonCameraDown.Text = "Down";
            this.buttonCameraDown.UseVisualStyleBackColor = true;
            this.buttonCameraDown.Click += new System.EventHandler(this.buttonCameraDown_Click);
            // 
            // buttonCameraUp
            // 
            this.buttonCameraUp.Dock = System.Windows.Forms.DockStyle.Top;
            this.buttonCameraUp.Location = new System.Drawing.Point(3, 183);
            this.buttonCameraUp.Name = "buttonCameraUp";
            this.buttonCameraUp.Size = new System.Drawing.Size(265, 28);
            this.buttonCameraUp.TabIndex = 1;
            this.buttonCameraUp.Text = "Up";
            this.buttonCameraUp.UseVisualStyleBackColor = true;
            this.buttonCameraUp.Click += new System.EventHandler(this.buttonCameraUp_Click);
            // 
            // listBoxCameras
            // 
            this.listBoxCameras.Dock = System.Windows.Forms.DockStyle.Top;
            this.listBoxCameras.FormattingEnabled = true;
            this.listBoxCameras.ItemHeight = 16;
            this.listBoxCameras.Location = new System.Drawing.Point(3, 3);
            this.listBoxCameras.Name = "listBoxCameras";
            this.listBoxCameras.Size = new System.Drawing.Size(265, 180);
            this.listBoxCameras.TabIndex = 0;
            this.listBoxCameras.SelectedIndexChanged += new System.EventHandler(this.listBoxCameras_SelectedIndexChanged);
            this.listBoxCameras.KeyDown += new System.Windows.Forms.KeyEventHandler(this.listBoxCameras_KeyDown);
            // 
            // openTextureDialog
            // 
            this.openTextureDialog.Filter = "Textura fotográfica|*.*";
            this.openTextureDialog.SupportMultiDottedExtensions = true;
            this.openTextureDialog.Title = "Seleccione la textura";
            // 
            // openHeightmapDialog
            // 
            this.openHeightmapDialog.Filter = "Archivo ráster|*.*|Archivo de elevación SRTM|*.hgt";
            this.openHeightmapDialog.Title = "Seleccionar un mapa de altura";
            // 
            // ExplorerForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(279, 741);
            this.CloseButton = false;
            this.CloseButtonVisible = false;
            this.Controls.Add(this.tabControl);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimumSize = new System.Drawing.Size(18, 256);
            this.Name = "ExplorerForm";
            this.Text = "Project Explorer";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ExplorerForm_FormClosing);
            this.tabControl.ResumeLayout(false);
            this.tabPageTiles.ResumeLayout(false);
            this.groupBoxTile.ResumeLayout(false);
            this.groupBoxTile.PerformLayout();
            this.tabPageCameras.ResumeLayout(false);
            this.tabPageCameras.PerformLayout();
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel2.PerformLayout();
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl;
        private System.Windows.Forms.TabPage tabPageTiles;
        private System.Windows.Forms.TabPage tabPageCameras;
        private System.Windows.Forms.ListBox listBoxTiles;
        private System.Windows.Forms.Button buttonDeleteTile;
        private System.Windows.Forms.Button buttonAddTile;
        private System.Windows.Forms.ListBox listBoxCameras;
        private System.Windows.Forms.Button buttonCameraDown;
        private System.Windows.Forms.Button buttonCameraUp;
        private System.Windows.Forms.Button buttonAddCamera;
        private System.Windows.Forms.Button buttonDeleteCamera;
        private System.Windows.Forms.GroupBox groupBoxTile;
        private System.Windows.Forms.TextBox textBoxLongitude;
        private System.Windows.Forms.Label labelLongitude;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBoxLatitude;
        private System.Windows.Forms.Label labelLatitude;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button buttonHeightmap;
        private System.Windows.Forms.Label labelHeightmap;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button buttonTexture;
        private System.Windows.Forms.Label labelTexture;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label labelCameraPosition;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.TextBox textBoxCameraPositionX;
        private System.Windows.Forms.Label labelCameraRotation;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox textBoxCameraPositionZ;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.TextBox textBoxCameraRotationX;
        private System.Windows.Forms.OpenFileDialog openTextureDialog;
        private System.Windows.Forms.OpenFileDialog openHeightmapDialog;

    }
}