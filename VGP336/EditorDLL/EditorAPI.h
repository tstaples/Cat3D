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

/* Serializes the currently selected GameObjects.
 * @param buffer: externally managed buffer to hold the serialized data.
 * @param size: size of the external buffer.
 * Returns the number of bytes copied to the buffer.
 */
EditorAPI unsigned int GetSelectedObjectData(unsigned char* dst, unsigned int size);

/* Updates the values of a particular component.
 * @param buffer: buffer containing the serialized component data.
 * @param size: size in bytes of the buffer.
 * Returns 1 if succeeded.
 */
EditorAPI int UpdateComponent(const unsigned char* buffer, unsigned int size);

/* Gets basic info about all objects in the world.
 * @param buffer: externally managed buffer to output the serialized data to.
 * @param size: size of the external buffer.
 * Returns the number of bytes copied to the buffer.
 */
EditorAPI unsigned int DiscoverGameObjects(unsigned char* dst, unsigned int size);

/* Gets the serialized data for a GameObject.
 * @param handle: handle of the desired GameObject.
 * @param buffer: output buffer to write the serialized data to.
 * @param size: size of the external buffer.
 * Returns the number of bytes copied to the buffer.
 */
EditorAPI unsigned int GetGameObject(Handle handle, unsigned char* dst, unsigned int size);

/* Selects a GameObject.
 * @param handle: handle of the GameObject to select.
 * Returns 1 if the GameObject was successfully selected.
 */
EditorAPI int SelectGameObject(Handle handle);

/* Creates a new GameObject from a template and adds it to the world.
 * @param tempalteFile: template to create the object from.
 * @param dst: externally managed buffer to write the new object's data to.
 * @param size: size of the external buffer.
 * Returns the number of bytes written to the dst. Returns 0 if fails.
 */
EditorAPI unsigned int CreateGameObjectFromTemplate(const char* templateFile, unsigned char* dst, unsigned int size);

EditorAPI int DestroyGameObject(Handle handle);

/* Renames a GameObject.
 * @param handle: handle of the desired GameObject.
 * @param name: new name for the GameObject.
 * Returns 1 if the GameObject was renamed successfully.
 */
EditorAPI int RenameGameObject(Handle handle, const char* name);

EditorAPI int AddComponent(Handle handle, const char* componentName);
EditorAPI int RemoveComponent(Handle handle, const char* componentName);

EditorAPI unsigned int GetMetaData(unsigned char* dst, unsigned int size);

EditorAPI int NewLevel(const char* filename);
EditorAPI int LoadLevel(const char* filename);
EditorAPI int SaveLevel(const char* filename);

EditorAPI int StartGame();
EditorAPI int PauseGame();
EditorAPI int StopGame();

#endif //#ifndef INCLUDED_EDITORAPI_H