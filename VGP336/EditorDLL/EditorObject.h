#ifndef INCLUDED_EDITOROBJECT_H
#define INCLUDED_EDITOROBJECT_H

#include <Engine.h>
#include "Gizmo.h"

class EditorObject
{
public:
    EditorObject(GameObject* gameObject);
    ~EditorObject();

    void DrawGizmo();
    Math::Vector3 GetSelectedAxis(const Math::Ray& mouseRay) const;

    void Translate(const Math::Vector3& v);

    Math::Vector3 GetPosition() const;
    const Math::AABB& GetCollider() const;

    GameObject* GetGameObject() const   { return mpGameObject; }
    bool IsSelected() const             { return mIsSelected; }
    void Select()                       { mIsSelected = true; }
    void DeSelect()                     { mIsSelected = false; }

private:
    // GameObject this class wraps
    GameObject* mpGameObject;   // Weak ptr

    // Scene graph data
    EditorObject* mpParent;
    EditorObject* mpChildren;
    u32 mNumChildren;

    bool mIsSelected;
    Gizmo mGizmo;
    Math::AABB mDefaultCollider;
};

#endif // #ifndef INCLUDED_EDITOROBJECT_H