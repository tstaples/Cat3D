using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace Editor
{
    public partial class EditorForm : Form
    {
        private Inspector InspectorPanel;

        public EditorForm()
        {
            InitializeComponent();

            // Get handles to current instance and window
            IntPtr hInstance = Marshal.GetHINSTANCE(this.GetType().Module);
            IntPtr hWnd = this.ViewPanel.Handle;

            // Initialize the engine within the view panel
            NativeMethods.Initialize(hInstance, IntPtr.Zero, hWnd, 1, this.ViewPanel.Width, this.ViewPanel.Height);

            ViewPanel.Focus();

            InspectorPanel = new Inspector(ref InspectorGrid);
        }

        private void Terminate()
        {
            NativeMethods.Terminate();
        }

        public void OnIdle(object sender, EventArgs e)
        {
            NativeMethods.UpdateFrame();
        }

        private void EditorForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            NativeMethods.Terminate();
        }

        public void OnResize(object sender, EventArgs e)
        {
            // TODO
        }

        public bool IsViewPortFocused()
        {
            // Offset cursor by window position to get relative position
            Point offset = this.Location;
            Point mpos = new Point(MousePosition.X - offset.X, MousePosition.Y - offset.Y);
            return ViewPanel.Bounds.Contains(mpos);
            //return ViewPanel.IsFocused;
        }

        private void InspectorGrid_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            string compName = e.ChangedItem.Parent.Label;
            InspectorPanel.OnComponentModified(compName, e.ChangedItem.Label, e.ChangedItem.Value);
        }
    }
}
