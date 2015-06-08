using System;
using System.Drawing;
using System.Windows.Forms;

namespace Editor
{
    // From: http://stackoverflow.com/a/3562449/3421589
    public class SelectablePanel : Panel
    {
        public bool IsFocused;

        public SelectablePanel()
        {
            IsFocused = false;

            this.SetStyle(ControlStyles.Selectable, true);
            this.TabStop = true;
        }

        public bool Contains(Point point)
        {
            Point rel = this.PointToClient(point);
            return this.Bounds.Contains(rel);
        }

        protected override void OnMouseDown(MouseEventArgs e)
        {
            this.Focus();
            IsFocused = true;
            base.OnMouseDown(e);
        }

        protected override bool IsInputKey(Keys keyData)
        {
            if (keyData == Keys.Up || keyData == Keys.Down) return true;
            if (keyData == Keys.Left || keyData == Keys.Right) return true;
            return base.IsInputKey(keyData);
        }

        protected override void OnEnter(EventArgs e)
        {
            this.Invalidate();
            IsFocused = true;
            base.OnEnter(e);
        }

        protected override void OnLeave(EventArgs e)
        {
            this.Invalidate();
            IsFocused = false;
            base.OnLeave(e);
        }

        protected override void OnPaint(PaintEventArgs pe)
        {
            base.OnPaint(pe);
            if (this.Focused)
            {
                var rc = this.ClientRectangle;
                rc.Inflate(-2, -2);
                ControlPaint.DrawFocusRectangle(pe.Graphics, rc);
            }
        }
    }
}
