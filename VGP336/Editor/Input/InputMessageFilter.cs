using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;

namespace Editor
{
    public class InputMessageFilter : IMessageFilter
    {
        private EditorForm Owner;
        private InputHandler Handler;
        private SelectablePanel Viewport;

        const int WM_KEYUP = 0x101;
        const int WM_KEYDOWN = 0x0100;
        const int WM_SCROLL = 0x020A;
        const int WM_MOUSEMOVE = 0x0200;
        const int WM_LBUTTONUP = 0x0202;
        const int WM_RBUTTONDOWN = 0x0204;
        const int WM_RBUTTONUP = 0x0205;
        const int WM_RBUTTONDBLCLICK = 0x0206;
        const int WM_MBUTTONUP = 0x0208;

        public InputMessageFilter(EditorForm fOwner)
        {
            Owner = fOwner;
            Viewport = Owner.Viewport;

            // Note: register in order of priority
            Handler = new InputHandler();
            Handler.Register(WM_LBUTTONUP, Owner.OnViewportFocus);
            Handler.Register(WM_RBUTTONUP, Owner.OnViewportFocus);
            Handler.Register(WM_MBUTTONUP, Owner.OnViewportFocus);
            Handler.Register(WM_LBUTTONUP, Owner.OnSelectObject);
            Handler.Register(WM_RBUTTONUP, Owner.OnRightClickInspector);
            Handler.Register(WM_KEYDOWN, Owner.OnDeleteGameObject);
        }

        // https://msdn.microsoft.com/en-us/library/system.windows.forms.imessagefilter.prefiltermessage(v=vs.110).aspx
        public bool PreFilterMessage(ref Message m)
        {
            Keys key = (Keys)m.WParam;
            bool handled = Handler.OnInput(m.Msg, key);
            if (!handled && Viewport.IsFocused)
            {
                if (m.Msg == WM_KEYUP || m.Msg == WM_KEYDOWN || m.Msg == WM_SCROLL)
                {
                    // Send the input to the engine for further processing
                    NativeMethods.WndProc(Owner.Handle, m.Msg, (int)m.WParam, (int)m.LParam);
                    return true; // Consume input
                }
            }
            return handled;
        }
    }
}
