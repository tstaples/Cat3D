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

            Handler = new InputHandler();
            Handler.Register(WM_LBUTTONUP, Owner.OnLeftClick);
        }

        public bool PreFilterMessage(ref Message m)
        {
            // Check for any of the mouse button clicks
            if (m.Msg == WM_LBUTTONUP || m.Msg == WM_RBUTTONUP || m.Msg == WM_MBUTTONUP)
            {
                // Update the viewport's focus flag since it can't seem to do it itself
                Point mpos = Owner.GetRelativeMousePos();
                Viewport.IsFocused = Viewport.Contains(mpos);
                Console.LogDebug("Editor", "Viewport focused: {0}", Viewport.IsFocused);
            }
            else if (m.Msg == WM_MOUSEMOVE)
            {
                // Temp
                Point mpos = Owner.GetRelativeMousePos();
                Console.LogDebug("Editor", "Mouse Position: {0} ", mpos.ToString());
            }

            bool ret = false;
            if (Viewport.IsFocused)
            {
                if (m.Msg == WM_KEYUP || m.Msg == WM_KEYDOWN || m.Msg == WM_SCROLL)
                {
                    // Send the input to the engine for further processing
                    NativeMethods.WndProc(Owner.Handle, m.Msg, (int)m.WParam, (int)m.LParam);
                }
                else
                {
                    Handler.OnInput(m.Msg);
                }
            }
            return ret;
        }
    }
}
