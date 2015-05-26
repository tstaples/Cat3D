using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace Editor
{
    public class SerializeIn
    {
        private byte[] buffer;
        private int offset;

        public SerializeIn(byte[] inbuffer)
        {
            buffer = inbuffer;
            offset = 0;
        }

        //public T Read<T>()
        //{
        //    var data = 0;
        //    if (typeof(T) == typeof(int))
        //    {
        //        data = BitConverter.ToInt32(buffer, offset);
        //    }
        //    else if (typeof(T) == typeof(short))
        //    {
        //        data = BitConverter.ToInt16(buffer, offset);
        //    }
        //    else if (typeof(T) == typeof(ushort))
        //    {
        //        data = BitConverter.ToUInt16(buffer, offset);
        //    }
        //    else
        //    {
        //        throw new Exception("Type unsupported");
        //    }
        //    offset += Marshal.SizeOf(typeof(T));
        //    return (T)Convert.ChangeType(data, typeof(T));
        //}

        public string ReadStringLE()
        {
            int len = ReadInt();
            string s = GetString(buffer, offset, len);
            offset += len;
            Debug.Assert(offset <= buffer.Length);
            return s;
        }

        public byte[] GetBlock(int size)
        {
            byte[] block = new byte[size];
            Buffer.BlockCopy(buffer, offset, block, 0, size);
            offset += size;
            Debug.Assert(offset <= buffer.Length);
            return block;
        }

        public int ReadInt()
        {
            int data = BitConverter.ToInt32(buffer, offset);
            offset += sizeof(int);
            Debug.Assert(offset <= buffer.Length);
            return data;
        }

        public uint ReadUInt()
        {
            uint data = BitConverter.ToUInt32(buffer, offset);
            offset += sizeof(uint);
            Debug.Assert(offset <= buffer.Length);
            return data;
        }

        public short ReadShort()
        {
            short data = BitConverter.ToInt16(buffer, offset);
            offset += sizeof(short);
            Debug.Assert(offset <= buffer.Length);
            return data;
        }

        public ushort ReadUShort()
        {
            ushort data = BitConverter.ToUInt16(buffer, offset);
            offset += sizeof(ushort);
            Debug.Assert(offset <= buffer.Length);
            return data;
        }

        public static string GetString(byte[] buff, int offset, int length)
        {
            char[] chars = new char[length];
            int c = 0;
            for (int i = offset; i < (offset + length); ++i)
            {
                char current = (char)buff[i];
                if (current != '\0')
                {
                    chars[c++] = current;
                }
            }
            char[] resizedStr = new char[c];
            Buffer.BlockCopy(chars, 0, resizedStr, 0, c * sizeof(char));
            return new string(resizedStr);
        }
    }
}
