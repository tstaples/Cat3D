#ifndef INCLUDED_EDITORAPI_H
#define INCLUDED_EDITORAPI_H

//====================================================================================================
// Filename:	EditorAPI.h
// Created by:	Tyler Staples
// Description: Engine API functions to allow compatibility with C#.
//====================================================================================================

//====================================================================================================
// Macros
//====================================================================================================

#define DllExport _declspec(dllexport)
#define EditorAPI extern "C" DllExport

//====================================================================================================
// API Function Prototypes
//====================================================================================================

extern "C" struct Matrix;
extern "C" struct Vector3;
extern "C" struct Handle;
extern "C" struct Error;

EditorAPI bool GetLastEditorError(Error& error);

EditorAPI void Initialize(
    int* instancePtrAddress,
    int* hPrevInstancePtrAddress,
    int* hWndPtrAddress,
    int nCmdShow,
    int screenWidth,
    int screenHeight);

EditorAPI void WndProc(
    int* hWndPtrAddress, 
    int msg, 
    int wParam, 
    int lParam);

EditorAPI void UpdateFrame();
EditorAPI void Terminate();
EditorAPI int IsGameRunning();

// Serializes the currently selected GameObjects.
// @param buffer: externally managed buffer to hold the serialized data.
// Returns the number of bytes copied to the buffer.
EditorAPI unsigned int GetSelectedObjectData(unsigned char* dst, unsigned int size);

// Updates the values of a particular component.
// @param buffer: buffer containing the serialized component data.
// @param size: size in bytes of the component data.
// Returns 0 if succeeded.
EditorAPI int UpdateComponent(const unsigned char* buffer, unsigned int size);

// Gets basic info about all objects in the world.
// @param buffer: externally managed buffer to output the serialized data to.
// Returns the number of bytes copied to the buffer.
EditorAPI unsigned int DiscoverGameObjects(unsigned char* dst, unsigned int size);

// Gets the serialized data for a GameObject.
// @param index: handle index of the desired GameObject.
// @param buffer: output buffer to write the serialized data to.
// Returns the number of bytes copied to the buffer.
EditorAPI unsigned int GetGameObject(Handle handle, unsigned char* dst, unsigned int size);

EditorAPI int SelectGameObject(Handle handle);

EditorAPI unsigned int CreateNewGameObject(unsigned char* dst, unsigned int size);

EditorAPI int RenameGameObject(Handle handle, const char* name);


#endif //#ifndef INCLUDED_EDITORAPI_H