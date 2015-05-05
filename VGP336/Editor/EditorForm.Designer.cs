namespace Editor
{
    partial class EditorForm
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
            this.ViewPanel = new System.Windows.Forms.Panel();
            this.EditorTableLayout = new System.Windows.Forms.TableLayoutPanel();
            this.InspectorBox = new System.Windows.Forms.GroupBox();
            this.InspectorLayout = new System.Windows.Forms.FlowLayoutPanel();
            this.TransformBox = new System.Windows.Forms.GroupBox();
            this.InspectorTransformPositionLabel = new System.Windows.Forms.Label();
            this.TransformPositionXTextBox = new System.Windows.Forms.TextBox();
            this.TransformPositionXLabel = new System.Windows.Forms.Label();
            this.TransformPositionYLabel = new System.Windows.Forms.Label();
            this.TransformPositionYTextBox = new System.Windows.Forms.TextBox();
            this.TransformPositionZLabel = new System.Windows.Forms.Label();
            this.TransformPositionZTextBox = new System.Windows.Forms.TextBox();
            this.EditorTableLayout.SuspendLayout();
            this.InspectorBox.SuspendLayout();
            this.InspectorLayout.SuspendLayout();
            this.TransformBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // ViewPanel
            // 
            this.ViewPanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.ViewPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ViewPanel.Location = new System.Drawing.Point(87, 72);
            this.ViewPanel.Name = "ViewPanel";
            this.ViewPanel.Size = new System.Drawing.Size(843, 562);
            this.ViewPanel.TabIndex = 0;
            // 
            // EditorTableLayout
            // 
            this.EditorTableLayout.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.EditorTableLayout.AutoSize = true;
            this.EditorTableLayout.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.EditorTableLayout.ColumnCount = 3;
            this.EditorTableLayout.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 9.065156F));
            this.EditorTableLayout.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 90.93484F));
            this.EditorTableLayout.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 318F));
            this.EditorTableLayout.Controls.Add(this.ViewPanel, 1, 1);
            this.EditorTableLayout.Controls.Add(this.InspectorBox, 2, 1);
            this.EditorTableLayout.Location = new System.Drawing.Point(0, 0);
            this.EditorTableLayout.Name = "EditorTableLayout";
            this.EditorTableLayout.RowCount = 3;
            this.EditorTableLayout.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 10.95335F));
            this.EditorTableLayout.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 89.04665F));
            this.EditorTableLayout.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 169F));
            this.EditorTableLayout.Size = new System.Drawing.Size(1252, 807);
            this.EditorTableLayout.TabIndex = 1;
            // 
            // InspectorBox
            // 
            this.InspectorBox.Controls.Add(this.InspectorLayout);
            this.InspectorBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.InspectorBox.Location = new System.Drawing.Point(936, 72);
            this.InspectorBox.Name = "InspectorBox";
            this.InspectorBox.Size = new System.Drawing.Size(313, 562);
            this.InspectorBox.TabIndex = 1;
            this.InspectorBox.TabStop = false;
            this.InspectorBox.Text = "Inspector";
            // 
            // InspectorLayout
            // 
            this.InspectorLayout.AutoScroll = true;
            this.InspectorLayout.Controls.Add(this.TransformBox);
            this.InspectorLayout.Dock = System.Windows.Forms.DockStyle.Fill;
            this.InspectorLayout.Location = new System.Drawing.Point(3, 16);
            this.InspectorLayout.Name = "InspectorLayout";
            this.InspectorLayout.Size = new System.Drawing.Size(307, 543);
            this.InspectorLayout.TabIndex = 0;
            // 
            // TransformBox
            // 
            this.TransformBox.Controls.Add(this.TransformPositionZLabel);
            this.TransformBox.Controls.Add(this.TransformPositionZTextBox);
            this.TransformBox.Controls.Add(this.TransformPositionYLabel);
            this.TransformBox.Controls.Add(this.TransformPositionYTextBox);
            this.TransformBox.Controls.Add(this.TransformPositionXLabel);
            this.TransformBox.Controls.Add(this.TransformPositionXTextBox);
            this.TransformBox.Controls.Add(this.InspectorTransformPositionLabel);
            this.TransformBox.Location = new System.Drawing.Point(3, 3);
            this.TransformBox.Name = "TransformBox";
            this.TransformBox.Size = new System.Drawing.Size(298, 119);
            this.TransformBox.TabIndex = 0;
            this.TransformBox.TabStop = false;
            this.TransformBox.Text = "Transform";
            // 
            // InspectorTransformPositionLabel
            // 
            this.InspectorTransformPositionLabel.AutoSize = true;
            this.InspectorTransformPositionLabel.Location = new System.Drawing.Point(6, 45);
            this.InspectorTransformPositionLabel.Name = "InspectorTransformPositionLabel";
            this.InspectorTransformPositionLabel.Size = new System.Drawing.Size(47, 13);
            this.InspectorTransformPositionLabel.TabIndex = 0;
            this.InspectorTransformPositionLabel.Text = "Position:";
            // 
            // TransformPositionXTextBox
            // 
            this.TransformPositionXTextBox.Location = new System.Drawing.Point(82, 42);
            this.TransformPositionXTextBox.Name = "TransformPositionXTextBox";
            this.TransformPositionXTextBox.Size = new System.Drawing.Size(36, 20);
            this.TransformPositionXTextBox.TabIndex = 1;
            // 
            // TransformPositionXLabel
            // 
            this.TransformPositionXLabel.AutoSize = true;
            this.TransformPositionXLabel.Location = new System.Drawing.Point(59, 45);
            this.TransformPositionXLabel.Name = "TransformPositionXLabel";
            this.TransformPositionXLabel.Size = new System.Drawing.Size(17, 13);
            this.TransformPositionXLabel.TabIndex = 2;
            this.TransformPositionXLabel.Text = "X:";
            // 
            // TransformPositionYLabel
            // 
            this.TransformPositionYLabel.AutoSize = true;
            this.TransformPositionYLabel.Location = new System.Drawing.Point(124, 45);
            this.TransformPositionYLabel.Name = "TransformPositionYLabel";
            this.TransformPositionYLabel.Size = new System.Drawing.Size(17, 13);
            this.TransformPositionYLabel.TabIndex = 4;
            this.TransformPositionYLabel.Text = "Y:";
            // 
            // TransformPositionYTextBox
            // 
            this.TransformPositionYTextBox.Location = new System.Drawing.Point(147, 42);
            this.TransformPositionYTextBox.Name = "TransformPositionYTextBox";
            this.TransformPositionYTextBox.Size = new System.Drawing.Size(36, 20);
            this.TransformPositionYTextBox.TabIndex = 3;
            // 
            // TransformPositionZLabel
            // 
            this.TransformPositionZLabel.AutoSize = true;
            this.TransformPositionZLabel.Location = new System.Drawing.Point(189, 45);
            this.TransformPositionZLabel.Name = "TransformPositionZLabel";
            this.TransformPositionZLabel.Size = new System.Drawing.Size(17, 13);
            this.TransformPositionZLabel.TabIndex = 6;
            this.TransformPositionZLabel.Text = "Z:";
            // 
            // TransformPositionZTextBox
            // 
            this.TransformPositionZTextBox.Location = new System.Drawing.Point(212, 42);
            this.TransformPositionZTextBox.Name = "TransformPositionZTextBox";
            this.TransformPositionZTextBox.Size = new System.Drawing.Size(36, 20);
            this.TransformPositionZTextBox.TabIndex = 5;
            // 
            // EditorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1253, 807);
            this.Controls.Add(this.EditorTableLayout);
            this.Name = "EditorForm";
            this.Text = "Editor";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.EditorForm_FormClosed);
            this.ResizeBegin += new System.EventHandler(this.OnResize);
            this.EditorTableLayout.ResumeLayout(false);
            this.InspectorBox.ResumeLayout(false);
            this.InspectorLayout.ResumeLayout(false);
            this.TransformBox.ResumeLayout(false);
            this.TransformBox.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel ViewPanel;
        private System.Windows.Forms.TableLayoutPanel EditorTableLayout;
        private System.Windows.Forms.GroupBox InspectorBox;
        private System.Windows.Forms.FlowLayoutPanel InspectorLayout;
        private System.Windows.Forms.GroupBox TransformBox;
        private System.Windows.Forms.Label TransformPositionZLabel;
        private System.Windows.Forms.TextBox TransformPositionZTextBox;
        private System.Windows.Forms.Label TransformPositionYLabel;
        private System.Windows.Forms.TextBox TransformPositionYTextBox;
        private System.Windows.Forms.Label TransformPositionXLabel;
        private System.Windows.Forms.TextBox TransformPositionXTextBox;
        private System.Windows.Forms.Label InspectorTransformPositionLabel;
    }
}

