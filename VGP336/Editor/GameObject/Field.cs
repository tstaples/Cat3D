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

        public static Field Load(ref SerializeIn sIn)
        {
            Field field = new Field();
            field.name = sIn.ReadStringLE();
            field.type = sIn.ReadInt();
            field.size = sIn.ReadInt();
            field.offset = sIn.ReadInt();
            field.dataType = NativeTypes.GetType(field.type);

            NativeTypes.EType eType = (NativeTypes.EType)field.type;
            if (eType == NativeTypes.EType.String || eType == NativeTypes.EType.WString)
            {
                // Read strings as length encoded size their field size will not reflect their actual size.
                field.value = sIn.ReadStringLE();
            }
            else
            {
                // Works for all cases where member size is static
                byte[] fdata = sIn.GetBlock(field.size);
                field.value = NativeTypes.ConvertToType(fdata, field.type);
            }
            return field;
        }
    }
}
