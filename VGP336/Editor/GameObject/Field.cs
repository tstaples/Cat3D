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
        public byte[] data;

        public Field()
        {
        }

        public Field(string fname, int ftype, int fsize, int foffset, byte[] fdata)
        {
            name = fname;
            type = ftype;
            size = fsize;
            offset = foffset;
            data = fdata;
        }
    }
}
