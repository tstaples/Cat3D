using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;

namespace Editor
{
    partial class EditorForm
    {
        public bool OnLeftClick()
        {
            byte[] data = new byte[2048];
            uint size = NativeMethods.GetSelectedObjectData(data);
            Debug.Assert(size < 2048);

            GameObject gameObject = new GameObject();
            gameObject.Deserialize(data, size);
            InspectorPanel.Display(gameObject);
            return true;
        }
    }
}
