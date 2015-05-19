﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.IO;

namespace Editor
{
    public class GameObject
    {
        public List<Component> Components;
        private ushort Index;
        private ushort Instance;
        private string Name;

        public GameObject()
        {
            Components = new List<Component>();
        }

        public GameObject(ushort index, ushort instance, string name)
        {
            Components = new List<Component>();
            Index = index;
            Instance = instance;
            Name = name;
        }

        public Component GetComponent(string name)
        {
            Component component = null;
            foreach (Component c in Components)
            {
                if (c.Name == name)
                {
                    component = c;
                    break;
                }
            }
            return component;
        }

        public void Deserialize(byte[] data, uint size)
        {
            SerializeIn sIn = new SerializeIn(data);

            // Get GameObject data
            Index = sIn.ReadUShort();
            Instance = sIn.ReadUShort();
            Name = sIn.ReadStringLE();
            
            uint numComponents = sIn.ReadUInt();
            for (int i = 0; i < numComponents; ++i)
            {
                // TODO: Find a way to not rely on comp name to instantiate it.
                // Ideally we wouldn't even need a TransformComponent class.
                string compName = sIn.ReadStringLE();
                Component component = CreateComponentFromName(compName);

                // Load the component's fields
                uint numFields = sIn.ReadUInt();
                Field[] fields = new Field[numFields];
                for (uint j = 0; j < numFields; ++j)
                {
                    Field field = new Field();
                    field.name = sIn.ReadStringLE();
                    field.type = sIn.ReadInt();
                    field.size = sIn.ReadInt();
                    field.offset = sIn.ReadInt();
                    field.dataType = Field.GetType(field.type);

                    // Use our hacky conversion method
                    byte[] fdata = sIn.GetBlock(field.size);
                    field.value = Field.ConvertToType(fdata, field.dataType);

                    fields[j] = field;
                }
                component.Fields = fields;
                Components.Add(component);
            }
        }

        public byte[] WriteComponent(Component c)
        {
            byte[] buffer = new byte[2048];
            SerializeOut sOut = new SerializeOut(buffer);

            // Write handle info so the engine can find which gameobject this
            // component belongs to.
            sOut.Write(Index);
            sOut.Write(Instance);
            sOut.WriteStringLE(c.Name);

            int numFields = c.Fields.Length;
            for (int i = 0; i < numFields; ++i)
            {
                Field field = c.Fields[i];
                byte[] fdata = Field.ConvertToBytes(field.value, field.dataType);
                sOut.WriteArray(fdata);
            }
            return buffer;
        }

        private Component CreateComponentFromName(string name)
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