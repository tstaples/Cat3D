using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Editor
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            EditorForm editorForm = new EditorForm();
            Application.Idle += new EventHandler(editorForm.OnIdle);
            Application.AddMessageFilter(new Editor.InputMessageFilter(editorForm));
            Application.Run(editorForm);
        }
    }
}
