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

namespace Editor
{
    public partial class EditorForm : Form
    {
        #region Accessors/members
        public SelectablePanel Viewport 
        { 
            get { return ViewPanel; } 
            private set {} 
        }

        private Inspector inspector;
        private SceneHierarchy sceneHierarchy;
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

            Console.Initialize(ref ConsoleList);
            inspector = new Inspector(ref InspectorGrid);
            sceneHierarchy = new SceneHierarchy(ref SceneHierarchyTree, ref inspector);
            sceneHierarchy.Popualate();
            Console.LogInfo("Editor", "Successfully initialized");
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

        private void EditorForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            Console.LogInfo("Editor", "Shutting down");
            NativeMethods.Terminate();
        }
        #endregion

        #region Util
        public Point GetRelativeMousePos()
        {
            return this.PointToClient(MousePosition);
        }
        #endregion Util

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
            Console.LogDebug("Editor", "{0}'s property: {1} changed from: {2} to {3}", compName, fieldName, e.OldValue, newVal);
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
        private void newGameObjectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Console.LogDebug("Editor", "Creating new GameObject");

            byte[] buffer = new byte[2048];
            uint size = NativeMethods.CreateNewGameObject(buffer, (uint)buffer.Length);
            if (size > 0)
            {
                // Select the newly created gameObject
                GameObject gameObject = GameObject.Deserialize(buffer, size);
                NativeMethods.SelectGameObject(gameObject.handle.ToNativeHandle());

                // Show the object's components in the inspector and update the scene hierarchy
                inspector.Display(gameObject);
                sceneHierarchy.Popualate();
            }
            else
            {
                Console.LogError("Editor", "Failed to create new GameObject");
            }
        }

        private void OnComponentMenuItem_Click(object sender, EventArgs e)
        {
            string componentName = sender.ToString();
        }
        #endregion Tool Menu Strip

        private void ConsoleList_Click(object sender, EventArgs e)
        {
            ListViewItem item = ConsoleList.SelectedItems[0];
            ConsoleMessageDetailsLabel.Text = Console.GetMessageDetails(item);
        }
    }
}
