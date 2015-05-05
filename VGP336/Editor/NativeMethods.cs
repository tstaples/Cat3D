using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace Editor
{
    class NativeMethods
    {
        const string kDLLName = "EditorDLL.dll";

        [DllImport(kDLLName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern void Initialize(
            IntPtr instancePtrAddress, 
            IntPtr hPrevInstancePtrAddress, 
            IntPtr hWndPtrAddress, 
            int nCmdShow, 
            int screenWidth, 
            int screenHeight);

        [DllImport(kDLLName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern void WndProc(IntPtr hWndPtrAddress, int msg, int wParam, int lParam);

        [DllImport(kDLLName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern void UpdateFrame();

        [DllImport(kDLLName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern void Terminate();

        [DllImport(kDLLName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern int IsGameRunning();

        [StructLayout(LayoutKind.Sequential)]
        public class Vector3
        {
            public float x, y, z;
        }
    }
}
