#ifndef INCLUDED_EDITORCOMMANDS_H
#define INCLUDED_EDITORCOMMANDS_H

#include "GameObject.h"

class EditorApp;
struct Handle;

class EditorCommands
{
public:
    EditorCommands(EditorApp& app);

    const u8* GetSelectedObjectData(u32& size);
    s32 UpdateComponent(const u8* buffer, u32 buffsize);
    const u8* DiscoverGameObjects(u32& buffsize);
    const u8* GetGameObject(Handle handle, u32& buffsize);
    void SelectGameObject(Handle handle);
    void CreateEmptyGameObject(Handle& handle);
    void RenameGameObject(Handle handle, const char* name);

private:
    EditorApp& mApp;
};

#endif