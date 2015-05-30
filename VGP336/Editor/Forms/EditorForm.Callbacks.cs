using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using System.Windows.Forms;
using System.Drawing;
using System.ComponentModel;

namespace Editor
{
    partial class EditorForm
    {
        public bool OnViewportFocus(Keys key)
        {
            // Update the viewport's focus flag since it can't seem to do it itself
            Point mpos = GetRelativeMousePos();
            Viewport.IsFocused = Viewport.Contains(mpos);
            Console.LogDebug("Editor", "Viewport focused: {0}", Viewport.IsFocused);
            return false; // Don't want to consume the input
        }

        public bool OnSelectObject(Keys key)
        {
            // We only care the the user clicked inside the viewport
            if (!Viewport.IsFocused)
            {
                return false;
            }

            byte[] data = new byte[2048];
            uint size = NativeMethods.GetSelectedObjectData(data, (uint)data.Length);
            Debug.Assert(size < 2048);
            if (size == 0 || size > 2048)
            {
                // Nothing was selected; de-select everything
                inspector.Clear();
                sceneHierarchy.ClearSelection();
                return false;
            }

            // Check to make sure the object that was selected wasn't aready selected (ie. moving gizmo)
            GameObject gameObject = GameObject.Deserialize(data, size);
            GameObject currentSelectedGameObject = inspector.CurrentGameObject;
            if (currentSelectedGameObject == null || 
                gameObject.handle != currentSelectedGameObject.handle)
            {
                inspector.Display(gameObject);
                sceneHierarchy.SelectNode(gameObject.handle);
                return true;
            }
            return false;
        }

        public bool OnRightClickInspector(Keys key)
        {
            // No component selected
            Inspector.SelectedComponentName = null;
            
            if (!Viewport.IsFocused)
            {
                // Clear any existing items so duplicated aren't added
                InspectorContextMenu.Items.Clear();

                // Check if the click was within the property grid
                Point p = InspectorGrid.PointToClient(MousePosition);
                if (!InspectorGrid.ClientRectangle.Contains(p))
                {
                    return false;
                }

                // Check if a valid control was selected
                Control control = InspectorGrid.GetChildAtPoint(p);
                if (control != null && Inspector.IsAnObjectSelected())
                {
                    // Get the component that was selected
                    PropertyDescriptor pd = InspectorGrid.SelectedGridItem.PropertyDescriptor;
                    if (pd != null) // Still need to check in case we click and invalid spot
                    {
                        string name = pd.PropertyType.Name;
                        Inspector.SelectedComponentName = name;
                        // Check an actual component was selected
                        if (Inspector.CurrentGameObject.GetComponent(name) == null)
                        {
                            return false;
                        }

                        ToolStripMenuItem item = new ToolStripMenuItem("Remove Component", null, OnRemoveComponent);
                        // TransformComponent is not removeable for now
                        if (name == "TransformComponent")
                        {
                            item.Enabled = false;
                        }
                        // Show the context menu
                        InspectorContextMenu.Items.Add(item);
                        InspectorContextMenu.Show(MousePosition);
                        return false; // Don't consume or the menu option won't select properly
                    }
                }
            }
            return false;
        }

        public bool OnDeleteGameObject(Keys key)
        {
            if (key == Keys.Delete)
            {
                // Ensure the command isn't meant for another window, ie. deleting an asset
                if (Inspector.IsAnObjectSelected() &&
                   (InspectorGrid.Focused || Viewport.IsFocused || SceneHierarchyTree.Focused) &&
                    IsEditorViewportActive())
                {
                    SceneHierarchy.DeleteSelectedNode();
                    Inspector.Clear();
                    levelManager.IsLevelDirty = true;
                    return true;
                }
            }
            return false;
        }
    }
}
