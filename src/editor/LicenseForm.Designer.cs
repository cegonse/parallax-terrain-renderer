namespace TerrenosHelper
{
    partial class LicenseForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(LicenseForm));
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPageGpl = new System.Windows.Forms.TabPage();
            this.textBoxGpl = new System.Windows.Forms.TextBox();
            this.tabPageLgpl = new System.Windows.Forms.TabPage();
            this.textBoxLgpl = new System.Windows.Forms.TextBox();
            this.tabPageOpentk = new System.Windows.Forms.TabPage();
            this.textBoxTk = new System.Windows.Forms.TextBox();
            this.tabPageBsd = new System.Windows.Forms.TabPage();
            this.textBoxBsd = new System.Windows.Forms.TextBox();
            this.tabPageMit = new System.Windows.Forms.TabPage();
            this.textBoxMit = new System.Windows.Forms.TextBox();
            this.buttonClose = new System.Windows.Forms.Button();
            this.tabControl1.SuspendLayout();
            this.tabPageGpl.SuspendLayout();
            this.tabPageLgpl.SuspendLayout();
            this.tabPageOpentk.SuspendLayout();
            this.tabPageBsd.SuspendLayout();
            this.tabPageMit.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPageGpl);
            this.tabControl1.Controls.Add(this.tabPageLgpl);
            this.tabControl1.Controls.Add(this.tabPageOpentk);
            this.tabControl1.Controls.Add(this.tabPageBsd);
            this.tabControl1.Controls.Add(this.tabPageMit);
            this.tabControl1.Location = new System.Drawing.Point(14, 13);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(509, 510);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPageGpl
            // 
            this.tabPageGpl.Controls.Add(this.textBoxGpl);
            this.tabPageGpl.ForeColor = System.Drawing.Color.Coral;
            this.tabPageGpl.Location = new System.Drawing.Point(4, 25);
            this.tabPageGpl.Name = "tabPageGpl";
            this.tabPageGpl.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageGpl.Size = new System.Drawing.Size(501, 481);
            this.tabPageGpl.TabIndex = 0;
            this.tabPageGpl.Text = "GPL V3";
            this.tabPageGpl.UseVisualStyleBackColor = true;
            // 
            // textBoxGpl
            // 
            this.textBoxGpl.Location = new System.Drawing.Point(6, 6);
            this.textBoxGpl.Multiline = true;
            this.textBoxGpl.Name = "textBoxGpl";
            this.textBoxGpl.ReadOnly = true;
            this.textBoxGpl.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBoxGpl.Size = new System.Drawing.Size(490, 468);
            this.textBoxGpl.TabIndex = 0;
            this.textBoxGpl.Text = resources.GetString("textBoxGpl.Text");
            // 
            // tabPageLgpl
            // 
            this.tabPageLgpl.Controls.Add(this.textBoxLgpl);
            this.tabPageLgpl.Location = new System.Drawing.Point(4, 25);
            this.tabPageLgpl.Name = "tabPageLgpl";
            this.tabPageLgpl.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageLgpl.Size = new System.Drawing.Size(501, 481);
            this.tabPageLgpl.TabIndex = 1;
            this.tabPageLgpl.Text = "LGPL V3";
            this.tabPageLgpl.UseVisualStyleBackColor = true;
            // 
            // textBoxLgpl
            // 
            this.textBoxLgpl.Location = new System.Drawing.Point(5, 6);
            this.textBoxLgpl.Multiline = true;
            this.textBoxLgpl.Name = "textBoxLgpl";
            this.textBoxLgpl.ReadOnly = true;
            this.textBoxLgpl.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBoxLgpl.Size = new System.Drawing.Size(490, 468);
            this.textBoxLgpl.TabIndex = 1;
            this.textBoxLgpl.Text = resources.GetString("textBoxLgpl.Text");
            // 
            // tabPageOpentk
            // 
            this.tabPageOpentk.Controls.Add(this.textBoxTk);
            this.tabPageOpentk.Location = new System.Drawing.Point(4, 25);
            this.tabPageOpentk.Name = "tabPageOpentk";
            this.tabPageOpentk.Size = new System.Drawing.Size(501, 481);
            this.tabPageOpentk.TabIndex = 2;
            this.tabPageOpentk.Text = "OpenTK";
            this.tabPageOpentk.UseVisualStyleBackColor = true;
            // 
            // textBoxTk
            // 
            this.textBoxTk.Location = new System.Drawing.Point(5, 6);
            this.textBoxTk.Multiline = true;
            this.textBoxTk.Name = "textBoxTk";
            this.textBoxTk.ReadOnly = true;
            this.textBoxTk.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBoxTk.Size = new System.Drawing.Size(490, 468);
            this.textBoxTk.TabIndex = 2;
            this.textBoxTk.Text = resources.GetString("textBoxTk.Text");
            // 
            // tabPageBsd
            // 
            this.tabPageBsd.Controls.Add(this.textBoxBsd);
            this.tabPageBsd.Location = new System.Drawing.Point(4, 25);
            this.tabPageBsd.Name = "tabPageBsd";
            this.tabPageBsd.Size = new System.Drawing.Size(501, 481);
            this.tabPageBsd.TabIndex = 3;
            this.tabPageBsd.Text = "BSD (LibTIFF)";
            this.tabPageBsd.UseVisualStyleBackColor = true;
            // 
            // textBoxBsd
            // 
            this.textBoxBsd.Location = new System.Drawing.Point(5, 6);
            this.textBoxBsd.Multiline = true;
            this.textBoxBsd.Name = "textBoxBsd";
            this.textBoxBsd.ReadOnly = true;
            this.textBoxBsd.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBoxBsd.Size = new System.Drawing.Size(490, 468);
            this.textBoxBsd.TabIndex = 3;
            this.textBoxBsd.Text = resources.GetString("textBoxBsd.Text");
            // 
            // tabPageMit
            // 
            this.tabPageMit.Controls.Add(this.textBoxMit);
            this.tabPageMit.Location = new System.Drawing.Point(4, 25);
            this.tabPageMit.Name = "tabPageMit";
            this.tabPageMit.Size = new System.Drawing.Size(501, 481);
            this.tabPageMit.TabIndex = 4;
            this.tabPageMit.Text = "MIT (WinForms Dock)";
            this.tabPageMit.UseVisualStyleBackColor = true;
            // 
            // textBoxMit
            // 
            this.textBoxMit.Location = new System.Drawing.Point(5, 6);
            this.textBoxMit.Multiline = true;
            this.textBoxMit.Name = "textBoxMit";
            this.textBoxMit.ReadOnly = true;
            this.textBoxMit.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.textBoxMit.Size = new System.Drawing.Size(490, 468);
            this.textBoxMit.TabIndex = 4;
            this.textBoxMit.Text = resources.GetString("textBoxMit.Text");
            // 
            // buttonClose
            // 
            this.buttonClose.Location = new System.Drawing.Point(412, 529);
            this.buttonClose.Name = "buttonClose";
            this.buttonClose.Size = new System.Drawing.Size(111, 32);
            this.buttonClose.TabIndex = 1;
            this.buttonClose.Text = "Close";
            this.buttonClose.UseVisualStyleBackColor = true;
            this.buttonClose.Click += new System.EventHandler(this.button1_Click);
            // 
            // LicenseForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(547, 611);
            this.ControlBox = false;
            this.Controls.Add(this.buttonClose);
            this.Controls.Add(this.tabControl1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "LicenseForm";
            this.Text = "License";
            this.tabControl1.ResumeLayout(false);
            this.tabPageGpl.ResumeLayout(false);
            this.tabPageGpl.PerformLayout();
            this.tabPageLgpl.ResumeLayout(false);
            this.tabPageLgpl.PerformLayout();
            this.tabPageOpentk.ResumeLayout(false);
            this.tabPageOpentk.PerformLayout();
            this.tabPageBsd.ResumeLayout(false);
            this.tabPageBsd.PerformLayout();
            this.tabPageMit.ResumeLayout(false);
            this.tabPageMit.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPageGpl;
        private System.Windows.Forms.TabPage tabPageLgpl;
        private System.Windows.Forms.TabPage tabPageOpentk;
        private System.Windows.Forms.TabPage tabPageBsd;
        private System.Windows.Forms.Button buttonClose;
        private System.Windows.Forms.TextBox textBoxGpl;
        private System.Windows.Forms.TextBox textBoxLgpl;
        private System.Windows.Forms.TextBox textBoxTk;
        private System.Windows.Forms.TextBox textBoxBsd;
        private System.Windows.Forms.TabPage tabPageMit;
        private System.Windows.Forms.TextBox textBoxMit;
    }
}