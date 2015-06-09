using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.Drawing;

namespace Editor
{
    public partial class EditorForm : Form
    {
        #region Accessors/members

        private enum GameState { Playing, Paused, Stopped }

        private Inspector inspector;
        private SceneHierarchy sceneHierarchy;
        private LevelManager levelManager;
        private ToolMenuHandler toolMenuHandler;
        private AssetManager assetManager;

        private GameState gameState;
        private System.Timers.Timer updateTimer;
        private const double interval = (1.0 / 60.0) * 1000.0; // ms

        public PropertyGrid InspectorGrid
        {
            get { return inspectorGrid; }
        }
        public SelectablePanel Viewport
        {
            get { return EditorViewPanel; }
            private set { }
        }
        public Inspector Inspector
        {
            get { return inspector; }
        }
        public SceneHierarchy SceneHierarchy
        {
            get { return sceneHierarchy; }
        }
        public LevelManager LevelManager
        {
            get { return levelManager; }
        }
        public ListView AssetListView
        {
            get { return assetListView; }
        }
        public TreeView AssetDirectoryTreeView
        {
            get { return assetDirectoryTreeView; }
        }
        public TrackBar ThumbnailSizeTrackback
        {
            get { return thumnailSizeTrackbar; }
        }
        #endregion Accessors/members

