namespace Plugin
{
	partial class VoronoiPlugin
	{
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		/// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
		protected override void Dispose( bool disposing )
		{
			if( disposing && ( components != null ) )
			{
				components.Dispose( );
			}
			base.Dispose( disposing );
		}

		#region Windows フォーム デザイナで生成されたコード

		/// <summary>
		/// デザイナ サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディタで変更しないでください。
		/// </summary>
		private void InitializeComponent( )
		{
            this.buttonCancel = new System.Windows.Forms.Button();
            this.buttonOK = new System.Windows.Forms.Button();
            this.label = new System.Windows.Forms.Label();
            this.numericDepth = new System.Windows.Forms.NumericUpDown();
            this.checkBoxRoot = new System.Windows.Forms.CheckBox();
            this.labelkidney = new System.Windows.Forms.Label();
            this.numericKidneyLabel = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.numericVesselLabel = new System.Windows.Forms.NumericUpDown();
            ((System.ComponentModel.ISupportInitialize)(this.numericDepth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericKidneyLabel)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericVesselLabel)).BeginInit();
            this.SuspendLayout();
            // 
            // buttonCancel
            // 
            this.buttonCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.buttonCancel.Location = new System.Drawing.Point(64, 126);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(50, 23);
            this.buttonCancel.TabIndex = 21;
            this.buttonCancel.Text = "Cancel";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler(this.OnCancel);
            // 
            // buttonOK
            // 
            this.buttonOK.Location = new System.Drawing.Point(16, 126);
            this.buttonOK.Name = "buttonOK";
            this.buttonOK.Size = new System.Drawing.Size(44, 23);
            this.buttonOK.TabIndex = 22;
            this.buttonOK.Text = "&OK";
            this.buttonOK.UseVisualStyleBackColor = true;
            this.buttonOK.Click += new System.EventHandler(this.OnOK);
            // 
            // label
            // 
            this.label.AutoSize = true;
            this.label.Location = new System.Drawing.Point(11, 68);
            this.label.Name = "label";
            this.label.Size = new System.Drawing.Size(37, 12);
            this.label.TabIndex = 20;
            this.label.Text = "Depth:";
            // 
            // numericDepth
            // 
            this.numericDepth.Location = new System.Drawing.Point(54, 64);
            this.numericDepth.Maximum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.numericDepth.Name = "numericDepth";
            this.numericDepth.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.numericDepth.Size = new System.Drawing.Size(51, 19);
            this.numericDepth.TabIndex = 19;
            this.numericDepth.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // checkBoxRoot
            // 
            this.checkBoxRoot.AutoSize = true;
            this.checkBoxRoot.Location = new System.Drawing.Point(17, 102);
            this.checkBoxRoot.Name = "checkBoxRoot";
            this.checkBoxRoot.Size = new System.Drawing.Size(88, 16);
            this.checkBoxRoot.TabIndex = 23;
            this.checkBoxRoot.Text = "Manual Root";
            this.checkBoxRoot.UseVisualStyleBackColor = true;
            // 
            // labelkidney
            // 
            this.labelkidney.AutoSize = true;
            this.labelkidney.Location = new System.Drawing.Point(11, 16);
            this.labelkidney.Name = "labelkidney";
            this.labelkidney.Size = new System.Drawing.Size(41, 12);
            this.labelkidney.TabIndex = 25;
            this.labelkidney.Text = "Kidney:";
            // 
            // numericKidneyLabel
            // 
            this.numericKidneyLabel.Location = new System.Drawing.Point(54, 12);
            this.numericKidneyLabel.Maximum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.numericKidneyLabel.Name = "numericKidneyLabel";
            this.numericKidneyLabel.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.numericKidneyLabel.Size = new System.Drawing.Size(51, 19);
            this.numericKidneyLabel.TabIndex = 24;
            this.numericKidneyLabel.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.numericKidneyLabel.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(11, 42);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(42, 12);
            this.label1.TabIndex = 27;
            this.label1.Text = "Vessel:";
            // 
            // numericVesselLabel
            // 
            this.numericVesselLabel.Location = new System.Drawing.Point(54, 38);
            this.numericVesselLabel.Maximum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.numericVesselLabel.Name = "numericVesselLabel";
            this.numericVesselLabel.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.numericVesselLabel.Size = new System.Drawing.Size(51, 19);
            this.numericVesselLabel.TabIndex = 26;
            this.numericVesselLabel.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.numericVesselLabel.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // VoronoiPlugin
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(135, 155);
            this.ControlBox = false;
            this.Controls.Add(this.label1);
            this.Controls.Add(this.numericVesselLabel);
            this.Controls.Add(this.labelkidney);
            this.Controls.Add(this.numericKidneyLabel);
            this.Controls.Add(this.checkBoxRoot);
            this.Controls.Add(this.buttonCancel);
            this.Controls.Add(this.buttonOK);
            this.Controls.Add(this.label);
            this.Controls.Add(this.numericDepth);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "VoronoiPlugin";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Voronoi";
            ((System.ComponentModel.ISupportInitialize)(this.numericDepth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericKidneyLabel)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericVesselLabel)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button buttonCancel;
		private System.Windows.Forms.Button buttonOK;
		private System.Windows.Forms.Label label;
        private System.Windows.Forms.NumericUpDown numericDepth;
        private System.Windows.Forms.CheckBox checkBoxRoot;
        private System.Windows.Forms.Label labelkidney;
        private System.Windows.Forms.NumericUpDown numericKidneyLabel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown numericVesselLabel;
	}
}