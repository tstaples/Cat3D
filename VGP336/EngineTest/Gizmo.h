#ifndef INCLUDED_GIZMO_H
#define INCLUDED_GIZMO_H

#include <Engine.h>

class EditorObject;
struct InputData;

class Gizmo
{
public:
    typedef std::vector<EditorObject*> Objects;

    Gizmo(Camera& camera, f32 extend, f32 armWidth);
    virtual ~Gizmo();

    // Checks if the gizmo is selected.
    // @param transform: transform of the object this gizmo is attached to.
    // @param mouseRay: mouse's ray in world space.
    // Returns true if selected, false if not.
    virtual bool IsSelected(const Objects& selectedObjs, const Math::Ray& mouseRay) = 0;

    virtual void Update(const Objects& selectedObjs, const InputData& input, u32 screenW, u32 screenH) = 0;

    virtual void Draw(const Objects& selectedObjs) = 0;

protected:
    Camera& mCamera;

    Math::Vector3 mSelectedAxis;
    f32 mExtend;
    f32 mArmWidth;
    u8 mSelectedArm; // using bitflag because multiple axes can be selected
};

class TranslateGizmo : public Gizmo
{
public:
    TranslateGizmo(Camera& camera, f32 extend, f32 armWidth);
    ~TranslateGizmo();

    virtual bool IsSelected(const Objects& selectedObjs, const Math::Ray& mouseRay);
    virtual void Update(const Objects& selectedObjs, const InputData& input, u32 screenW, u32 screenH);
    virtual void Draw(const Objects& selectedObjs);
};

#endif // #ifndef INCLUDED_GIZMO_H