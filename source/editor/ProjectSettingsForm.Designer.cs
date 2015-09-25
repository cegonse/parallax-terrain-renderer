namespace TerrenosHelper
{
    partial class ProjectSettingsForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ProjectSettingsForm));
            this.panel1 = new System.Windows.Forms.Panel();
            this.textBoxResolution = new System.Windows.Forms.TextBox();
            this.labelResolution = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.groupBoxCamera = new System.Windows.Forms.GroupBox();
            this.textBoxHeight = new System.Windows.Forms.TextBox();
            this.labelHeight = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.radioButtonRelativeHeight = new System.Windows.Forms.RadioButton();
            this.radioButtonConstantHeight = new System.Windows.Forms.RadioButton();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.textBoxAuthor = new System.Windows.Forms.TextBox();
            this.labelAuthor = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.textBoxName = new System.Windows.Forms.TextBox();
            this.labelName = new System.Windows.Forms.Label();
            this.panel1.SuspendLayout();
            this.groupBoxCamera.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.textBoxResolution);
            this.panel1.Controls.Add(this.labelResolution);
            this.panel1.Controls.Add(this.label5);
            this.panel1.Controls.Add(this.groupBoxCamera);
            this.panel1.Controls.Add(this.label2);
            this.panel1.Controls.Add(this.textBoxAuthor);
            this.panel1.Controls.Add(this.labelAuthor);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.textBoxName);
            this.panel1.Controls.Add(this.labelName);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Padding = new System.Windows.Forms.Padding(10);
            this.panel1.Size = new System.Drawing.Size(334, 453);
            this.panel1.TabIndex = 0;
            // 
            // textBoxResolution
            // 
            this.textBoxResolution.Dock = System.Windows.Forms.DockStyle.Top;
            this.textBoxResolution.Location = new System.Drawing.Point(10, 303);
            this.textBoxResolution.Name = "textBoxResolution";
            this.textBoxResolution.Size = new System.Drawing.Size(314, 22);
            this.textBoxResolution.TabIndex = 9;
            this.textBoxResolution.TextChanged += new System.EventHandler(this.textBoxResolution_TextChanged);
            // 
            // labelResolution
            // 
            this.labelResolution.AutoSize = true;
            this.labelResolution.Dock = System.Windows.Forms.DockStyle.Top;
            this.labelResolution.Location = new System.Drawing.Point(10, 286);
            this.labelResolution.Name = "labelResolution";
            this.labelResolution.Size = new System.Drawing.Size(221, 17);
            this.labelResolution.TabIndex = 8;
            this.labelResolution.Text = "Sample count (screenshot count):";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Dock = System.Windows.Forms.DockStyle.Top;
            this.label5.Location = new System.Drawing.Point(10, 269);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(16, 17);
            this.label5.TabIndex = 7;
            this.label5.Text = "  ";
            // 
            // groupBoxCamera
            // 
            this.groupBoxCamera.Controls.Add(this.textBoxHeight);
            this.groupBoxCamera.Controls.Add(this.labelHeight);
            this.groupBoxCamera.Controls.Add(this.label4);
            this.groupBoxCamera.Controls.Add(this.radioButtonRelativeHeight);
            this.groupBoxCamera.Controls.Add(this.radioButtonConstantHeight);
            this.groupBoxCamera.Controls.Add(this.label3);
            this.groupBoxCamera.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupBoxCamera.Location = new System.Drawing.Point(10, 122);
            this.groupBoxCamera.Name = "groupBoxCamera";
            this.groupBoxCamera.Size = new System.Drawing.Size(314, 147);
            this.groupBoxCamera.TabIndex = 6;
            this.groupBoxCamera.TabStop = false;
            this.groupBoxCamera.Text = "Camera";
            // 
            // textBoxHeight
            // 
            this.textBoxHeight.Dock = System.Windows.Forms.DockStyle.Top;
            this.textBoxHeight.Location = new System.Drawing.Point(3, 111);
            this.textBoxHeight.Name = "textBoxHeight";
            this.textBoxHeight.Size = new System.Drawing.Size(308, 22);
            this.textBoxHeight.TabIndex = 5;
            this.textBoxHeight.TextChanged += new System.EventHandler(this.textBoxHeight_TextChanged);
            // 
            // labelHeight
            // 
            this.labelHeight.AutoSize = true;
            this.labelHeight.Dock = System.Windows.Forms.DockStyle.Top;
            this.labelHeight.Location = new System.Drawing.Point(3, 94);
            this.labelHeight.Name = "labelHeight";
            this.labelHeight.Size = new System.Drawing.Size(53, 17);
            this.labelHeight.TabIndex = 4;
            this.labelHeight.Text = "Height:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Dock = System.Windows.Forms.DockStyle.Top;
            this.label4.Location = new System.Drawing.Point(3, 77);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(12, 17);
            this.label4.TabIndex = 3;
            this.label4.Text = " ";
            // 
            // radioButtonRelativeHeight
            // 
            this.radioButtonRelativeHeight.AutoSize = true;
            this.radioButtonRelativeHeight.Dock = System.Windows.Forms.DockStyle.Top;
            this.radioButtonRelativeHeight.Location = new System.Drawing.Point(3, 56);
            this.radioButtonRelativeHeight.Name = "radioButtonRelativeHeight";
            this.radioButtonRelativeHeight.Size = new System.Drawing.Size(308, 21);
            this.radioButtonRelativeHeight.TabIndex = 2;
            this.radioButtonRelativeHeight.TabStop = true;
            this.radioButtonRelativeHeight.Text = "Altura relativa";
            this.radioButtonRelativeHeight.UseVisualStyleBackColor = true;
            this.radioButtonRelativeHeight.Visible = false;
            this.radioButtonRelativeHeight.CheckedChanged += new System.EventHandler(this.radioButtonRelativeHeight_CheckedChanged);
            // 
            // radioButtonConstantHeight
            // 
            this.radioButtonConstantHeight.AutoSize = true;
            this.radioButtonConstantHeight.Dock = System.Windows.Forms.DockStyle.Top;
            this.radioButtonConstantHeight.Location = new System.Drawing.Point(3, 35);
            this.radioButtonConstantHeight.Name = "radioButtonConstantHeight";
            this.radioButtonConstantHeight.Size = new System.Drawing.Size(308, 21);
            this.radioButtonConstantHeight.TabIndex = 1;
            this.radioButtonConstantHeight.TabStop = true;
            this.radioButtonConstantHeight.Text = "Altura constante";
            this.radioButtonConstantHeight.UseVisualStyleBackColor = true;
            this.radioButtonConstantHeight.Visible = false;
            this.radioButtonConstantHeight.CheckedChanged += new System.EventHandler(this.radioButtonConstantHeight_CheckedChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Dock = System.Windows.Forms.DockStyle.Top;
            this.label3.Location = new System.Drawing.Point(3, 18);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(0, 17);
            this.label3.TabIndex = 0;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Dock = System.Windows.Forms.DockStyle.Top;
            this.label2.Location = new System.Drawing.Point(10, 105);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(0, 17);
            this.label2.TabIndex = 5;
            // 
            // textBoxAuthor
            // 
            this.textBoxAuthor.Dock = System.Windows.Forms.DockStyle.Top;
            this.textBoxAuthor.Location = new System.Drawing.Point(10, 83);
            this.textBoxAuthor.Name = "textBoxAuthor";
            this.textBoxAuthor.Size = new System.Drawing.Size(314, 22);
            this.textBoxAuthor.TabIndex = 4;
            this.textBoxAuthor.TextChanged += new System.EventHandler(this.textBoxAuthor_TextChanged);
            // 
            // labelAuthor
            // 
            this.labelAuthor.AutoSize = true;
            this.labelAuthor.Dock = System.Windows.Forms.DockStyle.Top;
            this.labelAuthor.Location = new System.Drawing.Point(10, 66);
            this.labelAuthor.Name = "labelAuthor";
            this.labelAuthor.Size = new System.Drawing.Size(101, 17);
            this.labelAuthor.TabIndex = 3;
            this.labelAuthor.Text = "Project author:";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Dock = System.Windows.Forms.DockStyle.Top;
            this.label1.Location = new System.Drawing.Point(10, 49);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(0, 17);
            this.label1.TabIndex = 2;
            // 
            // textBoxName
            // 
            this.textBoxName.Dock = System.Windows.Forms.DockStyle.Top;
            this.textBoxName.Location = new System.Drawing.Point(10, 27);
            this.textBoxName.Name = "textBoxName";
            this.textBoxName.Size = new System.Drawing.Size(314, 22);
            this.textBoxName.TabIndex = 1;
            this.textBoxName.TextChanged += new System.EventHandler(this.textBoxName_TextChanged);
            // 
            // labelName
            // 
            this.labelName.AutoSize = true;
            this.labelName.Dock = System.Windows.Forms.DockStyle.Top;
            this.labelName.Location = new System.Drawing.Point(10, 10);
            this.labelName.Name = "labelName";
            this.labelName.Size = new System.Drawing.Size(95, 17);
            this.labelName.TabIndex = 0;
            this.labelName.Text = "Project name:";
            // 
            // ProjectSettingsForm
            // 
            this.AllowEndUserDocking = false;
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(334, 453);
            this.CloseButton = false;
            this.CloseButtonVisible = false;
            this.Controls.Add(this.panel1);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ProjectSettingsForm";
            this.ShowHint = WeifenLuo.WinFormsUI.Docking.DockState.DockRightAutoHide;
            this.Text = "Settings";
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.groupBoxCamera.ResumeLayout(false);
            this.groupBoxCamera.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.TextBox textBoxAuthor;
        private System.Windows.Forms.Label labelAuthor;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBoxName;
        private System.Windows.Forms.Label labelName;
        private System.Windows.Forms.GroupBox groupBoxCamera;
        private System.Windows.Forms.TextBox textBoxHeight;
        private System.Windows.Forms.Label labelHeight;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.RadioButton radioButtonRelativeHeight;
        private System.Windows.Forms.RadioButton radioButtonConstantHeight;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBoxResolution;
        private System.Windows.Forms.Label labelResolution;
        private System.Windows.Forms.Label label5;
    }
}