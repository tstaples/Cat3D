#ifndef INCLUDED_ENGINE_SPHERE_H
#define INCLUDED_ENGINE_SPHERE_H

//====================================================================================================
// Filename:	Sphere.h
// Created by:	Tyler Staples
// Description:	Struct for a sphere.
//====================================================================================================

//====================================================================================================
// Structs
//====================================================================================================

namespace Math
{

struct Sphere
{
    Vector3 center;
    f32 radius;

    Sphere() 
        : center(Vector3::Zero()), radius(1.0f) 
    {}
    Sphere(const Vector3& _center, f32 rad) 
        : center(_center), radius(rad)
    {}
};

} // namespace Math

#endif // #ifndef INCLUDED_ENGINE_SPHERE_H