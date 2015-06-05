using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace Editor
{
    public class AssetManager
    {
        private TreeView assetTreeView;
        private ListView assetListView;
        private string currentAssetPath;

        public AssetManager(ref TreeView assetTree, ref ListView assetList)
        {
            assetTreeView = assetTree;
            assetListView = assetList;
        }

        public void Populate(string assetPath)
        {
            TreeNode root = new TreeNode("Assets");
            root.Expand();

            Traverse(assetPath, ref root);
            assetTreeView.Nodes.Add(root);
            currentAssetPath = assetPath;
        }

        private void Traverse(string directory, ref TreeNode node)
        {
            List<string> directories = new List<string>(Directory.EnumerateDirectories(directory));
            foreach (string dir in directories)
            {
                // Create a node for this directory
                string dirname = Path.GetFileName(dir);
                TreeNode dirnode = new TreeNode(dirname);

                // Traverse this directory recursively
                Traverse(dir, ref dirnode);
                
                // Add this directory and it's children to the parent in node
                node.Nodes.Add(dirnode);
            }
        }
    }
}
