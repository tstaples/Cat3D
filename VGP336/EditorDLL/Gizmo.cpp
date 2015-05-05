#include "Gizmo.h"

Gizmo::Gizmo()
{
}

Gizmo::Gizmo(Math::Vector3 c, f32 ext)
    : center(c), extend(ext)
{
    //arms[0] = GizmoArm(center, Math::Vector3::XAxis() * ext, Math::Vector3(ext, 1.0f, 1.0f));
    //arms[1] = GizmoArm(center, Math::Vector3::YAxis() * ext, Math::Vector3(1.0f, ext, 1.0f));
    //arms[2] = GizmoArm(center, Math::Vector3::ZAxis() * ext, Math::Vector3(1.0f, 1.0f, ext));
}

void Gizmo::Draw(const Math::Matrix& transform)
{
    xAxis = Math::TransformCoord(xAxis, transform);
    yAxis = Math::TransformCoord(yAxis, transform);
    zAxis = Math::TransformCoord(zAxis, transform);

    SimpleDraw::AddLine(center, xAxis, Color::Green());
    SimpleDraw::AddLine(center, yAxis, Color::Yellow());
    SimpleDraw::AddLine(center, zAxis, Color::Blue());

    //SimpleDraw::AddAABB(center, arms[0].collider.extend, Color::Cyan());
    //SimpleDraw::AddAABB(center, arms[1].collider.extend, Color::Cyan());
    //SimpleDraw::AddAABB(center, arms[2].collider.extend, Color::Cyan());
}

void Gizmo::Translate(const Math::Vector3& pos)
{
    center = pos;
}

Math::Vector3 Gizmo::IsArmSelected(const Math::Matrix& transform, const Math::Ray& mouseRay) const
{
    //Math::Matrix inv = Math::Inverse(transform);
    static Math::AABB colliders[3] = 
    { 
        Math::AABB(0.0f, 0.0f, 0.0f, Math::Vector3(extend, 1.0f, 1.0f)),
        Math::AABB(0.0f, 0.0f, 0.0f, Math::Vector3(1.0f, extend, 1.0f)),
        Math::AABB(0.0f, 0.0f, 0.0f, Math::Vector3(1.0f, 1.0f, extend))
    };

    static Math::Vector3 axes[3] =
    {
        Math::Vector3::XAxis(),
        Math::Vector3::YAxis(),
        Math::Vector3::ZAxis()
    };

    for (u32 i=0; i < 3; ++i)
    {
        f32 temp;
        if (Math::Intersect(mouseRay, arms[i].collider, temp, temp))
        {
            //return Math::Normalize(arms[i].end - center);
            return axes[i];
        }
    }
    return Math::Vector3::Zero();
}