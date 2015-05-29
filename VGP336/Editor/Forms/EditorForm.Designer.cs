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
            this.inspectorGrid = new System.Windows.Forms.PropertyGrid();
            this.InspectorContextMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.SceneHierarchyBox = new System.Windows.Forms.GroupBox();
            this.SceneHierarchyTree = new System.Windows.Forms.TreeView();
            this.MainMenuStrip = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newLevelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openLevelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveLevelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveLevelAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolMenuStrip = new System.Windows.Forms.MenuStrip();
            this.createToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newGameObjectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cameraToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.componentToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.boxColliderToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.meshToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.meshRendererToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ConsoleTabControl = new System.Windows.Forms.TabControl();
            this.ConsoleTab = new System.Windows.Forms.TabPage();
            this.ConsoleSplitContainer = new System.Windows.Forms.SplitContainer();
            this.ConsoleList = new System.Windows.Forms.ListView();
            this.SourceCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.LevelCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.MessageCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.CountCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.ConsoleMessageDetailsLabel = new System.Windows.Forms.Label();
            this.SceneHierarchyContextMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.renameToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.SaveFileBox = new System.Windows.Forms.SaveFileDialog();
            this.OpenFileBox = new System.Windows.Forms.OpenFileDialog();
            this.ViewPanel = new Editor.SelectablePanel();
            this.EditorTableLayout.SuspendLayout();
            this.InspectorBox.SuspendLayout();
            this.SceneHierarchyBox.SuspendLayout();
            this.MainMenuStrip.SuspendLayout();
            this.ToolMenuStrip.SuspendLayout();
            this.ConsoleTabControl.SuspendLayout();
            this.ConsoleTab.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.ConsoleSplitContainer)).BeginInit();
            this.ConsoleSplitContainer.Panel1.SuspendLayout();
            this.ConsoleSplitContainer.Panel2.SuspendLayout();
            this.ConsoleSplitContainer.SuspendLayout();
            this.SceneHierarchyContextMenu.SuspendLayout();
            this.SuspendLayout();
            // 
            // EditorTableLayout
            // 
            this.EditorTableLayout.AutoSize = true;
            this.EditorTableLayout.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.EditorTableLayout.CellBorderStyle = System.Windows.Forms.TableLayoutPanelCellBorderStyle.Single;
            this.EditorTableLayout.ColumnCount = 3;
            this.EditorTableLayout.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 21.67266F));
            this.EditorTableLayout.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 78.32734F));
            this.EditorTableLayout.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 475F));
            this.EditorTableLayout.Controls.Add(this.InspectorBox, 2, 1);
            this.EditorTableLayout.Controls.Add(this.ViewPanel, 1, 1);
            this.EditorTableLayout.Controls.Add(this.SceneHierarchyBox, 0, 1);
            this.EditorTableLayout.Controls.Add(this.MainMenuStrip, 0, 0);
            this.EditorTableLayout.Controls.Add(this.ToolMenuStrip, 1, 0);
            this.EditorTableLayout.Controls.Add(this.ConsoleTabControl, 1, 2);
            this.EditorTableLayout.Dock = System.Windows.Forms.DockStyle.Fill;
            this.EditorTableLayout.GrowStyle = System.Windows.Forms.TableLayoutPanelGrowStyle.AddColumns;
            this.EditorTableLayout.Location = new System.Drawing.Point(0, 0);
            this.EditorTableLayout.Name = "EditorTableLayout";
            this.EditorTableLayout.RowCount = 3;
            this.EditorTableLayout.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 10.95335F));
            this.EditorTableLayout.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 89.04665F));
            this.EditorTableLayout.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 270F));
            this.EditorTableLayout.Size = new System.Drawing.Size(1473, 862);
            this.EditorTableLayout.TabIndex = 1;
            // 
            // InspectorBox
            // 
            this.InspectorBox.AutoSize = true;
            this.InspectorBox.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.InspectorBox.Controls.Add(this.inspectorGrid);
            this.InspectorBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.InspectorBox.Location = new System.Drawing.Point(999, 69);
            this.InspectorBox.MinimumSize = new System.Drawing.Size(250, 500);
            this.InspectorBox.Name = "InspectorBox";
            this.InspectorBox.Size = new System.Drawing.Size(470, 517);
            this.InspectorBox.TabIndex = 1;
            this.InspectorBox.TabStop = false;
            this.InspectorBox.Text = "Inspector";
            // 
            // inspectorGrid
            // 
            this.inspectorGrid.ContextMenuStrip = this.InspectorContextMenu;
            this.inspectorGrid.Dock = System.Windows.Forms.DockStyle.Fill;
            this.inspectorGrid.Location = new System.Drawing.Point(3, 16);
            this.inspectorGrid.Name = "inspectorGrid";
            this.inspectorGrid.PropertySort = System.Windows.Forms.PropertySort.Categorized;
            this.inspectorGrid.Size = new System.Drawing.Size(464, 498);
            this.inspectorGrid.TabIndex = 0;
            this.inspectorGrid.ToolbarVisible = false;
            this.inspectorGrid.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.InspectorGrid_PropertyValueChanged);
            // 
            // InspectorContextMenu
            // 
            this.InspectorContextMenu.Name = "InspectorContextMenu";
            this.InspectorContextMenu.Size = new System.Drawing.Size(153, 26);
            // 
            // SceneHierarchyBox
            // 
            this.SceneHierarchyBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.SceneHierarchyBox.AutoSize = true;
            this.SceneHierarchyBox.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.SceneHierarchyBox.Controls.Add(this.SceneHierarchyTree);
            this.SceneHierarchyBox.Location = new System.Drawing.Point(4, 69);
            this.SceneHierarchyBox.MinimumSize = new System.Drawing.Size(200, 500);
            this.SceneHierarchyBox.Name = "SceneHierarchyBox";
            this.SceneHierarchyBox.Size = new System.Drawing.Size(209, 517);
            this.SceneHierarchyBox.TabIndex = 3;
            this.SceneHierarchyBox.TabStop = false;
            this.SceneHierarchyBox.Text = "Scene Hierarchy";
            // 
            // SceneHierarchyTree
            // 
            this.SceneHierarchyTree.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.SceneHierarchyTree.LabelEdit = true;
            this.SceneHierarchyTree.Location = new System.Drawing.Point(8, 13);
            this.SceneHierarchyTree.Name = "SceneHierarchyTree";
            this.SceneHierarchyTree.Size = new System.Drawing.Size(194, 501);
            this.SceneHierarchyTree.TabIndex = 0;
            this.SceneHierarchyTree.AfterLabelEdit += new System.Windows.Forms.NodeLabelEditEventHandler(this.SceneHierarchyTree_AfterLabelEdit);
            this.SceneHierarchyTree.NodeMouseClick += new System.Windows.Forms.TreeNodeMouseClickEventHandler(this.SceneHierarchyTree_NodeMouseClick);
            // 
            // MainMenuStrip
            // 
            this.MainMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.MainMenuStrip.Location = new System.Drawing.Point(1, 1);
            this.MainMenuStrip.Name = "MainMenuStrip";
            this.MainMenuStrip.Size = new System.Drawing.Size(215, 24);
            this.MainMenuStrip.TabIndex = 5;
            this.MainMenuStrip.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newLevelToolStripMenuItem,
            this.openLevelToolStripMenuItem,
            this.saveLevelToolStripMenuItem,
            this.saveLevelAsToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // newLevelToolStripMenuItem
            // 
            this.newLevelToolStripMenuItem.Name = "newLevelToolStripMenuItem";
            this.newLevelToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.N)));
            this.newLevelToolStripMenuItem.Size = new System.Drawing.Size(216, 22);
            this.newLevelToolStripMenuItem.Text = "New Level";
            this.newLevelToolStripMenuItem.Click += new System.EventHandler(this.OnNewLevel);
            // 
            // openLevelToolStripMenuItem
            // 
            this.openLevelToolStripMenuItem.Name = "openLevelToolStripMenuItem";
            this.openLevelToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
            this.openLevelToolStripMenuItem.Size = new System.Drawing.Size(216, 22);
            this.openLevelToolStripMenuItem.Text = "Open Level";
            this.openLevelToolStripMenuItem.Click += new System.EventHandler(this.OnOpenLevel);
            // 
            // saveLevelToolStripMenuItem
            // 
            this.saveLevelToolStripMenuItem.Name = "saveLevelToolStripMenuItem";
            this.saveLevelToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.saveLevelToolStripMenuItem.Size = new System.Drawing.Size(216, 22);
            this.saveLevelToolStripMenuItem.Text = "Save Level";
            this.saveLevelToolStripMenuItem.Click += new System.EventHandler(this.OnSaveLevel);
            // 
            // saveLevelAsToolStripMenuItem
            // 
            this.saveLevelAsToolStripMenuItem.Name = "saveLevelAsToolStripMenuItem";
            this.saveLevelAsToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)(((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Shift) 
            | System.Windows.Forms.Keys.S)));
            this.saveLevelAsToolStripMenuItem.Size = new System.Drawing.Size(216, 22);
            this.saveLevelAsToolStripMenuItem.Text = "Save Level As";
            this.saveLevelAsToolStripMenuItem.Click += new System.EventHandler(this.OnSaveLevelAs);
            // 
            // ToolMenuStrip
            // 
            this.ToolMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.createToolStripMenuItem,
            this.componentToolStripMenuItem});
            this.ToolMenuStrip.Location = new System.Drawing.Point(217, 1);
            this.ToolMenuStrip.Name = "ToolMenuStrip";
            this.ToolMenuStrip.Size = new System.Drawing.Size(778, 24);
            this.ToolMenuStrip.TabIndex = 4;
            this.ToolMenuStrip.Text = "menuStrip1";
            // 
            // createToolStripMenuItem
            // 
            this.createToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newGameObjectToolStripMenuItem,
            this.cameraToolStripMenuItem});
            this.createToolStripMenuItem.Name = "createToolStripMenuItem";
            this.createToolStripMenuItem.Size = new System.Drawing.Size(53, 20);
            this.createToolStripMenuItem.Text = "Create";
            // 
            // newGameObjectToolStripMenuItem
            // 
            this.newGameObjectToolStripMenuItem.Name = "newGameObjectToolStripMenuItem";
            this.newGameObjectToolStripMenuItem.Size = new System.Drawing.Size(167, 22);
            this.newGameObjectToolStripMenuItem.Text = "New GameObject";
            this.newGameObjectToolStripMenuItem.Click += new System.EventHandler(this.OnCreateNewGameObject);
            // 
            // cameraToolStripMenuItem
            // 
            this.cameraToolStripMenuItem.Name = "cameraToolStripMenuItem";
            this.cameraToolStripMenuItem.Size = new System.Drawing.Size(167, 22);
            this.cameraToolStripMenuItem.Text = "Camera";
            this.cameraToolStripMenuItem.Click += new System.EventHandler(this.OnCreateCamera);
            // 
            // componentToolStripMenuItem
            // 
            this.componentToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.boxColliderToolStripMenuItem,
            this.meshToolStripMenuItem,
            this.meshRendererToolStripMenuItem});
            this.componentToolStripMenuItem.Name = "componentToolStripMenuItem";
            this.componentToolStripMenuItem.Size = new System.Drawing.Size(83, 20);
            this.componentToolStripMenuItem.Text = "Component";
            // 
            // boxColliderToolStripMenuItem
            // 
            this.boxColliderToolStripMenuItem.Name = "boxColliderToolStripMenuItem";
            this.boxColliderToolStripMenuItem.Size = new System.Drawing.Size(153, 22);
            this.boxColliderToolStripMenuItem.Tag = "ColliderComponent";
            this.boxColliderToolStripMenuItem.Text = "Collider";
            this.boxColliderToolStripMenuItem.Click += new System.EventHandler(this.OnComponentMenuItem_Click);
            // 
            // meshToolStripMenuItem
            // 
            this.meshToolStripMenuItem.Name = "meshToolStripMenuItem";
            this.meshToolStripMenuItem.Size = new System.Drawing.Size(153, 22);
            this.meshToolStripMenuItem.Tag = "MeshComponent";
            this.meshToolStripMenuItem.Text = "Mesh";
            this.meshToolStripMenuItem.Click += new System.EventHandler(this.OnComponentMenuItem_Click);
            // 
            // meshRendererToolStripMenuItem
            // 
            this.meshRendererToolStripMenuItem.Name = "meshRendererToolStripMenuItem";
            this.meshRendererToolStripMenuItem.Size = new System.Drawing.Size(153, 22);
            this.meshRendererToolStripMenuItem.Tag = "MeshRendererComponent";
            this.meshRendererToolStripMenuItem.Text = "Mesh Renderer";
            this.meshRendererToolStripMenuItem.Click += new System.EventHandler(this.OnComponentMenuItem_Click);
            // 
            // ConsoleTabControl
            // 
            this.ConsoleTabControl.Controls.Add(this.ConsoleTab);
            this.ConsoleTabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ConsoleTabControl.Location = new System.Drawing.Point(220, 593);
            this.ConsoleTabControl.Name = "ConsoleTabControl";
            this.ConsoleTabControl.SelectedIndex = 0;
            this.ConsoleTabControl.Size = new System.Drawing.Size(772, 265);
            this.ConsoleTabControl.TabIndex = 6;
            // 
            // ConsoleTab
            // 
            this.ConsoleTab.Controls.Add(this.ConsoleSplitContainer);
            this.ConsoleTab.Location = new System.Drawing.Point(4, 22);
            this.ConsoleTab.Name = "ConsoleTab";
            this.ConsoleTab.Padding = new System.Windows.Forms.Padding(3);
            this.ConsoleTab.Size = new System.Drawing.Size(764, 239);
            this.ConsoleTab.TabIndex = 0;
            this.ConsoleTab.Text = "Console";
            this.ConsoleTab.UseVisualStyleBackColor = true;
            // 
            // ConsoleSplitContainer
            // 
            this.ConsoleSplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ConsoleSplitContainer.Location = new System.Drawing.Point(3, 3);
            this.ConsoleSplitContainer.Name = "ConsoleSplitContainer";
            this.ConsoleSplitContainer.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // ConsoleSplitContainer.Panel1
            // 
            this.ConsoleSplitContainer.Panel1.Controls.Add(this.ConsoleList);
            // 
            // ConsoleSplitContainer.Panel2
            // 
            this.ConsoleSplitContainer.Panel2.Controls.Add(this.ConsoleMessageDetailsLabel);
            this.ConsoleSplitContainer.Size = new System.Drawing.Size(758, 233);
            this.ConsoleSplitContainer.SplitterDistance = 168;
            this.ConsoleSplitContainer.TabIndex = 0;
            // 
            // ConsoleList
            // 
            this.ConsoleList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.SourceCol,
            this.LevelCol,
            this.MessageCol,
            this.CountCol});
            this.ConsoleList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ConsoleList.FullRowSelect = true;
            this.ConsoleList.Location = new System.Drawing.Point(0, 0);
            this.ConsoleList.Name = "ConsoleList";
            this.ConsoleList.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.ConsoleList.Size = new System.Drawing.Size(758, 168);
            this.ConsoleList.TabIndex = 0;
            this.ConsoleList.UseCompatibleStateImageBehavior = false;
            this.ConsoleList.View = System.Windows.Forms.View.Details;
            this.ConsoleList.Click += new System.EventHandler(this.ConsoleList_Click);
            // 
            // SourceCol
            // 
            this.SourceCol.Text = "Source";
            this.SourceCol.Width = 152;
            // 
            // LevelCol
            // 
            this.LevelCol.Text = "Level";
            this.LevelCol.Width = 100;
            // 
            // MessageCol
            // 
            this.MessageCol.Text = "Message";
            this.MessageCol.Width = 491;
            // 
            // CountCol
            // 
            this.CountCol.Text = "Count";
            // 
            // ConsoleMessageDetailsLabel
            // 
            this.ConsoleMessageDetailsLabel.AutoSize = true;
            this.ConsoleMessageDetailsLabel.Location = new System.Drawing.Point(4, 4);
            this.ConsoleMessageDetailsLabel.Name = "ConsoleMessageDetailsLabel";
            this.ConsoleMessageDetailsLabel.Size = new System.Drawing.Size(35, 13);
            this.ConsoleMessageDetailsLabel.TabIndex = 0;
            this.ConsoleMessageDetailsLabel.Text = "label1";
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
            this.ViewPanel.AutoSize = true;
            this.ViewPanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.ViewPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ViewPanel.Location = new System.Drawing.Point(220, 69);
            this.ViewPanel.MinimumSize = new System.Drawing.Size(480, 272);
            this.ViewPanel.Name = "ViewPanel";
            this.ViewPanel.Padding = new System.Windows.Forms.Padding(3);
            this.ViewPanel.Size = new System.Drawing.Size(772, 517);
            this.ViewPanel.TabIndex = 2;
            this.ViewPanel.TabStop = true;
            // 
            // EditorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.ClientSize = new System.Drawing.Size(1473, 862);
            this.Controls.Add(this.EditorTableLayout);
            this.Name = "EditorForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Editor";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.EditorForm_FormClosing);
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.EditorForm_FormClosed);
            this.EditorTableLayout.ResumeLayout(false);
            this.EditorTableLayout.PerformLayout();
            this.InspectorBox.ResumeLayout(false);
            this.SceneHierarchyBox.ResumeLayout(false);
            this.MainMenuStrip.ResumeLayout(false);
            this.MainMenuStrip.PerformLayout();
            this.ToolMenuStrip.ResumeLayout(false);
            this.ToolMenuStrip.PerformLayout();
            this.ConsoleTabControl.ResumeLayout(false);
            this.ConsoleTab.ResumeLayout(false);
            this.ConsoleSplitContainer.Panel1.ResumeLayout(false);
            this.ConsoleSplitContainer.Panel2.ResumeLayout(false);
            this.ConsoleSplitContainer.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.ConsoleSplitContainer)).EndInit();
            this.ConsoleSplitContainer.ResumeLayout(false);
            this.SceneHierarchyContextMenu.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel EditorTableLayout;
        private System.Windows.Forms.GroupBox InspectorBox;
        private SelectablePanel ViewPanel;
        private System.Windows.Forms.PropertyGrid inspectorGrid;
        private System.Windows.Forms.GroupBox SceneHierarchyBox;
        private System.Windows.Forms.TreeView SceneHierarchyTree;
        private System.Windows.Forms.MenuStrip ToolMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem createToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newGameObjectToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem componentToolStripMenuItem;
        private System.Windows.Forms.ContextMenuStrip SceneHierarchyContextMenu;
        private System.Windows.Forms.ToolStripMenuItem renameToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem boxColliderToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem meshToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem meshRendererToolStripMenuItem;
        private System.Windows.Forms.MenuStrip MainMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newLevelToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openLevelToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveLevelToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveLevelAsToolStripMenuItem;
        private System.Windows.Forms.TabControl ConsoleTabControl;
        private System.Windows.Forms.TabPage ConsoleTab;
        private System.Windows.Forms.ListView ConsoleList;
        private System.Windows.Forms.ColumnHeader SourceCol;
        private System.Windows.Forms.ColumnHeader LevelCol;
        private System.Windows.Forms.ColumnHeader MessageCol;
        private System.Windows.Forms.ColumnHeader CountCol;
        private System.Windows.Forms.SplitContainer ConsoleSplitContainer;
        private System.Windows.Forms.Label ConsoleMessageDetailsLabel;
        private System.Windows.Forms.SaveFileDialog SaveFileBox;
        private System.Windows.Forms.OpenFileDialog OpenFileBox;
        private System.Windows.Forms.ToolStripMenuItem cameraToolStripMenuItem;
        private System.Windows.Forms.ContextMenuStrip InspectorContextMenu;
    }
}

