#ifndef INCLUDED_EDITORCOMMANDS_H
#define INCLUDED_EDITORCOMMANDS_H

#include "GameObject.h"

class EditorApp;
struct Handle;

class EditorCommands
{
public:
    EditorCommands(EditorApp& app);

    const char* GetLastError();
    bool GetSelectedObjectData(u8* dst, u32 size, u32& bytesWritten);
    bool UpdateComponent(const u8* buffer, u32 buffsize);
    bool DiscoverGameObjects(u8* dst, u32 size, u32& bytesWritten);
    bool GetGameObject(Handle handle, u8* dst, u32 size, u32& bytesWritten);
    bool SelectGameObject(Handle handle);
    bool CreateEmptyGameObject(Handle& handle);
    bool RenameGameObject(Handle handle, const char* name);
    bool AddComponent(Handle handle, const char* componentName);

private:
    EditorApp& mApp;
};

#endif