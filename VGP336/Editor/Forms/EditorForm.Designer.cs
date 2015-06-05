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
            this.InspectorContextMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.SceneHierarchyContextMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.RenameToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.DeleteGameObjectMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.SaveFileBox = new System.Windows.Forms.SaveFileDialog();
            this.OpenFileBox = new System.Windows.Forms.OpenFileDialog();
            this.ViewportTabControl = new System.Windows.Forms.TabControl();
            this.EditorViewportTab = new System.Windows.Forms.TabPage();
            this.GameViewportTab = new System.Windows.Forms.TabPage();
            this.SceneHierarchyTree = new System.Windows.Forms.TreeView();
            this.miniToolStrip = new System.Windows.Forms.MenuStrip();
            this.MainMenuStrip = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newLevelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openLevelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveLevelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveLevelAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.createToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.newGameObjectToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.cameraToolStripMenuItem2 = new System.Windows.Forms.ToolStripMenuItem();
            this.geometryToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cubeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.componentToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.cameraToolStripMenuItem3 = new System.Windows.Forms.ToolStripMenuItem();
            this.colliderToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.meshToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.meshRendererToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.rigidBodyToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.inspectorGrid = new System.Windows.Forms.PropertyGrid();
            this.ConsoleTabControl = new System.Windows.Forms.TabControl();
            this.ConsoleTab = new System.Windows.Forms.TabPage();
            this.ConsoleSplitContainer = new System.Windows.Forms.SplitContainer();
            this.ConsoleList = new System.Windows.Forms.ListView();
            this.SourceCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.LevelCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.MessageCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.CountCol = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.ConsoleMessageDetailsLabel = new System.Windows.Forms.Label();
            this.MenuStripSplitContainer = new System.Windows.Forms.SplitContainer();
            this.StopButton = new System.Windows.Forms.Button();
            this.PauseButton = new System.Windows.Forms.Button();
            this.PlayButton = new System.Windows.Forms.Button();
            this.InspectorSplitContainer = new System.Windows.Forms.SplitContainer();
            this.ConsoleViewportSplitContainer = new System.Windows.Forms.SplitContainer();
            this.HierarchyViewportSplitContainer = new System.Windows.Forms.SplitContainer();
            this.SceneHierarchyTabControl = new System.Windows.Forms.TabControl();
            this.SceneHierarchyTab = new System.Windows.Forms.TabPage();
            this.InspectorTabControl = new System.Windows.Forms.TabControl();
            this.InspectorTab = new System.Windows.Forms.TabPage();
            this.planeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.sphereToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cylinderToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.coneToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.torusToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.AssetTab = new System.Windows.Forms.TabPage();
            this.AssetSplitContainer = new System.Windows.Forms.SplitContainer();
            this.AssetDirectoryTreeView = new System.Windows.Forms.TreeView();
            this.AssetListView = new System.Windows.Forms.ListView();
            this.EditorViewPanel = new Editor.SelectablePanel();
            this.GameViewPanel = new Editor.SelectablePanel();
            this.SceneHierarchyContextMenu.SuspendLayout();
            this.ViewportTabControl.SuspendLayout();
            this.EditorViewportTab.SuspendLayout();
            this.GameViewportTab.SuspendLayout();
            this.MainMenuStrip.SuspendLayout();
            this.ConsoleTabControl.SuspendLayout();
            this.ConsoleTab.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.ConsoleSplitContainer)).BeginInit();
            this.ConsoleSplitContainer.Panel1.SuspendLayout();
            this.ConsoleSplitContainer.Panel2.SuspendLayout();
            this.ConsoleSplitContainer.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.MenuStripSplitContainer)).BeginInit();
            this.MenuStripSplitContainer.Panel1.SuspendLayout();
            this.MenuStripSplitContainer.Panel2.SuspendLayout();
            this.MenuStripSplitContainer.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.InspectorSplitContainer)).BeginInit();
            this.InspectorSplitContainer.Panel1.SuspendLayout();
            this.InspectorSplitContainer.Panel2.SuspendLayout();
            this.InspectorSplitContainer.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.ConsoleViewportSplitContainer)).BeginInit();
            this.ConsoleViewportSplitContainer.Panel1.SuspendLayout();
            this.ConsoleViewportSplitContainer.Panel2.SuspendLayout();
            this.ConsoleViewportSplitContainer.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.HierarchyViewportSplitContainer)).BeginInit();
            this.HierarchyViewportSplitContainer.Panel1.SuspendLayout();
            this.HierarchyViewportSplitContainer.Panel2.SuspendLayout();
            this.HierarchyViewportSplitContainer.SuspendLayout();
            this.SceneHierarchyTabControl.SuspendLayout();
            this.SceneHierarchyTab.SuspendLayout();
            this.InspectorTabControl.SuspendLayout();
            this.InspectorTab.SuspendLayout();
            this.AssetTab.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.AssetSplitContainer)).BeginInit();
            this.AssetSplitContainer.Panel1.SuspendLayout();
            this.AssetSplitContainer.Panel2.SuspendLayout();
            this.AssetSplitContainer.SuspendLayout();
            this.SuspendLayout();
            // 
            // InspectorContextMenu
            // 
            this.InspectorContextMenu.Name = "InspectorContextMenu";
            this.InspectorContextMenu.Size = new System.Drawing.Size(61, 4);
            // 
            // SceneHierarchyContextMenu
            // 
            this.SceneHierarchyContextMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.RenameToolStripMenuItem,
            this.DeleteGameObjectMenuItem});
            this.SceneHierarchyContextMenu.Name = "SceneHierarchyContextMenu";
            this.SceneHierarchyContextMenu.Size = new System.Drawing.Size(118, 48);
            this.SceneHierarchyContextMenu.Text = "Hi";
            // 
            // RenameToolStripMenuItem
            // 
            this.RenameToolStripMenuItem.Name = "RenameToolStripMenuItem";
            this.RenameToolStripMenuItem.Size = new System.Drawing.Size(117, 22);
            this.RenameToolStripMenuItem.Text = "Rename";
            this.RenameToolStripMenuItem.Click += new System.EventHandler(this.RenameToolStripMenuItem_Click);
            // 
            // DeleteGameObjectMenuItem
            // 
            this.DeleteGameObjectMenuItem.Name = "DeleteGameObjectMenuItem";
            this.DeleteGameObjectMenuItem.Size = new System.Drawing.Size(117, 22);
            this.DeleteGameObjectMenuItem.Text = "Delete";
            this.DeleteGameObjectMenuItem.Click += new System.EventHandler(this.DeleteGameObjectMenuItem_Click);
            // 
            // ViewportTabControl
            // 
            this.ViewportTabControl.Controls.Add(this.EditorViewportTab);
            this.ViewportTabControl.Controls.Add(this.GameViewportTab);
            this.ViewportTabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ViewportTabControl.Location = new System.Drawing.Point(0, 0);
            this.ViewportTabControl.MinimumSize = new System.Drawing.Size(800, 400);
            this.ViewportTabControl.Name = "ViewportTabControl";
            this.ViewportTabControl.SelectedIndex = 0;
            this.ViewportTabControl.Size = new System.Drawing.Size(884, 541);
            this.ViewportTabControl.TabIndex = 7;
            // 
            // EditorViewportTab
            // 
            this.EditorViewportTab.Controls.Add(this.EditorViewPanel);
            this.EditorViewportTab.Location = new System.Drawing.Point(4, 22);
            this.EditorViewportTab.Name = "EditorViewportTab";
            this.EditorViewportTab.Padding = new System.Windows.Forms.Padding(3);
            this.EditorViewportTab.Size = new System.Drawing.Size(876, 515);
            this.EditorViewportTab.TabIndex = 0;
            this.EditorViewportTab.Text = "Editor";
            this.EditorViewportTab.UseVisualStyleBackColor = true;
            // 
            // GameViewportTab
            // 
            this.GameViewportTab.Controls.Add(this.GameViewPanel);
            this.GameViewportTab.Location = new System.Drawing.Point(4, 22);
            this.GameViewportTab.Name = "GameViewportTab";
            this.GameViewportTab.Padding = new System.Windows.Forms.Padding(3);
            this.GameViewportTab.Size = new System.Drawing.Size(876, 515);
            this.GameViewportTab.TabIndex = 1;
            this.GameViewportTab.Text = "Game";
            this.GameViewportTab.UseVisualStyleBackColor = true;
            // 
            // SceneHierarchyTree
            // 
            this.SceneHierarchyTree.Dock = System.Windows.Forms.DockStyle.Fill;
            this.SceneHierarchyTree.LabelEdit = true;
            this.SceneHierarchyTree.Location = new System.Drawing.Point(3, 3);
            this.SceneHierarchyTree.Name = "SceneHierarchyTree";
            this.SceneHierarchyTree.Size = new System.Drawing.Size(221, 509);
            this.SceneHierarchyTree.TabIndex = 0;
            this.SceneHierarchyTree.AfterLabelEdit += new System.Windows.Forms.NodeLabelEditEventHandler(this.SceneHierarchyTree_AfterLabelEdit);
            this.SceneHierarchyTree.NodeMouseClick += new System.Windows.Forms.TreeNodeMouseClickEventHandler(this.SceneHierarchyTree_NodeMouseClick);
            // 
            // miniToolStrip
            // 
            this.miniToolStrip.AutoSize = false;
            this.miniToolStrip.Dock = System.Windows.Forms.DockStyle.None;
            this.miniToolStrip.Location = new System.Drawing.Point(142, 2);
            this.miniToolStrip.Name = "miniToolStrip";
            this.miniToolStrip.Size = new System.Drawing.Size(144, 24);
            this.miniToolStrip.TabIndex = 4;
            // 
            // MainMenuStrip
            // 
            this.MainMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.createToolStripMenuItem1,
            this.componentToolStripMenuItem1});
            this.MainMenuStrip.Location = new System.Drawing.Point(0, 0);
            this.MainMenuStrip.Name = "MainMenuStrip";
            this.MainMenuStrip.Size = new System.Drawing.Size(1473, 24);
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
            // createToolStripMenuItem1
            // 
            this.createToolStripMenuItem1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newGameObjectToolStripMenuItem1,
            this.cameraToolStripMenuItem2,
            this.geometryToolStripMenuItem});
            this.createToolStripMenuItem1.Name = "createToolStripMenuItem1";
            this.createToolStripMenuItem1.Size = new System.Drawing.Size(53, 20);
            this.createToolStripMenuItem1.Text = "Create";
            // 
            // newGameObjectToolStripMenuItem1
            // 
            this.newGameObjectToolStripMenuItem1.Name = "newGameObjectToolStripMenuItem1";
            this.newGameObjectToolStripMenuItem1.Size = new System.Drawing.Size(167, 22);
            this.newGameObjectToolStripMenuItem1.Text = "New GameObject";
            this.newGameObjectToolStripMenuItem1.Click += new System.EventHandler(this.OnCreateNewGameObject);
            // 
            // cameraToolStripMenuItem2
            // 
            this.cameraToolStripMenuItem2.Name = "cameraToolStripMenuItem2";
            this.cameraToolStripMenuItem2.Size = new System.Drawing.Size(167, 22);
            this.cameraToolStripMenuItem2.Text = "Camera";
            this.cameraToolStripMenuItem2.Click += new System.EventHandler(this.OnCreateCamera);
            // 
            // geometryToolStripMenuItem
            // 
            this.geometryToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.cubeToolStripMenuItem,
            this.planeToolStripMenuItem,
            this.sphereToolStripMenuItem,
            this.cylinderToolStripMenuItem,
            this.coneToolStripMenuItem,
            this.torusToolStripMenuItem});
            this.geometryToolStripMenuItem.Name = "geometryToolStripMenuItem";
            this.geometryToolStripMenuItem.Size = new System.Drawing.Size(167, 22);
            this.geometryToolStripMenuItem.Text = "Geometry";
            // 
            // cubeToolStripMenuItem
            // 
            this.cubeToolStripMenuItem.Name = "cubeToolStripMenuItem";
            this.cubeToolStripMenuItem.Size = new System.Drawing.Size(118, 22);
            this.cubeToolStripMenuItem.Text = "Cube";
            this.cubeToolStripMenuItem.Click += new System.EventHandler(this.OnCreateGeometry);
            // 
            // componentToolStripMenuItem1
            // 
            this.componentToolStripMenuItem1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.cameraToolStripMenuItem3,
            this.colliderToolStripMenuItem,
            this.meshToolStripMenuItem1,
            this.meshRendererToolStripMenuItem1,
            this.rigidBodyToolStripMenuItem1});
            this.componentToolStripMenuItem1.Name = "componentToolStripMenuItem1";
            this.componentToolStripMenuItem1.Size = new System.Drawing.Size(83, 20);
            this.componentToolStripMenuItem1.Text = "Component";
            // 
            // cameraToolStripMenuItem3
            // 
            this.cameraToolStripMenuItem3.Name = "cameraToolStripMenuItem3";
            this.cameraToolStripMenuItem3.Size = new System.Drawing.Size(153, 22);
            this.cameraToolStripMenuItem3.Text = "Camera";
            this.cameraToolStripMenuItem3.Click += new System.EventHandler(this.OnComponentMenuItem_Click);
            // 
            // colliderToolStripMenuItem
            // 
            this.colliderToolStripMenuItem.Name = "colliderToolStripMenuItem";
            this.colliderToolStripMenuItem.Size = new System.Drawing.Size(153, 22);
            this.colliderToolStripMenuItem.Text = "Collider";
            this.colliderToolStripMenuItem.Click += new System.EventHandler(this.OnComponentMenuItem_Click);
            // 
            // meshToolStripMenuItem1
            // 
            this.meshToolStripMenuItem1.Name = "meshToolStripMenuItem1";
            this.meshToolStripMenuItem1.Size = new System.Drawing.Size(153, 22);
            this.meshToolStripMenuItem1.Text = "Mesh";
            this.meshToolStripMenuItem1.Click += new System.EventHandler(this.OnComponentMenuItem_Click);
            // 
            // meshRendererToolStripMenuItem1
            // 
            this.meshRendererToolStripMenuItem1.Name = "meshRendererToolStripMenuItem1";
            this.meshRendererToolStripMenuItem1.Size = new System.Drawing.Size(153, 22);
            this.meshRendererToolStripMenuItem1.Text = "Mesh Renderer";
            this.meshRendererToolStripMenuItem1.Click += new System.EventHandler(this.OnComponentMenuItem_Click);
            // 
            // rigidBodyToolStripMenuItem1
            // 
            this.rigidBodyToolStripMenuItem1.Name = "rigidBodyToolStripMenuItem1";
            this.rigidBodyToolStripMenuItem1.Size = new System.Drawing.Size(153, 22);
            this.rigidBodyToolStripMenuItem1.Text = "RigidBody";
            this.rigidBodyToolStripMenuItem1.Click += new System.EventHandler(this.OnComponentMenuItem_Click);
            // 
            // inspectorGrid
            // 
            this.inspectorGrid.ContextMenuStrip = this.InspectorContextMenu;
            this.inspectorGrid.Dock = System.Windows.Forms.DockStyle.Fill;
            this.inspectorGrid.Location = new System.Drawing.Point(3, 3);
            this.inspectorGrid.Name = "inspectorGrid";
            this.inspectorGrid.PropertySort = System.Windows.Forms.PropertySort.Categorized;
            this.inspectorGrid.Size = new System.Drawing.Size(332, 759);
            this.inspectorGrid.TabIndex = 0;
            this.inspectorGrid.ToolbarVisible = false;
            this.inspectorGrid.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.InspectorGrid_PropertyValueChanged);
            // 
            // ConsoleTabControl
            // 
            this.ConsoleTabControl.Controls.Add(this.ConsoleTab);
            this.ConsoleTabControl.Controls.Add(this.AssetTab);
            this.ConsoleTabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ConsoleTabControl.Location = new System.Drawing.Point(0, 0);
            this.ConsoleTabControl.Name = "ConsoleTabControl";
            this.ConsoleTabControl.SelectedIndex = 0;
            this.ConsoleTabControl.Size = new System.Drawing.Size(1123, 246);
            this.ConsoleTabControl.TabIndex = 6;
            // 
            // ConsoleTab
            // 
            this.ConsoleTab.Controls.Add(this.ConsoleSplitContainer);
            this.ConsoleTab.Location = new System.Drawing.Point(4, 22);
            this.ConsoleTab.Name = "ConsoleTab";
            this.ConsoleTab.Padding = new System.Windows.Forms.Padding(3);
            this.ConsoleTab.Size = new System.Drawing.Size(1115, 220);
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
            this.ConsoleSplitContainer.Size = new System.Drawing.Size(1109, 214);
            this.ConsoleSplitContainer.SplitterDistance = 153;
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
            this.ConsoleList.Size = new System.Drawing.Size(1109, 153);
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
            // MenuStripSplitContainer
            // 
            this.MenuStripSplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.MenuStripSplitContainer.Location = new System.Drawing.Point(0, 0);
            this.MenuStripSplitContainer.Name = "MenuStripSplitContainer";
            this.MenuStripSplitContainer.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // MenuStripSplitContainer.Panel1
            // 
            this.MenuStripSplitContainer.Panel1.Controls.Add(this.StopButton);
            this.MenuStripSplitContainer.Panel1.Controls.Add(this.PauseButton);
            this.MenuStripSplitContainer.Panel1.Controls.Add(this.PlayButton);
            this.MenuStripSplitContainer.Panel1.Controls.Add(this.MainMenuStrip);
            // 
            // MenuStripSplitContainer.Panel2
            // 
            this.MenuStripSplitContainer.Panel2.Controls.Add(this.InspectorSplitContainer);
            this.MenuStripSplitContainer.Size = new System.Drawing.Size(1473, 862);
            this.MenuStripSplitContainer.SplitterDistance = 67;
            this.MenuStripSplitContainer.TabIndex = 8;
            // 
            // StopButton
            // 
            this.StopButton.Location = new System.Drawing.Point(786, 27);
            this.StopButton.Name = "StopButton";
            this.StopButton.Size = new System.Drawing.Size(75, 23);
            this.StopButton.TabIndex = 8;
            this.StopButton.Text = "Stop";
            this.StopButton.UseVisualStyleBackColor = true;
            this.StopButton.Click += new System.EventHandler(this.StopButton_Click);
            // 
            // PauseButton
            // 
            this.PauseButton.Location = new System.Drawing.Point(705, 27);
            this.PauseButton.Name = "PauseButton";
            this.PauseButton.Size = new System.Drawing.Size(75, 23);
            this.PauseButton.TabIndex = 7;
            this.PauseButton.Text = "Pause";
            this.PauseButton.UseVisualStyleBackColor = true;
            this.PauseButton.Click += new System.EventHandler(this.PauseButton_Click);
            // 
            // PlayButton
            // 
            this.PlayButton.Location = new System.Drawing.Point(624, 27);
            this.PlayButton.Name = "PlayButton";
            this.PlayButton.Size = new System.Drawing.Size(75, 23);
            this.PlayButton.TabIndex = 6;
            this.PlayButton.Text = "Play";
            this.PlayButton.UseVisualStyleBackColor = true;
            this.PlayButton.Click += new System.EventHandler(this.PlayButton_Click);
            // 
            // InspectorSplitContainer
            // 
            this.InspectorSplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.InspectorSplitContainer.Location = new System.Drawing.Point(0, 0);
            this.InspectorSplitContainer.Name = "InspectorSplitContainer";
            // 
            // InspectorSplitContainer.Panel1
            // 
            this.InspectorSplitContainer.Panel1.Controls.Add(this.ConsoleViewportSplitContainer);
            // 
            // InspectorSplitContainer.Panel2
            // 
            this.InspectorSplitContainer.Panel2.Controls.Add(this.InspectorTabControl);
            this.InspectorSplitContainer.Size = new System.Drawing.Size(1473, 791);
            this.InspectorSplitContainer.SplitterDistance = 1123;
            this.InspectorSplitContainer.TabIndex = 0;
            // 
            // ConsoleViewportSplitContainer
            // 
            this.ConsoleViewportSplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ConsoleViewportSplitContainer.Location = new System.Drawing.Point(0, 0);
            this.ConsoleViewportSplitContainer.Name = "ConsoleViewportSplitContainer";
            this.ConsoleViewportSplitContainer.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // ConsoleViewportSplitContainer.Panel1
            // 
            this.ConsoleViewportSplitContainer.Panel1.Controls.Add(this.HierarchyViewportSplitContainer);
            // 
            // ConsoleViewportSplitContainer.Panel2
            // 
            this.ConsoleViewportSplitContainer.Panel2.Controls.Add(this.ConsoleTabControl);
            this.ConsoleViewportSplitContainer.Size = new System.Drawing.Size(1123, 791);
            this.ConsoleViewportSplitContainer.SplitterDistance = 541;
            this.ConsoleViewportSplitContainer.TabIndex = 8;
            // 
            // HierarchyViewportSplitContainer
            // 
            this.HierarchyViewportSplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.HierarchyViewportSplitContainer.Location = new System.Drawing.Point(0, 0);
            this.HierarchyViewportSplitContainer.Name = "HierarchyViewportSplitContainer";
            // 
            // HierarchyViewportSplitContainer.Panel1
            // 
            this.HierarchyViewportSplitContainer.Panel1.Controls.Add(this.SceneHierarchyTabControl);
            // 
            // HierarchyViewportSplitContainer.Panel2
            // 
            this.HierarchyViewportSplitContainer.Panel2.Controls.Add(this.ViewportTabControl);
            this.HierarchyViewportSplitContainer.Size = new System.Drawing.Size(1123, 541);
            this.HierarchyViewportSplitContainer.SplitterDistance = 235;
            this.HierarchyViewportSplitContainer.TabIndex = 0;
            // 
            // SceneHierarchyTabControl
            // 
            this.SceneHierarchyTabControl.Controls.Add(this.SceneHierarchyTab);
            this.SceneHierarchyTabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.SceneHierarchyTabControl.Location = new System.Drawing.Point(0, 0);
            this.SceneHierarchyTabControl.Name = "SceneHierarchyTabControl";
            this.SceneHierarchyTabControl.SelectedIndex = 0;
            this.SceneHierarchyTabControl.Size = new System.Drawing.Size(235, 541);
            this.SceneHierarchyTabControl.TabIndex = 0;
            // 
            // SceneHierarchyTab
            // 
            this.SceneHierarchyTab.Controls.Add(this.SceneHierarchyTree);
            this.SceneHierarchyTab.Location = new System.Drawing.Point(4, 22);
            this.SceneHierarchyTab.Name = "SceneHierarchyTab";
            this.SceneHierarchyTab.Padding = new System.Windows.Forms.Padding(3);
            this.SceneHierarchyTab.Size = new System.Drawing.Size(227, 515);
            this.SceneHierarchyTab.TabIndex = 0;
            this.SceneHierarchyTab.Text = "Scene Hierarchy";
            this.SceneHierarchyTab.UseVisualStyleBackColor = true;
            // 
            // InspectorTabControl
            // 
            this.InspectorTabControl.Controls.Add(this.InspectorTab);
            this.InspectorTabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.InspectorTabControl.Location = new System.Drawing.Point(0, 0);
            this.InspectorTabControl.Name = "InspectorTabControl";
            this.InspectorTabControl.SelectedIndex = 0;
            this.InspectorTabControl.Size = new System.Drawing.Size(346, 791);
            this.InspectorTabControl.TabIndex = 0;
            // 
            // InspectorTab
            // 
            this.InspectorTab.Controls.Add(this.inspectorGrid);
            this.InspectorTab.Location = new System.Drawing.Point(4, 22);
            this.InspectorTab.Name = "InspectorTab";
            this.InspectorTab.Padding = new System.Windows.Forms.Padding(3);
            this.InspectorTab.Size = new System.Drawing.Size(338, 765);
            this.InspectorTab.TabIndex = 0;
            this.InspectorTab.Text = "Inspector";
            this.InspectorTab.UseVisualStyleBackColor = true;
            // 
            // planeToolStripMenuItem
            // 
            this.planeToolStripMenuItem.Name = "planeToolStripMenuItem";
            this.planeToolStripMenuItem.Size = new System.Drawing.Size(118, 22);
            this.planeToolStripMenuItem.Text = "Plane";
            this.planeToolStripMenuItem.Click += new System.EventHandler(this.OnCreateGeometry);
            // 
            // sphereToolStripMenuItem
            // 
            this.sphereToolStripMenuItem.Name = "sphereToolStripMenuItem";
            this.sphereToolStripMenuItem.Size = new System.Drawing.Size(118, 22);
            this.sphereToolStripMenuItem.Text = "Sphere";
            this.sphereToolStripMenuItem.Click += new System.EventHandler(this.OnCreateGeometry);
            // 
            // cylinderToolStripMenuItem
            // 
            this.cylinderToolStripMenuItem.Name = "cylinderToolStripMenuItem";
            this.cylinderToolStripMenuItem.Size = new System.Drawing.Size(118, 22);
            this.cylinderToolStripMenuItem.Text = "Cylinder";
            this.cylinderToolStripMenuItem.Click += new System.EventHandler(this.OnCreateGeometry);
            // 
            // coneToolStripMenuItem
            // 
            this.coneToolStripMenuItem.Name = "coneToolStripMenuItem";
            this.coneToolStripMenuItem.Size = new System.Drawing.Size(118, 22);
            this.coneToolStripMenuItem.Text = "Cone";
            this.coneToolStripMenuItem.Click += new System.EventHandler(this.OnCreateGeometry);
            // 
            // torusToolStripMenuItem
            // 
            this.torusToolStripMenuItem.Name = "torusToolStripMenuItem";
            this.torusToolStripMenuItem.Size = new System.Drawing.Size(118, 22);
            this.torusToolStripMenuItem.Text = "Torus";
            this.torusToolStripMenuItem.Click += new System.EventHandler(this.OnCreateGeometry);
            // 
            // AssetTab
            // 
            this.AssetTab.Controls.Add(this.AssetSplitContainer);
            this.AssetTab.Location = new System.Drawing.Point(4, 22);
            this.AssetTab.Name = "AssetTab";
            this.AssetTab.Size = new System.Drawing.Size(1115, 220);
            this.AssetTab.TabIndex = 1;
            this.AssetTab.Text = "Assets";
            this.AssetTab.UseVisualStyleBackColor = true;
            // 
            // AssetSplitContainer
            // 
            this.AssetSplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.AssetSplitContainer.Location = new System.Drawing.Point(0, 0);
            this.AssetSplitContainer.Name = "AssetSplitContainer";
            // 
            // AssetSplitContainer.Panel1
            // 
            this.AssetSplitContainer.Panel1.Controls.Add(this.AssetDirectoryTreeView);
            // 
            // AssetSplitContainer.Panel2
            // 
            this.AssetSplitContainer.Panel2.Controls.Add(this.AssetListView);
            this.AssetSplitContainer.Size = new System.Drawing.Size(1115, 220);
            this.AssetSplitContainer.SplitterDistance = 302;
            this.AssetSplitContainer.TabIndex = 0;
            // 
            // AssetDirectoryTreeView
            // 
            this.AssetDirectoryTreeView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.AssetDirectoryTreeView.Location = new System.Drawing.Point(0, 0);
            this.AssetDirectoryTreeView.Name = "AssetDirectoryTreeView";
            this.AssetDirectoryTreeView.Size = new System.Drawing.Size(302, 220);
            this.AssetDirectoryTreeView.TabIndex = 0;
            // 
            // AssetListView
            // 
            this.AssetListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.AssetListView.Location = new System.Drawing.Point(0, 0);
            this.AssetListView.Name = "AssetListView";
            this.AssetListView.Size = new System.Drawing.Size(809, 220);
            this.AssetListView.TabIndex = 0;
            this.AssetListView.UseCompatibleStateImageBehavior = false;
            // 
            // EditorViewPanel
            // 
            this.EditorViewPanel.AutoSize = true;
            this.EditorViewPanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.EditorViewPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.EditorViewPanel.Location = new System.Drawing.Point(3, 3);
            this.EditorViewPanel.MinimumSize = new System.Drawing.Size(480, 272);
            this.EditorViewPanel.Name = "EditorViewPanel";
            this.EditorViewPanel.Padding = new System.Windows.Forms.Padding(3);
            this.EditorViewPanel.Size = new System.Drawing.Size(870, 509);
            this.EditorViewPanel.TabIndex = 2;
            this.EditorViewPanel.TabStop = true;
            // 
            // GameViewPanel
            // 
            this.GameViewPanel.AutoSize = true;
            this.GameViewPanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.GameViewPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.GameViewPanel.Location = new System.Drawing.Point(3, 3);
            this.GameViewPanel.MinimumSize = new System.Drawing.Size(480, 272);
            this.GameViewPanel.Name = "GameViewPanel";
            this.GameViewPanel.Padding = new System.Windows.Forms.Padding(3);
            this.GameViewPanel.Size = new System.Drawing.Size(870, 509);
            this.GameViewPanel.TabIndex = 3;
            this.GameViewPanel.TabStop = true;
            // 
            // EditorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.ClientSize = new System.Drawing.Size(1473, 862);
            this.Controls.Add(this.MenuStripSplitContainer);
            this.MinimumSize = new System.Drawing.Size(1024, 768);
            this.Name = "EditorForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Editor";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.EditorForm_FormClosing);
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.EditorForm_FormClosed);
            this.SceneHierarchyContextMenu.ResumeLayout(false);
            this.ViewportTabControl.ResumeLayout(false);
            this.EditorViewportTab.ResumeLayout(false);
            this.EditorViewportTab.PerformLayout();
            this.GameViewportTab.ResumeLayout(false);
            this.GameViewportTab.PerformLayout();
            this.MainMenuStrip.ResumeLayout(false);
            this.MainMenuStrip.PerformLayout();
            this.ConsoleTabControl.ResumeLayout(false);
            this.ConsoleTab.ResumeLayout(false);
            this.ConsoleSplitContainer.Panel1.ResumeLayout(false);
            this.ConsoleSplitContainer.Panel2.ResumeLayout(false);
            this.ConsoleSplitContainer.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.ConsoleSplitContainer)).EndInit();
            this.ConsoleSplitContainer.ResumeLayout(false);
            this.MenuStripSplitContainer.Panel1.ResumeLayout(false);
            this.MenuStripSplitContainer.Panel1.PerformLayout();
            this.MenuStripSplitContainer.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.MenuStripSplitContainer)).EndInit();
            this.MenuStripSplitContainer.ResumeLayout(false);
            this.InspectorSplitContainer.Panel1.ResumeLayout(false);
            this.InspectorSplitContainer.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.InspectorSplitContainer)).EndInit();
            this.InspectorSplitContainer.ResumeLayout(false);
            this.ConsoleViewportSplitContainer.Panel1.ResumeLayout(false);
            this.ConsoleViewportSplitContainer.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.ConsoleViewportSplitContainer)).EndInit();
            this.ConsoleViewportSplitContainer.ResumeLayout(false);
            this.HierarchyViewportSplitContainer.Panel1.ResumeLayout(false);
            this.HierarchyViewportSplitContainer.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.HierarchyViewportSplitContainer)).EndInit();
            this.HierarchyViewportSplitContainer.ResumeLayout(false);
            this.SceneHierarchyTabControl.ResumeLayout(false);
            this.SceneHierarchyTab.ResumeLayout(false);
            this.InspectorTabControl.ResumeLayout(false);
            this.InspectorTab.ResumeLayout(false);
            this.AssetTab.ResumeLayout(false);
            this.AssetSplitContainer.Panel1.ResumeLayout(false);
            this.AssetSplitContainer.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.AssetSplitContainer)).EndInit();
            this.AssetSplitContainer.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ContextMenuStrip SceneHierarchyContextMenu;
        private System.Windows.Forms.ToolStripMenuItem RenameToolStripMenuItem;
        private System.Windows.Forms.SaveFileDialog SaveFileBox;
        private System.Windows.Forms.OpenFileDialog OpenFileBox;
        private System.Windows.Forms.ContextMenuStrip InspectorContextMenu;
        private System.Windows.Forms.ToolStripMenuItem DeleteGameObjectMenuItem;
        private System.Windows.Forms.TabControl ViewportTabControl;
        private System.Windows.Forms.TabPage EditorViewportTab;
        private SelectablePanel EditorViewPanel;
        private System.Windows.Forms.TabPage GameViewportTab;
        private SelectablePanel GameViewPanel;
        private System.Windows.Forms.TreeView SceneHierarchyTree;
        private System.Windows.Forms.MenuStrip miniToolStrip;
        private System.Windows.Forms.MenuStrip MainMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newLevelToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openLevelToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveLevelToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveLevelAsToolStripMenuItem;
        private System.Windows.Forms.PropertyGrid inspectorGrid;
        private System.Windows.Forms.TabControl ConsoleTabControl;
        private System.Windows.Forms.TabPage ConsoleTab;
        private System.Windows.Forms.SplitContainer ConsoleSplitContainer;
        private System.Windows.Forms.ListView ConsoleList;
        private System.Windows.Forms.ColumnHeader SourceCol;
        private System.Windows.Forms.ColumnHeader LevelCol;
        private System.Windows.Forms.ColumnHeader MessageCol;
        private System.Windows.Forms.ColumnHeader CountCol;
        private System.Windows.Forms.Label ConsoleMessageDetailsLabel;
        private System.Windows.Forms.SplitContainer MenuStripSplitContainer;
        private System.Windows.Forms.SplitContainer HierarchyViewportSplitContainer;
        private System.Windows.Forms.TabControl SceneHierarchyTabControl;
        private System.Windows.Forms.TabPage SceneHierarchyTab;
        private System.Windows.Forms.SplitContainer InspectorSplitContainer;
        private System.Windows.Forms.SplitContainer ConsoleViewportSplitContainer;
        private System.Windows.Forms.TabControl InspectorTabControl;
        private System.Windows.Forms.TabPage InspectorTab;
        private System.Windows.Forms.ToolStripMenuItem createToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem newGameObjectToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem cameraToolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem componentToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem cameraToolStripMenuItem3;
        private System.Windows.Forms.ToolStripMenuItem colliderToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem meshToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem meshRendererToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem rigidBodyToolStripMenuItem1;
        private System.Windows.Forms.Button StopButton;
        private System.Windows.Forms.Button PauseButton;
        private System.Windows.Forms.Button PlayButton;
        private System.Windows.Forms.ToolStripMenuItem geometryToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem cubeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem planeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem sphereToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem cylinderToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem coneToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem torusToolStripMenuItem;
        private System.Windows.Forms.TabPage AssetTab;
        private System.Windows.Forms.SplitContainer AssetSplitContainer;
        private System.Windows.Forms.TreeView AssetDirectoryTreeView;
        private System.Windows.Forms.ListView AssetListView;
    }
}

