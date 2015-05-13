using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;

namespace Editor
{
    // https://msdn.microsoft.com/en-us/library/aa302326.aspx
    [DescriptionAttribute("Object's Transform")]
    public class TransformComponent : Component
    {
        private Vector3 pos;
        private Vector3 rot;
        private Vector3 scale;

        [CategoryAttribute("Transform")]
        public Vector3 Position
        {
            get { return pos; }
            set { pos = value; }
        }

        [CategoryAttribute("Transform")]
        public Vector3 Rotation
        {
            get { return rot; }
            set { rot = value; }
        }

        [CategoryAttribute("Transform")]
        public Vector3 Scale
        {
            get { return scale; }
            set { scale = value; }
        }

        public TransformComponent()
        {
            Name = "Transform";
        }

        public override void Load(Field[] fields)
        {
            // Store fields in base class
            base.Load(fields);

            float[] m = new float[9];
            Buffer.BlockCopy(fields[0].data, 0, m, 0, 9 * sizeof(float));
            Buffer.BlockCopy(fields[1].data, 0, m, 3, 9 * sizeof(float));
            Buffer.BlockCopy(fields[2].data, 0, m, 6, 9 * sizeof(float));
            
            pos = new Vector3(m[0], m[1], m[2]);
            rot = new Vector3(m[3], m[4], m[5]);
            scale = new Vector3(m[6], m[7], m[8]);
        }

        public override void OnModify(string propertyName, object newVal)
        {
            Vector3 v = (Vector3)newVal;
            if (propertyName == "Position")
            {
                Position = v;
            }
            else if (propertyName == "Rotation")
            {
                Rotation = v;
            }
            else if (propertyName == "Scale")
            {
                Scale = v;
            }
            // TODO: Reflect changes in field data
        }
    }
}
