using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using System.Diagnostics;

namespace Editor
{
    [DescriptionAttribute("Camera"),
    CategoryAttribute("Components")]
    public class CameraComponent : Component
    {
        #region accessors
        [CategoryAttribute("Camera")]
        public float Fov
        {
            get { return (float)GetField("Fov").value; }
            set { GetField("fov").value = value; }
        }

        [CategoryAttribute("Camera")]
        public float NearPlane
        {
            get { return (float)GetField("Near").value; }
            set { GetField("Near").value = value; }
        }

        [CategoryAttribute("Camera")]
        public float FarPlane
        {
            get { return (float)GetField("Far").value; }
            set { GetField("Far").value = value; }
        }
        #endregion accessors

        public CameraComponent()
        {
            base.DisplayName = "Camera";
            base.Name = "CameraComponent";
            base.Description = "Camera Component";
        }
    }
}
