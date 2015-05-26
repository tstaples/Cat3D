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
    public class SerializeOut
    {
        private byte[] buffer;
        private int offset;

        public SerializeOut(byte[] inbuffer)
        {
            buffer = inbuffer;
            offset = 0;
        }

        #region Native type write methods
        public void Write(int data)
        {
            byte[] bytes = BitConverter.GetBytes(data);
            Buffer.BlockCopy(bytes, 0, buffer, offset, sizeof(int));
            offset += sizeof(int);
            Debug.Assert(offset <= buffer.Length);
        }

        public void Write(short data)
        {
            byte[] bytes = BitConverter.GetBytes(data);
            Buffer.BlockCopy(bytes, 0, buffer, offset, sizeof(short));
            offset += sizeof(short);
            Debug.Assert(offset <= buffer.Length);
        }

        public void Write(ushort data)
        {
            byte[] bytes = BitConverter.GetBytes(data);
            Buffer.BlockCopy(bytes, 0, buffer, offset, sizeof(ushort));
            offset += sizeof(ushort);
            Debug.Assert(offset <= buffer.Length);
        }

        public void Write(float data)
        {
            byte[] bytes = BitConverter.GetBytes(data);
            Buffer.BlockCopy(bytes, 0, buffer, offset, sizeof(float));
            offset += sizeof(float);
            Debug.Assert(offset <= buffer.Length);
        }
        #endregion

        public void WriteStringLE(string s)
        {
            int len = s.Length;
            Buffer.BlockCopy(BitConverter.GetBytes(len), 0, buffer, offset, sizeof(int));
            offset += sizeof(int);
            // Use our own implementation to convert string to bytes since we want multibyte
            Buffer.BlockCopy(GetBytes(s), 0, buffer, offset, len);
            offset += len;
            Debug.Assert(offset <= buffer.Length);
        }

        public void WriteArray(byte[] arr)
        {
            Buffer.BlockCopy(arr, 0, buffer, offset, arr.Length);
            offset += arr.Length;
            Debug.Assert(offset <= buffer.Length);
        }

        public byte[] GetBuffer()
        {
            return buffer;
        }

        // Converts string to multibyte array
        public static byte[] GetBytes(string s)
        {
            byte[] bytes = new byte[s.Length];
            int c = 0;
            for (int i = 0; i < s.Length; ++i)
            {
                bytes[c++] = (byte)s[i];
            }
            return bytes;
        }

        // Writes a byte array as a wide string
        public static byte[] GetWBytes(string s)
        {
            // Length is doubled since we're inserting a null every other character
            int length = s.Length * 2;
            byte[] bytes = new byte[length];
            int c = 0;
            for (int i = 0; i < s.Length; ++i)
            {
                bytes[c++] = (byte)s[i];
                bytes[c++] = (byte)'\0';
            }
            return bytes;
        }
    }
}
