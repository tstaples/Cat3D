using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor
{
    public class ToolMenuHandler
    {
        private EditorForm owner;

        public ToolMenuHandler(EditorForm ownerForm)
        {
            owner = ownerForm;
        }

        public void OnCreateObject(string templateFile)
        {
            Console.LogDebug("Editor", "Creating new GameObject");

            GameObject gameObject = GameObject.CreateFromTemplate(templateFile);
            if (gameObject != null)
            {
                // Show the object's components in the inspector and update the scene hierarchy
                NativeMethods.SelectGameObject(gameObject.handle.ToNativeHandle());
                owner.Inspector.Display(gameObject);
                owner.SceneHierarchy.Popualate();
                owner.SceneHierarchy.SelectNode(gameObject.handle);
                owner.LevelManager.IsLevelDirty = true;
            }
            else
            {
                Console.LogError("Editor", "Failed to create new GameObject");
            }
        }
    }
}
