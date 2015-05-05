#ifndef INCLUDED_GIZMO_H
#define INCLUDED_GIZMO_H

#include <Engine.h>

struct GizmoArm
{
    GizmoArm() {}
    GizmoArm(Math::Vector3 c, Math::Vector3 end, Math::Vector3 ext)
    : end(end)
    {
        Math::Vector3 half = (end - c) / 2.0f;
        collider.center = half;
        collider.extend = ext;
    }

    Math::Vector3 end;
    Math::AABB collider;
};

class Gizmo
{
public:
    Gizmo();
    Gizmo(Math::Vector3 c, f32 ext);

    void Draw(const Math::Matrix& transform);
    void Translate(const Math::Vector3& pos);
    // Returns axis vector for corresponding arm. Zero vector if no collision
    Math::Vector3 IsArmSelected(const Math::Matrix& transform, const Math::Ray& mouseRay) const;

private:
    GizmoArm arms[3];
    Math::Vector3 xAxis;
    Math::Vector3 yAxis;
    Math::Vector3 zAxis;
    Math::Vector3 center;
    f32 extend;
};

#endif // #ifndef INCLUDED_GIZMO_H