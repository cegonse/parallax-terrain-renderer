namespace TerrenosHelper
{
    partial class IberpixDownloadProgressForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(IberpixDownloadProgressForm));
            this.labelDownloading = new System.Windows.Forms.Label();
            this.progressBarDownload = new System.Windows.Forms.ProgressBar();
            this.labelProgress = new System.Windows.Forms.Label();
            this.labelTotal = new System.Windows.Forms.Label();
            this.progressBarTotal = new System.Windows.Forms.ProgressBar();
            this.labelTotalProgress = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // labelDownloading
            // 
            this.labelDownloading.AutoSize = true;
            this.labelDownloading.Location = new System.Drawing.Point(12, 9);
            this.labelDownloading.Name = "labelDownloading";
            this.labelDownloading.Size = new System.Drawing.Size(115, 17);
            this.labelDownloading.TabIndex = 0;
            this.labelDownloading.Text = "Downloading tile:";
            // 
            // progressBarDownload
            // 
            this.progressBarDownload.Location = new System.Drawing.Point(12, 38);
            this.progressBarDownload.Name = "progressBarDownload";
            this.progressBarDownload.Size = new System.Drawing.Size(341, 23);
            this.progressBarDownload.Step = 1;
            this.progressBarDownload.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.progressBarDownload.TabIndex = 1;
            // 
            // labelProgress
            // 
            this.labelProgress.AutoSize = true;
            this.labelProgress.Location = new System.Drawing.Point(12, 75);
            this.labelProgress.Name = "labelProgress";
            this.labelProgress.Size = new System.Drawing.Size(80, 17);
            this.labelProgress.TabIndex = 2;
            this.labelProgress.Text = "0 KB / 0 KB";
            // 
            // labelTotal
            // 
            this.labelTotal.AutoSize = true;
            this.labelTotal.Location = new System.Drawing.Point(12, 114);
            this.labelTotal.Name = "labelTotal";
            this.labelTotal.Size = new System.Drawing.Size(104, 17);
            this.labelTotal.TabIndex = 3;
            this.labelTotal.Text = "Total progress:";
            // 
            // progressBarTotal
            // 
            this.progressBarTotal.Location = new System.Drawing.Point(15, 143);
            this.progressBarTotal.Name = "progressBarTotal";
            this.progressBarTotal.Size = new System.Drawing.Size(338, 23);
            this.progressBarTotal.Step = 1;
            this.progressBarTotal.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.progressBarTotal.TabIndex = 4;
            // 
            // labelTotalProgress
            // 
            this.labelTotalProgress.AutoSize = true;
            this.labelTotalProgress.Location = new System.Drawing.Point(12, 179);
            this.labelTotalProgress.Name = "labelTotalProgress";
            this.labelTotalProgress.Size = new System.Drawing.Size(36, 17);
            this.labelTotalProgress.TabIndex = 5;
            this.labelTotalProgress.Text = "0 / 0";
            // 
            // IberpixDownloadProgressForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(368, 211);
            this.Controls.Add(this.labelTotalProgress);
            this.Controls.Add(this.progressBarTotal);
            this.Controls.Add(this.labelTotal);
            this.Controls.Add(this.labelProgress);
            this.Controls.Add(this.progressBarDownload);
            this.Controls.Add(this.labelDownloading);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "IberpixDownloadProgressForm";
            this.Text = "Downloading";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label labelDownloading;
        private System.Windows.Forms.ProgressBar progressBarDownload;
        private System.Windows.Forms.Label labelProgress;
        private System.Windows.Forms.Label labelTotal;
        private System.Windows.Forms.ProgressBar progressBarTotal;
        private System.Windows.Forms.Label labelTotalProgress;
    }
}