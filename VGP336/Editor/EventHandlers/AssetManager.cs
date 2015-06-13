using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Drawing;
using System.Diagnostics;

namespace Editor
{
    internal struct AssetItem
    {
        public string filename;
        public DateTime lastModified; // Used for diffing
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
        private EditorForm owner;
        private TreeView assetTreeView;
        private ListView assetListView;
        private ContextMenuStrip assetViewContextMenu;

        private ImageList assetThumbnails;
        private TrackBar thumbnailSizeSlider;
        private List<Size> thumbnailSizes;
        private EThumbnailState thumbnailState;

        Dictionary<int, List<AssetItem>> assetMap;
        List<AssetItem> currentAssetItems;


        public AssetManager(EditorForm owner)
        {
            this.owner = owner;

            assetTreeView = owner.AssetDirectoryTreeView;
            assetTreeView.NodeMouseClick += OnNodeClicked;
            assetTreeView.AfterLabelEdit += AfterRenameItem;

            InitContextMenu();

            // Create a list of sizes to match thumbnail display state
            thumbnailSizes = new List<Size>();
            thumbnailSizes.Add(new Size(16, 16));
            thumbnailSizes.Add(new Size(32, 32));
            thumbnailSizes.Add(new Size(64, 64));
            thumbnailSizes.Add(new Size(128, 128));
            thumbnailState = EThumbnailState.List;

            // Init the image list that will hold our thumbails
            assetThumbnails = new ImageList();
            assetThumbnails.ImageSize = GetTBSize();
            
            assetListView = owner.AssetListView;
            assetListView.DragEnter += OnAssetListViewItemDragEnter;
            assetListView.DragDrop += OnAssetListViewItemDrag;
            assetListView.LargeImageList = assetThumbnails;
            assetListView.SmallImageList = assetThumbnails;
            assetListView.View = View.List;

            thumbnailSizeSlider = owner.ThumbnailSizeTrackback;
            thumbnailSizeSlider.Value = (int)thumbnailState;
            thumbnailSizeSlider.ValueChanged += OnThumbnailTrackbarValueChanged;

            assetMap = new Dictionary<int, List<AssetItem>>();
        }

        public void Populate(string assetPath)
        {
            TreeNode root = new TreeNode("Assets");
            root.Name = assetPath;
            root.Expand();

            Traverse(assetPath, ref root);
            assetTreeView.Nodes.Add(root);
            rootAssetPath = assetPath;
        }
        private void Traverse(string directory, ref TreeNode node)
        {
            // Get all the files in this directory, then pair the dir name hash with the thumbnails of the directory
            List<string> files = new List<string>(Directory.EnumerateFileSystemEntries(directory));
            int hash = directory.GetHashCode();
            //assetMap.Add(hash, LoadThumbnails(files));
            assetMap[hash] = LoadThumbnails(files);

            List<string> directories = new List<string>(Directory.EnumerateDirectories(directory));
            foreach (string dir in directories)
            {
                // Create a node for this directory
                string dirname = Path.GetFileName(dir);
                TreeNode dirnode = new TreeNode(dirname);
                dirnode.Name = dir;

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
                OnDisplayContextMenu(e.Node);
            }
            
            // Invalid input or same node was clicked; do nothing
            if (assetTreeView.SelectedNode != null && 
                assetTreeView.SelectedNode == e.Node)
            {
                return;
            }
            assetTreeView.SelectedNode = e.Node;

            // Clear the currently displayed items
            assetListView.Items.Clear();
            assetThumbnails.Images.Clear();

            int imageIndex = 0;
            int sizeIndex = (int)thumbnailState;
            string path = e.Node.Name;
            int hash = path.GetHashCode();
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
                // Use large icons for everything else since we manually scale the thumbnails
                assetListView.View = View.LargeIcon;
            }
            // Reload the thumbnails with the new size
            UpdateThumbnails();
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
                assetItem.lastModified = Directory.GetLastWriteTime(file);
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

        private Size GetTBSize()
        {
            return thumbnailSizes[(int)thumbnailState];
        }
        private bool IsValidPath(string path)
        {
            char[] invalidChars = Path.GetInvalidPathChars();
            foreach (char invalid in invalidChars)
            {
                if (path.Contains(invalid))
                {
                    return false;
                }
            }
            // Edge case since they're technically valid, but not on their own
            if (path == "" || path == " ")
            {
                return false;
            }
            return true;
        }

