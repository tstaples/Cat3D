﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;

namespace Editor
{
    public class NativeTypes
    {
        // TODO: Load this in from meta file generated by the engine
        public enum EType
        {
            Int = 0,
            Uint = 1,
            Short = 2,
            UShort = 3,
            Char = 4,
            UChar = 5,
            Float = 6,
            Double = 7,
            Bool = 8,
            String = 9,
            Vector3 = 10,
            Matrix = 11,
            AABB = 12,
            Class = 13,
            Array = 14,
            Pointer = 15
        };

        public static Dictionary<EType, Type> Types = new Dictionary<EType, Type>()
        {
            { EType.Int, typeof(int) },
            { EType.Uint, typeof(uint) },
            { EType.Short, typeof(short) },
            { EType.UShort, typeof(ushort) },
            { EType.Char, typeof(sbyte) }, // using sbyte since char in c# is 16bit
            { EType.UChar, typeof(byte) },
            { EType.Float, typeof(float) },
            { EType.Double, typeof(double) },
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

        public static object ConvertToType(byte[] bytes, Type type)
        {
            if (type == typeof(int))
            {
                return BitConverter.ToInt32(bytes, 0);
            }
            if (type == typeof(uint))
            {
                return BitConverter.ToUInt32(bytes, 0);
            }
            if (type == typeof(short))
            {
                return BitConverter.ToInt16(bytes, 0);
            }
            if (type == typeof(ushort))
            {
                return BitConverter.ToUInt16(bytes, 0);
            }
            if (type == typeof(sbyte))
            {
                return Convert.ToSByte(bytes[0]);
            }
            if (type == typeof(byte))
            {
                return bytes[0];
            }
            else if (type == typeof(float))
            {
                return BitConverter.ToSingle(bytes, 0);
            }
            else if (type == typeof(double))
            {
                return BitConverter.ToDouble(bytes, 0);
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
            if (type == typeof(uint))
            {
                return BitConverter.GetBytes((uint)obj);
            }
            if (type == typeof(short))
            {
                return BitConverter.GetBytes((short)obj);
            }
            if (type == typeof(ushort))
            {
                return BitConverter.GetBytes((ushort)obj);
            }
            if (type == typeof(sbyte))
            {
                return BitConverter.GetBytes((sbyte)obj);
            }
            if (type == typeof(byte))
            {
                return BitConverter.GetBytes((byte)obj);
            }
            else if (type == typeof(float))
            {
                return BitConverter.GetBytes((float)obj);
            }
            else if (type == typeof(double))
            {
                return BitConverter.GetBytes((double)obj);
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
