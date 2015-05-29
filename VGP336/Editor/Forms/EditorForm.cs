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

namespace Editor
{
    public partial class EditorForm : Form
    {
        #region Accessors/members

        private Inspector inspector;
        private SceneHierarchy sceneHierarchy;
        private LevelManager levelManager;
        private ToolMenuHandler toolMenuHandler;

        public PropertyGrid InspectorGrid
        {
            get { return inspectorGrid; }
        }
        public SelectablePanel Viewport
        {
            get { return ViewPanel; }
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
        #endregion Accessors/members

        #region API calls/constructor
        public EditorForm()
        {
            InitializeComponent();

            // Get handles to current instance and window
            IntPtr hInstance = Marshal.GetHINSTANCE(this.GetType().Module);
            IntPtr hWnd = this.ViewPanel.Handle;

            // Initialize the engine within the view panel
            NativeMethods.Initialize(hInstance, IntPtr.Zero, hWnd, 1, this.ViewPanel.Width, this.ViewPanel.Height);

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
        }
        private void Terminate()
        {
            Console.LogInfo("Editor", "Shutting down");
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
            Console.LogInfo("Editor", "Shutting down: {0}", e.CloseReason);
            NativeMethods.Terminate();
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
        private void renameToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (SceneHierarchyTree.SelectedNode != null)
            {
                Console.LogDebug("Editor", "Beginning edit on node: {0} with Handle: {1}", SceneHierarchyTree.SelectedNode.Text, SceneHierarchyTree.SelectedNode.Tag);
                SceneHierarchyTree.SelectedNode.BeginEdit();
            }
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
    }
}
