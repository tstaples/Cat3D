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
            if (!Viewport.IsFocused)
            {
                return false;
            }

            byte[] data = new byte[2048];
            uint size = NativeMethods.GetSelectedObjectData(data, (uint)data.Length);
            Debug.Assert(size < 2048);
            if (size == 0 || size > 2048)
            {
                inspector.Clear();
                return false;
            }

            GameObject gameObject = GameObject.Deserialize(data, size);
            inspector.Display(gameObject);
            sceneHierarchy.SelectNode(gameObject.handle);
            return true;
        }
    }
}
