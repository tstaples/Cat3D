﻿using System;
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
        private Inspector InspectorPanel;
        private SceneHierarchy SceneTree;

        public EditorForm()
        {
            InitializeComponent();

            // Get handles to current instance and window
            IntPtr hInstance = Marshal.GetHINSTANCE(this.GetType().Module);
            IntPtr hWnd = this.ViewPanel.Handle;

            // Initialize the engine within the view panel
            NativeMethods.Initialize(hInstance, IntPtr.Zero, hWnd, 1, this.ViewPanel.Width, this.ViewPanel.Height);

            ViewPanel.Focus();

            InspectorPanel = new Inspector(ref InspectorGrid);
            SceneTree = new SceneHierarchy(ref SceneHierarchyTree, ref InspectorPanel);
            SceneTree.Popualate();
        }

        private void Terminate()
        {
            NativeMethods.Terminate();
        }

        public void OnIdle(object sender, EventArgs e)
        {
            NativeMethods.UpdateFrame();
        }

        private void EditorForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            NativeMethods.Terminate();
        }

        public void OnResize(object sender, EventArgs e)
        {
            // TODO
        }

        public bool IsViewPortFocused()
        {
            // Offset cursor by window position to get relative position
            Point offset = this.Location;
            Point mpos = new Point(MousePosition.X - offset.X, MousePosition.Y - offset.Y);
            return ViewPanel.Bounds.Contains(mpos);
        }

        private void InspectorGrid_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            object newVal = e.ChangedItem.Value;
            string compName = e.ChangedItem.Parent.Label;
            string fieldName = e.ChangedItem.Label;

            // Hack: NotifyParentPropertyAttribute(true) currently used to identify children
            if (e.ChangedItem.PropertyDescriptor.Attributes.Contains(new NotifyParentPropertyAttribute(true)))
            {
                // Change to correct values
                compName = e.ChangedItem.Parent.Parent.Label;
                fieldName = e.ChangedItem.Parent.Label;
                newVal = e.ChangedItem.Parent.Value;
            }
            InspectorPanel.OnComponentModified(compName, fieldName, newVal);
        }

        private void SceneHierarchyTree_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                // Display the right-click context menu strip above the selected item
                SceneHierarchyContextMenu.Show(MousePosition);
            }
            SceneTree.OnNodeSelected(e.Node);
        }

        private void newGameObjectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            byte[] buffer = new byte[2048];
            uint size = NativeMethods.CreateAndSelectGameObject(buffer);
            GameObject gameObject = GameObject.Deserialize(buffer, size);
            InspectorPanel.Display(gameObject);
            SceneTree.Popualate();
        }

        private void renameToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (SceneHierarchyTree.SelectedNode != null)
            {
                SceneHierarchyTree.SelectedNode.BeginEdit();
            }
        }

        private void SceneHierarchyTree_AfterLabelEdit(object sender, NodeLabelEditEventArgs e)
        {
            // TODO: Check label for invalid characters
            string newLabel = e.Label;
            SceneTree.RenameSelectedNode(newLabel);
        }
    }
}
