using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.ComponentModel;

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

        [DllImport(kDLLName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern uint GetSelectedObjectData(byte[] buffer);

        [DllImport(kDLLName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern void GetMatrixRotation(float[] m, Vector3 v);
    }



    [StructLayout(LayoutKind.Sequential)]
    public struct Matrix
    {
        float _11, _12, _13, _14;
        float _21, _22, _23, _24;
        float _31, _32, _33, _34;
        float _41, _42, _43, _44;
    };
}
