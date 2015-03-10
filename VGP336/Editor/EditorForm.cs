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
        public EditorForm()
        {
            InitializeComponent();

            IntPtr hInstance = Marshal.GetHINSTANCE(this.GetType().Module);
            IntPtr hWnd = this.ViewPanel.Handle;

            NativeMethods.Initialize(hInstance, IntPtr.Zero, hWnd, 1, this.ViewPanel.Width, this.ViewPanel.Height);
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
    }
}