        private void OnAssetListViewItemDragEnter(object sender, DragEventArgs e)
        {
            e.Effect = DragDropEffects.Copy;
        }
        private void OnAssetListViewItemDrag(object sender, DragEventArgs e)
        {
            string importerPath = "../Tools/Importer/Importer.exe";
            string destPath = assetTreeView.SelectedNode.Name + "/";

            string[] files = (string[])e.Data.GetData(DataFormats.FileDrop);
            foreach (string file in files)
            {
                string ext = Path.GetExtension(file).ToLower();
                string filename = Path.GetFileNameWithoutExtension(file);

                // TODO: regex match against all supported types
                if (ext == "fbx" || ext == "x")
                {
                    ProcessStartInfo startInfo = new ProcessStartInfo();
                    startInfo.CreateNoWindow = true;
                    startInfo.WorkingDirectory = Directory.GetCurrentDirectory();
                    startInfo.Arguments = file + " " + destPath + filename + ".catm";

                    Process process = new Process();
                    process.StartInfo = startInfo;
                    process.Start();
                }
                else
                {
                    // Preserve the extension
                    try
                    {
                        File.Copy(file, destPath + Path.GetFileName(file));
                    } 
                    catch (Exception exception)
                    {
                        DialogResult res = MessageBox.Show(exception.Message);
                            //MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);
                        int i = 0;
                    }
                }
            }
            // Refresh asset view to show new items
        }

        #region Context menu
        private void OnDisplayContextMenu(TreeNode node)
        {
            if (node.Name == rootAssetPath)
            {
                // Disable renaming and deleting of the root folder
                assetViewContextMenu.Items[1].Enabled = false;
                assetViewContextMenu.Items[2].Enabled = false;
            }
            else
            {
                assetViewContextMenu.Items[1].Enabled = true;
                assetViewContextMenu.Items[2].Enabled = true;
            }
            assetViewContextMenu.Show(owner.GetMousePos());
        }
        private void InitContextMenu()
        {
            assetViewContextMenu = new ContextMenuStrip();
            assetViewContextMenu.Items.Add("Create subdirectory", null, OnCreateSubdirectoryClicked);
            assetViewContextMenu.Items.Add("Rename", null, OnRenameItemClicked);
            assetViewContextMenu.Items.Add("Delete");
            assetViewContextMenu.Items.Add("Refresh", null, OnRefreshItemClicked);
            assetViewContextMenu.Items.Add("Import Asset");
        }
        private void OnCreateSubdirectoryClicked(object sender, EventArgs e)
        {
            TreeNode node = new TreeNode();
            node.Text = "NewFolder"; // default
            string path = assetTreeView.SelectedNode.Name + "/" + node.Text;
            node.Name = path;
            assetMap.Add(path.GetHashCode(), new List<AssetItem>());
            Directory.CreateDirectory(path);

            // Add the new node as a child of the selected one
            int index = assetTreeView.SelectedNode.Nodes.Add(node);
            assetTreeView.SelectedNode.Nodes[index].BeginEdit();
        }
        private void OnRenameItemClicked(object sender, EventArgs e)
        {
            assetTreeView.SelectedNode.BeginEdit();
        }
        private void AfterRenameItem(object sender, NodeLabelEditEventArgs e)
        {
            string oldName = e.Node.Name;
            string newName = e.Label;
            if (newName == null || newName == "" || !IsValidPath(newName))
            {
                e.CancelEdit = true;
                return;
            }

            string path = e.Node.Name; // Get the current path from the node's name
            string dir = Directory.GetParent(path) + "/"; // This gives us the full path, but it still works
            string newPathName = dir + newName;
            Directory.Move(path, newPathName); // Rename the directory
            e.Node.Name = newPathName;

            // Move the asset items to the new key and remove the old one
            int oldHash = oldName.GetHashCode();
            List<AssetItem> assetItems = assetMap[oldHash];
            assetMap.Remove(oldHash);
            assetMap.Add(newPathName.GetHashCode(), assetItems);
        }
        private void OnRefreshItemClicked(object sender, EventArgs e)
        {
            string path = assetTreeView.SelectedNode.Name;
            List<string> files = new List<string>(Directory.EnumerateFileSystemEntries(path));

            // Reload the entire directory for now because i'm lazy
            // TODO: only add the new items
            int hash = path.GetHashCode();
            assetMap[hash].Clear();
            assetMap.Remove(hash);

            // Re-discover all directories under the selected one
            TreeNode node = assetTreeView.SelectedNode;
            node.Nodes.Clear();
            Traverse(path, ref node);
            assetTreeView.SelectedNode = node;
            
            // Update listview
        }
        #endregion Context menu
    }
}