        #region API calls/constructor
        public EditorForm()
        {
            InitializeComponent();

            // Get handles to current instance and window
            IntPtr hInstance = Marshal.GetHINSTANCE(this.GetType().Module);
            IntPtr EditorhWnd = this.EditorViewPanel.Handle;
            IntPtr GamehWnd = this.GameViewPanel.Handle;

            // Initialize the engine within the view panel
            NativeMethods.Initialize(hInstance, IntPtr.Zero, EditorhWnd, 1, this.EditorViewPanel.Width, this.EditorViewPanel.Height);

            // Load in metadata from the engine
            Meta.Initialize();

            Console.Initialize(ref ConsoleList);
            inspector = new Inspector(ref inspectorGrid);
            sceneHierarchy = new SceneHierarchy(ref SceneHierarchyTree, ref inspector);
            sceneHierarchy.Popualate();
            Console.LogInfo("Editor", "Successfully initialized");

            levelManager = new LevelManager(this, ref SaveFileBox, ref OpenFileBox);
            levelManager.IsLevelDirty = false;

            toolMenuHandler = new ToolMenuHandler(this);

            assetManager = new AssetManager(this);
            // TODO: Load from project path
            assetManager.Populate("../Data/");

            // Create the timer for updating the game when it is being played
            updateTimer = new System.Timers.Timer(interval);
            updateTimer.Elapsed += OnIdle;
            updateTimer.Enabled = false;
            gameState = GameState.Stopped;
        }
        private void Terminate()
        {
            Console.LogInfo("Editor", "Shutting down");
            if (gameState == GameState.Playing)
            {
                // Disable the timer so it doesn't potentially interrupt the shutdown
                updateTimer.Stop();
                gameState = GameState.Stopped;
            }
            NativeMethods.Terminate();
        }
        public void OnIdle(object sender, EventArgs e)
        {
            NativeMethods.UpdateFrame();
        }
        private void EditorForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (!levelManager.SaveIfLevelDirty())
            {
                // Cancel the close event
                e.Cancel = true;
            }
        }
        private void EditorForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            Terminate();
        }
        #endregion API calls/constructor

        #region Util
        public Point GetRelativeMousePos()
        {
            return this.PointToClient(MousePosition);
        }
        public Point GetMousePos()
        {
            return MousePosition;
        }
        public void UpdateTitleLevelStatus()
        {
            // levelname will be appended with a '*' if there are changes to the current level
            // that haven't been saved.
            string levelName = levelManager.CurrentLevel;
            if (levelManager.IsLevelDirty)
            {
                levelName += "*";
            }
            this.Text = "Editor - " + levelName;
        }
        public bool IsEditorViewportActive()
        {
            return (ViewportTabControl.SelectedIndex == 0);
        }
        #endregion Util

        private void ConsoleList_Click(object sender, EventArgs e)
        {
            ListViewItem item = ConsoleList.SelectedItems[0];
            ConsoleMessageDetailsLabel.Text = Console.GetMessageDetails(item);
        }

        #region Inspector
        private void InspectorGrid_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            object newVal = e.ChangedItem.Value;
            string compName = e.ChangedItem.Parent.Label;
            string fieldName = e.ChangedItem.Label;

            // No change
            if (newVal == e.OldValue)
            {
                return;
            }

            // Hack: NotifyParentPropertyAttribute(true) currently used to identify children
            if (e.ChangedItem.PropertyDescriptor.Attributes.Contains(new NotifyParentPropertyAttribute(true)))
            {
                // Change to correct values
                compName = e.ChangedItem.Parent.Parent.Label;
                fieldName = e.ChangedItem.Parent.Label;
                newVal = e.ChangedItem.Parent.Value;
            }
            inspector.OnComponentModified(compName, fieldName, newVal);
            levelManager.IsLevelDirty = true;
            Console.LogDebug("Editor", "{0}'s property: {1} changed from: {2} to {3}", compName, fieldName, e.OldValue, newVal);
        }
        private void OnRemoveComponent(object sender, EventArgs e)
        {
            string componentName = Inspector.SelectedComponentName;
            if (componentName != null)
            {
                if (Inspector.RemoveComponentFromCurrentObject(componentName))
                {
                    levelManager.IsLevelDirty = true;
                    Console.LogDebug("Editor", "Successfully removed {0}", componentName);
                }
                else
                {
                    Console.LogError("Editor", "Failed to removed {0}", componentName);
                }
            }
        }
        #endregion Inspector

        #region Scene Hierarchy
        private void SceneHierarchyTree_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                // Display the right-click context menu strip above the selected item
                SceneHierarchyContextMenu.Show(MousePosition);
                Console.LogDebug("Editor", "Showing SceneHierarchyContextMenu");
            }
            Console.LogDebug("Editor", "Node {0} with handle {1} selected", e.Node.Text, e.Node.Tag);
            sceneHierarchy.OnNodeSelected(e.Node);
        }
        private void SceneHierarchyTree_AfterLabelEdit(object sender, NodeLabelEditEventArgs e)
        {
            // TODO: Check label for invalid characters
            string newLabel = e.Label;
            // Label will be null if the user cancels the edit
            if (newLabel != null)
            {
                sceneHierarchy.RenameSelectedNode(newLabel);
            }
            levelManager.IsLevelDirty = true;
        }
        private void RenameToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (SceneHierarchyTree.SelectedNode != null)
            {
                Console.LogDebug("Editor", "Beginning edit on node: {0} with Handle: {1}", SceneHierarchyTree.SelectedNode.Text, SceneHierarchyTree.SelectedNode.Tag);
                SceneHierarchyTree.SelectedNode.BeginEdit();
            }
        }
        private void DeleteGameObjectMenuItem_Click(object sender, EventArgs e)
        {
            SceneHierarchy.DeleteSelectedNode();
            Inspector.Clear();
            levelManager.IsLevelDirty = true;
        }
        #endregion Scene Hierarchy

        #region Tool Menu Strip
        private void OnCreateNewGameObject(object sender, EventArgs e)
        {
            string templateFile = "../Data/GameObjects/default.json";
            toolMenuHandler.OnCreateObject(templateFile);
        }
        private void OnCreateCamera(object sender, EventArgs e)
        {
            string templateFile = "../Data/GameObjects/camera.json";
            toolMenuHandler.OnCreateObject(templateFile);
        }
        private void OnCreateGeometry(object sender, EventArgs e)
        {
            string geoName = sender.ToString().ToLower();
            string templateFile = "../Data/GameObjects/" + geoName + ".json";
            toolMenuHandler.OnCreateObject(templateFile);
        }
        private void OnCreateTerrain(object sender, EventArgs e)
        {
            string templateFile = "../Data/GameObjects/defaultTerrain.json";
            toolMenuHandler.OnCreateObject(templateFile);
        }
        private void OnComponentMenuItem_Click(object sender, EventArgs e)
        {
            // Hack: assuming the component name on the dropdown + "Component" is the meta name
            string componentName = sender.ToString();
            if (inspector.AddComponentToCurrentObject(componentName))
            {
                levelManager.IsLevelDirty = true;
                Console.LogInfo("Editor", "Added {0} Component to selected GameObject", componentName);
            }
            else
            {
                Console.LogError("Editor", "Failed to add {0} Component to selected GameObject", componentName);
            }
        }
        #endregion Tool Menu Strip

        #region Level events
        public void OnNewLevel(object sender, EventArgs e)
        {
            levelManager.OnNewLevel();
        }
        public void OnSaveLevelAs(object sender, EventArgs e)
        {
            levelManager.OnSaveLevelAs();
        }
        public void OnSaveLevel(object sender, EventArgs e)
        {
            levelManager.OnSaveLevel();
        }
        public void OnOpenLevel(object sender, EventArgs e)
        {
            levelManager.OnOpenLevel();
        }
        #endregion Level events

        #region Play in editor
        private void PlayButton_Click(object sender, EventArgs e)
        {
            if (gameState == GameState.Playing)
                return;

            // Remove the idle event since we only want our timer to call it
            Application.Idle -= this.OnIdle;
            gameState = GameState.Playing;
            updateTimer.Enabled = true;
            NativeMethods.StartGame();
        }

        private void PauseButton_Click(object sender, EventArgs e)
        {
            if (gameState == GameState.Paused)
                return;

            if (gameState == GameState.Playing)
            {
                // Only restore idle event if the game was being played.
                // This prevents adding it multiple times
                Application.Idle += this.OnIdle;
            }
            gameState = GameState.Paused;
            updateTimer.Enabled = false;
            NativeMethods.PauseGame();
        }

        private void StopButton_Click(object sender, EventArgs e)
        {
            if (gameState == GameState.Stopped)
                return;

            if (gameState == GameState.Playing)
            {
                // Only restore idle event if the game was being played.
                // This prevents adding it multiple times
                Application.Idle += this.OnIdle;
            }

            // Store the handle of the currenlty selected object so we can re-select it after
            GameObject.Handle selectedHandle = new GameObject.Handle();
            if (Inspector.IsAnObjectSelected())
            {
                selectedHandle = Inspector.CurrentGameObject.handle;
                // Hack: update instance count to match the new handle after the level is re-loaded
                selectedHandle.instance++;
            }

            gameState = GameState.Stopped;
            updateTimer.Enabled = false;
            NativeMethods.StopGame();
            
            // Update the scene hierarchy since the old items will be invalid
            SceneHierarchy.Popualate();
            if (selectedHandle.IsValid())
            {
                // Re-select the previously selected object
                NativeMethods.SelectGameObject(selectedHandle.ToNativeHandle());
            }
        }
        #endregion Play in editor

        //private void tabControl1_DrawItem(object sender, DrawItemEventArgs e)
        //{

        //}
        //private void OnDrawTabControl(object sender, DrawItemEventArgs e)
        //{
        //    Color bg = Color.FromArgb(0, 64, 64, 64);
        //    // fill in the whole rect
        //    SolidBrush br = new SolidBrush(Color.Red);
        //    e.Graphics.FillRectangle(br, ClientRectangle);

        //    // draw the tabs
        //    for (int i = 0; i < SceneHierarchyTabControl.TabPages.Count; ++i)
        //    {
        //        TabPage tab = SceneHierarchyTabControl.TabPages[i];
        //        // Get the text area of the current tab
        //        RectangleF tabTextArea = (RectangleF)SceneHierarchyTabControl.GetTabRect(i);

        //        // determine how to draw the tab based on which type of tab it is
        //        // draw the background
        //        e.Graphics.FillRectangle(br, tabTextArea); 

        //        // draw the tab header text
        //        SolidBrush brush = new SolidBrush(Color.WhiteSmoke);
        //        e.Graphics.DrawString(tab.Text, Font, brush, CreateTabHeaderTextRect(tabTextArea));
        //    }
        //}
        //private RectangleF CreateTabHeaderTextRect(RectangleF tabTextArea)
        //{
        //    tabTextArea.X += 3;
        //    tabTextArea.Y += 1;

        //    tabTextArea.Height -= 1;

        //    return tabTextArea;
        //}
    }
}
