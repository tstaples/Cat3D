using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Data;

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

        public void Clear()
        {
            CurrentGameObject = null;
        }

        public void Display(GameObject gameObject)
        {
            Clear();
            CurrentGameObject = gameObject;
            GridView.SelectedObjects = CurrentGameObject.Components.ToArray();
        }

        public void OnComponentModified(string name, string propertyName, object newVal)
        {
            // Name in property grid omits the "Component" part
            string fullname = name + "Component";
            Component c = CurrentGameObject.GetComponent(fullname);
            c.OnModify(propertyName, newVal);

            byte[] buffer = CurrentGameObject.WriteComponent(c);
            NativeMethods.UpdateComponent(buffer, (uint)buffer.Length);
            GridView.Refresh();
        }
    }
}
