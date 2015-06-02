#include "EditorAPI.h"

#include "EditorApp.h"
#include "EditorCommands.h"

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
    EditorCommands cmd(app);
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

//----------------------------------------------------------------------------------------------------

struct Handle
{
    u16 instance;
    u16 index;

    Handle()
    {
        instance = U16_MAX;
        index = U16_MAX;
    }
};

struct Error
{
    char message[2048];
};


//====================================================================================================
// Functions
//====================================================================================================

bool GetLastEditorError(Error& error)
{
    memset(error.message, 0, sizeof(error.message));
    const char* msg = cmd.GetLastError();
    if (msg)
    {
        strcpy_s(error.message, msg);
        return true;
    }
    strcpy_s(error.message, "None");
    return false;
}

//----------------------------------------------------------------------------------------------------

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

unsigned int GetSelectedObjectData(unsigned char* dst, unsigned int size)
{
    unsigned int written = 0;
    cmd.GetSelectedObjectData(dst, size, written);
    return written;
}

//----------------------------------------------------------------------------------------------------

int UpdateComponent(const unsigned char* buffer, unsigned int size)
{
    return (int)cmd.UpdateComponent(buffer, size);
}

//----------------------------------------------------------------------------------------------------

unsigned int DiscoverGameObjects(unsigned char* dst, unsigned int size)
{
    unsigned int written = 0;
    cmd.DiscoverGameObjects(dst, size, written);
    return written;
}

//----------------------------------------------------------------------------------------------------

unsigned int GetGameObject(Handle handle, unsigned char* dst, unsigned int size)
{
    unsigned int written = 0;
    cmd.GetGameObject(handle, dst, size, written);
    return written;
}

//----------------------------------------------------------------------------------------------------

int SelectGameObject(Handle handle)
{
    return (int)cmd.SelectGameObject(handle);
}

//----------------------------------------------------------------------------------------------------

unsigned int CreateGameObjectFromTemplate(const char* templateFile, unsigned char* dst, unsigned int size)
{
    unsigned int written = 0;

    Handle handle;
    if (cmd.CreateGameObjectFromTemplate(templateFile, handle))
    {
        cmd.GetGameObject(handle, dst, size, written);
    }
    return written;
}

//----------------------------------------------------------------------------------------------------

int DestroyGameObject(Handle handle)
{
    return (int)cmd.DestroyGameObject(handle);
}

//----------------------------------------------------------------------------------------------------

int RenameGameObject(Handle handle, const char* name)
{
    return (int)cmd.RenameGameObject(handle, name);
}

//----------------------------------------------------------------------------------------------------

int AddComponent(Handle handle, const char* componentName)
{
    return (int)cmd.AddComponent(handle, componentName);
}

//----------------------------------------------------------------------------------------------------

int RemoveComponent(Handle handle, const char* componentName)
{
    return (int)cmd.RemoveComponent(handle, componentName);
}

//----------------------------------------------------------------------------------------------------

unsigned int GetMetaData(unsigned char* dst, unsigned int size)
{
    unsigned int written = 0;
    VERIFY(cmd.GetMetaData(dst, size, written), "[EditorAPI] Failed to get meta data");
    return written;
}

//----------------------------------------------------------------------------------------------------

int NewLevel(const char* filename)
{
    return (int)cmd.NewLevel(filename);
}

//----------------------------------------------------------------------------------------------------

int LoadLevel(const char* filename)
{
    return (int)cmd.LoadLevel(filename);
}

//----------------------------------------------------------------------------------------------------

int SaveLevel(const char* filename)
{
    return (int)cmd.SaveLevel(filename);
}

//----------------------------------------------------------------------------------------------------

int StartGame()
{
    return (int)cmd.StartGame();
}

//----------------------------------------------------------------------------------------------------

int PauseGame()
{
    return (int)cmd.PauseGame();
}

//----------------------------------------------------------------------------------------------------

int StopGame()
{
    return (int)cmd.StopGame();
}