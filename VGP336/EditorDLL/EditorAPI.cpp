#include "EditorAPI.h"

#include "EditorApp.h"

//====================================================================================================
// Macros
//====================================================================================================

#define EXTERNAL_VEC(v)\
    *(Vector3*)&v
#define NATIVE_VEC(v)\
    *(Math::Vector3*)&v

#define EXTERNAL_MATRIX(v)\
    *(Matrix*)&v
#define NATIVE_MATRIX(v)\
    *(Math::Matrix*)&v

//====================================================================================================
// Namespaces
//====================================================================================================

namespace
{
    EditorApp app;
}

//====================================================================================================
// Structs
//====================================================================================================

struct Vector3
{
    float x, y, z;
};

//----------------------------------------------------------------------------------------------------

struct Matrix
{
    float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;
};

//====================================================================================================
// Functions
//====================================================================================================

void Initialize(int* instancePtrAddress, int* hPrevInstancePtrAddress, int* hWndPtrAddress, int nCmdShow, int screenWidth, int screenHeight)
{
    // C# passes HINSTANCE and HWND values to C++ DLL as (int*)
    HINSTANCE hInstance = (HINSTANCE)instancePtrAddress;
    HINSTANCE hPrevInstance = (HINSTANCE)hPrevInstancePtrAddress;
    HWND hWnd = (HWND)hWndPtrAddress;

    app.HookWindow(hWnd, false);
    app.Initialize(hInstance, "Editor", screenWidth, screenHeight);
}

//----------------------------------------------------------------------------------------------------

void WndProc(int* hWndPtrAddress, int msg, int wParam, int lParam)
{
    app.ForwardInput((HWND)hWndPtrAddress, msg, wParam, lParam);
}

//----------------------------------------------------------------------------------------------------

void UpdateFrame()
{
    app.Update();
}

//----------------------------------------------------------------------------------------------------

void Terminate()
{
    app.Terminate();
}

//----------------------------------------------------------------------------------------------------

int IsGameRunning()
{
    return (int)app.IsRunning();
}

//----------------------------------------------------------------------------------------------------

unsigned int GetSelectedObjectData(unsigned char* buffer)
{
    unsigned int size = 0;
    unsigned char* buff = (unsigned char*)app.GetSelectedObjectData(size);
    memcpy(buffer, buff, size);
    return size;
}

//----------------------------------------------------------------------------------------------------

void UpdateComponent(unsigned char* buffer, unsigned int size)
{
    app.UpdateComponent(buffer, size);
}