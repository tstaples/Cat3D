using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;

namespace Editor
{
    public class NativeTypes
    {
        public enum EType
        {
            Int = 0,
            Float = 1,
            Bool = 2,
            String = 3,
            Vector3 = 4,
            Matrix = 5,
            AABB = 6,
            Class = 7,
            Array = 8,
            Pointer = 9
        };

        public static Dictionary<EType, Type> Types = new Dictionary<EType, Type>()
        {
            { EType.Int, typeof(int) },
            { EType.Float, typeof(float) },
            { EType.Bool, typeof(bool) },
            { EType.String, typeof(string) },
            { EType.Vector3, typeof(Vector3) },
            { EType.Matrix, typeof(Matrix) },
            { EType.AABB, typeof(AABB) },
            { EType.Class, typeof(object) }, // todo
            { EType.Array, typeof(Array) },
            { EType.Pointer, typeof(object) } // todo
        };

        public static Type GetType(int t)
        {
            return Types[(EType)t];
        }

        public static bool IsNativeType(int type)
        {
            // Referring to enum value
            return (type > 3);
        }

        public static object ConvertToType(byte[] bytes, Type type)
        {
            if (type == typeof(int))
            {
                return BitConverter.ToInt32(bytes, 0);
            }
            else if (type == typeof(float))
            {
                return BitConverter.ToSingle(bytes, 0);
            }
            else if (type == typeof(bool))
            {
                return BitConverter.ToBoolean(bytes, 0);
            }
            else if (type == typeof(string))
            {
                return BitConverter.ToString(bytes, 0);
            }
            else if (type == typeof(Vector3))
            {
                Vector3 v = new Vector3();
                v = (Vector3)bytes;
                return v;
            }
            else if (type == typeof(AABB))
            {
                AABB aabb = new AABB();
                aabb = (AABB)bytes;
                return aabb;
            }
            return Convert.ChangeType(bytes, type);
        }

        public static byte[] ConvertToBytes(object obj, Type type)
        {
            if (type == typeof(int))
            {
                return BitConverter.GetBytes((int)obj);
            }
            else if (type == typeof(float))
            {
                return BitConverter.GetBytes((float)obj);
            }
            else if (type == typeof(bool))
            {
                return BitConverter.GetBytes((bool)obj);
            }
            else if (type == typeof(string))
            {
                return SerializeOut.GetBytes(obj.ToString());
            }
            else if (type == typeof(Vector3))
            {
                Vector3 v = (Vector3)obj;
                return v.ToBytes();
            }
            else if (type == typeof(AABB))
            {
                AABB aabb = (AABB)obj;
                return aabb.ToBytes();
            }
            Debug.Assert(false);
            return null;
        }
    }
}
