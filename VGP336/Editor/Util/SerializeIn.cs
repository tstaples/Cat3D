using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.Runtime.InteropServices;

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

        public T Read<T>()
        {
            var data = 0;
            if (typeof(T) == typeof(int))
            {
                data = BitConverter.ToInt32(buffer, offset);
            }
            else if (typeof(T) == typeof(short))
            {
                data = BitConverter.ToInt16(buffer, offset);
            }
            else
            {
                throw new Exception("Type unsupported");
            }
            offset += Marshal.SizeOf(typeof(T));
            return (T)Convert.ChangeType(data, typeof(T));
        }

        public string ReadStringLE()
        {
            int len = ReadInt();
            string s = GetString(buffer, offset, len);
            offset += len;
            return s;
        }

        public byte[] GetBlock(int size)
        {
            byte[] block = new byte[size];
            Buffer.BlockCopy(buffer, offset, block, 0, size);
            return block;
        }

        private int ReadInt()
        {
            int data = BitConverter.ToInt32(buffer, offset);
            offset += sizeof(int);
            return data;
        }

        private string GetString(byte[] buff, int offset, int length)
        {
            char[] chars = new char[length];
            int c = 0;
            for (int i = offset; i < (offset + length); ++i)
            {
                chars[c++] = (char)buff[i];
            }
            return new string(chars);
        }
    }
}
