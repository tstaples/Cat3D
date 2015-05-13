using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor
{


    public class Component
    {
        public string Name;
        public Field[] Fields;
        public ushort HandleIndex;
        public ushort HandleInstance;

        public virtual void Load(Field[] fields) 
        {
            Fields = fields;
        }

        public virtual void OnModify(string propertyName, object newVal)
        {

        }
    }
}
