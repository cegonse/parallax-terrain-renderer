namespace TerrenosHelper
{
    partial class FormFileLoad
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
            this.pathLabel = new System.Windows.Forms.Label();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.sizeReadLabel = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // pathLabel
            // 
            this.pathLabel.AutoSize = true;
            this.pathLabel.Location = new System.Drawing.Point(12, 19);
            this.pathLabel.Name = "pathLabel";
            this.pathLabel.Size = new System.Drawing.Size(34, 17);
            this.pathLabel.TabIndex = 0;
            this.pathLabel.Text = "File:";
            // 
            // progressBar1
            // 
            this.progressBar1.Location = new System.Drawing.Point(12, 82);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(435, 23);
            this.progressBar1.TabIndex = 1;
            // 
            // sizeReadLabel
            // 
            this.sizeReadLabel.AutoSize = true;
            this.sizeReadLabel.Location = new System.Drawing.Point(12, 132);
            this.sizeReadLabel.Name = "sizeReadLabel";
            this.sizeReadLabel.Size = new System.Drawing.Size(80, 17);
            this.sizeReadLabel.TabIndex = 2;
            this.sizeReadLabel.Text = "0 KB / 0 KB";
            // 
            // FormFileLoad
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(459, 168);
            this.Controls.Add(this.sizeReadLabel);
            this.Controls.Add(this.progressBar1);
            this.Controls.Add(this.pathLabel);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "FormFileLoad";
            this.Text = "Loading file";
            this.Load += new System.EventHandler(this.FormFileLoad_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label pathLabel;
        private System.Windows.Forms.ProgressBar progressBar1;
        private System.Windows.Forms.Label sizeReadLabel;
    }
}