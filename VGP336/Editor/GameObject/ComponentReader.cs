using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor
{
    public static class ComponentReader
    {
        private static Dictionary<string, Type> ComponentTypes = new Dictionary<string,Type>();

        static ComponentReader()
        {
            ComponentTypes.Add("TransformComponent", typeof(TransformComponent));
        }

        public static Component[] GetComponents(byte[] data, uint size)
        {
            SerializeIn sIn = new SerializeIn(data);

            string objName = sIn.ReadStringLE();
            short handleIndex = sIn.Read<short>();
            int numComponents = sIn.Read<int>();
            Component[] components = new Component[numComponents];

            for (int i = 0; i < numComponents; ++i)
            {
                string compName = sIn.ReadStringLE();
                Component component = CreateComponentFromName(compName);

                int numFields = sIn.Read<int>();
                Field[] fields = new Field[numFields];
                for (int j = 0; j < numFields; ++j)
                {
                    string fname = sIn.ReadStringLE();
                    int ftype = sIn.Read<int>();    // TODO: convert to local enum of types matching c++ ones
                    int fsize = sIn.Read<int>();
                    byte[] fdata= sIn.GetBlock(fsize);
                    fields[j] = new Field(fname, ftype, fsize, fdata);
                }
                component.Load(fields);
                components[i] = component;
            }
            return components;
        }

        private static Component CreateComponentFromName(string name)
        {
            Component component = null;
            if (name.CompareTo("TransformComponent") != -1)
            {
                component = new TransformComponent();
            }
            return component;
        }
    }
}
