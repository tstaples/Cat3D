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
    internal struct AssetItem
    {
        public string filename;
        public Bitmap[] thumbnails;
    }

    public class AssetManager
    {
        private enum EThumbnailState
        {
            List    = 0,
            Small   = 1,
            Medium  = 2,
            Large   = 3
        }
        
        private string rootAssetPath;
        private TreeView assetTreeView;
        private ListView assetListView;

        private ImageList assetThumbnails;
        private TrackBar thumbnailSizeSlider;
        private List<Size> thumbnailSizes;
        private EThumbnailState thumbnailState;

        Dictionary<int, List<AssetItem>> assetMap;
        List<AssetItem> currentAssetItems;

        public AssetManager(EditorForm owner)
        {
            assetMap = new Dictionary<int, List<AssetItem>>();

            assetTreeView = owner.AssetDirectoryTreeView;
            assetTreeView.NodeMouseClick += OnNodeClicked;

            // Create a list of sizes to match thumbnail display state
            thumbnailSizes = new List<Size>();
            thumbnailSizes.Add(new Size(16, 16));
            thumbnailSizes.Add(new Size(32, 32));
            thumbnailSizes.Add(new Size(64, 64));
            thumbnailSizes.Add(new Size(128, 128));
            thumbnailState = EThumbnailState.Medium;

            // Init the image list that will hold our thumbails
            assetThumbnails = new ImageList();
            assetThumbnails.ImageSize = GetTBSize();
            
            assetListView = owner.AssetListView;
            assetListView.LargeImageList = assetThumbnails;
            assetListView.View = View.LargeIcon;

            thumbnailSizeSlider = owner.ThumbnailSizeTrackback;
            thumbnailSizeSlider.Value = (int)thumbnailState;
            thumbnailSizeSlider.ValueChanged += OnThumbnailTrackbarValueChanged;
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

                // Get all the files in this directory, then pair the dir name hash with the thumbnails of the directory
                List<string> files = new List<string>(Directory.EnumerateFiles(dir));
                int hash = dirname.GetHashCode();
                assetMap.Add(hash, LoadThumbnails(files));

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

            int imageIndex = 0;
            int sizeIndex = (int)thumbnailState;
            int hash = e.Node.Text.GetHashCode();
            currentAssetItems = assetMap[hash];
            foreach (AssetItem assetItem in currentAssetItems)
            {
                ListViewItem item = new ListViewItem();
                item.Text = assetItem.filename;
                item.ImageIndex = imageIndex++; // index corresponds to image list

                assetThumbnails.Images.Add(assetItem.thumbnails[sizeIndex]);
                assetListView.Items.Add(item);
            }
        }

        private void OnThumbnailTrackbarValueChanged(object sender, EventArgs e)
        {
            thumbnailState = (EThumbnailState)thumbnailSizeSlider.Value;
            if (thumbnailState == EThumbnailState.List)
            {
                assetListView.View = View.List;
            }
            else
            {
                assetListView.View = View.LargeIcon;
                
                // Reload the thumbnails with the new size
                UpdateThumbnails();
            }
        }

        private void UpdateThumbnails()
        {
            assetThumbnails.ImageSize = GetTBSize();

            int sizeIndex = (int)thumbnailState;
            foreach (AssetItem assetItem in currentAssetItems)
            {
                // Re-assign the thumbnail with the correct sized one
                assetThumbnails.Images.Add(assetItem.thumbnails[sizeIndex]);
            }
        }

        private List<AssetItem> LoadThumbnails(List<string> files)
        {
            List<AssetItem> assetItems = new List<AssetItem>();
            foreach (string file in files)
            {
                AssetItem assetItem = new AssetItem();
                assetItem.filename = Path.GetFileName(file);
                assetItem.thumbnails = new Bitmap[thumbnailSizes.Count];

                // Load each thumbnail
                for (int i = 0; i < thumbnailSizes.Count; ++i)
                {
                    Size tbSize = thumbnailSizes[i];
                    assetItem.thumbnails[i] = WindowsThumbnailProvider.GetThumbnail(file, tbSize.Width, tbSize.Height, ThumbnailOptions.None);
                }
                assetItems.Add(assetItem);
            }
            return assetItems;
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

        private Size GetTBSize()
        {
            return thumbnailSizes[(int)thumbnailState];
        }
    }
}
