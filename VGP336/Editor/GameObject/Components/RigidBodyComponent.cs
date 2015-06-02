using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using System.Diagnostics;

namespace Editor
{
    [DescriptionAttribute("RigidBody"),
    CategoryAttribute("Components")]
    public class RigidBodyComponent : Component
    {
        #region accessors
        [CategoryAttribute("RigidBody")]
        public float Mass
        {
            get { return (float)GetField("Mass").value; }
            set { GetField("Mass").value = value; }
        }

        [CategoryAttribute("RigidBody")]
        public float Drag
        {
            get { return (float)GetField("Drag").value; }
            set { GetField("Drag").value = value; }
        }

        [CategoryAttribute("RigidBody")]
        public float AngularDrag
        {
            get { return (float)GetField("AngularDrag").value; }
            set { GetField("AngularDrag").value = value; }
        }

        [CategoryAttribute("RigidBody")]
        public bool UseGravity
        {
            get { return (bool)GetField("Use Gravity").value; }
            set { GetField("Use Gravity").value = value; }
        }

        [CategoryAttribute("RigidBody")]
        public bool IsKinematic
        {
            get { return (bool)GetField("Is Kinematic").value; }
            set { GetField("Is Kinematic").value = value; }
        }
        #endregion accessors

        public RigidBodyComponent()
        {
            base.DisplayName = "RigidBody";
            base.Name = "RigidBodyComponent";
            base.Description = "RigidBody Component";
        }
    }
}
