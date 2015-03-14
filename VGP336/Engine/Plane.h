#ifndef INCLUDED_ENGINE_PLANE_H
#define INCLUDED_ENGINE_PLANE_H

//====================================================================================================
// Filename:	Plane.h
// Created by:	Tyler Staples
// Description:	Struct for a Plane.
//====================================================================================================

//====================================================================================================
// Structs
//====================================================================================================

namespace Math
{

struct Plane
{
    Vector3 n;  // normal
    f32 d;      // dist from origin

    Plane() {}
    Plane(f32 x, f32 y, f32 z, f32 d)
        : n(x, y, z), d(d)
    {}
    Plane(const Vector3& normal, f32 d)
        : n(normal), d(d)
    {}
};

} // namespace Math

#endif // #ifndef INCLUDED_ENGINE_PLANE_H