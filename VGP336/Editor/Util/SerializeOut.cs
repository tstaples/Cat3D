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
    public class SerializeOut
    {
        private byte[] buffer;
        private int offset;

        public SerializeOut(byte[] inbuffer)
        {
            buffer = inbuffer;
            offset = 0;
        }

        public void Write(int data)
        {
            byte[] bytes = BitConverter.GetBytes(data);
            Buffer.BlockCopy(bytes, 0, buffer, offset, sizeof(int));
            offset += sizeof(int);
        }

        public void Write(short data)
        {
            byte[] bytes = BitConverter.GetBytes(data);
            Buffer.BlockCopy(bytes, 0, buffer, offset, sizeof(short));
            offset += sizeof(short);
        }

        public void Write(ushort data)
        {
            byte[] bytes = BitConverter.GetBytes(data);
            Buffer.BlockCopy(bytes, 0, buffer, offset, sizeof(ushort));
            offset += sizeof(ushort);
        }

        public void Write(float data)
        {
            byte[] bytes = BitConverter.GetBytes(data);
            Buffer.BlockCopy(bytes, 0, buffer, offset, sizeof(float));
            offset += sizeof(float);
        }

        public void WriteStringLE(string s)
        {
            int len = s.Length;
            Buffer.BlockCopy(BitConverter.GetBytes(len), 0, buffer, offset, sizeof(int));
            offset += sizeof(int);
            Buffer.BlockCopy(s.ToArray(), 0, buffer, offset, len);
            offset += len;
        }

        public void WriteArray(byte[] arr)
        {
            Buffer.BlockCopy(arr, 0, buffer, offset, arr.Length);
            offset += arr.Length;
        }

        public byte[] GetBuffer()
        {
            return buffer;
        }
    }
}
