using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Drawing;

namespace Editor
{
    public class AssetManager
    {
        private string rootAssetPath;
        private TreeView assetTreeView;
        private ListView assetListView;
        private ImageList assetThumbnails;
        private Size thumnailSize;

        public AssetManager(ref TreeView assetTree, ref ListView assetList)
        {
            assetTreeView = assetTree;
            assetTreeView.NodeMouseClick += OnNodeClicked;
            
            assetListView = assetList;

            thumnailSize = new Size(128, 128);
            assetThumbnails = new ImageList();
            assetThumbnails.ImageSize = thumnailSize;
        }

        public void Populate(string assetPath)
        {
            TreeNode root = new TreeNode("Assets");
            root.Expand();

            Traverse(assetPath, ref root);
            assetTreeView.Nodes.Add(root);
            rootAssetPath = assetPath + "/";
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

        public void OnNodeClicked(object sender, TreeNodeMouseClickEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                // TODO: show context menu
                return;
            }
            else if (e.Button != MouseButtons.Left)
            {
                return;
            }

            // Same node was clicked; do nothing
            if (assetTreeView.SelectedNode != null && assetTreeView.SelectedNode == e.Node)
            {
                return;
            }

            // Clear the currently displayed items
            assetListView.Items.Clear();
            assetThumbnails.Images.Clear();

            string pathToNode = rootAssetPath + GetPathToNode(e.Node);
            List<string> files = new List<string>(Directory.EnumerateFiles(pathToNode));
            int imageIndex = 0;
            foreach (string file in files)
            {
                ListViewItem item = new ListViewItem();
                item.Text = Path.GetFileName(file);
                item.ImageIndex = imageIndex++;

                Bitmap thumbnail = WindowsThumbnailProvider.GetThumbnail(file, thumnailSize.Width, thumnailSize.Height, ThumbnailOptions.None);
                assetThumbnails.Images.Add(thumbnail);

                assetListView.Items.Add(item);
            }
            assetListView.LargeImageList = assetThumbnails;
        }

        private string GetPathToNode(TreeNode node)
        {
            string pathToThisNode = "";
            TreeNode currentNode = node;
            while (currentNode.Parent != null)
            {
                pathToThisNode = currentNode.Text + "/" + pathToThisNode;
                currentNode = currentNode.Parent;
            }
            return pathToThisNode;
        }
    }
}
