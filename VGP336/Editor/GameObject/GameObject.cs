using System;
using System.Collections.Generic;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.IO;
using System.ComponentModel;
using System.Runtime.InteropServices;

namespace Editor
{
    public class GameObject
    {
        public class Handle
        {
            // Init to invalid
            public ushort instance;
            public ushort index;

            public Handle()
            {
                instance = ushort.MaxValue;
                index = ushort.MaxValue;
            }
            public Handle(NativeTypes.Handle nativeHandle)
            {
                instance = nativeHandle.instance;
                index = nativeHandle.index;
            }

            public NativeTypes.Handle ToNativeHandle()
            {
                NativeTypes.Handle handle = new NativeTypes.Handle();
                handle.instance = instance;
                handle.index = index;
                return handle;
            }
            public bool IsValid()
            {
                return (instance != ushort.MaxValue && index != ushort.MaxValue);
            }
            public static bool operator==(Handle lhs, Handle rhs)
            {
                return (lhs.instance == rhs.instance && lhs.index == rhs.index);
            }
            public static bool operator!=(Handle lhs, Handle rhs)
            {
                return !(lhs == rhs);
            }
            public override bool Equals(object obj)
            {
                return base.Equals(obj);
            }
            public override int GetHashCode()
            {
                return base.GetHashCode();
            }
            public override string ToString()
            {
                return instance.ToString() + " " + index.ToString();
            }
        }

        public ComponentCollection components;
        public Handle handle;
        private string name;

        public GameObject()
        {
            Components = new ComponentCollection();
        }
        public GameObject(Handle handle, string name)
        {
            Components = new ComponentCollection();
            this.handle = handle;
            this.name = name;
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
        public static GameObject Deserialize(byte[] data, uint size)
        {
            SerializeIn sIn = new SerializeIn(data);

            // Get GameObject data
            Handle handle = new Handle();
            handle.instance = sIn.ReadUShort();
            handle.index = sIn.ReadUShort();
            string name = sIn.ReadStringLE();
            GameObject gameObject = new GameObject(handle, name);
            
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
                    fields[j] = Field.Load(ref sIn);
                }
                component.Fields = fields;
                gameObject.Components.Add(component);
            }
            return gameObject;
        }
        public byte[] WriteComponent(Component c)
        {
            byte[] buffer = new byte[2048];
            SerializeOut sOut = new SerializeOut(buffer);

            // Write handle info so the engine can find which gameobject this
            // component belongs to.
            sOut.Write(handle.instance);
            sOut.Write(handle.index);
            sOut.WriteStringLE(c.Name);

            int numFields = c.Fields.Length;
            for (int i = 0; i < numFields; ++i)
            {
                Field field = c.Fields[i];
                byte[] fdata = NativeTypes.ConvertToBytes(field.value, field.type);
                sOut.WriteArray(fdata);
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
            else if (name == "ColliderComponent")
            {
                component = new ColliderComponent();
            }
            else if (name == "MeshComponent")
            {
                component = new MeshComponent();
            }
            else if (name == "MeshRendererComponent")
            {
                component = new MeshRendererComponent();
            }
            return component;
        }

        [TypeConverter(typeof(ComponentCollectionConverter)),
        CategoryAttribute("Components")]
        public ComponentCollection Components
        {
            get { return components; }
            set { components = value; }
        }
    }
}
