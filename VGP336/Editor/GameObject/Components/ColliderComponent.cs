using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using System.Diagnostics;

namespace Editor
{
    [DescriptionAttribute("Object's Collider"),
    CategoryAttribute("Components")]
    public class ColliderComponent : Component
    {
        #region accessors
        [CategoryAttribute("Collider")]
        public Vector3 Center
        {
            get { return (Vector3)GetField("Center").value; }
            set { GetField("Center").value = value; }
        }

        [CategoryAttribute("Collider")]
        public Vector3 Extend
        {
            get { return (Vector3)GetField("Extend").value; }
            set { GetField("Extend").value = value; }
        }

        [CategoryAttribute("Collider")]
        public bool IsTrigger
        {
            get { return (bool)GetField("IsTrigger").value; }
            set { GetField("IsTrigger").value = value; }
        }
        #endregion accessors

        public ColliderComponent()
        {
            base.DisplayName = "Collider";
            base.Name = "ColliderComponent";
            base.Description = "Collider Component";
        }
    }
}
