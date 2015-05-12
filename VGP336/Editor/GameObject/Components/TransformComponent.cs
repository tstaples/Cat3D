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

            float[] m = new float[16];
            Buffer.BlockCopy(fields[0].data, 0, m, 0, 16 * sizeof(float));

            pos = new Vector3(m[12], m[13], m[14]);
            rot = new Vector3(0.0f, 0.0f, 0.0f); // zero for now until i figure out how to get rotation
            //NativeMethods.GetMatrixRotation(m, rot); // Probably wrong
            scale = new Vector3(m[0], m[5], m[10]);
        }
    }
}
