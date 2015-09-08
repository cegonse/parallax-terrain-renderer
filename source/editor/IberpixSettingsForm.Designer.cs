namespace TerrenosHelper
{
    partial class IberpixSettingsForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(IberpixSettingsForm));
            this.labelLayer = new System.Windows.Forms.Label();
            this.comboBoxLayer = new System.Windows.Forms.ComboBox();
            this.labelUtm = new System.Windows.Forms.Label();
            this.textBoxUtm = new System.Windows.Forms.TextBox();
            this.labelResolution = new System.Windows.Forms.Label();
            this.textBoxResolution = new System.Windows.Forms.TextBox();
            this.labelMpix = new System.Windows.Forms.Label();
            this.labelicoord = new System.Windows.Forms.Label();
            this.textBoxStartI = new System.Windows.Forms.TextBox();
            this.labelStartI = new System.Windows.Forms.Label();
            this.labelStartJ = new System.Windows.Forms.Label();
            this.textBoxStartJ = new System.Windows.Forms.TextBox();
            this.labelDirectory = new System.Windows.Forms.Label();
            this.buttonBrowse = new System.Windows.Forms.Button();
            this.buttonDownload = new System.Windows.Forms.Button();
            this.folderBrowserDialog = new System.Windows.Forms.FolderBrowserDialog();
            this.labelEndI = new System.Windows.Forms.Label();
            this.textBoxEndI = new System.Windows.Forms.TextBox();
            this.labelEndJ = new System.Windows.Forms.Label();
            this.textBoxEndJ = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // labelLayer
            // 
            this.labelLayer.AutoSize = true;
            this.labelLayer.Location = new System.Drawing.Point(12, 9);
            this.labelLayer.Name = "labelLayer";
            this.labelLayer.Size = new System.Drawing.Size(44, 17);
            this.labelLayer.TabIndex = 0;
            this.labelLayer.Text = "Layer";
            // 
            // comboBoxLayer
            // 
            this.comboBoxLayer.FormattingEnabled = true;
            this.comboBoxLayer.Items.AddRange(new object[] {
            "SPOT5 Texture",
            "Heightmap"});
            this.comboBoxLayer.Location = new System.Drawing.Point(12, 29);
            this.comboBoxLayer.Name = "comboBoxLayer";
            this.comboBoxLayer.Size = new System.Drawing.Size(210, 24);
            this.comboBoxLayer.TabIndex = 1;
            // 
            // labelUtm
            // 
            this.labelUtm.AutoSize = true;
            this.labelUtm.Location = new System.Drawing.Point(12, 68);
            this.labelUtm.Name = "labelUtm";
            this.labelUtm.Size = new System.Drawing.Size(42, 17);
            this.labelUtm.TabIndex = 2;
            this.labelUtm.Text = "UTM:";
            // 
            // textBoxUtm
            // 
            this.textBoxUtm.Location = new System.Drawing.Point(12, 88);
            this.textBoxUtm.Name = "textBoxUtm";
            this.textBoxUtm.Size = new System.Drawing.Size(210, 22);
            this.textBoxUtm.TabIndex = 3;
            this.textBoxUtm.Text = "30";
            // 
            // labelResolution
            // 
            this.labelResolution.AutoSize = true;
            this.labelResolution.Location = new System.Drawing.Point(12, 127);
            this.labelResolution.Name = "labelResolution";
            this.labelResolution.Size = new System.Drawing.Size(79, 17);
            this.labelResolution.TabIndex = 4;
            this.labelResolution.Text = "Resolution:";
            // 
            // textBoxResolution
            // 
            this.textBoxResolution.Location = new System.Drawing.Point(12, 147);
            this.textBoxResolution.Name = "textBoxResolution";
            this.textBoxResolution.Size = new System.Drawing.Size(158, 22);
            this.textBoxResolution.TabIndex = 5;
            this.textBoxResolution.Text = "4";
            // 
            // labelMpix
            // 
            this.labelMpix.AutoSize = true;
            this.labelMpix.Location = new System.Drawing.Point(176, 150);
            this.labelMpix.Name = "labelMpix";
            this.labelMpix.Size = new System.Drawing.Size(48, 17);
            this.labelMpix.TabIndex = 6;
            this.labelMpix.Text = "m / pix";
            // 
            // labelicoord
            // 
            this.labelicoord.AutoSize = true;
            this.labelicoord.Location = new System.Drawing.Point(12, 186);
            this.labelicoord.Name = "labelicoord";
            this.labelicoord.Size = new System.Drawing.Size(88, 17);
            this.labelicoord.TabIndex = 7;
            this.labelicoord.Text = "Coordinates:";
            // 
            // textBoxStartI
            // 
            this.textBoxStartI.Location = new System.Drawing.Point(33, 211);
            this.textBoxStartI.Name = "textBoxStartI";
            this.textBoxStartI.Size = new System.Drawing.Size(58, 22);
            this.textBoxStartI.TabIndex = 8;
            // 
            // labelStartI
            // 
            this.labelStartI.AutoSize = true;
            this.labelStartI.Location = new System.Drawing.Point(12, 214);
            this.labelStartI.Name = "labelStartI";
            this.labelStartI.Size = new System.Drawing.Size(15, 17);
            this.labelStartI.TabIndex = 9;
            this.labelStartI.Text = "i:";
            // 
            // labelStartJ
            // 
            this.labelStartJ.AutoSize = true;
            this.labelStartJ.Location = new System.Drawing.Point(112, 214);
            this.labelStartJ.Name = "labelStartJ";
            this.labelStartJ.Size = new System.Drawing.Size(15, 17);
            this.labelStartJ.TabIndex = 10;
            this.labelStartJ.Text = "j:";
            // 
            // textBoxStartJ
            // 
            this.textBoxStartJ.Location = new System.Drawing.Point(133, 211);
            this.textBoxStartJ.Name = "textBoxStartJ";
            this.textBoxStartJ.Size = new System.Drawing.Size(58, 22);
            this.textBoxStartJ.TabIndex = 11;
            // 
            // labelDirectory
            // 
            this.labelDirectory.AutoSize = true;
            this.labelDirectory.Location = new System.Drawing.Point(12, 293);
            this.labelDirectory.Name = "labelDirectory";
            this.labelDirectory.Size = new System.Drawing.Size(123, 17);
            this.labelDirectory.TabIndex = 12;
            this.labelDirectory.Text = "Destination folder:";
            // 
            // buttonBrowse
            // 
            this.buttonBrowse.Location = new System.Drawing.Point(12, 313);
            this.buttonBrowse.Name = "buttonBrowse";
            this.buttonBrowse.Size = new System.Drawing.Size(210, 28);
            this.buttonBrowse.TabIndex = 13;
            this.buttonBrowse.Text = "Browse...";
            this.buttonBrowse.UseVisualStyleBackColor = true;
            this.buttonBrowse.Click += new System.EventHandler(this.buttonBrowse_Click);
            // 
            // buttonDownload
            // 
            this.buttonDownload.Location = new System.Drawing.Point(12, 358);
            this.buttonDownload.Name = "buttonDownload";
            this.buttonDownload.Size = new System.Drawing.Size(210, 28);
            this.buttonDownload.TabIndex = 14;
            this.buttonDownload.Text = "Download";
            this.buttonDownload.UseVisualStyleBackColor = true;
            this.buttonDownload.Click += new System.EventHandler(this.buttonDownload_Click);
            // 
            // folderBrowserDialog
            // 
            this.folderBrowserDialog.Description = "Carpeta destino:";
            this.folderBrowserDialog.RootFolder = System.Environment.SpecialFolder.MyDocuments;
            // 
            // labelEndI
            // 
            this.labelEndI.AutoSize = true;
            this.labelEndI.Location = new System.Drawing.Point(12, 251);
            this.labelEndI.Name = "labelEndI";
            this.labelEndI.Size = new System.Drawing.Size(15, 17);
            this.labelEndI.TabIndex = 15;
            this.labelEndI.Text = "i:";
            // 
            // textBoxEndI
            // 
            this.textBoxEndI.Location = new System.Drawing.Point(33, 248);
            this.textBoxEndI.Name = "textBoxEndI";
            this.textBoxEndI.Size = new System.Drawing.Size(58, 22);
            this.textBoxEndI.TabIndex = 16;
            // 
            // labelEndJ
            // 
            this.labelEndJ.AutoSize = true;
            this.labelEndJ.Location = new System.Drawing.Point(112, 251);
            this.labelEndJ.Name = "labelEndJ";
            this.labelEndJ.Size = new System.Drawing.Size(15, 17);
            this.labelEndJ.TabIndex = 17;
            this.labelEndJ.Text = "j:";
            // 
            // textBoxEndJ
            // 
            this.textBoxEndJ.Location = new System.Drawing.Point(133, 248);
            this.textBoxEndJ.Name = "textBoxEndJ";
            this.textBoxEndJ.Size = new System.Drawing.Size(58, 22);
            this.textBoxEndJ.TabIndex = 18;
            // 
            // IberpixSettingsForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(236, 396);
            this.Controls.Add(this.textBoxEndJ);
            this.Controls.Add(this.labelEndJ);
            this.Controls.Add(this.textBoxEndI);
            this.Controls.Add(this.labelEndI);
            this.Controls.Add(this.buttonDownload);
            this.Controls.Add(this.buttonBrowse);
            this.Controls.Add(this.labelDirectory);
            this.Controls.Add(this.textBoxStartJ);
            this.Controls.Add(this.labelStartJ);
            this.Controls.Add(this.labelStartI);
            this.Controls.Add(this.textBoxStartI);
            this.Controls.Add(this.labelicoord);
            this.Controls.Add(this.labelMpix);
            this.Controls.Add(this.textBoxResolution);
            this.Controls.Add(this.labelResolution);
            this.Controls.Add(this.textBoxUtm);
            this.Controls.Add(this.labelUtm);
            this.Controls.Add(this.comboBoxLayer);
            this.Controls.Add(this.labelLayer);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "IberpixSettingsForm";
            this.Text = "Settings";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label labelLayer;
        private System.Windows.Forms.ComboBox comboBoxLayer;
        private System.Windows.Forms.Label labelUtm;
        private System.Windows.Forms.TextBox textBoxUtm;
        private System.Windows.Forms.Label labelResolution;
        private System.Windows.Forms.TextBox textBoxResolution;
        private System.Windows.Forms.Label labelMpix;
        private System.Windows.Forms.Label labelicoord;
        private System.Windows.Forms.TextBox textBoxStartI;
        private System.Windows.Forms.Label labelStartI;
        private System.Windows.Forms.Label labelStartJ;
        private System.Windows.Forms.TextBox textBoxStartJ;
        private System.Windows.Forms.Label labelDirectory;
        private System.Windows.Forms.Button buttonBrowse;
        private System.Windows.Forms.Button buttonDownload;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog;
        private System.Windows.Forms.Label labelEndI;
        private System.Windows.Forms.TextBox textBoxEndI;
        private System.Windows.Forms.Label labelEndJ;
        private System.Windows.Forms.TextBox textBoxEndJ;

    }
}