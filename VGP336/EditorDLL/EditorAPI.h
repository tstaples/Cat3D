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

extern "C" DllExport unsigned int GetSelectedObjectData(unsigned char* buffer);
extern "C" DllExport void UpdateComponent(unsigned char* buffer, unsigned int size);
extern "C" DllExport unsigned int DiscoverGameObjects(unsigned char* buffer);
extern "C" DllExport unsigned int GetGameObject(unsigned short index, unsigned char* buffer);

extern "C" struct Matrix;
extern "C" struct Vector3;

#endif //#ifndef INCLUDED_EDITORAPI_H