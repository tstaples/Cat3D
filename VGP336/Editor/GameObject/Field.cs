using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor
{
    public class Field
    {
        public string name;
        public int type;
        public int size;
        public int offset;

        public object value;
        public Type dataType;

        #region Native type conversion stuff
        public enum EType
        {
            Int = 0,
            Float = 1,
            Bool = 2,
            String = 3,
            Vector3 = 4,
            Matrix = 5,
            Class = 6,
            Array = 7,
            Pointer = 8
        };

        public static Dictionary<EType, Type> Types = new Dictionary<EType, Type>()
        {
            { EType.Int, typeof(int) },
            { EType.Float, typeof(float) },
            { EType.Bool, typeof(bool) },
            { EType.String, typeof(string) },
            { EType.Vector3, typeof(Vector3) },
            { EType.Matrix, typeof(Matrix) },
            { EType.Class, typeof(object) }, // todo
            { EType.Array, typeof(Array) },
            { EType.Pointer, typeof(object) } // todo
        };

        public static Type GetType(int t)
        {
            return Types[(EType)t];
        }

        public static object ConvertToType(byte[] bytes, Type type)
        {
            if (type == typeof(Vector3))
            {
                Vector3 v = new Vector3();
                v = (Vector3)bytes;
                return v;
            }
            // TODO: matrix case
            return Convert.ChangeType(bytes, type);
        }

        public static byte[] ConvertToBytes(object obj, Type type)
        {
            if (type == typeof(Vector3))
            {
                Vector3 v = (Vector3)obj;
                float[] m = new float[3] { v.x, v.y, v.z };
                byte[] bytes = new byte[12];
                Buffer.BlockCopy(m, 0, bytes, 0, 12);
                return bytes;
            }
            // Probably won't work
            return (byte[])obj;
        }
        #endregion Native type conversion stuff
    }
}
