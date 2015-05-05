using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Editor
{
    public class InputMessageFilter : IMessageFilter
    {
        private EditorForm Owner;
        const int WM_KEYUP = 0x101;
        const int WM_KEYDOWN = 0x0100;
        const int WM_SCROLL = 0x020A;

        public InputMessageFilter(EditorForm fOwner)
        {
            Owner = fOwner;
        }

        public bool PreFilterMessage(ref Message m)
        {
            bool ret = false;
            if (m.Msg == WM_KEYUP || m.Msg == WM_KEYDOWN || m.Msg == WM_SCROLL)
            {
                if (Owner.PanelIsFocused())
                {
                    // Send the input to the engine for further processing
                    NativeMethods.WndProc(Owner.Handle, m.Msg, (int)m.WParam, (int)m.LParam);
                }
            }
            return ret;
        }
    }
}
