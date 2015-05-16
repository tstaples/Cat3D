using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor
{
    public static class ComponentReader
    {
        public static Component[] GetComponents(byte[] data, uint size)
        {
            SerializeIn sIn = new SerializeIn(data);

            // Get GameObject data
            string objName = sIn.ReadStringLE();
            ushort handleIndex = sIn.ReadUShort();
            ushort handleInstance = sIn.ReadUShort();
            string name = sIn.ReadStringLE();
            uint numComponents = sIn.ReadUInt();
            Component[] components = new Component[numComponents];

            for (int i = 0; i < numComponents; ++i)
            {
                // Create a component and store it's handle info for later use
                string compName = sIn.ReadStringLE();
                Component component = CreateComponentFromName(compName);
                component.HandleIndex = handleIndex;
                component.HandleInstance = handleInstance;

                // Load the component's fields
                uint numFields = sIn.ReadUInt();
                Field[] fields = new Field[numFields];
                for (uint j = 0; j < numFields; ++j)
                {
                    string fname = sIn.ReadStringLE();
                    int ftype = sIn.ReadInt();
                    int fsize = sIn.ReadInt();
                    int foffset = sIn.ReadInt();
                    byte[] fdata= sIn.GetBlock(fsize);
                    fields[j] = new Field(fname, ftype, fsize, foffset, fdata);
                }
                component.Load(fields);
                components[i] = component;
            }
            return components;
        }

        public static byte[] WriteComponent(Component c)
        {
            byte[] buffer = new byte[2048];
            SerializeOut sOut = new SerializeOut(buffer);

            // Write handle info so the engine can find which gameobject this
            // component belongs to.
            sOut.Write(c.HandleIndex);
            sOut.Write(c.HandleInstance);
            sOut.WriteStringLE(c.Name);

            int numFields = c.Fields.Length;
            for (int i = 0; i < numFields; ++i)
            {
                Field field = c.Fields[i];
                sOut.WriteArray(field.data);
            }
            return buffer;
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
