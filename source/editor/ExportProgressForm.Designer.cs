namespace TerrenosHelper
{
    partial class ExportProgressForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ExportProgressForm));
            this.labelActualFile = new System.Windows.Forms.Label();
            this.progressBar = new System.Windows.Forms.ProgressBar();
            this.labelCount = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // labelActualFile
            // 
            this.labelActualFile.AutoSize = true;
            this.labelActualFile.Location = new System.Drawing.Point(12, 18);
            this.labelActualFile.Name = "labelActualFile";
            this.labelActualFile.Size = new System.Drawing.Size(86, 17);
            this.labelActualFile.TabIndex = 0;
            this.labelActualFile.Text = "Writing file...";
            // 
            // progressBar
            // 
            this.progressBar.Location = new System.Drawing.Point(12, 47);
            this.progressBar.Name = "progressBar";
            this.progressBar.Size = new System.Drawing.Size(302, 23);
            this.progressBar.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.progressBar.TabIndex = 1;
            // 
            // labelCount
            // 
            this.labelCount.AutoSize = true;
            this.labelCount.Location = new System.Drawing.Point(12, 83);
            this.labelCount.Name = "labelCount";
            this.labelCount.Size = new System.Drawing.Size(36, 17);
            this.labelCount.TabIndex = 2;
            this.labelCount.Text = "0 / 0";
            // 
            // ExportProgressForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(330, 123);
            this.Controls.Add(this.labelCount);
            this.Controls.Add(this.progressBar);
            this.Controls.Add(this.labelActualFile);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ExportProgressForm";
            this.Text = "Building project";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label labelActualFile;
        private System.Windows.Forms.ProgressBar progressBar;
        private System.Windows.Forms.Label labelCount;
    }
}