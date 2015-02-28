#ifndef INCLUDED_ENGINE_PARTICLE_H
#define INCLUDED_ENGINE_PARTICLE_H

//====================================================================================================
// Filename:	Particle.h
// Created by:	Tyler Staples
// Description:	Class representing a physics particle.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "EngineMath.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

class Particle
{
public:
    Particle();
    Particle(f32 x, f32 y, f32 z, f32 radius, f32 invMass);

    void Render();

    void SetPosition(f32 x, f32 y, f32 z);
    void SetVelocity(f32 x, f32 y, f32 z);

    // Verlet integration
    Math::Vector3 pos;
    Math::Vector3 posOld;
    Math::Vector3 accel;

    f32 radius;
    f32 invMass;    // avoid dividing by 0
    f32 bounce;     // energy conserved during collision (0-1)
};

#endif // #ifndef INCLUDED_ENGINE_PARTICLE_H