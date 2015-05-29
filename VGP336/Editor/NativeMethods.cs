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
        public static int True = 1;
        public static int False = 0;

        const string kDLLName = "EditorDLL.dll";

        #region Application functions
        [DllImport(kDLLName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern void Initialize(
            IntPtr instancePtrAddress, 
            IntPtr hPrevInstancePtrAddress, 
            IntPtr hWndPtrAddress, 
            int nCmdShow, 
            int screenWidth, 
            int screenHeight);

        [DllImport(kDLLName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern bool GetLastEditorError(NativeTypes.Error error);

        [DllImport(kDLLName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern void WndProc(IntPtr hWndPtrAddress, int msg, int wParam, int lParam);

        [DllImport(kDLLName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern void UpdateFrame();

        [DllImport(kDLLName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern void Terminate();

        [DllImport(kDLLName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern int IsGameRunning();
        #endregion

        #region GameObject functions
        [DllImport(kDLLName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern uint GetSelectedObjectData(byte[] buffer, uint size);

        [DllImport(kDLLName, CallingConvention = CallingConvention.Cdecl, CharSet=CharSet.Ansi)]
        public unsafe static extern int UpdateComponent(byte[] buffer, uint size);

        [DllImport(kDLLName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern uint DiscoverGameObjects(byte[] buffer, uint size);

        [DllImport(kDLLName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern uint GetGameObject(NativeTypes.Handle handle, byte[] buffer, uint size);

        [DllImport(kDLLName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern int SelectGameObject(NativeTypes.Handle handle);

        [DllImport(kDLLName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern uint CreateNewGameObject(byte[] buffer, uint size);

        [DllImport(kDLLName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern int RenameGameObject(NativeTypes.Handle handle, string name);

        [DllImport(kDLLName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern int AddComponent(NativeTypes.Handle handle, string componentName);
        #endregion

        [DllImport(kDLLName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern int NewLevel(string filename);

        [DllImport(kDLLName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern int LoadLevel(string filename);

        [DllImport(kDLLName, CallingConvention = CallingConvention.Cdecl)]
        public unsafe static extern int SaveLevel(string filename);
    }
}
