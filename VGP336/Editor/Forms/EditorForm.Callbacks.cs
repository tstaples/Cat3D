using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor
{
    partial class EditorForm
    {
        public bool OnLeftClick()
        {
            byte[] data = new byte[2048];
            uint size = NativeMethods.GetSelectedObjectData(data);
            Component[] components = ComponentReader.GetComponents(data, size);

            // Clear any existing controls
            InspectorPanel.Clear();
            foreach (Component c in components)
            {
                InspectorPanel.AddComponent(c);
            }
            return true;
        }
    }
}
