using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using System.Diagnostics;

namespace Editor
{
    [DescriptionAttribute("Mesh Component"),
    CategoryAttribute("Components")]
    public class MeshComponent : Component
    {
        // Reflects the enum types in MeshComponent.h
        public static string[] FilterNames =
        {
            "Cube",
            "Plane",
            "Sphere",
            "Cylinder",
            "Torus",
            "Cone"
        };

        #region accessors
        [TypeConverter(typeof(FilterNameConverter))]
        [CategoryAttribute("Filter")]
        public string Filter
        {
            get { return FilterNames[(uint)GetField("Filter").value]; }
            set {
                string v = value;
                for (uint i = 0; i < FilterNames.Length; ++i) {
                    if (v == FilterNames[i]) {
                        GetField("Filter").value = i;
                        break;
                    }
                }
            }
        }
        #endregion accessors

        public override void OnModify(string propertyName, object newVal)
        {
            Filter = (string)newVal;
        }

        public MeshComponent()
        {
            base.DisplayName = "Mesh";
            base.Name = "MeshComponent";
            base.Description = "Mesh Component";
        }
    }

    // https://msdn.microsoft.com/en-us/library/aa302326.aspx
    internal class FilterNameConverter : StringConverter
    {
        public override bool GetStandardValuesSupported(ITypeDescriptorContext context)
        {
            return true;
        }

        public override StandardValuesCollection GetStandardValues(ITypeDescriptorContext context)
        {
            return new StandardValuesCollection(MeshComponent.FilterNames);
        }

        public override bool GetStandardValuesExclusive(ITypeDescriptorContext context)
        {
            return false;
        }
    }
}
