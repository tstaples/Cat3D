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

        public Field GetField(string name)
        {
            foreach (Field field in Fields)
            {
                if (field.name == name)
                {
                    return field;
                }
            }
            return null;
        }

        public virtual void OnModify(string propertyName, object newVal)
        {
            GetField(propertyName).value = newVal;
        }
    }
}
