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

        public void Popualate()
        {
            byte[] buffer = new byte[2048];
            uint size = NativeMethods.DiscoverGameObjects(buffer);

            SerializeIn sIn = new SerializeIn(buffer);
            uint numObjects = sIn.ReadUInt();
            for (uint i=0; i < numObjects; ++i)
            {
                ushort hIndex = sIn.ReadUShort();
                ushort hInstance = sIn.ReadUShort();
                string name = sIn.ReadStringLE();

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
            // find node by name (ensure unique names during object creation)
            // use object handle etc. too look up component data
            // tell inspector to display it
            string name = node.Text;
            ushort index = Convert.ToUInt16(node.Name);
            if (GameObjects.ContainsKey(index))
            {
                byte[] buffer = new byte[2048];
                uint size = NativeMethods.GetGameObject(index, buffer);
            }
        }
    }
}
