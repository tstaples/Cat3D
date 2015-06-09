#ifndef INCLUDED_EDITOROBJECT_H
#define INCLUDED_EDITOROBJECT_H

#include <Engine.h>

class EditorObject
{
public:
    EditorObject(GameObjectHandle gameObjectHandle);
    ~EditorObject();

    void Translate(const Math::Vector3& v);
    void Draw();

    Math::Vector3 GetPosition() const;
    Math::AABB GetCollider() const;

    GameObject* GetGameObject()         { return mGameObjectHandle.Get(); }
    GameObjectHandle GetHandle() const  { return mGameObjectHandle; }
    bool IsSelected() const             { return mIsSelected; }
    void Select()                       { mIsSelected = true; }
    void DeSelect()                     { mIsSelected = false; }

private:
    // GameObject this class wraps
    GameObjectHandle mGameObjectHandle;

    // Scene graph data
    EditorObject* mpParent;
    EditorObject* mpChildren;
    u32 mNumChildren;

    bool mIsSelected;
    Math::AABB mDefaultCollider;
};

#endif // #ifndef INCLUDED_EDITOROBJECT_H