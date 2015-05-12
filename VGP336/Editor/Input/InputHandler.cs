using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Editor
{
    public delegate bool InputCallback();

    public class InputHandler
    {
        private Dictionary<int, InputCallback> Callbacks;

        public InputHandler()
        {
            Callbacks = new Dictionary<int, InputCallback>();
        }

        public void Register(int key, InputCallback callback)
        {
            Callbacks.Add(key, callback);
        }

        public bool OnInput(int key)
        {
            if (Callbacks.ContainsKey(key))
            {
                return Callbacks[key]();
            }
            return false;
        }
    }
}
