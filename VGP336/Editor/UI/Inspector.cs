using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Data;
using System.Diagnostics;

namespace Editor
{
    public class Inspector
    {
        private PropertyGrid GridView;
        private GameObject currentGameObject; // Currently displayed in panel

        public GameObject CurrentGameObject
        {
            get { return currentGameObject; }
        }

        public Inspector(ref PropertyGrid gridView)
        {
            GridView = gridView;
        }

        public bool IsAnObjectSelected()
        {
            return (currentGameObject != null);
        }

        public void Clear()
        {
            // Don't display anything on the grid
            currentGameObject = null;
            GridView.SelectedObject = null;
            GridView.Refresh();
        }

        public void Display(GameObject gameObject)
        {
            //bool sameObject = (GameObject == GridView.SelectedObject);

            Clear();
            currentGameObject = gameObject;
            GridView.SelectedObject = currentGameObject.Components;

            // Expand each component
            GridItem root = GridView.SelectedGridItem;
            foreach (GridItem gridItem in GridView.SelectedGridItem.Parent.GridItems)
            {
                gridItem.Expanded = true;
            }
        }

        public bool OnComponentModified(string name, string propertyName, object newVal)
        {
            // Name in property grid omits the "Component" part
            
            string fullname = (name.Replace(" ", "")) + "Component";
            Component c = currentGameObject.GetComponent(fullname);
            Debug.Assert(c != null);
            c.OnModify(propertyName, newVal);

            byte[] buffer = currentGameObject.WriteComponent(c);
            if (NativeMethods.UpdateComponent(buffer, (uint)buffer.Length) != 0)
            {
                GridView.Refresh();
                return true;
            }
            return false;
        }

        public bool AddComponentToCurrentObject(string componentName)
        {
            // Make valid name
            string name = componentName.Replace(" ", "") + "Component";
            
            // See if there is anything selected to add a component to
            if (IsAnObjectSelected())
            {
                NativeTypes.Handle handle = currentGameObject.handle.ToNativeHandle();
                if (NativeMethods.AddComponent(handle, name) != 0)
                {
                    // Update to show the new component
                    byte[] buffer = new byte[2048];
                    uint size = NativeMethods.GetSelectedObjectData(buffer, (uint)buffer.Length);
                    GameObject gameObject = GameObject.Deserialize(buffer, size);
                    Display(gameObject);
                    return true;
                }
            }
            return false;
        }
    }
}
