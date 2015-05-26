using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;

namespace Editor
{
    [TypeConverterAttribute(typeof(ExpandableObjectConverter))]
    public class AABB
    {
        public Vector3 center;
        public Vector3 extend;

        [CategoryAttribute("Region")]
        public Vector3 Center
        {
            get { return center; }
            set { center = value; }
        }
        [CategoryAttribute("Region")]
        public Vector3 Extend
        {
            get { return extend; }
            set { extend = value; }
        }

        public static implicit operator AABB(byte[] bytes)
        {
            float[] m = new float[3];
            int size = 3 * sizeof(float);
            AABB aabb = new AABB();

            Buffer.BlockCopy(bytes, 0, m, 0, size);
            aabb.Center = new Vector3(m[0], m[1], m[2]);
            Buffer.BlockCopy(bytes, size, m, 0, size);
            aabb.Extend = new Vector3(m[0], m[1], m[2]);
            return aabb;
        }

        public byte[] ToBytes()
        {
            byte[] bytes = new byte[25];
            Buffer.BlockCopy(Center.ToBytes(), 0, bytes, 0, 12);
            Buffer.BlockCopy(Extend.ToBytes(), 0, bytes, 12, 12);
            return bytes;
        }
    }
}
