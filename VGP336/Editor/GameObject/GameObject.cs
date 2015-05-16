using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Editor
{
    public class GameObject
    {
        private List<Component> Components;
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
    }
}
