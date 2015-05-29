using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Editor
{
    public class LevelManager
    {
        private EditorForm owner;
        private SaveFileDialog saveFileDialog;
        private OpenFileDialog openFileDialog;

        private string currentLevel;
        private bool isLevelDirty; // have changes been made since the last save

        public bool IsLevelDirty
        {
            get { return isLevelDirty; }
            set 
            { 
                isLevelDirty = value;
                owner.UpdateTitleLevelStatus();
            }
        }
        public string CurrentLevel
        {
            get { return currentLevel; }
            set { currentLevel = value; }
        }

        public LevelManager(EditorForm ownerForm, ref SaveFileDialog sfd, ref OpenFileDialog ofd)
        {
            owner = ownerForm;
            saveFileDialog = sfd;
            openFileDialog = ofd;

            currentLevel = "Untitled";
        }

        private bool SaveIfLevelDirty()
        {
            if (isLevelDirty)
            {
                DialogResult res = MessageBox.Show("Save changes to " + currentLevel + "?", "Save level",
                    MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);
                if (res == DialogResult.Yes)
                {
                    OnSaveLevel();
                    return true;
                }
                else if (res == DialogResult.Cancel)
                {
                    return false;
                }
            }
            return true;
        }

        public void OnNewLevel()
        {
            if (SaveIfLevelDirty())
            {
                currentLevel = "Untitled";
                if (NativeMethods.NewLevel(currentLevel) == NativeMethods.True)
                {
                    IsLevelDirty = true;
                }
            }
        }

        public void OnSaveLevelAs()
        {
            // Display input box to get destination path from user
            saveFileDialog.InitialDirectory = "../Data/Levels/";
            DialogResult res = saveFileDialog.ShowDialog();
            if (res == DialogResult.OK)
            {
                // TODO: check for valid level file
                string selectedLevel = saveFileDialog.FileName;
                if (NativeMethods.SaveLevel(selectedLevel) == NativeMethods.True)
                {
                    // Update the current level name
                    currentLevel = selectedLevel;
                    IsLevelDirty = false;
                    Console.LogInfo("Editor", "Level: {0} saved successfully", selectedLevel);
                }
                else
                {
                    Console.LogError("Editor", "Failed to save level: {0}", selectedLevel);
                }
            }
        }

        public void OnSaveLevel()
        {
            if (!IsLevelDirty)
            {
                return; // No changes made
            }

            if (currentLevel == "Untitled")
            {
                OnSaveLevelAs();
            }
            else
            {
                if (NativeMethods.SaveLevel(currentLevel) == NativeMethods.True)
                {
                    IsLevelDirty = false;
                    Console.LogInfo("Editor", "Level: {0} saved successfully", currentLevel);
                }
                else
                {
                    Console.LogError("Editor", "Failed to save level: {0}", currentLevel);
                }
            }
        }

        public void OnOpenLevel()
        {
            if (!SaveIfLevelDirty())
            {
                return; // User cancelled
            }

            // Display input box to get destination path from user
            DialogResult res = openFileDialog.ShowDialog();
            if (res == DialogResult.OK)
            {
                // TODO: check for valid level file
                string selectedLevel = openFileDialog.FileName;
                if (NativeMethods.LoadLevel(selectedLevel) == NativeMethods.True)
                {
                    currentLevel = selectedLevel;
                    IsLevelDirty = false;
                    owner.SceneHierarchy.Popualate();

                    Console.LogInfo("Editor", "Loaded Level: {0} successfully", selectedLevel);
                }
                else
                {
                    Console.LogError("Editor", "Failed to load level: {0}", selectedLevel);
                }
            }
        }
    }
}
