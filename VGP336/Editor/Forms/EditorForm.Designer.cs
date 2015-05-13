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
            this.EditorTableLayout = new System.Windows.Forms.TableLayoutPanel();
            this.InspectorBox = new System.Windows.Forms.GroupBox();
            this.InspectorGrid = new System.Windows.Forms.PropertyGrid();
            this.ViewPanel = new Editor.SelectablePanel();
            this.EditorTableLayout.SuspendLayout();
            this.InspectorBox.SuspendLayout();
            this.SuspendLayout();
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
            this.EditorTableLayout.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 335F));
            this.EditorTableLayout.Controls.Add(this.InspectorBox, 2, 1);
            this.EditorTableLayout.Controls.Add(this.ViewPanel, 1, 1);
            this.EditorTableLayout.Location = new System.Drawing.Point(0, 0);
            this.EditorTableLayout.Name = "EditorTableLayout";
            this.EditorTableLayout.RowCount = 3;
            this.EditorTableLayout.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 10.95335F));
            this.EditorTableLayout.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 89.04665F));
            this.EditorTableLayout.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 169F));
            this.EditorTableLayout.Size = new System.Drawing.Size(1229, 784);
            this.EditorTableLayout.TabIndex = 1;
            // 
            // InspectorBox
            // 
            this.InspectorBox.Controls.Add(this.InspectorGrid);
            this.InspectorBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.InspectorBox.Location = new System.Drawing.Point(896, 70);
            this.InspectorBox.Name = "InspectorBox";
            this.InspectorBox.Size = new System.Drawing.Size(330, 541);
            this.InspectorBox.TabIndex = 1;
            this.InspectorBox.TabStop = false;
            this.InspectorBox.Text = "Inspector";
            // 
            // InspectorGrid
            // 
            this.InspectorGrid.Dock = System.Windows.Forms.DockStyle.Fill;
            this.InspectorGrid.Location = new System.Drawing.Point(3, 16);
            this.InspectorGrid.Name = "InspectorGrid";
            this.InspectorGrid.PropertySort = System.Windows.Forms.PropertySort.Categorized;
            this.InspectorGrid.Size = new System.Drawing.Size(324, 522);
            this.InspectorGrid.TabIndex = 0;
            this.InspectorGrid.ToolbarVisible = false;
            this.InspectorGrid.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.InspectorGrid_PropertyValueChanged);
            // 
            // ViewPanel
            // 
            this.ViewPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ViewPanel.Location = new System.Drawing.Point(84, 70);
            this.ViewPanel.Name = "ViewPanel";
            this.ViewPanel.Size = new System.Drawing.Size(806, 541);
            this.ViewPanel.TabIndex = 2;
            this.ViewPanel.TabStop = true;
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
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel EditorTableLayout;
        private System.Windows.Forms.GroupBox InspectorBox;
        private SelectablePanel ViewPanel;
        private System.Windows.Forms.PropertyGrid InspectorGrid;
    }
}

