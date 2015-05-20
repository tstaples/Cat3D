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
            this.components = new System.ComponentModel.Container();
            this.EditorTableLayout = new System.Windows.Forms.TableLayoutPanel();
            this.InspectorBox = new System.Windows.Forms.GroupBox();
            this.InspectorGrid = new System.Windows.Forms.PropertyGrid();
            this.SceneHierarchyBox = new System.Windows.Forms.GroupBox();
            this.SceneHierarchyTree = new System.Windows.Forms.TreeView();
            this.ToolMenuStrip = new System.Windows.Forms.MenuStrip();
            this.createToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newGameObjectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.componentToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.SceneHierarchyContextMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.renameToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ViewPanel = new Editor.SelectablePanel();
            this.EditorTableLayout.SuspendLayout();
            this.InspectorBox.SuspendLayout();
            this.SceneHierarchyBox.SuspendLayout();
            this.ToolMenuStrip.SuspendLayout();
            this.SceneHierarchyContextMenu.SuspendLayout();
            this.SuspendLayout();
            // 
            // EditorTableLayout
            // 
            this.EditorTableLayout.AutoSize = true;
            this.EditorTableLayout.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.EditorTableLayout.ColumnCount = 3;
            this.EditorTableLayout.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 21.67266F));
            this.EditorTableLayout.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 78.32734F));
            this.EditorTableLayout.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 344F));
            this.EditorTableLayout.Controls.Add(this.InspectorBox, 2, 1);
            this.EditorTableLayout.Controls.Add(this.ViewPanel, 1, 1);
            this.EditorTableLayout.Controls.Add(this.SceneHierarchyBox, 0, 1);
            this.EditorTableLayout.Controls.Add(this.ToolMenuStrip, 1, 0);
            this.EditorTableLayout.Dock = System.Windows.Forms.DockStyle.Fill;
            this.EditorTableLayout.Location = new System.Drawing.Point(0, 0);
            this.EditorTableLayout.Name = "EditorTableLayout";
            this.EditorTableLayout.RowCount = 3;
            this.EditorTableLayout.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 10.95335F));
            this.EditorTableLayout.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 89.04665F));
            this.EditorTableLayout.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 169F));
            this.EditorTableLayout.Size = new System.Drawing.Size(1473, 807);
            this.EditorTableLayout.TabIndex = 1;
            // 
            // InspectorBox
            // 
            this.InspectorBox.Controls.Add(this.InspectorGrid);
            this.InspectorBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.InspectorBox.Location = new System.Drawing.Point(1131, 72);
            this.InspectorBox.Name = "InspectorBox";
            this.InspectorBox.Size = new System.Drawing.Size(339, 562);
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
            this.InspectorGrid.Size = new System.Drawing.Size(333, 543);
            this.InspectorGrid.TabIndex = 0;
            this.InspectorGrid.ToolbarVisible = false;
            this.InspectorGrid.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.InspectorGrid_PropertyValueChanged);
            // 
            // SceneHierarchyBox
            // 
            this.SceneHierarchyBox.Controls.Add(this.SceneHierarchyTree);
            this.SceneHierarchyBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.SceneHierarchyBox.Location = new System.Drawing.Point(3, 72);
            this.SceneHierarchyBox.Name = "SceneHierarchyBox";
            this.SceneHierarchyBox.Size = new System.Drawing.Size(238, 562);
            this.SceneHierarchyBox.TabIndex = 3;
            this.SceneHierarchyBox.TabStop = false;
            this.SceneHierarchyBox.Text = "Scene Hierarchy";
            // 
            // SceneHierarchyTree
            // 
            this.SceneHierarchyTree.Dock = System.Windows.Forms.DockStyle.Fill;
            this.SceneHierarchyTree.LabelEdit = true;
            this.SceneHierarchyTree.Location = new System.Drawing.Point(3, 16);
            this.SceneHierarchyTree.Name = "SceneHierarchyTree";
            this.SceneHierarchyTree.Size = new System.Drawing.Size(232, 543);
            this.SceneHierarchyTree.TabIndex = 0;
            this.SceneHierarchyTree.AfterLabelEdit += new System.Windows.Forms.NodeLabelEditEventHandler(this.SceneHierarchyTree_AfterLabelEdit);
            this.SceneHierarchyTree.NodeMouseClick += new System.Windows.Forms.TreeNodeMouseClickEventHandler(this.SceneHierarchyTree_NodeMouseClick);
            // 
            // ToolMenuStrip
            // 
            this.ToolMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.createToolStripMenuItem,
            this.componentToolStripMenuItem});
            this.ToolMenuStrip.Location = new System.Drawing.Point(244, 0);
            this.ToolMenuStrip.Name = "ToolMenuStrip";
            this.ToolMenuStrip.Size = new System.Drawing.Size(884, 24);
            this.ToolMenuStrip.TabIndex = 4;
            this.ToolMenuStrip.Text = "menuStrip1";
            // 
            // createToolStripMenuItem
            // 
            this.createToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newGameObjectToolStripMenuItem});
            this.createToolStripMenuItem.Name = "createToolStripMenuItem";
            this.createToolStripMenuItem.Size = new System.Drawing.Size(53, 20);
            this.createToolStripMenuItem.Text = "Create";
            // 
            // newGameObjectToolStripMenuItem
            // 
            this.newGameObjectToolStripMenuItem.Name = "newGameObjectToolStripMenuItem";
            this.newGameObjectToolStripMenuItem.Size = new System.Drawing.Size(167, 22);
            this.newGameObjectToolStripMenuItem.Text = "New GameObject";
            this.newGameObjectToolStripMenuItem.Click += new System.EventHandler(this.newGameObjectToolStripMenuItem_Click);
            // 
            // componentToolStripMenuItem
            // 
            this.componentToolStripMenuItem.Name = "componentToolStripMenuItem";
            this.componentToolStripMenuItem.Size = new System.Drawing.Size(83, 20);
            this.componentToolStripMenuItem.Text = "Component";
            // 
            // SceneHierarchyContextMenu
            // 
            this.SceneHierarchyContextMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.renameToolStripMenuItem});
            this.SceneHierarchyContextMenu.Name = "SceneHierarchyContextMenu";
            this.SceneHierarchyContextMenu.Size = new System.Drawing.Size(118, 26);
            this.SceneHierarchyContextMenu.Text = "Hi";
            // 
            // renameToolStripMenuItem
            // 
            this.renameToolStripMenuItem.Name = "renameToolStripMenuItem";
            this.renameToolStripMenuItem.Size = new System.Drawing.Size(117, 22);
            this.renameToolStripMenuItem.Text = "Rename";
            this.renameToolStripMenuItem.Click += new System.EventHandler(this.renameToolStripMenuItem_Click);
            // 
            // ViewPanel
            // 
            this.ViewPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ViewPanel.Location = new System.Drawing.Point(247, 72);
            this.ViewPanel.Name = "ViewPanel";
            this.ViewPanel.Size = new System.Drawing.Size(878, 562);
            this.ViewPanel.TabIndex = 2;
            this.ViewPanel.TabStop = true;
            // 
            // EditorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1473, 807);
            this.Controls.Add(this.EditorTableLayout);
            this.MainMenuStrip = this.ToolMenuStrip;
            this.Name = "EditorForm";
            this.Text = "Editor";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.EditorForm_FormClosed);
            this.ResizeBegin += new System.EventHandler(this.OnResize);
            this.EditorTableLayout.ResumeLayout(false);
            this.EditorTableLayout.PerformLayout();
            this.InspectorBox.ResumeLayout(false);
            this.SceneHierarchyBox.ResumeLayout(false);
            this.ToolMenuStrip.ResumeLayout(false);
            this.ToolMenuStrip.PerformLayout();
            this.SceneHierarchyContextMenu.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel EditorTableLayout;
        private System.Windows.Forms.GroupBox InspectorBox;
        private SelectablePanel ViewPanel;
        private System.Windows.Forms.PropertyGrid InspectorGrid;
        private System.Windows.Forms.GroupBox SceneHierarchyBox;
        private System.Windows.Forms.TreeView SceneHierarchyTree;
        private System.Windows.Forms.MenuStrip ToolMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem createToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newGameObjectToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem componentToolStripMenuItem;
        private System.Windows.Forms.ContextMenuStrip SceneHierarchyContextMenu;
        private System.Windows.Forms.ToolStripMenuItem renameToolStripMenuItem;
    }
}

