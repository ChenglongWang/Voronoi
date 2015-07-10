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
            this.numericKidneyValue = new System.Windows.Forms.NumericUpDown();
            ((System.ComponentModel.ISupportInitialize)(this.numericDepth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericKidneyValue)).BeginInit();
            this.SuspendLayout();
            // 
            // buttonCancel
            // 
            this.buttonCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.buttonCancel.Location = new System.Drawing.Point(64, 92);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(50, 23);
            this.buttonCancel.TabIndex = 21;
            this.buttonCancel.Text = "Cancel";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler(this.OnCancel);
            // 
            // buttonOK
            // 
            this.buttonOK.Location = new System.Drawing.Point(16, 92);
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
            this.label.Location = new System.Drawing.Point(11, 47);
            this.label.Name = "label";
            this.label.Size = new System.Drawing.Size(37, 12);
            this.label.TabIndex = 20;
            this.label.Text = "Depth:";
            // 
            // numericDepth
            // 
            this.numericDepth.Location = new System.Drawing.Point(54, 43);
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
            this.checkBoxRoot.Location = new System.Drawing.Point(17, 68);
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
            // numericKidneyValue
            // 
            this.numericKidneyValue.Location = new System.Drawing.Point(54, 12);
            this.numericKidneyValue.Maximum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.numericKidneyValue.Name = "numericKidneyValue";
            this.numericKidneyValue.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.numericKidneyValue.Size = new System.Drawing.Size(51, 19);
            this.numericKidneyValue.TabIndex = 24;
            this.numericKidneyValue.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.numericKidneyValue.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // VoronoiPlugin
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(137, 127);
            this.ControlBox = false;
            this.Controls.Add(this.labelkidney);
            this.Controls.Add(this.numericKidneyValue);
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
            ((System.ComponentModel.ISupportInitialize)(this.numericKidneyValue)).EndInit();
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
        private System.Windows.Forms.NumericUpDown numericKidneyValue;
	}
}