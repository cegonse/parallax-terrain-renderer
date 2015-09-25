namespace TerrenosHelper
{
    partial class AboutForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AboutForm));
            this.labelAbout = new System.Windows.Forms.Label();
            this.linkLabelEmail = new System.Windows.Forms.LinkLabel();
            this.buttonAccept = new System.Windows.Forms.Button();
            this.buttonLicense = new System.Windows.Forms.Button();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // labelAbout
            // 
            this.labelAbout.AutoSize = true;
            this.labelAbout.Location = new System.Drawing.Point(159, 12);
            this.labelAbout.Name = "labelAbout";
            this.labelAbout.Size = new System.Drawing.Size(268, 204);
            this.labelAbout.TabIndex = 0;
            this.labelAbout.Text = resources.GetString("labelAbout.Text");
            // 
            // linkLabelEmail
            // 
            this.linkLabelEmail.AutoSize = true;
            this.linkLabelEmail.Location = new System.Drawing.Point(159, 229);
            this.linkLabelEmail.Name = "linkLabelEmail";
            this.linkLabelEmail.Size = new System.Drawing.Size(155, 17);
            this.linkLabelEmail.TabIndex = 1;
            this.linkLabelEmail.TabStop = true;
            this.linkLabelEmail.Text = "cegonse@alumni.uv.es";
            this.linkLabelEmail.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.linkLabelEmail_LinkClicked);
            // 
            // buttonAccept
            // 
            this.buttonAccept.Location = new System.Drawing.Point(346, 260);
            this.buttonAccept.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.buttonAccept.Name = "buttonAccept";
            this.buttonAccept.Size = new System.Drawing.Size(75, 27);
            this.buttonAccept.TabIndex = 4;
            this.buttonAccept.Text = "Close";
            this.buttonAccept.UseVisualStyleBackColor = true;
            this.buttonAccept.Click += new System.EventHandler(this.buttonAccept_Click);
            // 
            // buttonLicense
            // 
            this.buttonLicense.Location = new System.Drawing.Point(264, 260);
            this.buttonLicense.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.buttonLicense.Name = "buttonLicense";
            this.buttonLicense.Size = new System.Drawing.Size(75, 27);
            this.buttonLicense.TabIndex = 5;
            this.buttonLicense.Text = "License";
            this.buttonLicense.UseVisualStyleBackColor = true;
            this.buttonLicense.Click += new System.EventHandler(this.buttonLicense_Click);
            // 
            // pictureBox2
            // 
            this.pictureBox2.Image = global::TerrenosHelper.Properties.Resources.logo_etse;
            this.pictureBox2.Location = new System.Drawing.Point(12, 146);
            this.pictureBox2.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(128, 80);
            this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox2.TabIndex = 3;
            this.pictureBox2.TabStop = false;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = global::TerrenosHelper.Properties.Resources.logo_uv;
            this.pictureBox1.Location = new System.Drawing.Point(12, 12);
            this.pictureBox1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(128, 128);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox1.TabIndex = 2;
            this.pictureBox1.TabStop = false;
            // 
            // AboutForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(436, 299);
            this.Controls.Add(this.buttonLicense);
            this.Controls.Add(this.buttonAccept);
            this.Controls.Add(this.pictureBox2);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.linkLabelEmail);
            this.Controls.Add(this.labelAbout);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "AboutForm";
            this.Text = "About";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label labelAbout;
        private System.Windows.Forms.LinkLabel linkLabelEmail;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.Button buttonAccept;
        private System.Windows.Forms.Button buttonLicense;

    }
}