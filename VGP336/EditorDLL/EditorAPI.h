#ifndef INCLUDED_EDITORAPI_H
#define INCLUDED_EDITORAPI_H

#define DllExport _declspec(dllexport)

extern "C" DllExport void Initialize(
    int* instancePtrAddress,
    int* hPrevInstancePtrAddress,
    int* hWndPtrAddress,
    int nCmdShow,
    int screenWidth,
    int screenHeight);

extern "C" DllExport void WndProc(
    int* hWndPtrAddress, 
    int msg, 
    int wParam, 
    int lParam);

extern "C" DllExport void UpdateFrame();
extern "C" DllExport void Terminate();
extern "C" DllExport int IsGameRunning();


#endif //#ifndef INCLUDED_EDITORAPI_H