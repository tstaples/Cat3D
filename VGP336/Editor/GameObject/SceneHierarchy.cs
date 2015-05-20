using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Editor
{
    public class SceneHierarchy
    {
        private TreeView Tree;
        private Inspector InspectorPanel;
        private Dictionary<ushort, GameObject> GameObjects;

        public SceneHierarchy(ref TreeView tree, ref Inspector inspector)
        {
            Tree = tree;
            InspectorPanel = inspector;
            GameObjects = new Dictionary<ushort, GameObject>();
        }

        public void Clear()
        {
            Tree.Nodes.Clear();
            GameObjects.Clear();
        }

        public void Popualate()
        {
            // Clear any existing items
            Clear();

            byte[] buffer = new byte[2048];
            uint size = NativeMethods.DiscoverGameObjects(buffer);

            SerializeIn sIn = new SerializeIn(buffer);
            uint numObjects = sIn.ReadUInt();
            for (uint i=0; i < numObjects; ++i)
            {
                ushort hIndex = sIn.ReadUShort();
                ushort hInstance = sIn.ReadUShort();
                string name = sIn.ReadStringLE();

                // Map index to object for faster lookup, despite gameobject already holding index.
                GameObject gameObject = new GameObject(hIndex, hInstance, name);
                GameObjects.Add(hIndex, gameObject);

                // Hack: use index as key to identify the object later on
                TreeNode node = new TreeNode(name);
                node.Name = hIndex.ToString();
                Tree.Nodes.Add(node);
            }
        }

        public void OnNodeSelected(TreeNode node)
        {
            // Hilight this node to show it has been selected
            Tree.SelectedNode = node;

            // find node by name (TODO: ensure unique names during object creation)
            // use object handle etc. to look up component data
            // tell inspector to display it
            string name = node.Text;
            ushort index = Convert.ToUInt16(node.Name);
            if (GameObjects.ContainsKey(index))
            {
                // Get the data for this object
                byte[] buffer = new byte[2048];
                uint size = NativeMethods.GetGameObject(index, buffer);

                // Create an object on the editor side holding the info then tell the inspector to display it.
                GameObject gameObject = GameObject.Deserialize(buffer, size);
                InspectorPanel.Display(gameObject);
                
                // Tell the engine to set the currently selected object to the new one.
                NativeMethods.SelectGameObject(index);
            }
        }

        public void SelectNode(ushort index)
        {
            if (GameObjects.ContainsKey(index))
            {
                TreeNode[] nodes = Tree.Nodes.Find(index.ToString(), true);
                if (nodes.Length == 1)
                {
                    Tree.SelectedNode = nodes[0];
                }
            }
        }

        public void RenameSelectedNode(string name)
        {
            ushort index = Convert.ToUInt16(Tree.SelectedNode.Name);
            NativeMethods.RenameGameObject(index, name);
        }
    }
}
