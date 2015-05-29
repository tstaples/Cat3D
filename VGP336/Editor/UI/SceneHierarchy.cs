using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;

namespace Editor
{
    public class SceneHierarchy
    {
        private TreeView Tree;
        private Inspector InspectorPanel;
        private Dictionary<GameObject.Handle, GameObject> GameObjects;

        public SceneHierarchy(ref TreeView tree, ref Inspector inspector)
        {
            Tree = tree;
            InspectorPanel = inspector;
            GameObjects = new Dictionary<GameObject.Handle, GameObject>();
        }

        public void Clear()
        {
            Tree.Nodes.Clear();
            GameObjects.Clear();
        }

        public void ClearSelection()
        {
            Tree.SelectedNode = null;
        }

        public void Popualate()
        {
            // Clear any existing items
            Clear();

            byte[] buffer = new byte[2048];
            uint size = NativeMethods.DiscoverGameObjects(buffer, (uint)buffer.Length);
            SerializeIn sIn = new SerializeIn(buffer);

            uint numObjects = sIn.ReadUInt();
            Console.LogDebug("SceneHierarchy", "discovered {0} GameObjects", numObjects);
            for (uint i=0; i < numObjects; ++i)
            {
                GameObject.Handle handle = new GameObject.Handle();
                handle.instance = sIn.ReadUShort();
                handle.index = sIn.ReadUShort();
                string name = sIn.ReadStringLE();

                // Map index to object for faster lookup, despite gameobject already holding index.
                GameObject gameObject = new GameObject(handle, name);
                GameObjects.Add(handle, gameObject);

                // Store the handle in the node's tag for identification
                TreeNode node = new TreeNode(name);
                node.Tag = handle;
                Tree.Nodes.Add(node);
            }
        }

        public void OnNodeSelected(TreeNode node)
        {
            // Hilight this node to show it has been selected
            Tree.SelectedNode = node;

            // Get the object's handle from the tag and look for it
            // tell inspector to display it
            GameObject.Handle handle = (GameObject.Handle)node.Tag;
            if (GameObjects.ContainsKey(handle))
            {
                // Get the data for this object
                byte[] buffer = new byte[2048];
                uint size = NativeMethods.GetGameObject(handle.ToNativeHandle(), buffer, 2048u);

                // Create an object on the editor side holding the info then tell the inspector to display it.
                GameObject gameObject = GameObject.Deserialize(buffer, size);
                InspectorPanel.Display(gameObject);
                
                // Tell the engine to set the currently selected object to the new one.
                NativeMethods.SelectGameObject(handle.ToNativeHandle());
            }
        }

        public void SelectNode(GameObject.Handle handle)
        {
            // TODO: find out why GameObjects.Contains(handle) isn't working
            // so we don't have to do a linear search.
            foreach (var pair in GameObjects)
            {
                if (pair.Key == handle)
                {
                    // TODO: make this work.
                    Tree.SelectedNode = FindNode(handle);
                    Debug.Assert(Tree.SelectedNode != null);
                }
            }
        }

        public void RenameSelectedNode(string name)
        {
            GameObject.Handle handle = (GameObject.Handle)Tree.SelectedNode.Tag;
            NativeMethods.RenameGameObject(handle.ToNativeHandle(), name);
        }

        private TreeNode FindNode(GameObject.Handle handle)
        {
            foreach (TreeNode node in Tree.Nodes)
            {
                GameObject.Handle nodeHandle = (GameObject.Handle)node.Tag;
                if (nodeHandle == handle)
                {
                    return node;
                }
            }
            return null;
        }
    }
}
