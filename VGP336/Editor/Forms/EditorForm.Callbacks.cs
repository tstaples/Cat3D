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
    }
}
