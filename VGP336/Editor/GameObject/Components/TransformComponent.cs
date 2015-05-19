using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using System.Diagnostics;

namespace Editor
{
    // https://msdn.microsoft.com/en-us/library/aa302326.aspx
    [DescriptionAttribute("Object's Transform")]
    public class TransformComponent : Component
    {
        #region accessors
        [CategoryAttribute("Transform")]
        public Vector3 Position
        {
            get { return (Vector3)GetField("Position").value; }
            set { GetField("Position").value = value; }
        }

        [CategoryAttribute("Transform")]
        public Vector3 Rotation
        {
            get { return (Vector3)GetField("Rotation").value; }
            set { GetField("Rotation").value = value; }
        }

        [CategoryAttribute("Transform")]
        public Vector3 Scale
        {
            get { return (Vector3)GetField("Scale").value; }
            set { GetField("Scale").value = value; }
        }
        #endregion accessors

        public TransformComponent()
        {
            base.Name = "TransformComponent";
        }
    }
}
