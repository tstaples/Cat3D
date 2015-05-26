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
        private GameObject CurrentGameObject; // Currently displayed in panel

        public Inspector(ref PropertyGrid gridView)
        {
            GridView = gridView;
        }

        public bool IsAnObjectSelected()
        {
            return (CurrentGameObject != null);
        }

        public void Clear()
        {
            // Don't display anything on the grid
            CurrentGameObject = null;
            GridView.SelectedObject = null;
            GridView.Refresh();
        }

        public void Display(GameObject gameObject)
        {
            //bool sameObject = (GameObject == GridView.SelectedObject);

            Clear();
            CurrentGameObject = gameObject;
            GridView.SelectedObject = CurrentGameObject.Components;

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
            Component c = CurrentGameObject.GetComponent(fullname);
            Debug.Assert(c != null);
            c.OnModify(propertyName, newVal);

            byte[] buffer = CurrentGameObject.WriteComponent(c);
            if (NativeMethods.UpdateComponent(buffer, (uint)buffer.Length) == 0)
            {
                GridView.Refresh();
                return true;
            }
            return false;
        }
    }
}
