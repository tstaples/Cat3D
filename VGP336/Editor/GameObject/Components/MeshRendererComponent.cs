using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using System.Diagnostics;
using System.ComponentModel;
using System.Windows.Forms.Design;
using System.Drawing.Design;

namespace Editor
{
    [DescriptionAttribute("Mesh Renderer Component"),
    CategoryAttribute("Components")]
    public class MeshRendererComponent : Component
    {
        // TODO: https://social.msdn.microsoft.com/Forums/windows/en-US/07ad29f2-3040-4f1d-81c6-d55e0522afe7/property-grid-with-file-path?forum=winforms
        #region accessors
        [CategoryAttribute("Texture Path")]
        [Editor(typeof(FileNameEditor), typeof(UITypeEditor))]
        public string TexturePath
        {
            get { return (string)GetField("TexturePath").value; }
            set { GetField("TexturePath").value = value; }
        }
        #endregion accessors

        public MeshRendererComponent()
        {
            base.DisplayName = "Mesh Renderer";
            base.Name = "MeshRendererComponent";
            base.Description = "Mesh Renderer Component";
        }
    }
}
