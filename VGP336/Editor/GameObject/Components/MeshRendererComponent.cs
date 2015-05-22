using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using System.Diagnostics;

namespace Editor
{
    [DescriptionAttribute("Mesh Renderer Component"),
    CategoryAttribute("Components")]
    public class MeshRendererComponent : Component
    {
        #region accessors
        [CategoryAttribute("Texture Path")]
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
