using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing.Design;
using System.Windows.Forms.Design;

namespace Editor
{
    // https://msdn.microsoft.com/en-us/library/aa302326.aspx
    [DescriptionAttribute("A Terrain"),
    CategoryAttribute("Components")]
    public class TerrainComponent : Component
    {
        #region accessors
        [CategoryAttribute("Terrain")]
        public uint Width
        {
            get { return (uint)GetField("Width").value; }
            set { GetField("Width").value = value; }
        }
        [CategoryAttribute("Terrain")]
        public uint Length
        {
            get { return (uint)GetField("Length").value; }
            set { GetField("Length").value = value; }
        }
        [CategoryAttribute("Terrain")]
        public float MaxHeight
        {
            get { return (float)GetField("MaxHeight").value; }
            set { GetField("MaxHeight").value = value; }
        }
        [CategoryAttribute("Terrain")]
        [Editor(typeof(FileNameEditor), typeof(UITypeEditor))]
        public string HeightmapPath
        {
            get { return (string)GetField("HeightmapPath").value; }
            set { GetField("HeightmapPath").value = value; }
        }
        #endregion accessors

        public TerrainComponent()
        {
            base.DisplayName = "Terrain";
            base.Name = "TerrainComponent";
            base.Description = "Terrain Component";
        }
    }
}
