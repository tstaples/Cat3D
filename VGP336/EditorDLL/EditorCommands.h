#ifndef INCLUDED_EDITORCOMMANDS_H
#define INCLUDED_EDITORCOMMANDS_H

class EditorApp;

class EditorCommands
{
public:
    EditorCommands(EditorApp& app);

    const u8* GetSelectedObjectData(u32& size);
    s32 UpdateComponent(const u8* buffer, u32 buffsize);
    const u8* DiscoverGameObjects(u32& buffsize);
    const u8* GetGameObject(u16 index, u32& buffsize);
    void SelectGameObject(u16 index);
    void CreateEmptyGameObject(u16& index);
    void RenameGameObject(u16 index, const char* name);

private:
    EditorApp& mApp;
};

#endif