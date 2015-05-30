using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;

namespace Editor
{
    public class Meta
    {
        public class MetaClass
        {
            public string name;
            public Field[] fields;
            public MetaDependency[] dependencies;
        }

        public class MetaDependency
        {
            public string name;
            public string tag;
        }

        private static MetaClass[] metaClasses;

        public static void Initialize()
        {
            byte[] buffer =  new byte[8096];
            uint size = NativeMethods.GetMetaData(buffer, (uint)buffer.Length);
            Debug.Assert(size > 0);

            SerializeIn sIn = new SerializeIn(buffer);
            uint numClasses = sIn.ReadUInt();
            metaClasses = new MetaClass[numClasses];
            for (uint i=0; i < numClasses; ++i)
            {
                MetaClass metaClass = new MetaClass();
                metaClass.name = sIn.ReadStringLE();

                // Read fields
                uint numFields = sIn.ReadUInt();
                metaClass.fields = new Field[numFields];
                for (uint j=0; j < numFields; ++j)
                {
                    Field field = new Field();
                    field.name = sIn.ReadStringLE();
                    field.type = sIn.ReadInt();
                    field.size = sIn.ReadInt();
                    field.offset = sIn.ReadInt();
                    metaClass.fields[j] = field;
                }

                // Read dependencies
                uint numDependencies = sIn.ReadUInt();
                metaClass.dependencies = new MetaDependency[numDependencies];
                for (uint j = 0; j < numDependencies; ++j)
                {
                    MetaDependency metaDependency = new MetaDependency();
                    metaDependency.name = sIn.ReadStringLE();
                    metaDependency.tag = sIn.ReadStringLE();
                    metaClass.dependencies[j] = metaDependency;
                }
                metaClasses[i] = metaClass;
            }
        }

        public static MetaClass GetMetaClass(string name)
        {
            foreach (MetaClass mc in metaClasses)
            {
                if (mc.name == name)
                {
                    return mc;
                }
            }
            return null;
        }
    }
}
