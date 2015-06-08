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
            ((System.ComponentModel.ISupportInitialize)(this.numericDepth)).BeginInit();
            this.SuspendLayout();
            // 
            // buttonCancel
            // 
            this.buttonCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.buttonCancel.Location = new System.Drawing.Point(95, 44);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(81, 23);
            this.buttonCancel.TabIndex = 21;
            this.buttonCancel.Text = "キャンセル(&C)";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler(this.OnCancel);
            // 
            // buttonOK
            // 
            this.buttonOK.Location = new System.Drawing.Point(47, 44);
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
            this.label.Location = new System.Drawing.Point(8, 11);
            this.label.Name = "label";
            this.label.Size = new System.Drawing.Size(37, 12);
            this.label.TabIndex = 20;
            this.label.Text = "Depth:";
            // 
            // numericDepth
            // 
            this.numericDepth.Location = new System.Drawing.Point(56, 7);
            this.numericDepth.Maximum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.numericDepth.Name = "numericDepth";
            this.numericDepth.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.numericDepth.Size = new System.Drawing.Size(75, 19);
            this.numericDepth.TabIndex = 19;
            this.numericDepth.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.numericDepth.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // VoronoiPlugin
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(183, 70);
            this.ControlBox = false;
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
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button buttonCancel;
		private System.Windows.Forms.Button buttonOK;
		private System.Windows.Forms.Label label;
        private System.Windows.Forms.NumericUpDown numericDepth;
	}
}