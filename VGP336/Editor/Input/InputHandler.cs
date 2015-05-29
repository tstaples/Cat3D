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
        private Dictionary<int, List<InputCallback>> Callbacks;

        public InputHandler()
        {
            Callbacks = new Dictionary<int, List<InputCallback>>();
        }

        public void Register(int key, InputCallback callback)
        {
            if (!Callbacks.ContainsKey(key))
            {
                Callbacks.Add(key, new List<InputCallback>());
            }
            Callbacks[key].Add(callback);
        }

        public bool OnInput(int key)
        {
            if (Callbacks.ContainsKey(key))
            {
                foreach (InputCallback callback in Callbacks[key])
                {
                    if (callback())
                    {
                        // This callback consumed the input
                        return true;
                    }
                }
            }
            return false;
        }
    }
}
